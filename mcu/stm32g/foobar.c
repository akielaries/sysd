#include <stdint.h>

void wait() {
    for (uint32_t i = 0; i < 2000000; ++i)
        __asm__ volatile("nop");
}
