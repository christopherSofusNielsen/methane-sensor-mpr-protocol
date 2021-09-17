
#ifndef MRPP_STATE_H
#define MRPP_STATE_H

#include <stdint.h>

#define COLLECTION_DATA_META_SIZE 7
#define DR_BODY_PAYLOAD_SIZE 48
#define DR_SUBID_OVERHEAD 2

typedef enum {
    T_INT8=1,
    T_INT16=2,
    T_FLOAT=4
} MEASURE_TYPE;

typedef struct COLLECTION
{
    uint16_t samplings;
    uint16_t samplingInterval;
    MEASURE_TYPE type;
} COLLECTION;

typedef struct __COLLECTION_STATE
{
    uint8_t id;
    uint16_t startIndex;
    uint16_t length;
    MEASURE_TYPE type;
    uint16_t samplingInterval;
}__COLLECTION_STATE;


typedef struct MRPP_STATE
{
    uint8_t groupId;
    uint8_t lastSubId;
    uint8_t nCollections;
    __COLLECTION_STATE collections[12];

} MRPP_STATE;





void mrpp_state_init(MRPP_STATE *state, uint8_t groupId, COLLECTION collections[], uint8_t nCollections);


#endif