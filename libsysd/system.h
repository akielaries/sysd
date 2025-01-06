#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <stdint.h>

#define MAX_MODEL_LEN 64
#define MAX_HW_ID_LEN 32

/** @brief Enumeration of telemetry codes */
typedef enum {
  SYSD_CPU_MODEL  = 0x01, // CPU model
  SYSD_CPU_HW_ID  = 0x02, // CPU hardware ID
  SYSD_CPU_COUNT  = 0x03, // core count
  SYSD_CPU_USAGE  = 0x04, // usage
  SYSD_CPU_LOAD   = 0x05, // load
  SYSD_CPU_TEMP   = 0x06, // temp
  SYSD_PROC_COUNT = 0x07, // process count
  SYSD_VRAM_TOTAL = 0x08, // virtual RAM total
  SYSD_VRAM_USED  = 0x09, // virtual RAM used
  SYSD_VRAM_FREE  = 0x0A, // virtual RAM free
  SYSD_PRAM_TOTAL = 0x0B, // physical RAM total
  SYSD_PRAM_USED  = 0x0C, // physical RAM used
  SYSD_PRAM_FREE  = 0x0D, // phsyical RAM free
  SYSD_STRG_TOTAL = 0x0E, // storage total
  SYSD_STRG_USED  = 0x0F, // storage used
  SYSD_STRG_FREE  = 0x10, // storage free
  //SYSD_NUM_TELEMETRY_CODES = SYSD_STRG_FREE - 1,  // total number of telemetry codes
  SYSD_NUM_TELEMETRY_CODES,  // total number of telemetry codes
} sysd_telemetry_e;

/** @brief Struct for system telemetry function error codes */
typedef enum {
  SYSD_TEMP_ERROR = -1,
} sysd_api_error_codes_e;

/** @brief Struct for static CPU device info */
typedef struct {
  char cpu_model[MAX_MODEL_LEN]; // CPU model
  char hw_id[MAX_HW_ID_LEN];     // CPU hardware id
  uint8_t cpu_count;             // CPU core count
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

/** @brief Struct for telemetry information. packed to remove padding in memory */
typedef struct __attribute__((packed)){
  sysd_cpu_info_t cpu_info; // CPU static info
  float cpu_load;           // CPU load
  float cpu_temp;           // CPU current temp
  float cpu_idle_temp;      // CPU idle temp
  uint16_t proc_count;      // process count
  sysd_ram_info_t ram_info; // RAM info
  sysd_ssd_info_t ssd_info; // storage info
} sysd_telemetry_t;


/**
 * @brief get telemetry from the device like number of running processes, 
 * CPU temp, etc
 *
 * @return populated sysd_telemetry_t pointer with system telemetry
 */
sysd_telemetry_t sysd_get_telemetry();

float sysd_cpu_temp();

#endif
