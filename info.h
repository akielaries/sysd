#ifndef __INFO_H__
#define __INFO_H__

#include <stdint.h>

typedef struct System {
    /* CPU/PROC info */
    char cpu_model[256]; // proc model
    double bogus_mips;   // proc speed
    int num_proc;        // # of processors

    /* Idle CPU temp set in cpu_idle_temp() */
    double cpu_temp_idle;
    float cpu_use;

    /* VIRTUAL MEMORY IN KB */
    uint64_t v_mem_total;
    uint64_t v_mem_used;
    uint64_t v_mem_free;

    /* PHYSICAL MEMORY IN KB */
    uint64_t p_mem_total;
    uint64_t p_mem_used;
    uint64_t p_mem_free;
} System;

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

int has_nvidia_gpu();

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
