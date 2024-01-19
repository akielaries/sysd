#include "../libsysd/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// parse the configuration file
void parse(const char *filename, struct Config *cfg) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        // removes trailing newline character
        line[strcspn(line, "\n")] = '\0';

        // ignore comments and empty lines
        if (line[0] == '#' || line[0] == '\0')
            continue;

        // for every key, get its assigned value
        // check for IPV4_PUB line
        if (strstr(line, "IPV4_SUB")) {
            const char *value_start = strchr(line, '=') + 1;
            size_t value_length = strlen(value_start);
            // cfg->IPV4_SUB = malloc(value_length + 1);
            cfg->IPV4_SUB = (char *)malloc(value_length + 1);

            if (cfg->IPV4_SUB == NULL) {
                perror("Error allocating memory");
                exit(EXIT_FAILURE);
            }
            strcpy(cfg->IPV4_SUB, value_start);
        }
        // check for PORT_PUB line
        else if (strstr(line, "PORT")) {
            cfg->PORT = atoi(strchr(line, '=') + 1);
        }
        // check for log directory
        else if (strstr(line, "LOG_DIR")) {
            const char *value_start = strchr(line, '=') + 1;
            size_t value_length = strlen(value_start);
            // cfg->LOG_DIR = malloc(value_length + 1);
            cfg->LOG_DIR = (char *)malloc(value_length + 1);

            if (cfg->LOG_DIR == NULL) {
                perror("Error allocating memory");
                exit(EXIT_FAILURE);
            }
            strcpy(cfg->LOG_DIR, value_start);
        }
        // check for LCD I2C address
        else if (strstr(line, "I2C_LCD")) {
            cfg->I2C_LCD = strtol(strchr(line, '=') + 1, NULL, 0);
        }
    }

    fclose(file);
}

int foo(const char *str) {
    char buf[4];
    memcpy(buf, str, sizeof(buf));
    if (buf[0] == 'F' && buf[1] == 'U' && buf[2] == 'Z' && buf[3] == 'Z') {
        return 1;
    }
    return 0;
}
