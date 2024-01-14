#ifndef __NET_H__
#define __NET_H__

#include <stddef.h>
#include <stdint.h>

#define BUFF_SZ 1024

#define T_DOUBLE 0x01
#define T_INT32 0x02
#define T_FLOAT 0x03
#define T_CHAR 0x04

// msg struct
struct Mesg {
    uint8_t type;
    uint16_t len;
    char val[BUFF_SZ];
};

int conn_init(const char *ip_address, const uint16_t port);

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
