#ifndef __INFO_H__
#define __INFO_H__

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

/** @brief Struct for telemetry information */
typedef struct __attribute__((__packed__)) {
    char        *cpu_model;     // CPU model
    char        *hardware;      // CPU hardware name
    uint64_t    cpu_count;      // CPU core count
    float       cpu_load;       // CPU load
    float       cpu_temp;       // CPU current temp
    float       cpu_idle_temp;  // CPU idle temp
    uint64_t    vram_total;     // virtual RAM total
    uint64_t    vram_used;      // virtual RAM used
    uint64_t    vram_free;      // virtual RAM free
    uint64_t    pram_total;     // physical RAM total
    uint64_t    pram_used;      // physical RAM used
    uint64_t    pram_free;      // phsyical RAM free
    float       storage_total;  // storage total
    float       storage_used;   // storage used
    float       storage_free;   // storage free
} sysd_telemetry_t;

char *read_file(const char *filename);

int ps_count();

void cpu_usage();

void cpu_info(struct System *sys);

double cpu_temp();

double cpu_idle_temp();

void cpu_idle(double idle_temp);

double cpu_load();

void mem_info();

void mem_stats(struct System *sys);

#ifdef __HAS_NVCC__
/* If CUDA capable device */

/* NVIDIA GPU INFO */
struct GPUInfo {
    /* GPU name */
    char name[256];
    /* CUDA driver version */
    int nvd_driver_version;
    /* CUDA runtime version */
    int cuda_version;
    int major;
    int minor;
    /* total memory in KB */
    float total_glbl_mem;
    /* GPU count */
    int gpu_mp_count;
    /* CUDA GPU cores */
    int cuda_cores;
    /* max GPU clock rate in GHz */
    float max_clock_rt;
    /* total GPU memory */
    size_t total_const_mem;
    /* shared memory / block */
    size_t shared_mem_pb;
    /* max threads per GPU */
    int max_mp_threads;
    /* max thread per GPU block */
    int max_pb_threads;
    /* GPU threads MAX dimension */
    int max_dim_threads[3];
    /* GPU MAX grid size */
    int max_grid_size[3];
    size_t gpu_mem_total;
    size_t gpu_mem_used;
    size_t gpu_mem_free;
};

#endif

#endif
