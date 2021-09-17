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

}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_init);
    return UNITY_END();
}