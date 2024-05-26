#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdint.h>

#if (__SIZEOF_POINTER__) == 8

typedef uint32_t l_uint_t;
typedef int32_t l_int_t;
typedef double real_t;

#elif (__SIZEOF_POINTER__) == 4

typedef uint32_t l_uint_t;
typedef int32_t l_int_t;
typedef float real_t;

#endif



int msleep(int msec);

#endif
