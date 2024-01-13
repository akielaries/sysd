#include "libnet/net.h"

int main() {
    const char *ip_address = "192.168.255.2";
    int port = 20000;

    subscribe(ip_address, port);

    return 0;
}
