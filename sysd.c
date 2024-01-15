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
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>


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

    // get socket_fd
    int socket_fd = conn_init(ip_address, port);

    /* setup daemon process */
    // PID: Process ID
    // SID: Session ID
    pid_t pid, sid;
    // forks parent process
    pid = fork();

    // check if fork() failed
    if (pid < 0) {
        //perror("fork() failed");
        exit(EXIT_FAILURE);
    }

    // if fork() succeeded
    if (pid > 0) {
        // this will bounce us out of the current process
        exit(EXIT_SUCCESS);
    }

    // setsid creates a new session
    sid = setsid();

    if (sid < 0) {
        //perror("setsid() failed");
        exit(EXIT_FAILURE);
    }

    // sets working directory
    if ((chdir("/")) < 0) {
        //perror("chdir() failed");
        exit(EXIT_FAILURE);
    }
    // close stdin, stdout, and stderr
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // append log file, use this later TODO
    int log_file = open("sysd.log", O_APPEND | O_WRONLY | O_CREAT, 0640);

    // redirects stdout and stderr
    dup2(log_file, STDOUT_FILENO);
    dup2(log_file, STDERR_FILENO);

    // closes original file
    close(log_file);

    // System struct obj
    struct System sys;

    // gets processor information (mode, bogoMIPS, cores)
    cpu_info(&sys);

    /* LCD configuration */
    int rows = 2;
    int cols = 16;
    int addr = 0x27;
    LCD *hc = lcd_create(addr, rows, cols);
    char *error = NULL;
    char *dev = "/dev/i2c-1"; // i2c dev bus for LCD

    printf("CPU Model: %s\n", sys.cpu_model);
    printf("BogoMIPS: %lf\n", sys.bogus_mips);
    printf("Number of CPUs: %d\n\n", sys.num_proc);

    int proc_count;
    double load, temp_cpu, temp_gpu;

    char s[50];

    if (lcd_init(dev, hc, &error)) {
        lcd_clear(hc);

        while (1) {
            /* collect system info */
            mem_stats(&sys);
            load = cpu_load();
            proc_count = ps_count();
            temp_cpu = cpu_temp();

            /*printf("CPU Temp: %lf\n", temp_cpu);
            printf("CPU Usage: %lf%%\n", load);
            printf("Process count: %d\n", ps_count());
            printf("vMemory Total: %lu KB\n", sys.v_mem_total);
            printf("vMemory Used: %lu KB\n", sys.v_mem_used);
            printf("vMemory Free: %lu KB\n", sys.v_mem_free);

            printf("pMemory Total: %lu KB\n", sys.p_mem_total);
            printf("pMemory Used: %lu KB\n", sys.p_mem_used);
            printf("pMemory Free: %lu KB\n", sys.p_mem_free);
            printf("\n");*/

            /* publish system info */
            publish(socket_fd, T_DOUBLE, sizeof(load), &load);
            publish(socket_fd, T_INT32, sizeof(proc_count), &proc_count);
            publish(socket_fd, T_DOUBLE, sizeof(temp_cpu), &temp_cpu);

            /* display to LCD if available */
            sprintf(s, "ps:%dtmp:%lf", proc_count, cpu_temp());
            lcd_write_string_at(hc, 0, 0, (unsigned char *)s, 0);
            sprintf(s, "ld:%lf", load);
            lcd_write_string_at(hc, 1, 0, (unsigned char *)s, 0);

            // sleep for 5 seconds before next interval
            sleep(5);
        }
        lcd_terminate(hc);
        lcd_destroy(hc);
    } else {
        fprintf(stderr, "%s\n", error);
        free(error);
    }

    close(socket_fd);
    exit(EXIT_SUCCESS);
}
