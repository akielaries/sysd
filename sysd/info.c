#include "../libsysd/info.h"

#ifdef __LIBTMP36__

// TMP36 temperature sensor driver
#include <libtmp36/libtmp36.h>

#endif

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <sys/statvfs.h>

/* function to read and return the contents of a file */
char *read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        char *error_message = (char *)malloc(256);
        snprintf(error_message, 256, "Error: Unable to read %s", filename);
        // snprintf_s(error_message, 256, 255, "Error: Unable to read %s",
        // filename);

        return error_message;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *buffer = (char *)malloc(length);
    if (buffer) {
        fread(buffer, 1, length, file);
    }
    fclose(file);
    return buffer;
}

/* function to get the number of running processes */
/*int ps_count() {
    FILE *proc_pipe = popen("ps aux | wc -l", "r");
    if (!proc_pipe) {
        fprintf(stderr, "popen failed for processes.\n");
        return -1;
    }

    char proc_buffer[128];
    if (fgets(proc_buffer, sizeof(proc_buffer), proc_pipe) != NULL) {
        int num_proc = atoi(proc_buffer);
        pclose(proc_pipe);
        return num_proc;
    } else {
        fprintf(stderr, "Failed to read the output of 'ps'.\n");
        pclose(proc_pipe);
        return -1;
    }
}*/
int ps_count() {
    int num_proc = -1;
    struct sysinfo si;
    if (sysinfo(&si) == 0) {
        num_proc = si.procs;
    }
    return num_proc;
}

/* function to display CPU usage */
void cpu_usage() {
    char *cpu_usage = read_file("/proc/loadavg");
    printf("CPU Usage (load average): %s", cpu_usage);
    free(cpu_usage);
}

/* function to extract CPU BogoMIPS and model name */
void cpu_info(System *system) {
    // get CPU model
    FILE *cpuinfo = fopen("/proc/cpuinfo", "r");
    if (cpuinfo == NULL) {
        perror("Error opening /proc/cpuinfo");
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), cpuinfo)) {
        // extract device model
        if (strstr(line, "Model") || strstr(line, "model")) {
            char *model = strchr(line, ':');
            if (model != NULL) {
                // moves past semi colon and space/tab
                model += 2;
                strncpy(system->device, model, sizeof(system->device) - 1);
                // ensure null termination
                system->device[sizeof(system->device) - 1] = '\0';
            }
        }
        // extract hardware line
        if (strstr(line, "Hardware")) {
            char *hardware = strchr(line, ':');
            if (hardware != NULL) {
                hardware += 2;
                strncpy(system->cpu_model,
                        hardware,
                        sizeof(system->cpu_model) - 1);
                system->cpu_model[sizeof(system->cpu_model) - 1] = '\0';
            }
        }
    }
    fclose(cpuinfo);

    // use sysconf to get number of CPU cores
    long nprocs = sysconf(_SC_NPROCESSORS_ONLN);
    if (nprocs < 1) {
        perror("Error getting number of CPU cores");
        exit(EXIT_FAILURE);
    }
    system->cpu_cores = (int)nprocs;
}

/* function to display CPU temperature */
double cpu_temp() {
#ifdef __LIBTMP36__

    // try reading from the LIBTMP36 on board sensor

#else

    char *cpu_temp = read_file("/sys/class/thermal/thermal_zone0/temp");
    if (strncmp(cpu_temp, "Error", 5) == 0) {
        printf("Error reading CPU temperature.\n");
        free(cpu_temp);
        return 0.0;
    } else {
        double temp_c = atof(cpu_temp) / 1000.0; // convert to Celsius
        free(cpu_temp);
        return temp_c;
    }

#endif
}

double cpu_idle_temp() {
    printf("Determining idle CPU temperature...\n");

    int duration = 20;

    double temperatures[20];
    time_t time_start = time(NULL);
    int index = 0;
    while (1) {
        time_t time_now = time(NULL);
        int time_elapsed = difftime(time_now, time_start);
        if (time_elapsed >= duration) {
            break;
        }
        double temperature = cpu_temp();
        temperatures[index++] = temperature;
        sleep(1);
    }
    double sum = 0.0;
    for (int i = 0; i < index; i++) {
        sum += temperatures[i];
    }
    double idle_temp = sum / index;

    return idle_temp;
}

/* wait until idle CPU temp is met */
void cpu_idle(double idle_temp) {
    while (1) {
        double temp_now = cpu_temp();

        if (temp_now <= idle_temp) {
            break;
        }
        sleep(1);
    }
}

/* get CPU usage */
double cpu_load() {
    FILE *file = fopen("/proc/stat", "r");
    if (!file) {
        fprintf(stderr, "Failed to open /proc/stat\n");
        return -1.0;
    }

    char line[256];
    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        fprintf(stderr, "Failed to read /proc/stat\n");
        return -1.0;
    }

    fclose(file);

    // Ensure the line starts with "cpu"
    if (strncmp(line, "cpu", 3) != 0) {
        fprintf(stderr, "Failed to parse /proc/stat\n");
        return -1.0;
    }

    uint64_t user, nice, system, idle, iowait, irq, softirq;
    int count = sscanf(line,
                       "cpu %lu %lu %lu %lu %lu %lu %lu",
                       &user,
                       &nice,
                       &system,
                       &idle,
                       &iowait,
                       &irq,
                       &softirq);

    if (count < 7) {
        fprintf(stderr, "Failed to parse /proc/stat\n");
        return -1.0;
    }

    uint64_t idle_time = idle + iowait;
    uint64_t total_time = user + nice + system + idle + iowait + irq + softirq;

    // CPU usage percentage
    static uint64_t prev_idle_time = 0, prev_total_time = 0;
    double usage = 0.0;

    if (prev_total_time != 0 || prev_idle_time != 0) {
        uint64_t delta_total = total_time - prev_total_time;
        uint64_t delta_idle = idle_time - prev_idle_time;
        usage = (delta_total - delta_idle) * 100.0 / delta_total;
    }

    prev_total_time = total_time;
    prev_idle_time = idle_time;

    return usage;
}

/* function to display memory statistics */
void mem_info() {
    FILE *meminfo_file = fopen("/proc/meminfo", "r");
    if (meminfo_file) {
        char line[256];
        char *fetched_stats[] =
            {"MemTotal:", "MemFree:", "MemAvailable:", "Buffers:", "Cached:"};

        while (fgets(line, sizeof(line), meminfo_file) != NULL) {
            for (int i = 0; i < 5; i++) {
                if (strstr(line, fetched_stats[i]) != NULL) {
                    printf("%s", line);
                }
            }
        }
        fclose(meminfo_file);
    } else {
        fprintf(stderr, "Failed to read /proc/meminfo.\n");
    }
}

/* get memory usage info programmatically */
void mem_stats(struct System *sys) {
    struct sysinfo ram_info;

    sysinfo(&ram_info);

    uint64_t virt_total =
        (ram_info.totalram + ram_info.totalswap) * ram_info.mem_unit;
    uint64_t virt_used = (ram_info.totalram - ram_info.freeram +
                          ram_info.totalswap - ram_info.freeswap) *
                         ram_info.mem_unit;

    uint64_t phys_total = ram_info.totalram * ram_info.mem_unit;
    uint64_t phys_used =
        (ram_info.totalram - ram_info.freeram) * ram_info.mem_unit;

    /* VIRTUAL MEM in KB */
    sys->v_mem_total = virt_total / 1000;
    sys->v_mem_used = virt_used / 1000;
    sys->v_mem_free = (virt_total - virt_used) / 1000;

    /* PHYSICAL MEM in KB */
    sys->p_mem_total = phys_total / 1000;
    sys->p_mem_used = phys_used / 1000;
    sys->p_mem_free = (phys_total - phys_used) / 1000;
}

void print_disk_usage(const char *path) {
    struct statvfs stat;

    if (statvfs(path, &stat) != 0) {
        perror("statvfs");
        return;
    }   

    unsigned long total = stat.f_blocks * stat.f_frsize;
    unsigned long free = stat.f_bfree * stat.f_frsize;
    unsigned long used = total - free;

    /*printf("Disk usage for %s:\n", path);
    printf("Total: %.2f GB\n", (double)total / SYSD_GB_SIZE);
    printf("Free: %.2f GB\n", (double)free / SYSD_GB_SIZE);
    printf("Used: %.2f GB\n", (double)used / SYSD_GB_SIZE);
    */
}
