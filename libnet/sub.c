#include "net.h"

int main() {
    const char *ip_address = "127.0.0.1";
    int port = 20000;

    subscribe(ip_address, port);

    return 0;
}
