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

void MRPP_add_collection_data_INT16(uint8_t collectionId, uint8_t timestamp[], uint16_t values[]){
    uint8_t metadata[6];
    uint16_t begin;
    uint16_t length;
    mrpp_state_set_collection(&state, collectionId, timestamp, metadata);
    mrpp_state_get_collection_address(&state, collectionId, &begin, &length);

    mrpp_data_add_int16(metadata, values, begin, length);
}