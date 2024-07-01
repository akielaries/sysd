#include "../libsysd/system.h"
#include "network/protocol.h"
#include <stdio.h>

int main() {
    printf("publishing the following: \n");

    sysd_telemetry_t publish_telemetry = sysd_get_telemetry();
    printf("model       : %s\n", publish_telemetry.cpu_info.cpu_model);
    printf("hw id       : %s\n", publish_telemetry.cpu_info.hw_id);
    printf("cores       : %d\n", publish_telemetry.cpu_info.cpu_count);
    printf("load        : %f\n", publish_telemetry.cpu_load);
    printf("temp        : %.2fC\n", publish_telemetry.cpu_temp);
    printf("procs       : %d\n", publish_telemetry.proc_count);

    printf("vram total  : %f gb\n", publish_telemetry.ram_info.vram_total);
    printf("vram used   : %f gb\n", publish_telemetry.ram_info.vram_used);
    printf("vram free   : %f gb\n", publish_telemetry.ram_info.vram_free);

    printf("pram total  : %f gb\n", publish_telemetry.ram_info.pram_total);
    printf("pram used   : %f gb\n", publish_telemetry.ram_info.pram_used);
    printf("pram free   : %f gb\n", publish_telemetry.ram_info.pram_free);

    printf("ssd total   : %f gb\n", publish_telemetry.ssd_info.storage_total);
    printf("ssd used    : %f gb\n", publish_telemetry.ssd_info.storage_used);
    printf("ssd free    : %f gb\n", publish_telemetry.ssd_info.storage_free);

    int pub = sysd_publish_telemetry(&publish_telemetry);

    sysd_telemetry_t subscribe_telemetry;

    int sub = sysd_subscribe_telemetry(&subscribe_telemetry);

    printf("received the following: \n");

    printf("load        : %f\n", subscribe_telemetry.cpu_load);

    return 0;
}
