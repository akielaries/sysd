#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <stddef.h>
#include <stdint.h>

#define BUFF_SZ 1024

#define SYSD_START_BYTE_A 0xBA
#define SYSD_START_BYTE_B 0xAB

/** @brief Enumeration of telemetry codes */
typedef enum {
    SYSD_CPU_INFO   = 0x01, // manufacturer info
    SYSD_CPU_COUNT  = 0x02, // core count
    SYSD_CPU_USAGE  = 0x03, // usage
    SYSD_CPU_LOAD   = 0x04, // load
    SYSD_CPU_TEMP   = 0x05, // temp
    SYSD_PROC_COUNT = 0x06, // process count
    SYSD_VRAM_TOTAL = 0x07, // virtual RAM total
    SYSD_VRAM_USED  = 0x08, // virtual RAM used
    SYSD_VRAM_FREE  = 0x09, // virtual RAM free
    SYSD_PRAM_TOTAL = 0x10, // physical RAM total
    SYSD_PRAM_USED  = 0x11, // physical RAM used
    SYSD_PRAM_FREE  = 0x12, // phsyical RAM 
    SYSD_STRG_TOTAL = 0x13, // storage total
    SYSD_STRG_USED  = 0x14, // storage used
    SYSD_STRG_FREE  = 0x15, // storage free
} sysd_telemetry_e;

/** @brief Enumeration of data type codes */
typedef enum {
    SYSD_TYPE_FLOAT     = 0x01,
    SYSD_TYPE_DOUBLE    = 0x02,
    SYSD_TYPE_INT8      = 0x03,
    SYSD_TYPE_UINT8     = 0x04,
    SYSD_TYPE_INT16     = 0x05,
    SYSD_TYPE_UINT16    = 0x06,
    SYSD_TYPE_INT32     = 0x07,
    SYSD_TYPE_UINT32    = 0x08,
    SYSD_TYPE_INT64     = 0x09,
    SYSD_TYPE_UINT64    = 0x10,
} sysd_datatypes_e;

/** @brief Enumeration for frame byte offsets */
typedef enum {
    SYSD_OFFSET_START_BYTE_A    = 0,
    SYSD_OFFSET_START_BYTE_B    = 1,
    SYSD_OFFSET_DEST_IPV4       = 2,
    SYSD_OFFSET_TELEM_CODE      = 3,
    SYSD_OFFSET_DATA_TYPE_CODE  = 4,
} sysd_offets_e;

/** @brief Enumeration for sizes */
typedef enum {
    SYSD_GB_SIZE = (1024 * 1024 * 1024),
    SYSD_MB_SIZE = (1024 * 1024),



} sysd_sizes_e;

/** @brief Struct for message frames */
typedef struct  {
    uint8_t type;
    uint16_t len;
    char *buffer;
} sysd_frame_t;

int conn_init(uint16_t port);

int conn_dest(const int sock);

/**
 * @brief function to publish data
 */
// void publish(const int sock, const char *message);
void publish(const int sock, uint8_t type, size_t len, const void *val);

/**
 * @brief function to retrieve data
 */
void subscribe(const char *ip_address, const uint16_t port);

#endif
