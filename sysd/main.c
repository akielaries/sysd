#include "../libsysd/system.h"
#include "network/protocol.h"
#include <stdio.h>

int main() {
    printf("....\n");

    sysd_telemetry_t telemetry = sysd_get_telemetry();
    printf("model       : %s\n", telemetry.cpu_info.cpu_model);
    printf("hw id       : %s\n", telemetry.cpu_info.hw_id);
    printf("cores       : %d\n", telemetry.cpu_info.cpu_count);
    printf("load        : %f\n", telemetry.cpu_load);
    printf("temp        : %.2fC\n", telemetry.cpu_temp);
    printf("procs       : %d\n", telemetry.proc_count);

    printf("vram total  : %f gb\n", telemetry.ram_info.vram_total);
    printf("vram used   : %f gb\n", telemetry.ram_info.vram_used);
    printf("vram free   : %f gb\n", telemetry.ram_info.vram_free);

    printf("pram total  : %f gb\n", telemetry.ram_info.pram_total);
    printf("pram used   : %f gb\n", telemetry.ram_info.pram_used);
    printf("pram free   : %f gb\n", telemetry.ram_info.pram_free);

    printf("ssd total   : %f gb\n", telemetry.ssd_info.storage_total);
    printf("ssd used    : %f gb\n", telemetry.ssd_info.storage_used);
    printf("ssd free    : %f gb\n", telemetry.ssd_info.storage_free);

    int ret = sysd_publish_telemetry(&telemetry);

    return 0;
}
