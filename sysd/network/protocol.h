#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <stddef.h>
#include <stdint.h>

#define BUFF_SZ 1024

#define SYSD_START_BYTE_A 0xBA
#define SYSD_START_BYTE_B 0xAB

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
    SYSD_TYPE_UINT64 = 0x10,
} proto_datatypes_e;

/** @brief Enumeration for frame byte offsets */
typedef enum {
    SYSD_OFFSET_START_BYTE_A   = 0,
    SYSD_OFFSET_START_BYTE_B   = 1,
    SYSD_OFFSET_DEST_IPV4      = 2,
    SYSD_OFFSET_TELEM_CODE     = 3,
    SYSD_OFFSET_DATA_TYPE_CODE = 4,
} proto_offets_e;

/** @brief Enumeration for sizes */
typedef enum {
    SYSD_HEADER_SIZE      = 4,
    SYSD_MAX_MESSAGE_SIZE = 69,
} proto_sizes_e;

/** @brief Struct for message frames */
typedef struct {
    uint8_t  type;
    uint16_t len;
    char    *buffer;
} proto_frame_t;

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
