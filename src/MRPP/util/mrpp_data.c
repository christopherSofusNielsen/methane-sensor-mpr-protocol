#include "mrpp_data.h"

uint8_t storage[800];

void mrpp_data_add_int16(uint8_t metadata[7], uint16_t values[], uint16_t begin, uint16_t length){
    
    //add meta
    for (uint8_t i = begin; i < begin+7; i++)
    {
        storage[i]=metadata[i-begin];
    }
    length-=7;

    //add values  
    uint16_t valuesLength=length/2;
    const uint16_t offset_MSB=begin+7;
    const uint16_t offset_LSB=begin+8;
    for (uint16_t i = 0; i < valuesLength; i++)
    {
        storage[offset_MSB + i*2]=values[i]>>8;
        storage[offset_LSB + i*2]=values[i];
    }

}
