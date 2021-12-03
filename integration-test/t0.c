#include <stdio.h>

#include "util/util.h"

#include "../src/MRPP/mrpp.h"
#include "../src/MRPP/types.h"


COLLECTION collections[]={
    {.samplingInterval=2, .samplings=10, .type=T_INT16}
};

uint8_t fake_ts[]={0xAA, 0xBB, 0xCC, 0xDD};
char str[400]={'\0'};

int main(int argc, char const *argv[])
{

    MRPP_init_group(collections, 1, 0);
    uint16_t data[10];
    util_fill_data(data, 0xABCD, 10 );

    MRPP_add_collection_data_INT16(1, fake_ts, data);

    int16_t bodyIndex;
    if(!MRPP_is_body_package_ready(&bodyIndex)){
        printf("ERROR, bodyindex not ready\n");
        return 0;
    }

    uint8_t package[51];
    uint8_t package_length;

    //Create
    FILE *fp;
    fp=fopen("output/t0.txt", "w");
    

    //header
    MRPP_get_header_package(package, &package_length);
    util_2_hex_str(str, package, package_length);
    printf("%s\n", str);
    fputs(str, fp);
    fputs("\n", fp);


    //body
    MRPP_get_ready_body_package(bodyIndex, package, &package_length);
    util_2_hex_str(str, package, package_length);
    printf("%s\n", str);
    fputs(str, fp);
    fputs("\n", fp);

    //tail
    MRPP_get_tail_package(package, &package_length);
    util_2_hex_str(str, package, package_length);
    printf("%s\n", str);
    fputs(str, fp);
    fputs("\n", fp);

    fclose(fp);

    return 0;
}
