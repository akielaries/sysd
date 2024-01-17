#include "../libsysd/info.h"
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
    // Get CPU model
    FILE *cpuinfo = fopen("/proc/cpuinfo", "r");
    if (cpuinfo == NULL) {
        perror("Error opening /proc/cpuinfo");
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), cpuinfo)) {
        // extract device model
        if (strstr(line, "Model")) {
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
    char *cpu_temp = read_file("/sys/class/thermal/thermal_zone0/temp");
    if (strncmp(cpu_temp, "Error", 5) == 0) {
        printf("Error reading CPU temperature.\n");
        free(cpu_temp);
        return -1.0;
    } else {
        double temp_c = atof(cpu_temp) / 1000.0; // convert to Celsius
        free(cpu_temp);
        return temp_c;
    }
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

    char cpuLabel[16];
    sscanf(line, "%s", cpuLabel);

    if (strcmp(cpuLabel, "cpu") != 0) {
        fprintf(stderr, "Failed to parse /proc/stat\n");
        return -1.0;
    }

    uint64_t values[7];
    int count = sscanf(line,
                       "%*s %lu %lu %lu %lu %lu %lu %lu",
                       &values[0],
                       &values[1],
                       &values[2],
                       &values[3],
                       &values[4],
                       &values[5],
                       &values[6]);

    if (count < 7) {
        fprintf(stderr, "Failed to parse /proc/stat\n");
        return -1.0;
    }

    uint64_t idle = values[3];
    uint64_t total = 0;
    for (int i = 0; i < 7; i++) {
        total += values[i];
    }

    /* Calculate CPU usage as a percentage */
    double usage = 100.0 * (1.0 - (double)idle / total);
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
    struct sysinfo mem_info;

    sysinfo(&mem_info);

    uint64_t virt_total =
        (mem_info.totalram + mem_info.totalswap) * mem_info.mem_unit;
    uint64_t virt_used = (mem_info.totalram - mem_info.freeram +
                          mem_info.totalswap - mem_info.freeswap) *
                         mem_info.mem_unit;

    uint64_t phys_total = mem_info.totalram * mem_info.mem_unit;
    uint64_t phys_used =
        (mem_info.totalram - mem_info.freeram) * mem_info.mem_unit;

    /* VIRTUAL MEM in KB */
    sys->v_mem_total = virt_total / 1000;
    sys->v_mem_used = virt_used / 1000;
    sys->v_mem_free = (virt_total - virt_used) / 1000;

    /* PHYSICAL MEM in KB */
    sys->p_mem_total = phys_total / 1000;
    sys->p_mem_used = phys_used / 1000;
    sys->p_mem_free = (phys_total - phys_used) / 1000;
}
