#include "../libsysd/system.h"
#include "network/protocol.h"
#include <stdio.h>

int main() {
    printf("....\n");

    sysd_telemetry_t telemetry = sysd_get_telemetry();

    return 0;
}
