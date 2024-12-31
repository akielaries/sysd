#ifndef PROTO_QUEUE_H
#define PROTO_QUEUE_H

#include "proto.h"

/** @brief Struct for the message queue nodes */
typedef struct proto_queue_mesg_t {
  proto_frame_t *proto_frame;
  struct proto_queue_mesg_t *next_frame;
} proto_queue_mesg_t;

/** @brief Struct for the message of frames */
typedef struct proto_queue_t {
  proto_queue_mesg_t *front;
  proto_queue_mesg_t *rear;
} proto_queue_t;

void init_queue(proto_queue_t *queue);

void enqueue(proto_queue_t *queue, proto_frame_t *frame);

proto_frame_t *dequeue(proto_queue_t *queue);

int queue_status(proto_queue_t *queue);

int queue_size(proto_queue_t *queue);

#endif
