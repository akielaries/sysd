#include "../libsysd/system.h"
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


/* function to display CPU temperature */
double cpu_temp() {
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

}

