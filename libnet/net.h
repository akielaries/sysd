#ifndef __NET_H__
#define __NET_H__


/**
 * @brief function to publish data 
 */
void publish(const char *message, const int port);

/**
 * @brief function to retrieve data
 */
void subscribe(const char *ip, const int port);


#endif
