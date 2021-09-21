
#ifndef MRPP_H
#define MRPP_H

#include "util/mrpp_state.h"
#include "util/mrpp_data.h"

#ifdef __TEST_MODE
extern MRPP_STATE state;
extern uint8_t _groupId;
#endif

void MRPP_init_group(COLLECTION collections[], uint8_t nCollections);


#endif