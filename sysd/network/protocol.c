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
#include <stdlib.h>
#include <sys/statvfs.h>
#include "protocol.h"

#include "../../libsysd/info.h"

char string_cpu_temp[] = {SYSD_START_BYTE_A,
                          SYSD_START_BYTE_B,
                          192,
                          168,
                          1,
                          24,
                          SYSD_CPU_TEMP,
                          SYSD_TYPE_FLOAT,
                          0x42,
                          0x05,
                          0x99,
                          0x9a,
                        };



int main () {

    printf("\nproto\n");

    printf("transmitting CPU temp: \n");




    return 0;
}
