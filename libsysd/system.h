#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <stdint.h>

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
} sysd_telemetry_e;

/** @brief Struct for system telemetry function error codes */
typedef enum {
    SYSD_TEMP_ERROR = -1,
} sysd_api_error_codes_e;

/** @brief Struct for static CPU device info */
typedef struct {
    char   *cpu_model; // CPU model
    char   *hw_id;     // CPU hardware id
    uint8_t cpu_count; // CPU core count
} sysd_cpu_info_t;

/** @brief Struct for memory info */
typedef struct {
    float vram_total; // virtual RAM total
    float vram_used;  // virtual RAM used
    float vram_free;  // virtual RAM free
    float pram_total; // physical RAM total
    float pram_used;  // physical RAM used
    float pram_free;  // phsyical RAM free
} sysd_ram_info_t;

/** @brief Struct for memory info */
typedef struct {
    float storage_total; // storage total
    float storage_used;  // storage used
    float storage_free;  // storage free
} sysd_ssd_info_t;

/** @brief Struct for telemetry information */
typedef struct __attribute__((__packed__)) {
    sysd_cpu_info_t cpu_info;      // CPU static info
    double          cpu_load;      // CPU load
    float           cpu_temp;      // CPU current temp
    float           cpu_idle_temp; // CPU idle temp
    uint16_t        proc_count;    // process count
    sysd_ram_info_t ram_info;      // RAM info
    sysd_ssd_info_t ssd_info;      // storage info
} sysd_telemetry_t;

sysd_telemetry_t sysd_get_telemetry();

float sysd_cpu_temp();

sysd_telemetry_t sysd_get_telemetry();

#endif
