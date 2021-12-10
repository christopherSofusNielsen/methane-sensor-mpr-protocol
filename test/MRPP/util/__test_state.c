#include "../../../Unity/src/unity.h"
#include "../../../src/MRPP/util/mrpp_state.h"

#include <stdbool.h>

void setUp(){}

void tearDown(){}

/**
 * Test that setup is working
 */

void test_setup(){
    TEST_ASSERT_EQUAL_UINT8(1, 1);
}

/*
Test that all values is set correct, no limits test
*/
void test_init_1(){
    MRPP_STATE state;
    COLLECTION cols[2]={
        {.samplings=10, .type=T_INT16, .samplingInterval=30},
        {.samplings=20, .type=T_INT16, .samplingInterval=15}
    };
    mrpp_state_init(&state, 1, cols, 2);

    //uint8_t nBodies=((10*2+6)+(20*2+6)-1)/DR_BODY_PAYLOAD_SIZE+1

    TEST_ASSERT_EQUAL_UINT8(1, state.groupId);
    TEST_ASSERT_EQUAL_UINT8(2, state.lastSubId);
    TEST_ASSERT_EQUAL_UINT8(1, state.nBodies);
    TEST_ASSERT_EQUAL_UINT8(2, state.nCollections);

    //Test bodies
    TEST_ASSERT_EACH_EQUAL_UINT8(WAITING, state.bodies, 1);
   
    //Test collections
    TEST_ASSERT_EQUAL_UINT16(0, state.collections[0].startIndex);
    TEST_ASSERT_EQUAL_UINT16(26, state.collections[0].length);
    TEST_ASSERT_EQUAL_UINT8(T_INT16, state.collections[0].type);
    TEST_ASSERT_EQUAL_UINT16(30, state.collections[0].samplingInterval);
    TEST_ASSERT_EQUAL_UINT8(0, state.collections[0].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(0, state.collections[0].endsInBody);
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[0].status);
    TEST_ASSERT_EQUAL_UINT8(10, state.collections[0].nSamples);

    TEST_ASSERT_EQUAL_UINT16(26, state.collections[1].startIndex);
    TEST_ASSERT_EQUAL_UINT16(46, state.collections[1].length);
    TEST_ASSERT_EQUAL_UINT8(T_INT16, state.collections[1].type);
    TEST_ASSERT_EQUAL_UINT16(15, state.collections[1].samplingInterval);
    TEST_ASSERT_EQUAL_UINT8(0, state.collections[1].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(0, state.collections[1].endsInBody);
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[1].status);
    TEST_ASSERT_EQUAL_UINT8(20, state.collections[1].nSamples);

}
/**
 * Test limits
 * 
 */
void test_init_2(){
    MRPP_STATE state;
    COLLECTION cols[2]={
        {.samplings=91, .type=T_INT8, .samplingInterval=30},
        {.samplings=91, .type=T_INT8, .samplingInterval=30},
    };
    mrpp_state_init(&state, 10, cols, 2);

    TEST_ASSERT_EQUAL_UINT8(10, state.groupId);
    TEST_ASSERT_EQUAL_UINT8(3, state.lastSubId);
    TEST_ASSERT_EQUAL_UINT8(2, state.nBodies);
    TEST_ASSERT_EQUAL_UINT8(2, state.nCollections);

    //Test bodies
    TEST_ASSERT_EACH_EQUAL_UINT8(WAITING, state.bodies, 2);

    //Test collection 1
    TEST_ASSERT_EQUAL_UINT16(0, state.collections[0].startIndex);
    TEST_ASSERT_EQUAL_UINT16(97, state.collections[0].length);
    TEST_ASSERT_EQUAL_UINT8(T_INT8, state.collections[0].type);
    TEST_ASSERT_EQUAL_UINT16(30, state.collections[0].samplingInterval);
    TEST_ASSERT_EQUAL_UINT8(0, state.collections[0].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(0, state.collections[0].endsInBody);
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[0].status);
    TEST_ASSERT_EQUAL_UINT8(91, state.collections[0].nSamples);



    //Test collection 2
    TEST_ASSERT_EQUAL_UINT16(97, state.collections[1].startIndex);
    TEST_ASSERT_EQUAL_UINT16(97, state.collections[1].length);
    TEST_ASSERT_EQUAL_UINT8(T_INT8, state.collections[1].type);
    TEST_ASSERT_EQUAL_UINT16(30, state.collections[1].samplingInterval);
    TEST_ASSERT_EQUAL_UINT8(1, state.collections[1].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(1, state.collections[1].endsInBody);
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[1].status);
    TEST_ASSERT_EQUAL_UINT8(91, state.collections[1].nSamples);

}

/**
 * Test that header is created  correct
 * 
 */
void test_get_header_1(){
    uint8_t test_package[]={
        0x0, 0x2, //id & last id 
        0x0, //status bit
        0x2, //N col
        0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, //Data types 
        0x0, 0x0, 0x0, 0x1a, 
        0x0, 0x1a, 0x0, 0x1a
    };
    MRPP_STATE state={
        .nCollections=2,
        .lastSubId=2,
        .collections={
            {.startIndex=0, .length=26, .type=T_INT16}, //samples=10
            {.startIndex=26, .length=26, .type=T_INT16} //samples=10
        }
    };
    uint8_t package[100];
    uint8_t length=mrpp_state_get_header(&state, package);

    TEST_ASSERT_EQUAL_INT8(18, length);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(test_package, package, 18);
}

void test_get_header_2(){
    uint8_t test_package[]={
        0x0, 0x3, //id & last id 
        0x0, //status bit
        0x8, //N col
        0x00, 0x00, 0x00, 0x00, 0x55, 0xAA, //Data types 
        0x0, 0x0, 0x0, 0x1a, 
        0x0, 0x1a, 0x0, 0x1a,
        0x0, 0x34, 0x0, 0x1a,
        0x0, 0x4E, 0x0, 0x1a,
        0x0, 0x68, 0x0, 0x1a,
        0x0, 0x82, 0x0, 0x1a,
        0x0, 0x9C, 0x0, 0x1a,
        0x0, 0xB6, 0x0, 0x1a,
    };
    MRPP_STATE state={
        .nCollections=8,
        .lastSubId=3,
        .collections={
            {.startIndex=0, .length=26, .type=T_INT16}, //samples=10
            {.startIndex=26, .length=26, .type=T_INT16}, //samples=10
            {.startIndex=52, .length=26, .type=T_INT16}, //samples=10
            {.startIndex=78, .length=26, .type=T_INT16}, //samples=10
            {.startIndex=104, .length=26, .type=T_INT8}, //samples=20
            {.startIndex=130, .length=26, .type=T_INT8}, //samples=20
            {.startIndex=156, .length=26, .type=T_INT8}, //samples=20
            {.startIndex=182, .length=26, .type=T_INT8} //samples=20
        }
    };
    uint8_t package[100];
    uint8_t length=mrpp_state_get_header(&state, package);

    TEST_ASSERT_EQUAL_INT8(42, length);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(test_package, package, 42);
}


void test_set_collection_1(){
     MRPP_STATE state={
            .nCollections=2,
            .lastSubId=3,
            .collections={
                {
                    .startIndex=0, 
                    .length=86,
                    .type=T_INT16,
                    .samplingInterval=30,
                    .beginsInBody=0,
                    .endsInBody=0,
                    .status=WAITING
                },
                {
                    .startIndex=86, 
                    .length=46,
                    .type=T_INT16,
                    .samplingInterval=15,
                    .beginsInBody=0,
                    .endsInBody=1,
                    .status=WAITING 
                }
            },
            .nBodies=2,
            .bodies={WAITING, WAITING}
        };

        uint8_t timestamp[]={0x00, 0x01, 0x02, 0x03};
        uint8_t collectionId=1;
        uint8_t metadata[6];

        mrpp_state_set_collection(&state, collectionId, timestamp, metadata);
        
        TEST_ASSERT_EQUAL_INT8(DONE, state.collections[0].status);
        
        //test ts
        TEST_ASSERT_EQUAL_HEX8_ARRAY(timestamp, metadata, 4);
        
        //test sampling interval
        uint16_t samplingInterval=(uint16_t)metadata[4]<<8 | (uint16_t)metadata[5];
        TEST_ASSERT_EQUAL_INT16(state.collections[0].samplingInterval, samplingInterval);

        //test body is updated
        TEST_ASSERT_EQUAL_INT8(WAITING, state.bodies[0]);

}

void test_set_collection_2(){
     MRPP_STATE state={
            .nCollections=2,
            .lastSubId=4,
            .collections={
                {
                    .startIndex=0, 
                    .length=126,
                    .type=T_INT16,
                    .samplingInterval=300,
                    .beginsInBody=0,
                    .endsInBody=1,
                    .status=WAITING
                },
                {
                    .startIndex=126, 
                    .length=126,
                    .type=T_INT16,
                    .samplingInterval=40,
                    .beginsInBody=1,
                    .endsInBody=2,
                    .status=WAITING 
                }
            },
            .nBodies=3,
            .bodies={WAITING, WAITING, WAITING}
        };

        uint8_t timestamp[]={0x00, 0x01, 0x02, 0x03};
        uint8_t collectionId=2;
        uint8_t metadata[6];

        mrpp_state_set_collection(&state, collectionId, timestamp, metadata);
        
        TEST_ASSERT_EQUAL_INT8(DONE, state.collections[1].status);
        
        //test ts
        TEST_ASSERT_EQUAL_HEX8_ARRAY(timestamp, metadata, 4);
        
        //test sampling interval
        uint16_t samplingInterval=(uint16_t)metadata[4]<<8 | (uint16_t)metadata[5];
        TEST_ASSERT_EQUAL_INT16(state.collections[1].samplingInterval, samplingInterval);


        //test body is updated
        TEST_ASSERT_EQUAL_INT8(WAITING, state.bodies[0]);
        TEST_ASSERT_EQUAL_INT8(WAITING, state.bodies[1]);
        TEST_ASSERT_EQUAL_INT8(READY, state.bodies[2]);
}

void test_set_collection_3(){
     MRPP_STATE state={
            .nCollections=3,
            .lastSubId=5,
            .collections={
                { 
                    .startIndex=0, 
                    .length=126,
                    .type=T_INT16,
                    .samplingInterval=300,
                    .beginsInBody=0,
                    .endsInBody=1,
                    .status=DONE
                },
                { 
                    .startIndex=126, 
                    .length=26,
                    .type=T_INT16,
                    .samplingInterval=40,
                    .beginsInBody=1,
                    .endsInBody=1,
                    .status=WAITING 
                },
                    {
                    .startIndex=152, 
                    .length=206,
                    .type=T_INT16,
                    .samplingInterval=600,
                    .beginsInBody=1,
                    .endsInBody=3,
                    .status=WAITING 
                }
            },
            .nBodies=4,
            .bodies={READY, WAITING, WAITING, WAITING}
        };

        uint8_t timestamp[]={0x00, 0x01, 0x02, 0x03};
        uint8_t collectionId=3;
        uint8_t metadata[6];

        mrpp_state_set_collection(&state, collectionId, timestamp, metadata);
        
        TEST_ASSERT_EQUAL_INT8(DONE, state.collections[2].status);
        
        //test ts
        TEST_ASSERT_EQUAL_HEX8_ARRAY(timestamp, metadata, 4);
        
        //test sampling interval
        uint16_t samplingInterval=(uint16_t)metadata[4]<<8 | (uint16_t)metadata[5];
        TEST_ASSERT_EQUAL_INT16(state.collections[2].samplingInterval, samplingInterval);


        //test body is updated
        TEST_ASSERT_EQUAL_INT8(READY, state.bodies[0]);
        TEST_ASSERT_EQUAL_INT8(WAITING, state.bodies[1]);
        TEST_ASSERT_EQUAL_INT8(READY, state.bodies[2]);
        TEST_ASSERT_EQUAL_INT8(READY, state.bodies[3]);

}

void test_set_collection_4(){
     MRPP_STATE state={
            .nCollections=3,
            .lastSubId=5,
            .collections={
                { 
                    .startIndex=0, 
                    .length=126,
                    .type=T_INT16,
                    .samplingInterval=300,
                    .beginsInBody=0,
                    .endsInBody=1,
                    .status=DONE
                },
                { 
                    .startIndex=126, 
                    .length=26,
                    .type=T_INT16,
                    .samplingInterval=40,
                    .beginsInBody=1,
                    .endsInBody=1,
                    .status=WAITING 
                },
                    {
                    .startIndex=152, 
                    .length=206,
                    .type=T_INT16,
                    .samplingInterval=600,
                    .beginsInBody=1,
                    .endsInBody=3,
                    .status=DONE 
                }
            },
            .nBodies=4,
            .bodies={READY, WAITING, READY, READY}
        };

        uint8_t timestamp[]={0x00, 0x01, 0x02, 0x03};
        uint8_t collectionId=2;
        uint8_t metadata[6];

        mrpp_state_set_collection(&state, collectionId, timestamp, metadata);
        
        TEST_ASSERT_EQUAL_INT8(DONE, state.collections[1].status);
        
        //test ts
        TEST_ASSERT_EQUAL_HEX8_ARRAY(timestamp, metadata, 4);
        
        //test sampling interval
        uint16_t samplingInterval=(uint16_t)metadata[4]<<8 | (uint16_t)metadata[5];
        TEST_ASSERT_EQUAL_INT16(state.collections[1].samplingInterval, samplingInterval);


        //test body is updated
        TEST_ASSERT_EQUAL_INT8(READY, state.bodies[0]);
        TEST_ASSERT_EQUAL_INT8(READY, state.bodies[1]);
        TEST_ASSERT_EQUAL_INT8(READY, state.bodies[2]);
        TEST_ASSERT_EQUAL_INT8(READY, state.bodies[3]);

}

void test_get_body_info_1(){
     MRPP_STATE state={
            .nCollections=3,
            .lastSubId=5,
            .collections={
                 { 
                    .startIndex=0, 
                    .length=126,
                    .type=T_INT16,
                    .samplingInterval=300,
                    .beginsInBody=0,
                    .endsInBody=1,
                    .status=DONE
                },
                { 
                    .startIndex=126, 
                    .length=26,
                    .type=T_INT16,
                    .samplingInterval=40,
                    .beginsInBody=1,
                    .endsInBody=1,
                    .status=WAITING 
                },
                    {
                    .startIndex=152, 
                    .length=206,
                    .type=T_INT16,
                    .samplingInterval=600,
                    .beginsInBody=1,
                    .endsInBody=3,
                    .status=DONE 
                }
            },
            .nBodies=4,
            .bodies={READY, WAITING, WAITING, WAITING}
        };

    uint8_t subId;
    uint8_t lastSubId;
    uint16_t begin;
    uint8_t len;

    mrpp_state_get_ready_body(&state, 0, &subId, &lastSubId, &begin, &len);

    TEST_ASSERT_EQUAL_INT8(1, subId);
    TEST_ASSERT_EQUAL_INT8(5, lastSubId);
    TEST_ASSERT_EQUAL_INT16(0, begin);
    TEST_ASSERT_EQUAL_INT8(97, len);
}



int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_setup);
    RUN_TEST(test_init_1);
    RUN_TEST(test_init_2);
    RUN_TEST(test_get_header_1);
    RUN_TEST(test_get_header_2);
    RUN_TEST(test_set_collection_1);
    RUN_TEST(test_set_collection_2);
    RUN_TEST(test_set_collection_3);
    RUN_TEST(test_set_collection_4);
    RUN_TEST(test_get_body_info_1);
    return UNITY_END();
}