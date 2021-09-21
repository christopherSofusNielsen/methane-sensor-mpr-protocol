#include "../../../Unity/src/unity.h"
#include "../../../src/MRPP/util/mrpp_data.h"

#include <stdbool.h>


bool test_array_range(uint8_t expect[], uint16_t expect_from, uint8_t actual[], uint16_t actual_from, uint16_t num_elements);

void setUp(){}

void tearDown(){}

void test_add_int16(){
    uint8_t metadata[]={0x00, 0x01, 0x02,0x03,0x04,0x05,0x06};
    uint16_t values[]={1000, 100, 2002};
    uint8_t values_test[]={0x03, 0xE8, 0x00, 0x64, 0x07, 0xD2};
    uint16_t begin=0;
    uint16_t length=13;

    mrpp_data_add_int16(metadata, values, begin, length);
    TEST_ASSERT_TRUE(test_array_range(metadata, 0, storage, begin, 7));
    TEST_ASSERT_TRUE(test_array_range(values_test, 0, storage, begin+7, 6));
}

bool test_array_range(uint8_t expect[], uint16_t expect_from, uint8_t actual[], uint16_t actual_from, uint16_t num_elements){
    for (uint16_t i = 0; i < num_elements; i++)
    {
        if(expect[expect_from+i]!=actual[actual_from+i]){
        printf("Expected: 0x%02X, Was 0x%02X at index=%u\n",expect[expect_from+i],actual[actual_from+i], i );
        return false;
        }
    }
}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_add_int16);
    return UNITY_END();
}