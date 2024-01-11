#include "net.h"
#include <stdint.h>
#include <string.h>
#include <unistd.h>

int main() {
    const char *message = "Hello, World!";
    double num_d = 22.3;
    float num_f = 99.345;
    int32_t num_i = 123;
    const char *ip_address = "192.168.255.2";
    const int port = 8888;

    int socket = conn_init(ip_address, port);

    while (1) {
        publish(socket, message, strlen(message));
        publish(socket, &num_d, sizeof(num_d));
        publish(socket, &num_i, sizeof(num_i));
        sleep(5);
    }

    return 0;
}
