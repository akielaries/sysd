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

        // Ignore comments and empty lines
        if (line[0] == '#' || line[0] == '\0')
            continue;

        // for every key, get its assigned value
        // check for IPV4_PUB line
        if (strstr(line, "IPV4_SUB")) {
            cfg->IPV4_SUB = strdup(strchr(line, '=') + 1);
        }
        // check for PORT_PUB line
        else if (strstr(line, "PORT")) {
            cfg->PORT = atoi(strchr(line, '=') + 1);
        } else if (strstr(line, "LOG_DIR")) {
            cfg->LOG_DIR = strdup(strchr(line, '=') + 1);
        } else if (strstr(line, "I2C_LCD")) {
            cfg->I2C_LCD = strtol(strchr(line, '=') + 1, NULL, 0);
        }
    }

    fclose(file);
}

/*int main() {
    struct Config cfg;

    char *cfg_file = "../sysd.conf";

    // Parse the configuration file
    parse(cfg_file, &cfg);

    // Print the parsed values
    printf("IPV4: %s\n", cfg.IPV4_SUB);
    printf("PORT: %d\n", cfg.PORT);
    printf("LOG DIR: %s\n", cfg.LOG_DIR);
    printf("I2C LCD: 0x%02hhX\n", cfg.I2C_LCD);

    // free allocated memory from strdup
    free(cfg.IPV4_SUB);
    free(cfg.LOG_DIR);

    return 0;
}*/
