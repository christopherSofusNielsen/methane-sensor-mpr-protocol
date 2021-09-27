#include "../../Unity/src/unity.h"
#include "../../src/MRPP/mrpp.h"
#include "../../src/MRPP/util/mrpp_state.h"

MRPP_STATE _state;

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



int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_init_group__group_id_count);
    RUN_TEST(test_init_group__state_init_1);
    RUN_TEST(test_init_group__state_init_2);
    RUN_TEST(test_init_group__state_init_real_world);
    return UNITY_END();
}