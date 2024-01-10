#ifndef __NET_H__
#define __NET_H__

#include <stdint.h>
#include <stddef.h>

int conn_init(const char *ip_address, const uint16_t port);

int conn_dest(const int sock);

/**
 * @brief function to publish data
 */
//void publish(const int sock, const char *message);
void publish(const int sock, const void *data, size_t size);

/**
 * @brief function to retrieve data
 */
void subscribe(const char *ip_address, const uint16_t port);

#endif
