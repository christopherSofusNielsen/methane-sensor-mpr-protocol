#include "../../../Unity/src/unity.h"
#include "../../../src/MRPP/util/mrpp_state.h"

void setUp(){}

void tearDown(){}

void test_init(){
    MRPP_STATE state;
    COLLECTION cols[2]={
        {.samplings=10, .type=T_INT16, .samplingInterval=30},
        {.samplings=20, .type=T_INT16, .samplingInterval=15}
    };
    mrpp_state_init(&state, 10, cols, 2);

    TEST_ASSERT_EQUAL_INT8(10, state.groupId);
    uint8_t lastSubId=(10*2+7+20*2+7)/48+1+2;
    TEST_ASSERT_EQUAL_INT16(lastSubId, state.lastSubId);

    TEST_ASSERT_EQUAL_INT8(1, state.collections[0].id);
    TEST_ASSERT_EQUAL_INT8(2, state.collections[1].id);
    TEST_ASSERT_EQUAL_INT16(0, state.collections[0].startIndex);
    TEST_ASSERT_EQUAL_INT16(10*2+7, state.collections[1].startIndex);
    TEST_ASSERT_EQUAL_INT16(10*2+7, state.collections[0].length);
    TEST_ASSERT_EQUAL_INT16(20*2+7, state.collections[1].length);
    TEST_ASSERT_EQUAL_INT16(30, state.collections[0].samplingInterval);
    TEST_ASSERT_EQUAL_INT16(15, state.collections[1].samplingInterval);
    
    //start and ending body
    TEST_ASSERT_EQUAL_INT8(0, state.collections[0].beginsInBody);
    TEST_ASSERT_EQUAL_INT8(0, state.collections[0].endsInBody);
    TEST_ASSERT_EQUAL_INT8(0, state.collections[1].beginsInBody);
    TEST_ASSERT_EQUAL_INT8(1, state.collections[1].endsInBody);

    //Collection status
    TEST_ASSERT_EQUAL(WAITING, state.collections[0].status);
    TEST_ASSERT_EQUAL(WAITING, state.collections[1].status);

    //Bodies
    TEST_ASSERT_EQUAL_INT8(2, state.nBodies);
    TEST_ASSERT_EACH_EQUAL_INT(WAITING, state.bodies, 2);
}

void test_get_header(){
    uint8_t test_package[]={
        0x00, //subId=0 
        0x03, //lastSubId=3
        0x00, 0x00, 0x00, 0x11, //startIndex=0, length=17
        0x00, 0x11,0x00, 0x13 //startIndex=17, length=19
        };

    MRPP_STATE state={
        .nCollections=2,
        .lastSubId=3,
        .collections={
            {.id=1, .startIndex=0, .length=10+7},
            {.id=2, .startIndex=10+7, .length=12+7}
        }
    };
    uint8_t package[51];
    uint8_t length=mrpp_state_get_header(&state, package);

    TEST_ASSERT_EQUAL_INT8(2+2*4, length);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(test_package, package, 10);
}


void test_get_tail(){
        MRPP_STATE state={
            .nCollections=2,
            .lastSubId=3,
            .collections={
                {.id=1, .startIndex=0, .length=10+7, },
                {.id=2, .startIndex=10+7, .length=12+7 }
            }
            };

         uint8_t test_package[]={
            0x03, //subId=3 
            0x03, //lastSubId=3
            0x00, 0x00, 0x00, 0x11, //startIndex=0, length=17
            0x00, 0x11,0x00, 0x13 //startIndex=17, length=19
            };

        uint8_t package[51];
        uint8_t length=mrpp_state_get_tail(&state, package);

        TEST_ASSERT_EQUAL_INT8(2+2*4, length);
        TEST_ASSERT_EQUAL_HEX8_ARRAY(test_package, package, 10);

}

void test_set_collection_1(){
     MRPP_STATE state={
            .nCollections=2,
            .lastSubId=3,
            .collections={
                {
                    .id=1, 
                    .startIndex=0, 
                    .length=10+7,
                    .type=T_INT16,
                    .samplingInterval=300,
                    .beginsInBody=0,
                    .endsInBody=0,
                    .status=WAITING
                },
                {
                    .id=2, 
                    .startIndex=10+7, 
                    .length=12+7,
                    .type=T_INT16,
                    .samplingInterval=40,
                    .beginsInBody=0,
                    .endsInBody=0,
                    .status=WAITING 
                }
            },
            .nBodies=1,
            .bodies={WAITING}
        };

        uint8_t timestamp[]={0x00, 0x01, 0x02, 0x03};
        uint8_t collectionId=1;
        uint8_t metadata[7];

        mrpp_state_set_collection(&state, collectionId, timestamp, metadata);
        
        //test ts
        TEST_ASSERT_EQUAL_INT8(DONE, state.collections[0].status);
        TEST_ASSERT_EQUAL_HEX8_ARRAY(timestamp, metadata, 4);
        
        //test sampling interval
        uint16_t samplingInterval=(uint16_t)metadata[4]<<8 | (uint16_t)metadata[5];
        TEST_ASSERT_EQUAL_INT16(state.collections[0].samplingInterval, samplingInterval);

        //test type
        TEST_ASSERT_EQUAL_INT8(state.collections[0].type, metadata[6]);

        //test body is updated
        TEST_ASSERT_EQUAL_INT8(WAITING, state.bodies[0]);

}

void test_set_collection_2(){
     MRPP_STATE state={
            .nCollections=2,
            .lastSubId=3,
            .collections={
                {
                    .id=1, 
                    .startIndex=0, 
                    .length=50+7,
                    .type=T_INT16,
                    .samplingInterval=300,
                    .beginsInBody=0,
                    .endsInBody=1,
                    .status=WAITING
                },
                {
                    .id=2, 
                    .startIndex=50+7, 
                    .length=40+7,
                    .type=T_INT16,
                    .samplingInterval=40,
                    .beginsInBody=1,
                    .endsInBody=3,
                    .status=WAITING 
                }
            },
            .nBodies=4,
            .bodies={WAITING, WAITING, WAITING, WAITING}
        };

        uint8_t timestamp[]={0x00, 0x01, 0x02, 0x03};
        uint8_t collectionId=2;
        uint8_t metadata[7];

        mrpp_state_set_collection(&state, collectionId, timestamp, metadata);
        
        //test ts
        TEST_ASSERT_EQUAL_INT8(DONE, state.collections[1].status);
        TEST_ASSERT_EQUAL_HEX8_ARRAY(timestamp, metadata, 4);
        
        //test sampling interval
        uint16_t samplingInterval=(uint16_t)metadata[4]<<8 | (uint16_t)metadata[5];
        TEST_ASSERT_EQUAL_INT16(state.collections[1].samplingInterval, samplingInterval);

        //test type
        TEST_ASSERT_EQUAL_INT8(state.collections[1].type, metadata[6]);

        //test body is updated
        TEST_ASSERT_EQUAL_INT8(WAITING, state.bodies[0]);
        TEST_ASSERT_EQUAL_INT8(WAITING, state.bodies[1]);
        TEST_ASSERT_EQUAL_INT8(READY, state.bodies[2]);
        TEST_ASSERT_EQUAL_INT8(READY, state.bodies[3]);

}

void test_set_collection_3(){
     MRPP_STATE state={
            .nCollections=3,
            .lastSubId=3,
            .collections={
                {
                    .id=1, 
                    .startIndex=0, 
                    .length=60+7,
                    .type=T_INT16,
                    .samplingInterval=300,
                    .beginsInBody=0,
                    .endsInBody=1,
                    .status=DONE
                },
                {
                    .id=2, 
                    .startIndex=60+7, 
                    .length=10+7,
                    .type=T_INT16,
                    .samplingInterval=40,
                    .beginsInBody=1,
                    .endsInBody=1,
                    .status=WAITING 
                },
                    {
                    .id=3, 
                    .startIndex=84, 
                    .length=70+7,
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
        uint8_t metadata[7];

        mrpp_state_set_collection(&state, collectionId, timestamp, metadata);
        
        //test ts
        TEST_ASSERT_EQUAL_INT8(DONE, state.collections[2].status);
        TEST_ASSERT_EQUAL_HEX8_ARRAY(timestamp, metadata, 4);
        
        //test sampling interval
        uint16_t samplingInterval=(uint16_t)metadata[4]<<8 | (uint16_t)metadata[5];
        TEST_ASSERT_EQUAL_INT16(state.collections[2].samplingInterval, samplingInterval);

        //test type
        TEST_ASSERT_EQUAL_INT8(state.collections[2].type, metadata[6]);

        //test body is updated
        TEST_ASSERT_EQUAL_INT8(READY, state.bodies[0]);
        TEST_ASSERT_EQUAL_INT8(WAITING, state.bodies[1]);
        TEST_ASSERT_EQUAL_INT8(READY, state.bodies[2]);
        TEST_ASSERT_EQUAL_INT8(READY, state.bodies[3]);

}

void test_set_collection_4(){
     MRPP_STATE state={
            .nCollections=3,
            .lastSubId=3,
            .collections={
                {
                    .id=1, 
                    .startIndex=0, 
                    .length=60+7,
                    .type=T_INT16,
                    .samplingInterval=300,
                    .beginsInBody=0,
                    .endsInBody=1,
                    .status=DONE
                },
                {
                    .id=2, 
                    .startIndex=60+7, 
                    .length=10+7,
                    .type=T_INT16,
                    .samplingInterval=40,
                    .beginsInBody=1,
                    .endsInBody=1,
                    .status=WAITING 
                },
                    {
                    .id=3, 
                    .startIndex=84, 
                    .length=70+7,
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
        
        //test ts
        TEST_ASSERT_EQUAL_INT8(DONE, state.collections[1].status);
        TEST_ASSERT_EQUAL_HEX8_ARRAY(timestamp, metadata, 4);
        
        //test sampling interval
        uint16_t samplingInterval=(uint16_t)metadata[4]<<8 | (uint16_t)metadata[5];
        TEST_ASSERT_EQUAL_INT16(state.collections[1].samplingInterval, samplingInterval);

        //test type
        TEST_ASSERT_EQUAL_INT8(state.collections[1].type, metadata[6]);

        //test body is updated
        TEST_ASSERT_EQUAL_INT8(READY, state.bodies[0]);
        TEST_ASSERT_EQUAL_INT8(READY, state.bodies[1]);
        TEST_ASSERT_EQUAL_INT8(READY, state.bodies[2]);
        TEST_ASSERT_EQUAL_INT8(READY, state.bodies[3]);

}





int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_init);
    RUN_TEST(test_get_header);
    RUN_TEST(test_get_tail);
    RUN_TEST(test_set_collection_1);
    RUN_TEST(test_set_collection_2);
    RUN_TEST(test_set_collection_3);
    RUN_TEST(test_set_collection_4);
    return UNITY_END();
}