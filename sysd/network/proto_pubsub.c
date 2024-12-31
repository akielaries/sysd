#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../../libsysd/system.h"
#include "../../libsysd/utils.h"
#include "proto_pubsub.h"
#include "proto_queue.h"

int sysd_publish_telemetry(sysd_telemetry_t *telemetry,
                           char *dest_ip,
                           uint16_t port) {
  int ret = 0;
  int len = SYSD_MAX_MESSAGE_SIZE;
  // char dest_ip[] = "127.0.0.1";

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

  printf("QUEUE SIZE: %d\n", queue_size(&proto_queue));
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
  server_addr.sin_port        = htons(port);        // Use any port
  server_addr.sin_addr.s_addr = inet_addr(dest_ip); // localhost

  // Print out serialized data before sending
  printf("Serialized data being sent: \n");
  // Send each serialized frame over UDP to localhost
  while (!queue_status(&proto_queue)) {
    proto_frame_t *frame = dequeue(&proto_queue);
    if (!frame) {
      perror("Failed to dequeue frame");
      return -1;
    }

    for (int i = 0; i < frame->length; i++) {
      printf("0x%02X ", frame->buffer[i]);
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
int sysd_subscribe_telemetry(sysd_telemetry_t *telemetry, uint16_t port) {
  printf("starting subscribe function...\n");
  int sockfd;
  struct sockaddr_in server_addr, client_addr;
  uint8_t buffer[SYSD_MAX_MESSAGE_SIZE];
  socklen_t addr_len = sizeof(client_addr);

  // Create a UDP socket
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket creation failed");
    return -1;
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family      = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port        = htons(port);

  // Bind the socket to listen on localhost
  if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) <
      0) {
    perror("bind failed");
    return -1;
  }
  // Receive multiple frames
  int total_bytes = 0;
  while (1) {
    printf("GETTING DATA\n");
    // TODO
    // Receive serialized data, this will cause the while loop to wait
    // infinitely, there should maybe be a timeout and retry mechanism here
    int n = recvfrom(sockfd,
                     (uint8_t *)buffer,
                     SYSD_MAX_MESSAGE_SIZE,
                     0,
                     (struct sockaddr *)&client_addr,
                     &addr_len);
    printf("RECEIVED %d BYTES\n", n);
    if (n <= 0) {
      printf("recvfrom failed or no more data");
      break;
    }

    // printf("Received bytes:\n");
    // decode the incoming messages
    deserialize(&buffer, n, NULL);

    for (int i = 0; i < n; i++) {
      printf("0x%02X ", (uint8_t)buffer[i]);
    }
    printf("\n");

    total_bytes += n;

    // if (n < SYSD_MAX_MESSAGE_SIZE) {
    //     break;
    // }
    // TODO: this could be an internal telemetry/log point
    printf("Total bytes received: %d\n", total_bytes);
  }

  // Close socket
  close(sockfd);

  return 0;
}
