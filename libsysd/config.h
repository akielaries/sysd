#ifndef __CONFIG_H__
#define __CONFIG_H__

#define MAX_LINE_LENGTH 80

typedef struct Config {
    char    *IPV4_SUB;
    int     PORT;
    char    *LOG_DIR;
    int     I2C_LCD;
} Config;

void parse(const char *filename, struct Config *cfg);

#endif
