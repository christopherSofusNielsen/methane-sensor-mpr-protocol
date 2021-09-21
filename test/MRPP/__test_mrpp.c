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
    TEST_ASSERT_EQUAL_UINT16(67, state.collections[0].length);
    TEST_ASSERT_EQUAL_UINT8(T_INT16, state.collections[0].type);
    TEST_ASSERT_EQUAL_UINT16(30, state.collections[0].samplingInterval);
    TEST_ASSERT_EQUAL_UINT8(0, state.collections[0].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(1, state.collections[0].endsInBody);
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[0].status);

    TEST_ASSERT_EQUAL_UINT16(67, state.collections[1].startIndex);
    TEST_ASSERT_EQUAL_UINT16(27, state.collections[1].length);
    TEST_ASSERT_EQUAL_UINT8(T_INT16, state.collections[1].type);
    TEST_ASSERT_EQUAL_UINT16(120, state.collections[1].samplingInterval);
    TEST_ASSERT_EQUAL_UINT8(1, state.collections[1].beginsInBody);
    TEST_ASSERT_EQUAL_UINT8(1, state.collections[1].endsInBody);
    TEST_ASSERT_EQUAL_UINT8(WAITING, state.collections[1].status);

}

void test_init_group__state_init_2(){
    COLLECTION collections[]={
        {.samplingInterval=30, .samplings=41, .type=T_INT8},
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



int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_init_group__group_id_count);
    RUN_TEST(test_init_group__state_init_1);
    RUN_TEST(test_init_group__state_init_2);
    return UNITY_END();
}