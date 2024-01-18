#include "libsysd/config.h"
//#include "libsysd/info.h"
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

/*extern "C" int LLVMFuzzerTestOneInput(const char *filename) {
    struct Config cfg;
    parse(filename, &cfg);
    return 0;
}*/
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Call the vulnerable function with the input data
  foo(reinterpret_cast<const char*>(data));
  return 0;
}

/*extern "C" int LLVMFuzzerTestOneInput(struct System *sys) {
    cpu_info(sys);


    return 0;
}*/
