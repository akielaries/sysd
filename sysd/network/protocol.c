/**
 * sysd frame structure:
 *
 * | OFFSET | ITEM             | SIZE BYTES |
 * | ------ | ---------------- | ---------- |
 * | 0      | start byte A     | 1          |
 * | 1      | start byte B     | 1          |
 * | 2      | destination IPv4 | 4          |
 * | 3      | telemetry code   | 1          |
 * | 4      | data type code   | 1          |
 * | 5-69   | data             | 8-64       |
 */
#include <stdio.h>
#include "protocol.h"

char string_cpu_temp[] = {SYSD_START_BYTE_A,
                          SYSD_START_BYTE_B,
                          192,
                          168,
                          1,
                          24,
                          SYSD_CPU_TEMP,
                          SYSD_TYPE_FLOAT,
                        };

int main () {
    printf("proto\n");

    printf("transmitting CPU temp: \n");


    return 0;
}
