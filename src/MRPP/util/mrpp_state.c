#include "mrpp_state.h"

static void update_bodies(MRPP_STATE *state, uint8_t collectionId);

void mrpp_state_init(MRPP_STATE *state, uint8_t groupId, COLLECTION collections[], uint8_t nCollections){
    state->groupId=groupId;
    state->nCollections=nCollections;

    uint16_t startingIndex=0;
    for (uint8_t i = 0; i < nCollections; i++)
    {
        state->collections[i].id=i+1;
        state->collections[i].samplingInterval=collections[i].samplingInterval;

        //type
        state->collections[i].type=collections[i].type;

        //Calculate length and set starting index
        state->collections[i].startIndex=startingIndex;
        uint16_t len=collections[i].samplings*collections[i].type+COLLECTION_DATA_META_SIZE;
        state->collections[i].length=len;
        

        //calculate starting and ending body
        state->collections[i].beginsInBody=startingIndex/DR_BODY_PAYLOAD_SIZE;
        uint8_t endsInBody=(startingIndex+len)/DR_BODY_PAYLOAD_SIZE;
        //handling edge cases where data matches exact size of bodies fx 48, 96 and so on
        endsInBody=(startingIndex+len)%DR_BODY_PAYLOAD_SIZE==0?endsInBody-1:endsInBody;
        state->collections[i].endsInBody=endsInBody;
        

        //set status
        state->collections[i].status=WAITING;


        startingIndex+=len;      
    }

    //Calculate lastSubId
    uint8_t lastSubId=startingIndex/DR_BODY_PAYLOAD_SIZE+DR_SUBID_OVERHEAD;
    //handling edge cases where data matches exact size of bodies fx 48, 96 and so on
    lastSubId=startingIndex%DR_BODY_PAYLOAD_SIZE==0?lastSubId-1:lastSubId;
    state->lastSubId=lastSubId;
    
    //calculate bodies
    uint8_t nBodies=startingIndex/DR_BODY_PAYLOAD_SIZE;
    //handling edge cases where data matches exact size of bodies fx 48, 96 and so on
    nBodies=startingIndex%DR_BODY_PAYLOAD_SIZE==0?nBodies:nBodies+1;
    state->nBodies=nBodies;

    for (uint8_t i = 0; i < nBodies; i++)
    {
        state->bodies[i]=WAITING;
    }
}


uint8_t mrpp_state_get_header(MRPP_STATE *state, uint8_t package[]){
    package[0]=0;
    package[1]=state->lastSubId;

    for (uint8_t i = 0; i < state->nCollections; i++)
    {
        package[i*4+2]=state->collections[i].startIndex >> 8;
        package[i*4+3]=state->collections[i].startIndex;
        package[i*4+4]=state->collections[i].length>>8;
        package[i*4+5]=state->collections[i].length;
    }
    
    return 2+state->nCollections*DR_HEADER_COLLECTION_META_SIZE;
}

uint8_t mrpp_state_get_tail(MRPP_STATE *state, uint8_t package[]){
    package[0]=state->lastSubId;
    package[1]=state->lastSubId;

    for (uint8_t i = 0; i < state->nCollections; i++)
    {
        package[i*4+2]=state->collections[i].startIndex >> 8;
        package[i*4+3]=state->collections[i].startIndex;
        package[i*4+4]=state->collections[i].length>>8;
        package[i*4+5]=state->collections[i].length;
    }
    return 2+state->nCollections*DR_HEADER_COLLECTION_META_SIZE;
}

void mrpp_state_set_collection(MRPP_STATE *state, uint8_t collectionId, uint8_t timestamp[4], uint8_t metadata[7]){
    state->collections[collectionId-1].status=DONE;

    //Copy ts
    for (uint8_t i = 0; i < 4; i++)
    {
        metadata[i]=timestamp[i];
    }

    //add sampling interval
    metadata[4]=state->collections[collectionId-1].samplingInterval>>8;
    metadata[5]=state->collections[collectionId-1].samplingInterval;

    //add type
    metadata[6]=state->collections[collectionId-1].type;

    //Update bodies
    update_bodies(state, collectionId);
}


static void update_bodies(MRPP_STATE *state, uint8_t collectionId){
    
    //update begin
    bool beginIsDone=true;
    uint8_t beginsInBody=state->collections[collectionId-1].beginsInBody;
    for (uint8_t i = 0; i < collectionId-1; i++)
    {
        if(i==collectionId-1) continue;

        if( state->collections[i].endsInBody==beginsInBody){
            if(state->collections[i].status!=DONE){
                beginIsDone=false;
                break;
            }
        }
    }

    if(beginIsDone){
        state->bodies[beginsInBody]=READY;
    }
    

    //update end
    bool endIsDone=true;
    uint8_t endsInBody=state->collections[collectionId-1].endsInBody;
    for (uint8_t i = collectionId; i < state->nCollections; i++)
    {
         if(i==collectionId-1) continue;

        if(state->collections[i].beginsInBody==endsInBody){
            if(state->collections[i].status!=DONE){
                endIsDone=false;
                break;
            }
        }
    }

    if(endIsDone){
        state->bodies[endsInBody]=READY;
    }else{
        //to handle cases where there are multiple collection in same body that starts and ends there
        state->bodies[endsInBody]=WAITING;
    }
    
    //update between
    if(endsInBody-beginsInBody<2) return;

    for (uint8_t i = beginsInBody+1; i < endsInBody; i++)
    {
        state->bodies[i]=READY;
    }
    
}

bool mrpp_state_is_body_ready(MRPP_STATE *state){
    bool isReady=false;
    for (uint8_t i = 0; i < state->nBodies; i++)
    {
        if(state->bodies[i]==READY){
            isReady=true;
            break;
        }
    }
    return isReady;
}

void mrpp_state_get_body_address(MRPP_STATE *state, uint16_t *begin, uint8_t*length){
    
    if(!mrpp_state_is_body_ready(state)){
        *begin=0;
        *length=0;
        return;
    }

    uint8_t readyIndex;
    for (uint8_t i = 0; i < state->nBodies; i++)
    {
        if(state->bodies[i]==READY){
            readyIndex=i;
            break;
        }
    }

    *begin=readyIndex*DR_BODY_PAYLOAD_SIZE;
    if(readyIndex<state->nBodies-1){
        *length=DR_BODY_PAYLOAD_SIZE;
    }else{
        *length=(state->collections[state->nCollections-1].startIndex+state->collections[state->nCollections-1].length)%DR_BODY_PAYLOAD_SIZE; 
    }  
}


void mrpp_state_get_collection_address(MRPP_STATE *state, uint8_t collectionId, uint16_t *begin, uint16_t*length){
    *begin=state->collections[collectionId-1].startIndex;
    *length=state->collections[collectionId-1].length;
}
