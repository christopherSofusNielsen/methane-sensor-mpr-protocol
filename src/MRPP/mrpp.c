#include "mrpp.h"

MRPP_STATE state;
uint8_t _groupId=0;

uint8_t get_group_id();
void set_group_id(uint8_t groupId);

void MRPP_init_group(COLLECTION collections[], uint8_t nCollections){
    uint8_t groupId=get_group_id();

    mrpp_state_init(&state, groupId, collections, nCollections);



    groupId++;
    set_group_id(groupId);
}


uint8_t get_group_id(){
    return _groupId;
}

void set_group_id(uint8_t groupId){
    _groupId=groupId;
}