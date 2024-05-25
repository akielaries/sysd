#include "../libsysd/config.h"
#include "../libsysd/info.h"
#include "../libsysd/net.h"
#include <errno.h>
#include <fcntl.h>
#include <liblcd/liblcd.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>


// TODO break this up into some separate functions ??
int main(int argc, char *argv[]) {
    /**
     * MAIN DRIVER:
     * 0. Process some command line arguments when this binary is invoked
     * 1. Set some configurations based on a sysd.conf file
     * 2. Collect system information from info.c
     * 3. Publish information to destination IP & PORT using libnet/net.c
     * 4. Display information to 16x02 LCD using liblcd (TODO make this
     * optional)
     *
     * TODO use cmd line args? conf file to parse and config daemon from? FIXME
     */
    // config struct
    struct Config cfg;
    char *cfg_file = "sysd.conf";
    // flags for pub, sub, and default modes
    _Bool pub_flg = 0, sub_flg = 0, default_flg = 0;
    int socket_fd = 0;
    // append log file, use this later TODO
    int log_file = open("~/sysd.log", O_APPEND | O_WRONLY | O_CREAT, 0640);
    // PID: Process ID
    // SID: Session ID
    pid_t pid, sid;

    // parse config file for daemon configuration
    parse(cfg_file, &cfg);

    // default flag conditions
    if (argc == 1) {
        default_flg = 1;
    }
    // publish flag
    else if (strcmp(argv[1], "-p") == 0) {
        // create socket_fd with connection initialization function
        socket_fd = conn_init(cfg.PORT);
        pub_flg = 1;
    }
    // subscribe flag
    else if (strcmp(argv[1], "-s") == 0) {
        sub_flg = 1;
    }

    // we can free the memory allocated to IPV4 and LOG_DIR right after use
    free(cfg.IPV4_SUB);
    free(cfg.LOG_DIR);

    /************************** setup daemon process **************************/
    // PID: Process ID
    // SID: Session ID
    // forks parent process
    /*
    pid = fork();

    // check if fork() failed
    if (pid < 0) {
        perror("fork() failed");
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
        perror("setsid() failed");
        exit(EXIT_FAILURE);
    }

    // sets working directory
    if ((chdir("/")) < 0) {
        perror("chdir() failed");
        exit(EXIT_FAILURE);
    }
    // close stdin, stdout, and stderr
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // redirects stdout and stderr
    dup2(log_file, STDOUT_FILENO);
    dup2(log_file, STDERR_FILENO);

    // closes original file
    close(log_file);
    */
    /**************************** end daemon setup ****************************/

    // TODO if LCD is defined in config
    /**************************** LCD configuration ***************************/
    char *error = "";
    LCD *hc = NULL;

    // if LCD is present and NOT in subscriber mode. supporting the LCD util
    // is really for my own use case so this should be edited so it acts as an
    // additional module? For now using the sysd.conf is a feasible solution
    // TODO
    if (cfg.I2C_LCD != 0 && sub_flg == 0) {
        char *dev;
        printf("I2C LCD found 0x%02X\n", cfg.I2C_LCD);
        // TODO: read these in from config?
        int rows = 2;
        int cols = 16;
        int addr = 0x27;
        hc = lcd_create(addr, rows, cols);
        error = NULL;
        dev = "/dev/i2c-1"; // i2c dev bus for LCD

        // if LCD defined
        if (cfg.I2C_LCD > 0) {
            if (lcd_init(dev, hc, &error)) {
                lcd_clear(hc);
            }
        }
    }
    /***************************** end LCD config *****************************/

    /***************************** publisher mode *****************************/
    // if default mode OR publish flag is passed in
    if (default_flg == 1 || pub_flg == 1) {
        /************************** base system info **************************/
        // System struct obj
        struct System sys;
        // gets processor information (cpu_model, bogus_mips, num_proc)
        cpu_info(&sys);
        printf("Device: %s\n", sys.cpu_model);
        printf("Hardware: %s\n", sys.device);
        // printf("BogoMIPS: %lf\n", sys.bogus_mips);
        printf("Number of CPUs: %d\n\n", sys.cpu_cores);

        // process count
        int proc_count;
        double load, temp_cpu; //, temp_gpu;

        /*************************** MAIN WHILE LOOP **************************/
        while (1) {
            /* collect system info */
            mem_stats(&sys);
            load = cpu_load();
            proc_count = ps_count();
            temp_cpu = cpu_temp();

            printf("CPU Temp: %lf\n", temp_cpu);
            printf("CPU Usage: %lf%%\n", load);
            printf("Process count: %d\n", ps_count());
            printf("vMemory Total: %lu KB\n", sys.v_mem_total);
            printf("vMemory Used: %lu KB\n", sys.v_mem_used);
            printf("vMemory Free: %lu KB\n", sys.v_mem_free);

            printf("pMemory Total: %lu KB\n", sys.p_mem_total);
            printf("pMemory Used: %lu KB\n", sys.p_mem_used);
            printf("pMemory Free: %lu KB\n", sys.p_mem_free);
            printf("\n");

            // TODO for having checks in an infinite while loops sounds ugly
            // FIXME
            if (pub_flg == 1) {
                /* publish system info */
                publish(socket_fd, T_DOUBLE, sizeof(load), &load);
                publish(socket_fd, T_INT32, sizeof(proc_count), &proc_count);
                publish(socket_fd, T_DOUBLE, sizeof(temp_cpu), &temp_cpu);
            }
            // I2C_LCD is present in config
            if (cfg.I2C_LCD > 0) {
                // calculates required size for line 01 including null term
                int sz_line01 =
                    snprintf(NULL, 0, "ps:%dtmp:%lf", proc_count, cpu_temp()) +
                    1;
                // calculates requires size for line 02 including null term
                int sz_line02 = snprintf(NULL, 0, "ld:%lf", load) + 1;
                // total required size for both lines
                int sz_total = sz_line01 + sz_line02;

                // dynamically allocate memory for both lines
                char *s = (char *)malloc(sz_total);
                if (s == NULL) {
                    return -1;
                }

                // format strings to print to LCD
                // LINE 01 : running processes - CPU temp
                sprintf(s, "ps:%d tmp:%.2lf", proc_count, cpu_temp());
                lcd_write_string_at(hc, 0, 0, (unsigned char *)s, 0);
                // LINE 02 : CPU load -
                sprintf(s, "ld:%lf", load);
                lcd_write_string_at(hc, 1, 0, (unsigned char *)s, 0);

                // free memory allocated for array s
                free(s);
            }

            // sleep for 5 seconds before next interval
            sleep(5);
        }
    }
    /***************************** subscribe mode *****************************/
    else if (sub_flg == 1) {
        const char *ip_address = "192.168.86.32";
        int port = 20000;

        subscribe(ip_address, port);
    }

    if (cfg.I2C_LCD > 0) {
        // terminate and destroy obj/memory related to LCD display
        lcd_terminate(hc);
        lcd_destroy(hc);
    }

    // some case that isn't accounted for
    else {
        fprintf(stderr, "%s\n", error);
        // free(error);
    }

    close(socket_fd);
    exit(EXIT_SUCCESS);
}
