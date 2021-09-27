
#ifndef MRPP_STATE_H
#define MRPP_STATE_H

#include <stdint.h>
#include <stdbool.h>

#define COLLECTION_DATA_META_SIZE 6
#define DR_BODY_PAYLOAD_SIZE 48
#define DR_SUBID_OVERHEAD 2
#define MAX_BODIES 30
#define DR_HEADER_COLLECTION_META_SIZE 4

typedef enum {
    T_INT8=1,
    T_INT16=2,
    T_FLOAT=4
} MEASURE_TYPE;

typedef enum {
    WAITING,
    READY,
    SENT,
    DONE,
} DR_STATUS;

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
    uint8_t beginsInBody;
    uint8_t endsInBody;
    DR_STATUS status;
}__COLLECTION_STATE;


typedef struct MRPP_STATE
{
    uint8_t groupId;
    uint8_t lastSubId;
    uint8_t nCollections;
    __COLLECTION_STATE collections[12];
    uint8_t nBodies;
    DR_STATUS bodies[MAX_BODIES];

} MRPP_STATE;





void mrpp_state_init(MRPP_STATE *state, uint8_t groupId, COLLECTION collections[], uint8_t nCollections);
uint8_t mrpp_state_get_header(MRPP_STATE *state, uint8_t package[]);
uint8_t mrpp_state_get_tail(MRPP_STATE *state, uint8_t package[]);
void mrpp_state_set_collection(MRPP_STATE *state, uint8_t collectionId, uint8_t timestamp[4], uint8_t metadata[6]);
bool mrpp_state_is_body_ready(MRPP_STATE *state);
void mrpp_state_get_body_address(MRPP_STATE *state, uint16_t *begin, uint8_t*length);
void mrpp_state_get_collection_address(MRPP_STATE *state, uint8_t collectionId, uint16_t *begin, uint16_t*length);
#endif