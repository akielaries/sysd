#include <stdlib.h>
#include <stdint.h>

#include "proto_queue.h"
#include "proto.h"

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
        return;
    }
    queue->rear->next_frame = temp;
    queue->rear             = temp;
}

/** @brief remove frame from the queue */
proto_frame_t *dequeue(proto_queue_t *queue) {
    if (queue->front == NULL) {
        return NULL;
    }
    proto_queue_mesg_t *temp  = queue->front;
    proto_frame_t      *frame = temp->proto_frame;
    queue->front              = queue->front->next_frame;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(temp);
    return frame;
}

int queue_status(proto_queue_t *queue) {
    return queue->front == NULL;
}

