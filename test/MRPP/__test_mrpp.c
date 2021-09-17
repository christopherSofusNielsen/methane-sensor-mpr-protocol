#include "../../Unity/src/unity.h"
#include "../../src/MRPP/mrpp.h"

void setUp(){}

void tearDown(){}

void test_payload_ready(){
    bool res=payload_ready();
    TEST_ASSERT_FALSE_MESSAGE(res, "Test 1");
    TEST_ASSERT_TRUE_MESSAGE(res, "TEST 2");
}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_payload_ready);
    return UNITY_END();
}