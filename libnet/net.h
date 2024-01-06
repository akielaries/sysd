#ifndef __NET_H__
#define __NET_H__

int conn_init(const char *ip_address, const int port);

int conn_dest(const int sock);

/**
 * @brief function to publish data 
 */
//void publish(const char *message, const int port);
void publish(int sock, const char *message);

/**
 * @brief function to retrieve data
 */
void subscribe(const char *ip_address, const int port);


#endif

