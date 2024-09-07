#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "../../libsysd/system.h"
#include "../../libsysd/utils.h"
#include "proto_pubsub.h"
#include "proto_queue.h"

int sysd_publish_telemetry(sysd_telemetry_t *telemetry) {
    int  ret       = 0;
    int  len       = SYSD_MAX_MESSAGE_SIZE;
    char dest_ip[] = "127.0.0.1";

    // message queue
    proto_queue_t proto_queue;
    init_queue(&proto_queue);

    proto_frame_t *proto_frame;

    // serialize CPU load
    proto_frame = serialize(SYSD_CPU_LOAD,
                            SYSD_TYPE_DOUBLE,
                            &telemetry->cpu_load,
                            dest_ip,
                            &len);
    enqueue(&proto_queue, proto_frame);

    // serialize CPU temp
    proto_frame = serialize(SYSD_CPU_TEMP,
                            SYSD_TYPE_FLOAT,
                            &telemetry->cpu_temp,
                            dest_ip,
                            &len);
    enqueue(&proto_queue, proto_frame);

    // serialize process count
    proto_frame = serialize(SYSD_PROC_COUNT,
                            SYSD_TYPE_UINT16,
                            &telemetry->proc_count,
                            dest_ip,
                            &len);
    enqueue(&proto_queue, proto_frame);

    // serialize VRAM info
    proto_frame = serialize(SYSD_VRAM_TOTAL,
                            SYSD_TYPE_FLOAT,
                            &telemetry->ram_info.vram_total,
                            dest_ip,
                            &len);
    enqueue(&proto_queue, proto_frame);
    proto_frame = serialize(SYSD_VRAM_USED,
                            SYSD_TYPE_FLOAT,
                            &telemetry->ram_info.vram_used,
                            dest_ip,
                            &len);
    enqueue(&proto_queue, proto_frame);
    proto_frame = serialize(SYSD_VRAM_FREE,
                            SYSD_TYPE_FLOAT,
                            &telemetry->ram_info.vram_free,
                            dest_ip,
                            &len);
    enqueue(&proto_queue, proto_frame);

    // serialize PRAM info
    proto_frame = serialize(SYSD_PRAM_TOTAL,
                            SYSD_TYPE_FLOAT,
                            &telemetry->ram_info.pram_total,
                            dest_ip,
                            &len);
    enqueue(&proto_queue, proto_frame);
    proto_frame = serialize(SYSD_PRAM_USED,
                            SYSD_TYPE_FLOAT,
                            &telemetry->ram_info.pram_used,
                            dest_ip,
                            &len);
    enqueue(&proto_queue, proto_frame);
    proto_frame = serialize(SYSD_PRAM_FREE,
                            SYSD_TYPE_FLOAT,
                            &telemetry->ram_info.pram_free,
                            dest_ip,
                            &len);
    enqueue(&proto_queue, proto_frame);

    // serialize storage info
    proto_frame = serialize(SYSD_STRG_TOTAL,
                            SYSD_TYPE_FLOAT,
                            &telemetry->ssd_info.storage_total,
                            dest_ip,
                            &len);
    enqueue(&proto_queue, proto_frame);
    proto_frame = serialize(SYSD_STRG_USED,
                            SYSD_TYPE_FLOAT,
                            &telemetry->ssd_info.storage_used,
                            dest_ip,
                            &len);
    enqueue(&proto_queue, proto_frame);
    proto_frame = serialize(SYSD_STRG_FREE,
                            SYSD_TYPE_FLOAT,
                            &telemetry->ssd_info.storage_free,
                            dest_ip,
                            &len);
    enqueue(&proto_queue, proto_frame);
    // TODO publish data to localhost for now, get this working before figuring
    // out destination as a parameter

    printf("publishing data to %s\n", dest_ip);
    // Setup UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(8080);            // Use any port
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost

    // Send each serialized frame over UDP to localhost
    while (!queue_status(&proto_queue)) {
        proto_frame_t *frame = dequeue(&proto_queue);
        if (!frame) {
            perror("Failed to dequeue frame");
            return -1;
        }

        // Print out serialized data before sending
        printf("Serialized data being sent: ");
        for (int i = 0; i < frame->length; i++) {
            printf(
                "0x%02X ",
                frame->buffer[i]); // Print each byte as a two-digit hex value
        }
        printf("\n");

        // Send serialized data
        sendto(sockfd,
               frame->buffer,
               frame->length,
               0,
               (struct sockaddr *)&server_addr,
               sizeof(server_addr));

        free(frame->buffer);
        free(frame);
    }

    close(sockfd);

    /*
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
    */

    return ret;
}
/** @brief subscribe for sysd telemetry data */
int sysd_subscribe_telemetry(sysd_telemetry_t *telemetry) {
    int ret = 0;

    // should read in packets and deserialize them into readable info

    // populate telemetry struct with received information
    telemetry->cpu_load = 99.9;

    return ret;
}
