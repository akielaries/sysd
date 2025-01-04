#ifndef PROTO_H
#define PROTO_H

#include "proto_pubsub.h"

/**
 * packets look like the following:
 * 
 * byte   offset   marker
 * ----- HEADER -----
 * 0        0     start byte A
 * 1        1     start byte B
 * 2        2     destination IP [0]
 * 3        3     destination IP [1]
 * 4        4     destination IP [2]
 * 5        5     destination IP [3]
 * 6        6     payload size
 * -----  BODY  -----
 * 7        0     telemetry code
 * 8        1     telemetry data type
 * 9        2     telemetry item #1
 *
 * example for CPU temperature as a 4 byte floating point number in hex :
 *
 * byte     data
 *        --------
 *  0     | 0xAB | start byte A
 *  1     | 0xBA | start byte B
 *  2     | 0xC0 | ipv4 0 (192)
 *  3     | 0xA8 | ipv4 1 (168)
 *  4     | 0x56 | ipv4 2 (86)
 *  5     | 0x35 | ipv4 3 (53)
 *  6     | 0x06 | payload size
 *  7     | 0x05 | telemetry code, CPU temp
 *  8     | 0x02 | telemetry data type
 *  9     | 0x42 | CPU temp [byte 0] (33.5)
 * 10     | 0x06 | CPU temp [byte 1]
 * 11     | 0x00 | CPU temp [byte 2]
 * 12     | 0x00 | CPU temp [byte 3]
 *        --------
 */

#define SYSD_START_BYTE_A 0xAB
#define SYSD_START_BYTE_B 0xBA

/** @brief Enumeration of data type codes */
typedef enum {
  SYSD_TYPE_FLOAT  = 0x01,
  SYSD_TYPE_DOUBLE = 0x02,
  SYSD_TYPE_INT8   = 0x03,
  SYSD_TYPE_UINT8  = 0x04,
  SYSD_TYPE_INT16  = 0x05,
  SYSD_TYPE_UINT16 = 0x06,
  SYSD_TYPE_INT32  = 0x07,
  SYSD_TYPE_UINT32 = 0x08,
  SYSD_TYPE_INT64  = 0x09,
  SYSD_TYPE_UINT64 = 0x0A,
} proto_datatypes_e;

/** @brief Enumeration for frame byte offsets */
typedef enum {
  SYSD_OFFSET_START_BYTE_A   = 0, // byte 0
  SYSD_OFFSET_START_BYTE_B   = 1, // byte 1
  SYSD_OFFSET_DEST_IPV4      = 2, // byte 2-5
  SYSD_OFFSET_TELEM_CODE     = 6, // byte 6
  SYSD_OFFSET_DATA_TYPE_CODE = 7, // byte 7
  //SYSD_OFFSET_DATA_TYPE_CODE = 8, // byte 8
  SYSD_OFFSET_PAYLOAD        = 8, // byte 8-40
} proto_offets_e;

/** @brief Enumeration for sizes */
typedef enum {
  SYSD_HEADER_SIZE      = 4,
  SYSD_MAX_MESSAGE_SIZE = 32,
} proto_sizes_e;

/** @brief Struct for message frames */
typedef struct {
  uint8_t type;
  uint16_t length;
  char *buffer;
} proto_frame_t;

proto_frame_t *serialize(uint8_t telemetry_code,
                         proto_datatypes_e data_type,
                         void *data,
                         const char *destination_ip,
                         uint32_t *out_len);

void deserialize(const uint8_t *buffer,
                 uint16_t buffer_size,
                 sysd_telemetry_t *telemetry);

#endif
