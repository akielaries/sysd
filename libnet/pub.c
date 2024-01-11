#include "net.h"
#include <unistd.h>
#include <string.h>

int main() {
    const char *message = "Hello, World!";
    double num = 22.3;
    const char *ip_address = "192.168.255.2";
    const int port = 8888;

    int socket = conn_init(ip_address, port);


    while (1) {
        publish(socket, message, strlen(message));
        publish(socket, &num, sizeof(num));
        sleep(5);
    }

    return 0;
}
