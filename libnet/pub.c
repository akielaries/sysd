#include "net.h"
#include <unistd.h>

int main() {
    const char *message = "Hello, World!";
    const char *ip_address = "192.168.255.2";
    const int port = 8888;

    int socket = conn_init(ip_address, port);

    while (1) {
        publish(socket, message);
        publish(socket, "poobar?");
        sleep(5);
    }

    return 0;
}
