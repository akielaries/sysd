#include "net.h"
#include <stdint.h>
#include <string.h>
#include <unistd.h>

int main() {
    const char *message = "Hello, World!";
    double num_d = 22.3;
    float num_f = 99.345;
    int32_t num_i = 123;
    const char *ip_address = "127.0.0.1";
    const int port = 20000;

    int socket = conn_init(ip_address, port);

    while (1) {
        publish(socket, T_CHAR, strlen(message), message);
        publish(socket, T_FLOAT, sizeof(num_f), &num_f);
        publish(socket, T_DOUBLE, sizeof(num_d), &num_d);
        publish(socket, T_INT32, sizeof(num_i), &num_i);
        sleep(5);
    }

    return 0;
}
