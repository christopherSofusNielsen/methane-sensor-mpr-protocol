#include "../../../Unity/src/unity.h"
#include "../../../src/MRPP/util/mrpp_state.h"

#include <stdbool.h>

void setUp(){}

void tearDown(){}

void test_init_1(){
    MRPP_STATE state;
    COLLECTION cols[2]={
        {.samplings=10, .type=T_INT16, .samplingInterval=30},
        {.samplings=20, .type=T_INT16, .samplingInterval=15}
    };
    mrpp_state_init(&state, 1, cols, 2);

    TEST_ASSERT_EQUAL_UINT8(1, state.groupId);
    TEST_ASSERT_EQUAL_UINT8(3, state.lastSubId);
    TEST_ASSERT_EQUAL_UINT8(2, state.nBodies);
    TEST_ASSERT_EQUAL_UINT8(2, state.nCollections);

    //Test bodies
    TEST_ASSERT_EACH_EQUAL_UINT8(WAITING, state.bodies, 2);
   
    //Test collections
    TEST_ASSERT_EQUAL_UINT16(0, state.collections[0].startIndex);
    TEST_ASSERT_EQUAL_UINT16(26, state.collections[0].length);
    TEST_ASSERT_EQUAL_UINT8(T_INT16, state.collections[0].type);
    TEST_ASSERT_EQUAL_UINT16(30, state.collections[0].samplingInterval);
    TEST_ASSERT_EQUAL_UINT8(0, state.collections[0].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(0, state.collections[0].endsInBody);
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[0].status);

    TEST_ASSERT_EQUAL_UINT16(26, state.collections[1].startIndex);
    TEST_ASSERT_EQUAL_UINT16(46, state.collections[1].length);
    TEST_ASSERT_EQUAL_UINT8(T_INT16, state.collections[1].type);
    TEST_ASSERT_EQUAL_UINT16(15, state.collections[1].samplingInterval);
    TEST_ASSERT_EQUAL_UINT8(0, state.collections[1].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(1, state.collections[1].endsInBody);
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[1].status);
}

void test_init_2(){
    MRPP_STATE state;
    COLLECTION cols[2]={
        {.samplings=42, .type=T_INT8, .samplingInterval=30},
    };
    mrpp_state_init(&state, 10, cols, 1);

    TEST_ASSERT_EQUAL_UINT8(10, state.groupId);
    TEST_ASSERT_EQUAL_UINT8(2, state.lastSubId);
    TEST_ASSERT_EQUAL_UINT8(1, state.nBodies);
    TEST_ASSERT_EQUAL_UINT8(1, state.nCollections);

    //Test bodies
    TEST_ASSERT_EACH_EQUAL_UINT8(WAITING, state.bodies, 1);

    //Test collections
    TEST_ASSERT_EQUAL_UINT16(0, state.collections[0].startIndex);
    TEST_ASSERT_EQUAL_UINT16(48, state.collections[0].length);
    TEST_ASSERT_EQUAL_UINT8(T_INT8, state.collections[0].type);
    TEST_ASSERT_EQUAL_UINT16(30, state.collections[0].samplingInterval);
    TEST_ASSERT_EQUAL_UINT8(0, state.collections[0].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(0, state.collections[0].endsInBody);
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[0].status);
}

void test_init_3(){
    MRPP_STATE state;
    COLLECTION cols[2]={
        {.samplings=20, .type=T_INT16, .samplingInterval=15},
        {.samplings=44, .type=T_INT8, .samplingInterval=15}
    };
    mrpp_state_init(&state, 1, cols, 2);

    TEST_ASSERT_EQUAL_UINT8(1, state.groupId);
    TEST_ASSERT_EQUAL_UINT8(3, state.lastSubId);
    TEST_ASSERT_EQUAL_UINT8(2, state.nBodies);
    TEST_ASSERT_EQUAL_UINT8(2, state.nCollections);

    //Test bodies
    TEST_ASSERT_EACH_EQUAL_UINT8(WAITING, state.bodies, 2);
   
    //Test collections
    TEST_ASSERT_EQUAL_UINT16(0, state.collections[0].startIndex);
    TEST_ASSERT_EQUAL_UINT16(46, state.collections[0].length);
    TEST_ASSERT_EQUAL_UINT8(T_INT16, state.collections[0].type);
    TEST_ASSERT_EQUAL_UINT16(15, state.collections[0].samplingInterval);
    TEST_ASSERT_EQUAL_UINT8(0, state.collections[0].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(0, state.collections[0].endsInBody);
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[0].status);

    TEST_ASSERT_EQUAL_UINT16(46, state.collections[1].startIndex);
    TEST_ASSERT_EQUAL_UINT16(50, state.collections[1].length);
    TEST_ASSERT_EQUAL_UINT8(T_INT8, state.collections[1].type);
    TEST_ASSERT_EQUAL_UINT16(15, state.collections[1].samplingInterval);
    TEST_ASSERT_EQUAL_UINT8(0, state.collections[1].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(1, state.collections[1].endsInBody);
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[1].status);
}

void test_get_header_1(){
    uint8_t test_package[]={
        0x0, 0x3, 
        0x0, 
        0x0, 0x0, 0xa, 
        0x0, 0x0, 0x0, 0x1a, 
        0x0, 0x1a, 0x0, 0x2e
    };
    MRPP_STATE state={
        .nCollections=2,
        .lastSubId=3,
        .collections={
            {.startIndex=0, .length=26, .type=T_INT16},
            {.startIndex=26, .length=46, .type=T_INT16}
        }
    };
    uint8_t package[51];
    uint8_t length=mrpp_state_get_header(&state, package);

    TEST_ASSERT_EQUAL_INT8(14, length);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(test_package, package, 14);
}

void test_get_header_2(){
    uint8_t test_package[]={
        0x0, 0x10, 
        0x0, 
        0x0, 0xf, 0xa5, 
        0x0, 0x0, 0x1, 0x96, 
        0x1, 0x96, 0x0, 0x1a, 
        0x1, 0xb0, 0x0, 0x2e, 
        0x1, 0xde, 0x0, 0x2e, 
        0x2, 0xc, 0x0, 0x56, 
        0x2, 0x62, 0x0, 0x56
    };
    MRPP_STATE state={
        .nCollections=6,
        .lastSubId=16,
        .collections={
            {.startIndex=0, .length=406, .type=T_INT8},
            {.startIndex=406, .length=26, .type=T_INT8},
            {.startIndex=432, .length=46, .type=T_INT16},
            {.startIndex=478, .length=46, .type=T_INT16},
            {.startIndex=524, .length=86, .type=T_FLOAT},
            {.startIndex=610, .length=86, .type=T_FLOAT},
        }
    };
    uint8_t package[51];
    uint8_t length=mrpp_state_get_header(&state, package);

    TEST_ASSERT_EQUAL_INT8(30, length);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(test_package, package, 30);
}


void test_get_tail_1(){

        uint8_t test_package[]={
                0x3, 0x3, 
                0x0, 
                0x0, 0x0, 0xa, 
                0x0, 0x0, 0x0, 0x1a, 
                0x0, 0x1a, 0x0, 0x2e
            };

        MRPP_STATE state={
                .nCollections=2,
                .lastSubId=3,
                .collections={
                    { .startIndex=0, .length=26, .type=T_INT16},
                    { .startIndex=26, .length=46, .type=T_INT16}
                }
            };

        uint8_t package[51];
        uint8_t length=mrpp_state_get_tail(&state, package);

        TEST_ASSERT_EQUAL_INT8(14, length);
        TEST_ASSERT_EQUAL_HEX8_ARRAY(test_package, package, 14);

}

void test_set_collection_1(){
     MRPP_STATE state={
            .nCollections=2,
            .lastSubId=3,
            .collections={
                {
                    .startIndex=0, 
                    .length=26,
                    .type=T_INT16,
                    .samplingInterval=30,
                    .beginsInBody=0,
                    .endsInBody=0,
                    .status=WAITING
                },
                {
                    .startIndex=26, 
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
                    .length=56,
                    .type=T_INT16,
                    .samplingInterval=300,
                    .beginsInBody=0,
                    .endsInBody=1,
                    .status=WAITING
                },
                {
                    .startIndex=56, 
                    .length=66,
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
                    .length=66,
                    .type=T_INT16,
                    .samplingInterval=300,
                    .beginsInBody=0,
                    .endsInBody=1,
                    .status=DONE
                },
                { 
                    .startIndex=66, 
                    .length=16,
                    .type=T_INT16,
                    .samplingInterval=40,
                    .beginsInBody=1,
                    .endsInBody=1,
                    .status=WAITING 
                },
                    {
                    .startIndex=82, 
                    .length=76,
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
                    .length=66,
                    .type=T_INT16,
                    .samplingInterval=300,
                    .beginsInBody=0,
                    .endsInBody=1,
                    .status=DONE
                },
                { 
                    .startIndex=66, 
                    .length=16,
                    .type=T_INT16,
                    .samplingInterval=40,
                    .beginsInBody=1,
                    .endsInBody=1,
                    .status=WAITING 
                },
                    {
                    .startIndex=82, 
                    .length=76,
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
        uint8_t metadata[7];

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

void test_is_body_ready(){
    MRPP_STATE state={
        .nBodies=3,
        .bodies={WAITING, WAITING, WAITING}
    };

    bool isReady=mrpp_state_is_body_ready(&state);
    TEST_ASSERT_FALSE(isReady);

    state.bodies[0]=READY;
    isReady=mrpp_state_is_body_ready(&state);
    TEST_ASSERT_TRUE(isReady);

}

void test_get_body_info_1(){
     MRPP_STATE state={
            .nCollections=3,
            .lastSubId=5,
            .collections={
                { 
                    .startIndex=0, 
                    .length=66,
                    .type=T_INT16,
                    .samplingInterval=300,
                    .beginsInBody=0,
                    .endsInBody=1,
                    .status=DONE
                },
                { 
                    .startIndex=66, 
                    .length=16,
                    .type=T_INT16,
                    .samplingInterval=40,
                    .beginsInBody=1,
                    .endsInBody=1,
                    .status=WAITING 
                },
                    {
                    .startIndex=82, 
                    .length=76,
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

    uint8_t subId;
    uint8_t lastSubId;
    uint16_t begin;
    uint8_t len;

    mrpp_state_get_ready_body(&state, &subId, &lastSubId, &begin, &len);

    TEST_ASSERT_EQUAL_INT8(1, subId);
    TEST_ASSERT_EQUAL_INT8(5, lastSubId);
    TEST_ASSERT_EQUAL_INT16(0, begin);
    TEST_ASSERT_EQUAL_INT8(48, len);

    //Make sure bodies is updated
    TEST_ASSERT_EQUAL_INT8(SENT, state.bodies[0]);
}

void test_get_body_info_2(){
     MRPP_STATE state={
            .nCollections=3,
            .lastSubId=5,
            .collections={
                { 
                    .startIndex=0, 
                    .length=66,
                    .type=T_INT16,
                    .samplingInterval=300,
                    .beginsInBody=0,
                    .endsInBody=1,
                    .status=DONE
                },
                { 
                    .startIndex=66, 
                    .length=16,
                    .type=T_INT16,
                    .samplingInterval=40,
                    .beginsInBody=1,
                    .endsInBody=1,
                    .status=DONE 
                },
                    {
                    .startIndex=82, 
                    .length=76,
                    .type=T_INT16,
                    .samplingInterval=600,
                    .beginsInBody=1,
                    .endsInBody=3,
                    .status=DONE 
                }
            },
            .nBodies=4,
            .bodies={SENT, SENT, SENT, READY}
        };

    uint8_t subId;
    uint8_t lastSubId;
    uint16_t begin;
    uint8_t len;

    mrpp_state_get_ready_body(&state, &subId, &lastSubId, &begin, &len);

    TEST_ASSERT_EQUAL_INT8(4, subId);
    TEST_ASSERT_EQUAL_INT8(5, lastSubId);
    TEST_ASSERT_EQUAL_INT16(144, begin);
    TEST_ASSERT_EQUAL_INT8(14, len);

    //Make sure bodies is updated
    TEST_ASSERT_EQUAL_INT8(SENT, state.bodies[3]);
}



void test_get_collection_address_1(){
     MRPP_STATE state={
        .nCollections=2,
        .collections={
            {
                .startIndex=0,
                .length=86
            },
            {
                .startIndex=86,
                .length=46
            }
        },
    };


    uint16_t begin;
    uint16_t length;

    mrpp_state_get_collection_address(&state, 1, &begin, &length);
    TEST_ASSERT_EQUAL_INT16(0, begin);
    TEST_ASSERT_EQUAL_INT16(86, length);

    mrpp_state_get_collection_address(&state, 2, &begin, &length);
    TEST_ASSERT_EQUAL_INT16(86, begin);
    TEST_ASSERT_EQUAL_INT16(46, length);

}







int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_init_1);
    RUN_TEST(test_init_2);
    RUN_TEST(test_init_3);
    RUN_TEST(test_get_header_1);
    RUN_TEST(test_get_header_2);
    RUN_TEST(test_get_tail_1);
    RUN_TEST(test_set_collection_1);
    RUN_TEST(test_set_collection_2);
    RUN_TEST(test_set_collection_3);
    RUN_TEST(test_set_collection_4);
    RUN_TEST(test_is_body_ready);
    RUN_TEST(test_get_body_info_1);
    RUN_TEST(test_get_body_info_2);
    RUN_TEST(test_get_collection_address_1);
    return UNITY_END();
}