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




        startingIndex+=len;      
    }

    //Calculate lastSubId
    uint8_t lastSubId=startingIndex/DR_BODY_PAYLOAD_SIZE+1+DR_SUBID_OVERHEAD;
    state->lastSubId=lastSubId;
    
}