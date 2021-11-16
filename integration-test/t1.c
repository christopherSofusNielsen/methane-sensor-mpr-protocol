#include <stdio.h>

#include "util/util.h"

#include "../src/MRPP/mrpp.h"
#include "../src/MRPP/types.h"


COLLECTION collections[]={
    {.samplingInterval=2, .samplings=2, .type=T_INT16},
    {.samplingInterval=2, .samplings=2, .type=T_INT16},
    {.samplingInterval=2, .samplings=2, .type=T_INT16},
    {.samplingInterval=2, .samplings=2, .type=T_INT16},
    {.samplingInterval=2, .samplings=2, .type=T_INT16},
};

uint8_t fake_ts[]={0xFF, 0xFF, 0xFF, 0xFF};


int main(int argc, char const *argv[])
{

    MRPP_init_group(collections, 5, 0);
    uint16_t data[30];
    
    //Add data for collection 1
    util_fill_data(data, 0xAAAA, 2 );
    MRPP_add_collection_data_INT16(1, fake_ts, data);

    //Add data for collection 2
    util_fill_data(data, 0xBBBB, 2 );
    MRPP_add_collection_data_INT16(2, fake_ts, data);

    //Add data for collection 3
    util_fill_data(data, 0xCCCC, 2 );
    MRPP_add_collection_data_INT16(3, fake_ts, data);

    //Add data for collection 4
    util_fill_data(data, 0xDDDD, 2 );
    MRPP_add_collection_data_INT16(4, fake_ts, data);

    //Add data for collection 5
    util_fill_data(data, 0xEEEE, 2 );
    MRPP_add_collection_data_INT16(5, fake_ts, data);


    //Setup variables to use
    char str[300]={'\0'};
    int16_t bodyIndex;
    uint8_t package[51];
    uint8_t package_length;
    FILE *fp;
    fp=fopen("output/t1.txt", "w");

    //Write header
    MRPP_get_header_package(package, &package_length);
    util_2_hex_str(str, package, package_length);
    printf("%s\n", str);
    fputs(str, fp);
    fputs("\n", fp);


    //Wite bodies as long they are available

    while (MRPP_is_body_package_ready(&bodyIndex))
    {

        //Write body
        MRPP_get_ready_body_package(bodyIndex, package, &package_length);
        util_2_hex_str(str, package, package_length);
        printf("%s\n", str);
        fputs(str, fp);
        fputs("\n", fp);

        //Set package as sent
        MRPP_set_body_sent(bodyIndex);
    }
    

    //Make sure all packages is done
    if(!MRPP_all_body_package_sent()){
        printf("ERROR, not all bodies is sent");
        return 0;
    }
  

    //Write tail
    MRPP_get_tail_package(package, &package_length);
    util_2_hex_str(str, package, package_length);
    printf("%s\n", str);
    fputs(str, fp);
    fputs("\n", fp);

    fclose(fp);

    return 0;
}
