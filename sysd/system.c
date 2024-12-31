#include "../libsysd/system.h"
#include "../libsysd/utils.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/statvfs.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* @brief function to read and return the contents of a file */
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

/** @brief function to extract CPU BogoMIPS, model name, and hardware */
sysd_cpu_info_t sysd_cpu_info() {
  sysd_cpu_info_t cpu_info;
  memset(&cpu_info, 0, sizeof(cpu_info));

  // Open /proc/cpuinfo file
  FILE *cpuinfo = fopen("/proc/cpuinfo", "r");
  if (cpuinfo == NULL) {
    perror("Error opening /proc/cpuinfo");
    return cpu_info; // Return structure with empty strings
  }

  char line[256];

  while (fgets(line, sizeof(line), cpuinfo)) {
    // Extract device model
    if (strstr(line, "Model") || strstr(line, "model")) {
      char *model = strchr(line, ':');
      if (model != NULL) {
        model += 2; // Move past the colon and space/tab
        size_t model_len = strlen(model);
        if (model_len >= MAX_MODEL_LEN) {
          model_len = MAX_MODEL_LEN - 1;
        }
        strncpy(cpu_info.cpu_model, model, model_len);
        cpu_info.cpu_model[model_len] = '\0'; // Ensure null termination
      }
    }
    // Extract hardware line
    if (strstr(line, "Hardware")) {
      char *hardware = strchr(line, ':');
      if (hardware != NULL) {
        hardware += 2; // Move past the colon and space/tab
        size_t hardware_len = strlen(hardware);
        if (hardware_len >= MAX_HW_ID_LEN) {
          hardware_len = MAX_HW_ID_LEN - 1;
        }
        strncpy(cpu_info.hw_id, hardware, hardware_len);
        cpu_info.hw_id[hardware_len] = '\0'; // Ensure null termination
      }
    }
  }
  fclose(cpuinfo);

  // Use sysconf to get number of CPU cores
  uint8_t nprocs = sysconf(_SC_NPROCESSORS_ONLN);
  if (nprocs < 1) {
    perror("Error getting number of CPU cores");
    cpu_info.cpu_count = 0;
    return cpu_info; // Return structure with default values
  }
  cpu_info.cpu_count = nprocs;

  return cpu_info;
}

/** @brief function to get CPU load */
double sysd_cpu_load() {
  FILE *fp;
  char line[128]; // Buffer to read each line from /proc/stat
  double load_avg = 0.0;

  fp = fopen("/proc/stat", "r");
  if (fp == NULL) {
    perror("Failed to open /proc/stat");
    return -1.0; // Return -1.0 on error
  }

  // Read the first line from /proc/stat
  if (fgets(line, sizeof(line), fp)) {
    // Check if the line starts with "cpu "
    if (strncmp(line, "cpu ", 4) == 0) {
      // Extract CPU load information from the line
      uint64_t user, nice, system, idle, iowait, irq, softirq, steal, guest,
        guest_nice;
      sscanf(line + 5,
             "%lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
             &user,
             &nice,
             &system,
             &idle,
             &iowait,
             &irq,
             &softirq,
             &steal,
             &guest,
             &guest_nice);

      // Calculate total and idle time
      uint64_t total_time =
        user + nice + system + idle + iowait + irq + softirq + steal;
      uint64_t idle_time = idle + iowait;

      // Calculate CPU load
      load_avg = 1.0 - ((double)idle_time / total_time);
    }
  }

  fclose(fp);
  return load_avg * 100.0;
}

/** @brief function to get CPU temperature */
float sysd_cpu_temp() {
  char *cpu_temp = read_file("/sys/class/thermal/thermal_zone0/temp");
  if (strncmp(cpu_temp, "Error", 5) == 0) {
    printf("Error reading CPU temperature.\n");
    free(cpu_temp);
    return 0.0;
  } else {
    float temp_c = atof(cpu_temp) / 1000.0; // convert to Celsius
    free(cpu_temp);
    return temp_c;
  }
}

/** @brief function to get number of running processes */
uint16_t sysd_proc_count() {
  uint16_t num_proc = -1;
  struct sysinfo si;
  if (sysinfo(&si) == 0) {
    num_proc = si.procs;
  }
  return num_proc;
}

/** @brief function to get memory information */
sysd_ram_info_t sysd_ram_info() {
  sysd_ram_info_t ram_info;

  struct sysinfo sysram;

  sysinfo(&sysram);

  /* VIRTUAL MEM in GB */
  // TOTAL virtual memory
  ram_info.vram_total =
    (float)((sysram.totalram + sysram.totalswap) * sysram.mem_unit) /
    SYSD_GB_SIZE;
  // USED virtual memory
  ram_info.vram_used = (float)(((sysram.totalram - sysram.freeram) +
                                (sysram.totalswap - sysram.freeswap)) *
                               sysram.mem_unit) /
                       SYSD_GB_SIZE;
  // FREE virtual memory
  ram_info.vram_free = (ram_info.vram_total - ram_info.vram_used);

  /* PHYSICAL MEM in GB */
  // TOTAL physical memory
  ram_info.pram_total =
    (float)(sysram.totalram * sysram.mem_unit) / SYSD_GB_SIZE;
  // USED physical memory
  ram_info.pram_used =
    (float)((sysram.totalram - sysram.freeram) * sysram.mem_unit) /
    SYSD_GB_SIZE;

  // FREE physical memory
  ram_info.pram_free = ram_info.pram_total - ram_info.pram_used;

  return ram_info;
}

/** @brief function to get storage information */
sysd_ssd_info_t sysd_ssd_info() {
  sysd_ssd_info_t ssd_info;
  struct statvfs stat;

  if (statvfs("/", &stat) != 0) {
    printf("error with statvfs...\n");
  }

  ssd_info.storage_total =
    (float)(stat.f_blocks * stat.f_frsize) / SYSD_GB_SIZE;
  ssd_info.storage_free = (float)(stat.f_bfree * stat.f_frsize) / SYSD_GB_SIZE;
  ssd_info.storage_used =
    (float)(ssd_info.storage_total - ssd_info.storage_free);

  return ssd_info;
}

/** @brief sweep thru and get all telemetry information */
sysd_telemetry_t sysd_get_telemetry() {
  sysd_telemetry_t telemetry;
  telemetry.cpu_info   = sysd_cpu_info();   // get CPU info
  telemetry.cpu_load   = sysd_cpu_load();   // get CPU load
  telemetry.cpu_temp   = sysd_cpu_temp();   // get CPU temp
  telemetry.proc_count = sysd_proc_count(); // get number of processes
  telemetry.ram_info   = sysd_ram_info();   // get RAM info
  telemetry.ssd_info   = sysd_ssd_info();   // get storage info

  return telemetry;
}
