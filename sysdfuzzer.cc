//#include "libsysd/config.h"
#include "libsysd/info.h"
#include <stddef.h>
#include <stdio.h>

/*extern "C" int LLVMFuzzerTestOneInput(const char *filename, 
                                      struct Config *cfg) {

    parse(filename, cfg);
    return 0;
}*/

extern "C" int LLVMFuzzerTestOneInput(struct System *sys) {
    cpu_info(sys);


    return 0;
}
