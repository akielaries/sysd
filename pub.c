#include "info.h"
#include "libnet/net.h"
#include <errno.h>
#include <liblcd/liblcd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>



int main() {
    /**
     * MAIN DRIVER:
     * 1. Collect system information from info.c
     * 2. Publish information to destination IP & PORT using libnet/net.c
     * 3. Display information to 16x02 LCD using liblcd (TODO make this
     * optional)
     *
     * TODO use cmd line args? conf file to parse and config daemon from? FIXME
     */
    // IPv4 to publish to
    const char *ip_address = "192.168.255.5";
    // port to publish on
    const int port = 20000;

    // get socket
    int socket = conn_init(ip_address, port);

    // System struct obj
    struct System sys;

    cpu_info(&sys);

    /* LCD configuration */
    int rows = 2;
    int cols = 16;
    int addr = 0x27;
    LCD *hc = lcd_create(addr, rows, cols);
    char *error = NULL;
    char *dev = "/dev/i2c-1"; // i2c dev bus

    printf("CPU Model: %s\n", sys.cpu_model);
    printf("BogoMIPS: %lf\n", sys.bogus_mips);
    printf("Number of CPUs: %d\n\n", sys.num_proc);

    int proc_count;
    double load;

    char s[50];

    if (lcd_init(dev, hc, &error)) {
        lcd_clear(hc);

        while (1) {
            /* collect system info */
            mem_stats(&sys);
            load = cpu_load();
            proc_count = ps_count();

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
            publish(socket, T_INT32, sizeof(proc_count), &proc_count);

            /* display to LCD if available */
            sprintf(s, "ps:%d", proc_count);
            lcd_write_string_at(hc, 0, 0, (unsigned char *)s, 0);

            /*
            publish(socket, T_CHAR, strlen(message), message);
            publish(socket, T_FLOAT, sizeof(num_f), &num_f);
            publish(socket, T_DOUBLE, sizeof(num_d), &num_d);
            publish(socket, T_INT32, sizeof(num_i), &num_i);
            */
            sleep(5);
        }
        lcd_terminate(hc);
        lcd_destroy(hc);
    } else {
        fprintf(stderr, "%s\n", error);
        free(error);
    }

    return 0;
}
