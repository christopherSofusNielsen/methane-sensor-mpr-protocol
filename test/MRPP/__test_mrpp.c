#include "../../Unity/src/unity.h"
#include "../../src/MRPP/mrpp.h"
#include "../../src/MRPP/util/mrpp_state.h"
#include "../../src/MRPP/types.h"
#include "../helpers/helpers.h"

//MRPP_STATE _state;

void setUp(){}

void tearDown(){}

void test_init_group__group_id_count(){
    COLLECTION collections[]={
        {.samplingInterval=30, .samplings=300, .type=T_INT16},
        {.samplingInterval=500, .samplings=10, .type=T_INT16},
    };

    uint8_t groupId=MRPP_init_group(collections, 2, 1);
    TEST_ASSERT_EQUAL_UINT8(1, groupId);
    groupId=MRPP_init_group(collections, 2, 2);
    TEST_ASSERT_EQUAL_UINT8(2, groupId);
}

void test_init_group__state_init_1(){
    COLLECTION collections[]={
        {.samplingInterval=30, .samplings=30, .type=T_INT16},
        {.samplingInterval=120, .samplings=10, .type=T_INT16},
    };
    
    MRPP_init_group(collections, 2, 0);

    TEST_ASSERT_EQUAL_UINT8(0, state.groupId);
    TEST_ASSERT_EQUAL_UINT8(2, state.lastSubId);
    TEST_ASSERT_EQUAL_UINT8(2, state.nCollections);
    TEST_ASSERT_EQUAL_UINT8(1, state.nBodies);

    //Test bodies
    TEST_ASSERT_EACH_EQUAL_UINT8(WAITING, state.bodies, 1);

    //Test collections
    TEST_ASSERT_EQUAL_UINT16(0, state.collections[0].startIndex);
    TEST_ASSERT_EQUAL_UINT16(66, state.collections[0].length);
    TEST_ASSERT_EQUAL_UINT8(T_INT16, state.collections[0].type);
    TEST_ASSERT_EQUAL_UINT16(30, state.collections[0].samplingInterval);
    TEST_ASSERT_EQUAL_UINT8(0, state.collections[0].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(0, state.collections[0].endsInBody);
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[0].status);

    TEST_ASSERT_EQUAL_UINT16(66, state.collections[1].startIndex);
    TEST_ASSERT_EQUAL_UINT16(26, state.collections[1].length);
    TEST_ASSERT_EQUAL_UINT8(T_INT16, state.collections[1].type);
    TEST_ASSERT_EQUAL_UINT16(120, state.collections[1].samplingInterval);
    TEST_ASSERT_EQUAL_UINT8(0, state.collections[1].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(0, state.collections[1].endsInBody);
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[0].status);

}

void test_init_group__state_init_2(){
    COLLECTION collections[]={
        {.samplingInterval=30, .samplings=91, .type=T_INT8},
    };
    
    MRPP_init_group(collections, 1, 0);

    TEST_ASSERT_EQUAL_UINT8(0, state.groupId);
    TEST_ASSERT_EQUAL_UINT8(2, state.lastSubId);
    TEST_ASSERT_EQUAL_UINT8(1, state.nCollections);
    TEST_ASSERT_EQUAL_UINT8(1, state.nBodies);

    //Test bodies
    TEST_ASSERT_EACH_EQUAL_UINT8(WAITING, state.bodies, 1);

    //Test collections
    TEST_ASSERT_EQUAL_UINT16(0, state.collections[0].startIndex);
    TEST_ASSERT_EQUAL_UINT16(97, state.collections[0].length);
    TEST_ASSERT_EQUAL_UINT8(T_INT8, state.collections[0].type);
    TEST_ASSERT_EQUAL_UINT16(30, state.collections[0].samplingInterval);
    TEST_ASSERT_EQUAL_UINT8(0, state.collections[0].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(0, state.collections[0].endsInBody);
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[0].status);
}

void test_init_group__state_init_real_world(){
    COLLECTION collections[]={
        {.samplingInterval=60, .samplings=60, .type=T_INT16},
        {.samplingInterval=60, .samplings=10, .type=T_INT16},
        {.samplingInterval=60, .samplings=10, .type=T_INT16},
        {.samplingInterval=10, .samplings=30, .type=T_INT16},
        {.samplingInterval=10, .samplings=30, .type=T_INT16},
        {.samplingInterval=2, .samplings=150, .type=T_INT16},
        {.samplingInterval=60, .samplings=5, .type=T_INT16},
    };
    
    MRPP_init_group(collections, 7, 0);

    TEST_ASSERT_EQUAL_UINT8(0, state.groupId);
    TEST_ASSERT_EQUAL_UINT8(8, state.lastSubId);
    TEST_ASSERT_EQUAL_UINT8(7, state.nCollections);
    TEST_ASSERT_EQUAL_UINT8(7, state.nBodies);

    //Test bodies
    TEST_ASSERT_EACH_EQUAL_UINT8(WAITING, state.bodies, 17);

    //Test collections 0
    TEST_ASSERT_EQUAL_UINT8(0, state.collections[0].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(1, state.collections[0].endsInBody);
    TEST_ASSERT_EQUAL_UINT16(126, state.collections[0].length);
    TEST_ASSERT_EQUAL_UINT16(0, state.collections[0].startIndex);
    TEST_ASSERT_EQUAL_UINT16(60, state.collections[0].samplingInterval);
    TEST_ASSERT_EQUAL_UINT8(T_INT16, state.collections[0].type);
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[0].status);

    //Test collections 1
    TEST_ASSERT_EQUAL_UINT8(1, state.collections[1].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(1, state.collections[1].endsInBody);
    TEST_ASSERT_EQUAL_UINT16(26, state.collections[1].length);
    TEST_ASSERT_EQUAL_UINT16(126, state.collections[1].startIndex);
    TEST_ASSERT_EQUAL_UINT16(60, state.collections[1].samplingInterval);
    TEST_ASSERT_EQUAL_UINT8(T_INT16, state.collections[1].type);
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[1].status);

    //Test collections 2
    TEST_ASSERT_EQUAL_UINT8(1, state.collections[2].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(1, state.collections[2].endsInBody);
    TEST_ASSERT_EQUAL_UINT16(26, state.collections[2].length);
    TEST_ASSERT_EQUAL_UINT16(152, state.collections[2].startIndex);
    TEST_ASSERT_EQUAL_UINT16(60, state.collections[2].samplingInterval);
    TEST_ASSERT_EQUAL_UINT8(T_INT16, state.collections[2].type);
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[2].status);

    //Test collections 3
    TEST_ASSERT_EQUAL_UINT8(1, state.collections[3].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(2, state.collections[3].endsInBody);
    TEST_ASSERT_EQUAL_UINT16(66, state.collections[3].length);
    TEST_ASSERT_EQUAL_UINT16(178, state.collections[3].startIndex);
    TEST_ASSERT_EQUAL_UINT16(10, state.collections[3].samplingInterval);
    TEST_ASSERT_EQUAL_UINT8(T_INT16, state.collections[3].type);
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[3].status);

    //Test collections 4
    TEST_ASSERT_EQUAL_UINT8(2, state.collections[4].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(3, state.collections[4].endsInBody);
    TEST_ASSERT_EQUAL_UINT16(66, state.collections[4].length);
    TEST_ASSERT_EQUAL_UINT16(244, state.collections[4].startIndex);
    TEST_ASSERT_EQUAL_UINT16(10, state.collections[4].samplingInterval);
    TEST_ASSERT_EQUAL_UINT8(T_INT16, state.collections[4].type);
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[4].status);

    //Test collections 5
    TEST_ASSERT_EQUAL_UINT8(3, state.collections[5].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(6, state.collections[5].endsInBody);
    TEST_ASSERT_EQUAL_UINT16(306, state.collections[5].length);
    TEST_ASSERT_EQUAL_UINT16(310, state.collections[5].startIndex);
    TEST_ASSERT_EQUAL_UINT16(2, state.collections[5].samplingInterval);
    TEST_ASSERT_EQUAL_UINT8(T_INT16, state.collections[5].type);
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[5].status);

    //Test collections 6
    TEST_ASSERT_EQUAL_UINT8(6, state.collections[6].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(6, state.collections[6].endsInBody);
    TEST_ASSERT_EQUAL_UINT16(16, state.collections[6].length);
    TEST_ASSERT_EQUAL_UINT16(616, state.collections[6].startIndex);
    TEST_ASSERT_EQUAL_UINT16(60, state.collections[6].samplingInterval);
    TEST_ASSERT_EQUAL_UINT8(T_INT16, state.collections[6].type);
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[6].status);
}

void test_add_collection_data_INT16_1(){
    MRPP_STATE _state={
        .collections={
            {
                .beginsInBody=0,
                .endsInBody=0,
                .length=26,
                .startIndex=0,
                .type=T_INT16,
                .samplingInterval=300,
                .status=WAITING
            },
            {
                .beginsInBody=0,
                .endsInBody=1,
                .length=46,
                .startIndex=26,
                .type=T_INT16,
                .samplingInterval=15,
                .status=WAITING
            },
        },
        .nCollections=2,
        .groupId=0,
        .lastSubId=3,
        .nBodies=2,
        .bodies={WAITING, WAITING}
    };

    state=_state;

    uint8_t ts[]={0x00, 0x01, 0x02, 0x03};
    uint8_t si_test[]={0x1, 0x2c};
    uint16_t values[]={48834, 21224, 57007, 40076, 44966, 23420, 9378, 30260, 6366, 51168};
    uint8_t values_test[]={
        0xbe, 0xc2, 
        0x52, 0xe8, 
        0xde, 0xaf, 
        0x9c, 0x8c, 
        0xaf, 0xa6, 
        0x5b, 0x7c, 
        0x24, 0xa2, 
        0x76, 0x34, 
        0x18, 0xde, 
        0xc7, 0xe0
        };

    MRPP_add_collection_data_INT16(1, ts, values);

    //Check collection is updated
    TEST_ASSERT_EQUAL_UINT8(DONE, state.collections[0].status);

    //Check bodies are updated
    TEST_ASSERT_EACH_EQUAL_UINT8(WAITING, state.bodies, 2);

    //Check ts
    TEST_ASSERT_EQUAL_HEX8_ARRAY(ts, storage, 4);

    //Check sampling interval
    TEST_ASSERT_TRUE(test_array_range(si_test, 0, storage, 4, 2));

    //Check data
    TEST_ASSERT_TRUE(test_array_range(values_test, 0, storage, 6, 20));
}

void test_add_collection_data_INT16_2(){
    MRPP_STATE _state={
        .collections={
            {
                .beginsInBody=0,
                .endsInBody=0,
                .length=26,
                .startIndex=0,
                .type=T_INT16,
                .samplingInterval=300,
                .status=WAITING
            },
            {
                .beginsInBody=0,
                .endsInBody=1,
                .length=46,
                .startIndex=26,
                .type=T_INT16,
                .samplingInterval=15,
                .status=WAITING
            },
        },
        .nCollections=2,
        .groupId=0,
        .lastSubId=3,
        .nBodies=2,
        .bodies={WAITING, WAITING}
    };

    state=_state;

    uint8_t ts[]={0x00, 0x01, 0x02, 0x03};
    uint8_t si_test[]={0x0, 0xF};
    uint16_t values[]={37651, 40286, 23106, 31474, 48816, 9620, 10577, 5320, 53830, 53586, 40041, 43655, 39307, 59526, 41893, 8538, 48885, 18484, 52942, 43025};
    uint8_t values_test[]={
        0x93, 0x13, 
        0x9d, 0x5e, 
        0x5a, 0x42, 
        0x7a, 0xf2, 
        0xbe, 0xb0, 
        0x25, 0x94, 
        0x29, 0x51, 
        0x14, 0xc8, 
        0xd2, 0x46, 
        0xd1, 0x52, 
        0x9c, 0x69, 
        0xaa, 0x87, 
        0x99, 0x8b, 
        0xe8, 0x86, 
        0xa3, 0xa5, 
        0x21, 0x5a, 
        0xbe, 0xf5, 
        0x48, 0x34, 
        0xce, 0xce, 
        0xa8, 0x11
        };

    MRPP_add_collection_data_INT16(2, ts, values);

    //Check collection is updated
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[0].status);
    TEST_ASSERT_EQUAL_UINT8(DONE, state.collections[1].status);

    //Check bodies are updated
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.bodies[0]);
    TEST_ASSERT_EQUAL_UINT8(READY, state.bodies[1]);


    //Check ts
    TEST_ASSERT_TRUE(test_array_range(ts, 0, storage, 26, 4));

    //Check sampling interval
    TEST_ASSERT_TRUE(test_array_range(si_test, 0, storage, 4+26, 2));

    //Check data
    TEST_ASSERT_TRUE(test_array_range(values_test, 0, storage, 6+26, 40));
}

void test_is_body_package_ready_1(){
    MRPP_STATE _state={
        .nBodies=2,
        .bodies={WAITING, WAITING}
    };
    state=_state;
    int16_t bodyIndex;

    bool res=MRPP_is_body_package_ready(&bodyIndex);
    TEST_ASSERT_FALSE(res);
    TEST_ASSERT_EQUAL_INT16(-1, bodyIndex);
}

void test_is_body_package_ready_2(){
    MRPP_STATE _state={
        .nBodies=2,
        .bodies={SENT, READY}
    };
    state=_state;
    int16_t bodyIndex;

    bool res=MRPP_is_body_package_ready(&bodyIndex);
    TEST_ASSERT_TRUE(res);
    TEST_ASSERT_EQUAL_INT16(1, bodyIndex);
}

void test_get_ready_body_package_1(){
    MRPP_STATE _state={
        .collections={
            {
                .beginsInBody=0,
                .endsInBody=0,
                .length=26,
                .startIndex=0,
                .type=T_INT16,
                .samplingInterval=300,
                .status=DONE
            }
        },
        .nCollections=1,
        .groupId=0,
        .lastSubId=2,
        .nBodies=1,
        .bodies={READY}
    };
    state=_state;
    uint8_t package[51];
    uint8_t package_len;
    uint8_t ts[]={0x00, 0x01, 0x02, 0x03};
    uint8_t si_test[]={0x1, 0x2c};
    uint8_t values_test[]={
        0xbe, 0xc2, 
        0x52, 0xe8, 
        0xde, 0xaf, 
        0x9c, 0x8c, 
        0xaf, 0xa6, 
        0x5b, 0x7c, 
        0x24, 0xa2, 
        0x76, 0x34, 
        0x18, 0xde, 
        0xc7, 0xe0
        };
    merge_array(storage, ts, 0, 4);
    merge_array(storage, si_test, 4, 2);
    merge_array(storage, values_test, 6, 20);

    bool res=MRPP_get_ready_body_package(0, package,  &package_len);

    //Test res
    TEST_ASSERT_TRUE(res);

    //Test length
    TEST_ASSERT_EQUAL_UINT8(29, package_len);

    //Test group ID
    TEST_ASSERT_EQUAL_HEX8(0, package[0]);

    //TEST subId
    TEST_ASSERT_EQUAL_UINT8(1, package[1]);
    TEST_ASSERT_EQUAL_UINT8(2, package[2]);

    //Test data
    TEST_ASSERT_TRUE(test_array_range(ts, 0, package, 3, 4));
    TEST_ASSERT_TRUE(test_array_range(si_test, 0, package, 7, 2));
    TEST_ASSERT_TRUE(test_array_range(values_test, 0, package, 9, 20));
}

void test_get_ready_body_package_2(){
    MRPP_STATE _state={
        .collections={
            {
                .beginsInBody=0,
                .endsInBody=0,
                .length=97,
                .startIndex=0,
                .type=T_INT8,
                .samplingInterval=300,
                .status=WAITING
            },
            {
                .beginsInBody=0,
                .endsInBody=1,
                .length=46,
                .startIndex=97,
                .type=T_INT16,
                .samplingInterval=15,
                .status=DONE
            },
        },
        .nCollections=2,
        .groupId=1,
        .lastSubId=3,
        .nBodies=2,
        .bodies={WAITING, READY}
    };

    state=_state;
    uint8_t package[100];
    uint8_t package_len;
    uint8_t ts[]={0x00, 0x01, 0x02, 0x03};
    uint8_t si_test[]={0x0, 0xF};
    uint8_t values_test[]={
        0x93, 0x13, 
        0x9d, 0x5e, 
        0x5a, 0x42, 
        0x7a, 0xf2, 
        0xbe, 0xb0, 
        0x25, 0x94, 
        0x29, 0x51, 
        0x14, 0xc8, 
        0xd2, 0x46, 
        0xd1, 0x52, 
        0x9c, 0x69, 
        0xaa, 0x87, 
        0x99, 0x8b, 
        0xe8, 0x86, 
        0xa3, 0xa5, 
        0x21, 0x5a, 
        0xbe, 0xf5, 
        0x48, 0x34, 
        0xce, 0xce, 
        0xa8, 0x11
        };
  
    merge_array(storage, ts, 97, 4);
    merge_array(storage, si_test, 101, 2);
    merge_array(storage, values_test, 103, 40);

    bool res=MRPP_get_ready_body_package(1, package, &package_len);

    //Test res
    TEST_ASSERT_TRUE(res);

    //Test length
    //The last one (26+46)%48
    TEST_ASSERT_EQUAL_UINT8(49, package_len);

    //Test group ID
    TEST_ASSERT_EQUAL_HEX8(1, package[0]);

    //TEST subId
    TEST_ASSERT_EQUAL_UINT8(2, package[1]);
    TEST_ASSERT_EQUAL_UINT8(3, package[2]);

    //Test data in the end of body 0, therefore not tested
    TEST_ASSERT_TRUE(test_array_range(ts, 0, package, 3, 4));
    TEST_ASSERT_TRUE(test_array_range(si_test, 0, package, 7, 2));
    
    TEST_ASSERT_TRUE(test_array_range(values_test, 0, package, 9, 40));
}

void test_set_body_sent(){
    MRPP_STATE _state={
        .nBodies=2,
        .bodies={WAITING, READY}
    };

    MRPP_set_body_sent(1);

    TEST_ASSERT_EQUAL_INT8(SENT, state.bodies[1]);
}

void test_get_header_package_1(){
    MRPP_STATE _state={
        .collections={
            {
                .length=26,
                .startIndex=0,
                .nSamples=10,
                .type=T_INT16,
            },
            {
                .length=406,
                .startIndex=26,
                .nSamples=200,
                .type=T_INT16,
            },
        },
        .nCollections=2,
        .groupId=1,
        .lastSubId=6,
    };
    state=_state;

    uint8_t test_package[]={
        0x1,                    //GroupID
        0x0, 0x6,               //subId and lastSubId
        0x0,                    //Status bit
        0x2,                    //Collections
        0x0A,                   //Data types 
        10,
        200
    };

    uint8_t package[100];
    uint8_t package_len;

    MRPP_get_header_package(package, &package_len);

    TEST_ASSERT_EQUAL_UINT8(8, package_len);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(test_package, package, 8);
}

void test_get_tail_package_1(){
    MRPP_STATE _state={
        .collections={
            {
                .length=26,
                .startIndex=0,
                .nSamples=10,
                .type=T_INT16,
            },
            {
                .length=26,
                .startIndex=26,
                .nSamples=10,
                .type=T_INT16,
            },
             {
                .length=26,
                .startIndex=52,
                .nSamples=10,
                .type=T_INT16,
            },
             {
                .length=26,
                .startIndex=78,
                .nSamples=10,
                .type=T_INT16,
            },
             {
                .length=26,
                .startIndex=104,
                .nSamples=10,
                .type=T_INT16,
            },
        },
        .nCollections=5,
        .groupId=1,
        .lastSubId=3,
    };
    state=_state;

     uint8_t test_package[]={
        0x1,                    //GroupID
        0x3, 0x3,               //subId and lastSubId
        0x0,                    //Status bit
        0x5,                    //Collections
        0x02, 0xAA,                   //Data types 
        10,
        10,
        10,
        10,
        10,
    };

    uint8_t package[51];
    uint8_t package_len;

    MRPP_get_tail_package(package, &package_len);

    TEST_ASSERT_EQUAL_UINT8(12, package_len);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(test_package, package, 12);
}

void test_all_body_package_sent_1(){
    MRPP_STATE _state={
        .nBodies=4,
        .bodies={SENT, SENT, WAITING, WAITING}
    };
    state=_state;

    bool res=MRPP_all_body_package_sent();
    TEST_ASSERT_FALSE(res);
}

void test_all_body_package_sent_2(){
    MRPP_STATE _state={
        .nBodies=4,
        .bodies={SENT, SENT, SENT, SENT}
    };
    state=_state;

    bool res=MRPP_all_body_package_sent();
    TEST_ASSERT_TRUE(res);
}

void test_validate_collections_space_1(){
    COLLECTION collections[]={
        {.samplingInterval=30, .samplings=200, .type=T_INT16},
        {.samplingInterval=30, .samplings=388, .type=T_INT8},
    };

    bool res=MRPP_validate_collections_space(collections, 2);
    TEST_ASSERT_TRUE(res);
}

void test_validate_collections_space_2(){
    COLLECTION collections[]={
        {.samplingInterval=30, .samplings=200, .type=T_INT16},
        {.samplingInterval=30, .samplings=389, .type=T_INT8},
    };

    bool res=MRPP_validate_collections_space(collections, 2);
    TEST_ASSERT_FALSE(res);
}


int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_init_group__group_id_count);
    RUN_TEST(test_init_group__state_init_1);
    RUN_TEST(test_init_group__state_init_2);
    RUN_TEST(test_init_group__state_init_real_world);
    RUN_TEST(test_add_collection_data_INT16_1);
    RUN_TEST(test_add_collection_data_INT16_2);
    RUN_TEST(test_is_body_package_ready_1);
    RUN_TEST(test_is_body_package_ready_2);
    RUN_TEST(test_get_ready_body_package_1);
    RUN_TEST(test_get_ready_body_package_2);
    RUN_TEST(test_set_body_sent);
    RUN_TEST(test_get_header_package_1);
    RUN_TEST(test_get_tail_package_1);
    RUN_TEST(test_all_body_package_sent_1);
    RUN_TEST(test_all_body_package_sent_2);
    RUN_TEST(test_validate_collections_space_1);
    RUN_TEST(test_validate_collections_space_2);

    return UNITY_END();
}