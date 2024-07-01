/**
 * telemetry frame structure:
 *
 * | OFFSET | ITEM             | SIZE BYTES |
 * | ------ | ---------------- | ---------- |
 * | 0      | start byte A     | 1          |
 * | 1      | start byte B     | 1          |
 * | 2-5    | destination IPv4 | 4          |
 * | 6      | telemetry code   | 1          |
 * | 7      | data type code   | 1          |
 * | 8-15   | data             | 1-8        |
 */
#include "protocol.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/statvfs.h>

#include "../../libsysd/system.h"
#include "../../libsysd/utils.h"

// start bytes, ipv4, temp, float, 33.4
char string_cpu_temp[] = {
    SYSD_START_BYTE_A,
    SYSD_START_BYTE_B,
    192,
    168,
    1,
    24,
    SYSD_CPU_TEMP,
    SYSD_TYPE_FLOAT,
    0x42,
    0x05,
    0x99,
    0x9a,
};

/** @brief serial data top publish */
// TODO FIXME BUG TODO:!!!
//  make this more general purpose to be used outside of this project. force
//  project specific code into the caller and outside of this file!
//
proto_frame_t *serialize(uint8_t           telemetry_code,
                         proto_datatypes_e data_type,
                         void             *data,
                         const char       *destination_ip,
                         uint32_t         *out_len) {
    // Allocate memory for proto_frame
    proto_frame_t *proto_frame = (proto_frame_t *)malloc(sizeof(proto_frame_t));
    if (!proto_frame) {
        perror("Error allocating memory for proto_frame");
        exit(EXIT_FAILURE);
    }

    // Allocate buffer for the maximum message size
    proto_frame->buffer = (uint8_t *)malloc(SYSD_MAX_MESSAGE_SIZE);
    if (!proto_frame->buffer) {
        free(proto_frame);
        perror("Error allocating memory for proto_frame->buffer");
        exit(EXIT_FAILURE);
    }

    uint32_t offset = 0;

    // Add start bytes
    proto_frame->buffer[offset++] = SYSD_START_BYTE_A;
    proto_frame->buffer[offset++] = SYSD_START_BYTE_B;

    // Add destination IPv4 address
    uint32_t dest_ip = inet_addr(destination_ip);
    memcpy(proto_frame->buffer + offset, &dest_ip, sizeof(dest_ip));
    offset += sizeof(dest_ip);

    // Add telemetry code and data type
    proto_frame->buffer[offset++] = telemetry_code;
    proto_frame->buffer[offset++] = data_type;

    // Add data based on the type
    switch (data_type) {
    case SYSD_TYPE_UINT8: {
        proto_frame->buffer[offset++] = *(uint8_t *)data;
        break;
    }

    case SYSD_TYPE_UINT16: {
        memcpy(proto_frame->buffer + offset, data, sizeof(uint16_t));
        offset += sizeof(uint16_t);
        break;
    }

    case SYSD_TYPE_UINT32: {
        memcpy(proto_frame->buffer + offset, data, sizeof(uint32_t));
        offset += sizeof(uint32_t);
        break;
    }
    case SYSD_TYPE_UINT64: {
        memcpy(proto_frame->buffer + offset, data, sizeof(uint64_t));
        offset += sizeof(uint64_t);
        break;
    }

    case SYSD_TYPE_FLOAT: {
        uint8_t float_data[4];
        sysd_pack_float(*(float *)data, float_data);
        memcpy(proto_frame->buffer + offset, float_data, sizeof(float_data));
        offset += sizeof(float_data);
        break;
    }
    case SYSD_TYPE_DOUBLE: {
        uint8_t double_data[8];
        sysd_pack_double(*(double *)data, double_data);
        memcpy(proto_frame->buffer + offset, double_data, sizeof(double_data));
        offset += sizeof(double_data);
        break;
    }

    default:
        // Handle unknown data type
        free(proto_frame->buffer);
        free(proto_frame);
        perror("Unknown data type");
        exit(EXIT_FAILURE);
    }

    // Set the actual length of the serialized data
    proto_frame->length = offset;
    *out_len            = offset;

    return proto_frame;
}

/** @brief function to deserialize frame data */
proto_frame_t *deserialize(const uint8_t *buffer, uint16_t buffer_size) {
    for (uint8_t i = 0; i < buffer_size; i++) {
        printf("0x%02X ", buffer[i]);
    }
    printf("\n");
}

/** @brief initialize message queue */
void init_queue(proto_queue_t *queue) {
    queue->front = queue->rear = NULL;
}

/** @brief add frame to the queue */
void enqueue(proto_queue_t *queue, proto_frame_t *frame) {
    proto_queue_mesg_t *temp =
        (proto_queue_mesg_t *)malloc(sizeof(proto_queue_mesg_t));

    temp->proto_frame = frame;
    temp->next_frame  = NULL;

    if (queue->rear == NULL) {
        queue->front = queue->rear = temp;
        printf("queue->rear is NULL... setting front & rear = temp\n");
        return;
    }
    queue->rear->next_frame = temp;
    queue->rear             = temp;
}

/** @brief remove frame from the queue */
proto_frame_t *dequeue(proto_queue_t *queue) {
    if (queue->front == NULL) {
        printf("queue->front is NULL...\n");
        return NULL;
    }
    proto_queue_mesg_t *temp  = queue->front;
    proto_frame_t      *frame = temp->proto_frame;
    queue->front              = queue->front->next_frame;

    if (queue->front == NULL) {
        printf("queue->front is STILL NULL...\n");
        queue->rear = NULL;
    }
    free(temp);
    return frame;
}

int queue_status(proto_queue_t *queue) {
    return queue->front == NULL;
}

/** @brief publish sysd telemetry data */
int sysd_publish_telemetry(sysd_telemetry_t *telemetry) {
    int ret = 0;
    int len = SYSD_MAX_MESSAGE_SIZE;
    // message queue
    proto_queue_t proto_queue;
    init_queue(&proto_queue);

    // serial packet for cpu temp
    proto_frame_t *proto_frame;
    proto_frame = serialize(SYSD_CPU_TEMP,
                            SYSD_TYPE_FLOAT,
                            &telemetry->cpu_temp,
                            "192.168.1.10",
                            &len);
    // add frame to the message queue
    enqueue(&proto_queue, proto_frame);

    while (!queue_status(&proto_queue)) {
        proto_frame_t *frame = dequeue(&proto_queue);
        if (!frame) {
            perror("Failed to dequeue frame");
            return -1;
        }

        printf("data after serialization: ");
        for (uint32_t i = 0; i < frame->length; i++) {
            printf("0x%02X ", (unsigned char)frame->buffer[i]);
        }
        printf("\n");

        free(frame->buffer);
        free(frame);
    }

    /*
    printf("Serialized data (hex): ");
    for (uint32_t i = 0; i < len; i++) {
        printf("0x%02X ", proto_frame->buffer[i]);
    }
    printf("\n");
    */

    return ret;
}

/** @brief subscribe for sysd telemetry data */
int sysd_subscribe_telemetry(sysd_telemetry_t *telemetry) {
    int ret = 0;

    // populate telemetry struct with received information

    return ret;
}
