#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdint.h>

#define MAX_LINE_LENGTH 80

typedef enum {
    CPU_TEMP    = 0x01, // CPU temperature in C
    CPU_USAGE   = 0x02, // CPU usage %
    PROC_COUNT  = 0x03, // running process count
    
    VMEM_TOTAL  = 0x04, // virtual memory total
    VMEM_USED   = 0x05, // virtual memory used
    VMEM_FREE   = 0x06, // virtual memory free
    
    PMEM_TOTAL  = 0x07, // physical memory total
    PMEM_USED   = 0x08, // physical memory used
    PMEM_FREE   = 0x09, // physical memory free

    PROC_ID     = 0x10, // processor ID
    DEV_NAME    = 0x11, // device name 
    NUM_CPUS    = 0x12, // number of CPU cores

} telemetry_ids;

typedef enum {
    START_BIT_A = 0xFF,
    START_BIT_B = 0xFE,

} transport_header;

typedef struct Config {
    char *IPV4_SUB;
    int PORT;
    char *LOG_DIR;
    int I2C_LCD;
} Config;

void parse(const char *filename, struct Config *cfg);

/**
 * @brief encodes a telemetry item for transport
 *
 * For example to publish information about CPU temperature, the following
 * message is constructed and published:
 *
 * | 0xFF 0xFE 0x08 0x01 | 0x0A 0xD7 0x0A 0x42 | 0x66 0x51 0x68 0xDA |
 *      HEADER     TELEMID  34.71C in hex(f32)      unix time in hex
 *
 * vMemory free:
 * | 0xFF 0xFE 0x07 0x06 | 0x4C 0xDF 0x37 | 0x66 0x51 0x68 0xDA |
 *      HEADER    TELEMID  5037879 in hex     unix time in hex
 * 
 */
int encode(uint8_t telem_id, float val, uint64_t time);

int foo(const char *str);

#endif
