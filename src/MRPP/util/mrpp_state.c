#include "mrpp_state.h"


void mrpp_state_init(MRPP_STATE *state, uint8_t groupId, COLLECTION collections[], uint8_t nCollections){
    state->groupId=groupId;

    uint16_t startingIndex=0;
    for (uint8_t i = 0; i < nCollections; i++)
    {
        state->collections[i].id=i+1;
        state->collections[i].startIndex=startingIndex;
        state->collections[i].samplingInterval=collections[i].samplingInterval;
        uint16_t len=collections[i].samplings*collections[i].type+7;
        state->collections[i].length=len;
        startingIndex+=len;
    }
    
}