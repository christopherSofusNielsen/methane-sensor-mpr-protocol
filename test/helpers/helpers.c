#include "helpers.h"

void load_array(uint8_t arr[], uint16_t begin, uint16_t length, uint8_t value){
    for (uint16_t i = begin; i < begin+length; i++)
    {
        arr[i]=value;
    }
}

void merge_array(uint8_t arr[], uint8_t data[], uint16_t begin, uint16_t length){
    for (uint16_t i = 0; i < length; i++)
    {
        arr[begin+i]=data[i];
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