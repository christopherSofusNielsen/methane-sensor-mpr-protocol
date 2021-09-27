#include "../../../Unity/src/unity.h"
#include "../../../src/MRPP/util/mrpp_data.h"

#include <stdbool.h>


bool test_array_range(uint8_t expect[], uint16_t expect_from, uint8_t actual[], uint16_t actual_from, uint16_t num_elements);
void load_array(uint8_t arr[], uint16_t begin, uint16_t length, uint8_t value);

void setUp(){}

void tearDown(){}

void test_add_int16(){
    uint8_t metadata[]={0x00, 0x01, 0x02,0x03,0x04,0x05};
    uint16_t values[]={1000, 100, 2002};
    uint8_t values_test[]={0x03, 0xE8, 0x00, 0x64, 0x07, 0xD2};
    uint16_t begin=0;
    uint16_t length=12;

    mrpp_data_add_int16(metadata, values, begin, length);
    TEST_ASSERT_TRUE(test_array_range(metadata, 0, storage, begin, 6));
    TEST_ASSERT_TRUE(test_array_range(values_test, 0, storage, begin+6, 6));
}

void test_get_1(){
    uint8_t data[50];
    uint8_t data_test[]={0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};
    load_array(storage, 0, 10, 0xBB);
    load_array(storage, 10, 13, 0xAA);

    
    mrpp_data_get(data, 10, 13);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(data_test, data, 13);
}

void test_get_2(){
    uint8_t data[50];
    uint8_t data_test[]={0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};
    load_array(storage, 0, 13, 0xAA);

    
    mrpp_data_get(data, 0, 13);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(data_test, data, 13);
}

void load_array(uint8_t arr[], uint16_t begin, uint16_t length, uint8_t value){
    for (uint16_t i = begin; i < begin+length; i++)
    {
        arr[i]=value;
    }
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
    RUN_TEST(test_get_1);
    RUN_TEST(test_get_2);
    return UNITY_END();
}