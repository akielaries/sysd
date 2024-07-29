#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../libsysd/system.h"
#include "../../libsysd/utils.h"
#include "proto_pubsub.h"
#include "proto_queue.h"

int sysd_publish_telemetry(sysd_telemetry_t *telemetry) {
    int ret = 0;
    int len = SYSD_MAX_MESSAGE_SIZE;

    // message queue
    proto_queue_t proto_queue;
    init_queue(&proto_queue);

    proto_frame_t *proto_frame;

    // serialize CPU load
    proto_frame = serialize(SYSD_CPU_LOAD,
                            SYSD_TYPE_DOUBLE,
                            &telemetry->cpu_load,
                            "192.168.1.10",
                            &len);
    enqueue(&proto_queue, proto_frame);

    // serialize CPU temp
    proto_frame = serialize(SYSD_CPU_TEMP,
                            SYSD_TYPE_FLOAT,
                            &telemetry->cpu_temp,
                            "192.168.1.10",
                            &len);
    enqueue(&proto_queue, proto_frame);

    // serialize process count
    proto_frame = serialize(SYSD_PROC_COUNT,
                            SYSD_TYPE_UINT16,
                            &telemetry->proc_count,
                            "192.168.1.10",
                            &len);
    enqueue(&proto_queue, proto_frame);

    // serialize VRAM info
    proto_frame = serialize(SYSD_VRAM_TOTAL,
                            SYSD_TYPE_FLOAT,
                            &telemetry->ram_info.vram_total,
                            "192.168.1.10",
                            &len);
    enqueue(&proto_queue, proto_frame);
    proto_frame = serialize(SYSD_VRAM_USED,
                            SYSD_TYPE_FLOAT,
                            &telemetry->ram_info.vram_used,
                            "192.168.1.10",
                            &len);
    enqueue(&proto_queue, proto_frame);
    proto_frame = serialize(SYSD_VRAM_FREE,
                            SYSD_TYPE_FLOAT,
                            &telemetry->ram_info.vram_free,
                            "192.168.1.10",
                            &len);
    enqueue(&proto_queue, proto_frame);

    // serialize PRAM info
    proto_frame = serialize(SYSD_PRAM_TOTAL,
                            SYSD_TYPE_FLOAT,
                            &telemetry->ram_info.pram_total,
                            "192.168.1.10",
                            &len);
    enqueue(&proto_queue, proto_frame);
    proto_frame = serialize(SYSD_PRAM_USED,
                            SYSD_TYPE_FLOAT,
                            &telemetry->ram_info.pram_used,
                            "192.168.1.10",
                            &len);
    enqueue(&proto_queue, proto_frame);
    proto_frame = serialize(SYSD_PRAM_FREE,
                            SYSD_TYPE_FLOAT,
                            &telemetry->ram_info.pram_free,
                            "192.168.1.10",
                            &len);
    enqueue(&proto_queue, proto_frame);

    // serialize storage info
    proto_frame = serialize(SYSD_STRG_TOTAL,
                            SYSD_TYPE_FLOAT,
                            &telemetry->ssd_info.storage_total,
                            "192.168.1.10",
                            &len);
    enqueue(&proto_queue, proto_frame);
    proto_frame = serialize(SYSD_STRG_USED,
                            SYSD_TYPE_FLOAT,
                            &telemetry->ssd_info.storage_used,
                            "192.168.1.10",
                            &len);
    enqueue(&proto_queue, proto_frame);
    proto_frame = serialize(SYSD_STRG_FREE,
                            SYSD_TYPE_FLOAT,
                            &telemetry->ssd_info.storage_free,
                            "192.168.1.10",
                            &len);
    enqueue(&proto_queue, proto_frame);
    // TODO publish data to localhost for now, get this working before figuring
    // out destination as a parameter

    printf("data after serialization: \n");
    while (!queue_status(&proto_queue)) {
        proto_frame_t *frame = dequeue(&proto_queue);
        if (!frame) {
            perror("Failed to dequeue frame");
            return -1;
        }

        for (uint32_t i = 0; i < frame->length; i++) {
            printf("0x%02X ", (unsigned char)frame->buffer[i]);
        }
        printf("\n");

        free(frame->buffer);
        free(frame);
    }
    return ret;
}
/** @brief subscribe for sysd telemetry data */
int sysd_subscribe_telemetry(sysd_telemetry_t *telemetry) {
    int ret = 0;

    // populate telemetry struct with received information
    telemetry->cpu_load = 99.9;

    return ret;
}
