#include "libsysd/config.h"
#include <stddef.h>
#include <stdio.h>

extern "C" int LLVMFuzzerTestOneInput(const char *filename, 
                                      struct Config *cfg) {

    parse(filename, cfg);
    return 0;
}
