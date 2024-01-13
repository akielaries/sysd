#include<stdio.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

    int main() {
    int file = open("/dev/i2c-1", O_RDWR);
    if (file < 0) {
        perror("Error opening I2C device");
        exit(1);
    }

    unsigned char address = 0x27; // Replace with your LCD address
    if (ioctl(file, I2C_SLAVE, address) < 0) {
        perror("Error setting I2C address");
        exit(1);
    }

    close(file);
    return 0;
}
