
#ifndef MRPP_H
#define MRPP_H

#include "util/mrpp_state.h"
#include "util/mrpp_data.h"
#include <stdbool.h>

#ifdef __TEST_MODE
extern MRPP_STATE state;
extern uint8_t _groupId;
#endif

void MRPP_init_group(COLLECTION collections[], uint8_t nCollections);
void MRPP_add_collection_data_INT16(uint8_t collectionId, uint8_t timestamp[], uint16_t values[]);
bool MRPP_is_body_package_ready();
bool MRPP_get_ready_body_package(uint8_t package[], uint8_t *package_length);

#endif