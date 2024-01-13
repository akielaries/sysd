#include "libnet/net.h"
#include "info.h"
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <liblcd/liblcd.h>
#include <stdio.h>


int main() {
    const char *message = "Hello, World!";
    double num_d = 22.3;
    float num_f = 99.345;
    int32_t num_i = 123;

    // IPv4 to publish to
    const char *ip_address = "192.168.255.5";
    // port to publish on
    const int port = 20000;

    // get socket
    int socket = conn_init(ip_address, port);

    // System struct obj
    struct System sys;

    cpu_info(&sys);

    printf("CPU Model: %s\n", sys.cpu_model);
    printf("BogoMIPS: %lf\n", sys.bogus_mips);
    printf("Number of CPUs: %d\n\n", sys.num_proc);

    int proc_count;
    double load;

    while (1) {
        /* collect system info */
        mem_stats(&sys);
        load = cpu_load();
        printf("CPU Usage: %lf%%\n", load);
        printf("Process count: %d\n", ps_count());
        printf("vMemory Total: %lu KB\n", sys.v_mem_total);
        printf("vMemory Used: %lu KB\n", sys.v_mem_used);
        printf("vMemory Free: %lu KB\n", sys.v_mem_free);

        printf("pMemory Total: %lu KB\n", sys.p_mem_total);
        printf("pMemory Used: %lu KB\n", sys.p_mem_used);
        printf("pMemory Free: %lu KB\n", sys.p_mem_free);
        printf("\n");

        /* publish system info */
        publish(socket, T_DOUBLE, sizeof(load), &load);
        /*
        publish(socket, T_CHAR, strlen(message), message);
        publish(socket, T_FLOAT, sizeof(num_f), &num_f);
        publish(socket, T_DOUBLE, sizeof(num_d), &num_d);
        publish(socket, T_INT32, sizeof(num_i), &num_i);
        */
        sleep(5);
    }

    return 0;
}
