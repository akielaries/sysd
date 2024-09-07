#include "../libsysd/system.h"
#include "network/proto.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // For sleep

int main(int argc, char *argv[]) {
    int publish_flag = 0;
    int subscribe_flag = 0;
    int opt;

    // Parse command-line arguments
    while ((opt = getopt(argc, argv, "ps")) != -1) {
        switch (opt) {
            case 'p':
                publish_flag = 1;
                break;
            case 's':
                subscribe_flag = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-p] [-s]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (publish_flag) {
        // Publishing telemetry
        sysd_telemetry_t publish_telemetry = sysd_get_telemetry();
        printf("Publishing the following telemetry:\n");
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
        printf("Publish return code: %d\n", pub);
    }

    if (subscribe_flag) {
        // Subscribing for telemetry
        sysd_telemetry_t subscribe_telemetry;
        int sub = sysd_subscribe_telemetry(&subscribe_telemetry);
        printf("Subscribe return code: %d\n", sub);

        if (sub == 0) {
            printf("Received telemetry:\n");
            printf("load        : %f\n", subscribe_telemetry.cpu_load);
        }
    }

    if (!publish_flag && !subscribe_flag) {
        fprintf(stderr, "Usage: %s [-p] [-s]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    return 0;
}

/*
void *subscribe_thread(void *arg) {
    sysd_telemetry_t subscribe_telemetry;
    
    while (1) {
        int sub = sysd_subscribe_telemetry(&subscribe_telemetry);
        if (sub == 0) {
            printf("received the following telemetry data:\n");
            printf("load        : %f\n", subscribe_telemetry.cpu_load);
            // Print other telemetry data as needed
        } else {
            printf("Failed to subscribe\n");
        }
    }
    
    return NULL;
}

void *publish_thread(void *arg) {
    for (int i = 0; i < 1; i++) {
        sysd_telemetry_t publish_telemetry = sysd_get_telemetry();

        printf("publishing the following telemetry data: \n");
        printf("model       : %s\n", publish_telemetry.cpu_info.cpu_model);
        // Print other telemetry data as needed

        int pub = sysd_publish_telemetry(&publish_telemetry);
        printf("publish rc: %d\n", pub);

        sleep(1); // Delay between publishing
    }
    
    return NULL;
}

int main() {
    pthread_t sub_thread, pub_thread;

    // Create the subscribe thread
    if (pthread_create(&sub_thread, NULL, subscribe_thread, NULL)) {
        fprintf(stderr, "Error creating subscribe thread\n");
        return 1;
    }

    // Create the publish thread
    if (pthread_create(&pub_thread, NULL, publish_thread, NULL)) {
        fprintf(stderr, "Error creating publish thread\n");
        return 1;
    }

    // Join the threads to the main process
    pthread_join(sub_thread, NULL);
    pthread_join(pub_thread, NULL);

    return 0;
}
*/

/*
int main() {
    printf("publishing the following: \n");

    // while (1) {
    //  get device telemetry
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

    // PUBLISH telemetry
    int pub = sysd_publish_telemetry(&publish_telemetry);
    printf("publish rc: %d\n", pub);

    // SUBSCRIBE for telemetry
    sysd_telemetry_t subscribe_telemetry;

    int sub = sysd_subscribe_telemetry(&subscribe_telemetry);
    printf("subscribe rc: %d\n", sub);

    printf("received the following: \n");

    printf("load        : %f\n", subscribe_telemetry.cpu_load);

    return 0;
}
*/
