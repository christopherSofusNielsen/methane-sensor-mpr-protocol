
#ifndef MRPP_PARAMS_H
#define MRPP_PARAMS_H

/**
 * Length of buffer
 */
#define BODY_BUFFER_DATA_SIZE 800

/**
 * Maximum bodies
 * 
 */
#define MAX_BODIES 30


/**
 * Maximum payload size
 */
#define MAX_PAYLOAD_SIZE 100


/**
 * Constants, do not change
 */
#define DR_BODY_PAYLOAD_SIZE (MAX_PAYLOAD_SIZE-3)
#define DR_HEADER_COLLECTION_META_SIZE 4
#define DR_BODY_PACKAGE_META_SIZE 3
#define COLLECTION_DATA_META_SIZE 6


#endif