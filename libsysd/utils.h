#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdbool.h>
#include <stdint.h>

#define SYSD_GB_SIZE (1024 * 1024 * 1024)
#define SYSD_MB_SIZE (1024 * 1024)
#define SYSD_KB_SIZE (1024)

#if (__SIZEOF_POINTER__) == 8

typedef uint32_t l_uint_t;
typedef int32_t  l_int_t;
typedef double   real_t;

#elif (__SIZEOF_POINTER__) == 4

typedef uint32_t l_uint_t;
typedef int32_t  l_int_t;
typedef float    real_t;

#endif

typedef struct {
    // error handling
    bool     last_error;
    int8_t   last_error_code;
    uint32_t error_count;
    uint32_t consecutive_error_count;
} sysd_error_t;

int msleep(int msec);

void sysd_pack_float(float input_val, uint8_t *output_array);

void sysd_pack_double(double input_val, uint8_t *output_array);

#endif
