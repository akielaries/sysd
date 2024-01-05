#include "net.h"

int main() {
    const char *message = "Hello, World!";
    const char *ip_address = "192.168.255.2";
    const int port = 8888;

    publish(message, port);

    return 0;
}

