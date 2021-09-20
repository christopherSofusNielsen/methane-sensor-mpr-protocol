#include "mrpp_state.h"


void mrpp_state_init(MRPP_STATE *state, uint8_t groupId, COLLECTION collections[], uint8_t nCollections){
    state->groupId=groupId;

    uint16_t startingIndex=0;
    for (uint8_t i = 0; i < nCollections; i++)
    {
        state->collections[i].id=i+1;
        state->collections[i].samplingInterval=collections[i].samplingInterval;

        //Calculate length and set starting index
        state->collections[i].startIndex=startingIndex;
        uint16_t len=collections[i].samplings*collections[i].type+COLLECTION_DATA_META_SIZE;
        state->collections[i].length=len;
        

        //calculate starting and ending body
        state->collections[i].beginsInBody=startingIndex/DR_BODY_PAYLOAD_SIZE;
        state->collections[i].endsInBody=(startingIndex+len)/DR_BODY_PAYLOAD_SIZE;

        //set status
        state->collections[i].status=WAITING;


        startingIndex+=len;      
    }

    //Calculate lastSubId
    uint8_t lastSubId=startingIndex/DR_BODY_PAYLOAD_SIZE+1+DR_SUBID_OVERHEAD;
    state->lastSubId=lastSubId;
    
    //calculate bodies
    uint8_t nBodies=startingIndex/DR_BODY_PAYLOAD_SIZE+1;
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