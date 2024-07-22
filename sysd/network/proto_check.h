#ifndef PROTO_CHECK_H
#define PROTO_CHECK_H

#include <stdint.h>

// crc-16 MODBUS: https://crccalc.com/
uint16_t get_crc16(const uint8_t *data, size_t length);

uint8_t get_checksum(const uint8_t *data, size_t length);

#endif
