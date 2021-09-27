#include "../../Unity/src/unity.h"
#include "../../src/MRPP/mrpp.h"
#include "../../src/MRPP/util/mrpp_state.h"
#include "../helpers/helpers.h"

//MRPP_STATE _state;

void setUp(){
    _groupId=0;
}

void tearDown(){}

void test_init_group__group_id_count(){
    COLLECTION collections[]={
        {.samplingInterval=30, .samplings=300, .type=T_INT16},
        {.samplingInterval=500, .samplings=10, .type=T_INT16},
    };

    MRPP_init_group(collections, 2);
    TEST_ASSERT_EQUAL_UINT8(1, _groupId);
    MRPP_init_group(collections, 2);
    TEST_ASSERT_EQUAL_UINT8(2, _groupId);
}

void test_init_group__state_init_1(){
    COLLECTION collections[]={
        {.samplingInterval=30, .samplings=30, .type=T_INT16},
        {.samplingInterval=120, .samplings=10, .type=T_INT16},
    };
    
    MRPP_init_group(collections, 2);

    TEST_ASSERT_EQUAL_UINT8(0, state.groupId);
    TEST_ASSERT_EQUAL_UINT8(3, state.lastSubId);
    TEST_ASSERT_EQUAL_UINT8(2, state.nCollections);
    TEST_ASSERT_EQUAL_UINT8(2, state.nBodies);

    //Test bodies
    TEST_ASSERT_EACH_EQUAL_UINT8(WAITING, state.bodies, 2);

    //Test collections
    TEST_ASSERT_EQUAL_UINT16(0, state.collections[0].startIndex);
    TEST_ASSERT_EQUAL_UINT16(66, state.collections[0].length);
    TEST_ASSERT_EQUAL_UINT8(T_INT16, state.collections[0].type);
    TEST_ASSERT_EQUAL_UINT16(30, state.collections[0].samplingInterval);
    TEST_ASSERT_EQUAL_UINT8(0, state.collections[0].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(1, state.collections[0].endsInBody);
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[0].status);

    TEST_ASSERT_EQUAL_UINT16(66, state.collections[1].startIndex);
    TEST_ASSERT_EQUAL_UINT16(26, state.collections[1].length);
    TEST_ASSERT_EQUAL_UINT8(T_INT16, state.collections[1].type);
    TEST_ASSERT_EQUAL_UINT16(120, state.collections[1].samplingInterval);
    TEST_ASSERT_EQUAL_UINT8(1, state.collections[1].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(1, state.collections[1].endsInBody);
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[1].status);

}

void test_init_group__state_init_2(){
    COLLECTION collections[]={
        {.samplingInterval=30, .samplings=42, .type=T_INT8},
    };
    
    MRPP_init_group(collections, 1);

    TEST_ASSERT_EQUAL_UINT8(0, state.groupId);
    TEST_ASSERT_EQUAL_UINT8(2, state.lastSubId);
    TEST_ASSERT_EQUAL_UINT8(1, state.nCollections);
    TEST_ASSERT_EQUAL_UINT8(1, state.nBodies);

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
    
    MRPP_init_group(collections, 7);

    TEST_ASSERT_EQUAL_UINT8(0, state.groupId);
    TEST_ASSERT_EQUAL_UINT8(15, state.lastSubId);
    TEST_ASSERT_EQUAL_UINT8(7, state.nCollections);
    TEST_ASSERT_EQUAL_UINT8(14, state.nBodies);

    //Test bodies
    TEST_ASSERT_EACH_EQUAL_UINT8(WAITING, state.bodies, 14);

    //Test collections 0
    TEST_ASSERT_EQUAL_UINT8(0, state.collections[0].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(2, state.collections[0].endsInBody);
    TEST_ASSERT_EQUAL_UINT16(126, state.collections[0].length);
    TEST_ASSERT_EQUAL_UINT16(0, state.collections[0].startIndex);
    TEST_ASSERT_EQUAL_UINT16(60, state.collections[0].samplingInterval);
    TEST_ASSERT_EQUAL_UINT8(T_INT16, state.collections[0].type);
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[0].status);

    //Test collections 1
    TEST_ASSERT_EQUAL_UINT8(2, state.collections[1].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(3, state.collections[1].endsInBody);
    TEST_ASSERT_EQUAL_UINT16(26, state.collections[1].length);
    TEST_ASSERT_EQUAL_UINT16(126, state.collections[1].startIndex);
    TEST_ASSERT_EQUAL_UINT16(60, state.collections[1].samplingInterval);
    TEST_ASSERT_EQUAL_UINT8(T_INT16, state.collections[1].type);
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[1].status);

    //Test collections 2
    TEST_ASSERT_EQUAL_UINT8(3, state.collections[2].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(3, state.collections[2].endsInBody);
    TEST_ASSERT_EQUAL_UINT16(26, state.collections[2].length);
    TEST_ASSERT_EQUAL_UINT16(152, state.collections[2].startIndex);
    TEST_ASSERT_EQUAL_UINT16(60, state.collections[2].samplingInterval);
    TEST_ASSERT_EQUAL_UINT8(T_INT16, state.collections[2].type);
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[2].status);

    //Test collections 3
    TEST_ASSERT_EQUAL_UINT8(3, state.collections[3].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(5, state.collections[3].endsInBody);
    TEST_ASSERT_EQUAL_UINT16(66, state.collections[3].length);
    TEST_ASSERT_EQUAL_UINT16(178, state.collections[3].startIndex);
    TEST_ASSERT_EQUAL_UINT16(10, state.collections[3].samplingInterval);
    TEST_ASSERT_EQUAL_UINT8(T_INT16, state.collections[3].type);
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[3].status);

    //Test collections 4
    TEST_ASSERT_EQUAL_UINT8(5, state.collections[4].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(6, state.collections[4].endsInBody);
    TEST_ASSERT_EQUAL_UINT16(66, state.collections[4].length);
    TEST_ASSERT_EQUAL_UINT16(244, state.collections[4].startIndex);
    TEST_ASSERT_EQUAL_UINT16(10, state.collections[4].samplingInterval);
    TEST_ASSERT_EQUAL_UINT8(T_INT16, state.collections[4].type);
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[4].status);

    //Test collections 5
    TEST_ASSERT_EQUAL_UINT8(6, state.collections[5].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(12, state.collections[5].endsInBody);
    TEST_ASSERT_EQUAL_UINT16(306, state.collections[5].length);
    TEST_ASSERT_EQUAL_UINT16(310, state.collections[5].startIndex);
    TEST_ASSERT_EQUAL_UINT16(2, state.collections[5].samplingInterval);
    TEST_ASSERT_EQUAL_UINT8(T_INT16, state.collections[5].type);
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[5].status);

    //Test collections 6
    TEST_ASSERT_EQUAL_UINT8(12, state.collections[6].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(13, state.collections[6].endsInBody);
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



int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_init_group__group_id_count);
    RUN_TEST(test_init_group__state_init_1);
    RUN_TEST(test_init_group__state_init_2);
    RUN_TEST(test_init_group__state_init_real_world);
    RUN_TEST(test_add_collection_data_INT16_1);
    RUN_TEST(test_add_collection_data_INT16_2);
    return UNITY_END();
}