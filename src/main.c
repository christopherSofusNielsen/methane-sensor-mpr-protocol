#include <stdio.h>
#include "MRPP/mrpp.h"
#include "MRPP/util/mrpp_state.h"

int main(int argc, char const *argv[])
{
    payload_ready();
    mrpp_state_init();
    return 0;
}
