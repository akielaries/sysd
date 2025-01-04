#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../libsysd/system.h"
#include "../../libsysd/utils.h"
#include "proto.h"
#include "proto_check.h"
#include "proto_queue.h"

/** @brief serialize outgoing packets */
proto_frame_t *serialize(uint8_t telemetry_code,
                         proto_datatypes_e data_type,
                         void *data,
                         const char *destination_ip,
                         uint32_t *out_len) {
  // allocate memory for proto_frame
  proto_frame_t *proto_frame = (proto_frame_t *)malloc(sizeof(proto_frame_t));
  if (!proto_frame) {
    perror("Error allocating memory for proto_frame");
    exit(EXIT_FAILURE);
  }

  // allocate buffer for the maximum message size
  proto_frame->buffer = (uint8_t *)malloc(SYSD_MAX_MESSAGE_SIZE);
  if (!proto_frame->buffer) {
    free(proto_frame);
    perror("Error allocating memory for proto_frame->buffer");
    exit(EXIT_FAILURE);
  }

  uint32_t offset = 0;

  // add start bytes
  proto_frame->buffer[offset++] = SYSD_START_BYTE_A;
  proto_frame->buffer[offset++] = SYSD_START_BYTE_B;

  // add destination IPv4 address
  uint32_t dest_ip = inet_addr(destination_ip);
  memcpy(proto_frame->buffer + offset, &dest_ip, sizeof(dest_ip));
  offset += sizeof(dest_ip);

  // add telemetry code and data type
  proto_frame->buffer[offset++] = telemetry_code;
  proto_frame->buffer[offset++] = data_type;
  // size of payload
  //proto_frame->buffer[offset++] = sizeof(data);
  //printf("\n[!] payload size: %ld\n", sizeof(data));

  // the payload

  // add data based on the type
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
      // handle unknown data type
      free(proto_frame->buffer);
      free(proto_frame);
      perror("Unknown data type");
      exit(EXIT_FAILURE);
  }
  /*
  // calculate and add CRC16
  uint16_t crc16 = get_crc16(proto_frame->buffer, offset);
  memcpy(proto_frame->buffer + offset, &crc16, sizeof(crc16));
  offset += sizeof(crc16);

  // calculate and add checksum
  uint8_t checksum              = get_checksum(proto_frame->buffer, offset);
  proto_frame->buffer[offset++] = checksum;
  */
  // set the actual length of the serialized data
  proto_frame->length = offset;
  *out_len            = offset;

  return proto_frame;
}

/** @brief function to deserialize frame data */
void deserialize(const uint8_t *buffer,
                 uint16_t buffer_size,
                 sysd_telemetry_t *telemetry) {
  uint32_t offset = 0;

  printf("buffer contents: \n");
  for (uint16_t i = 0; i < buffer_size; i++) {
    printf("0x%X ", buffer[i]);
  }
  printf("\n\n");

  // verify start bytes
  if (buffer[SYSD_OFFSET_START_BYTE_A] != SYSD_START_BYTE_A ||
      buffer[SYSD_OFFSET_START_BYTE_B] != SYSD_START_BYTE_B) {
    printf("Unknown start bytes : 0x%X | 0x%X\n", buffer[SYSD_OFFSET_START_BYTE_A],
                                            buffer[SYSD_OFFSET_START_BYTE_B]);
    return;
  }
  printf("[+] Start bytes : 0x%X | 0x%X\n", buffer[SYSD_OFFSET_START_BYTE_A],
                                            buffer[SYSD_OFFSET_START_BYTE_B]);
  printf("[+] Dest IPv4   : %d.%d.%d.%d\n", buffer[SYSD_OFFSET_DEST_IPV4],
                                            buffer[SYSD_OFFSET_DEST_IPV4+1],
                                            buffer[SYSD_OFFSET_DEST_IPV4+2],
                                            buffer[SYSD_OFFSET_DEST_IPV4+3]);
  printf("[+] TLM code    : 0x%X\n", buffer[SYSD_OFFSET_TELEM_CODE]);
  printf("[+] Data type   : 0x%X\n", buffer[SYSD_OFFSET_DATA_TYPE_CODE]);

  // calculate and validate CRC16
  /*
  uint16_t received_crc16;
  memcpy(&received_crc16,
         buffer + buffer_size - sizeof(uint16_t) - 1,
         sizeof(received_crc16));
  uint16_t calculated_crc16 =
      get_crc16(buffer, buffer_size - sizeof(uint16_t) - 1);
  if (received_crc16 != calculated_crc16) {
      perror("CRC16 mismatch");
      exit(EXIT_FAILURE);
  }

  // calculate and validate checksum
  uint8_t received_checksum   = buffer[buffer_size - 1];
  uint8_t calculated_checksum = get_checksum(buffer, buffer_size - 1);
  if (received_checksum != calculated_checksum) {
      perror("Checksum mismatch");
      exit(EXIT_FAILURE);
  }
  */

  /*
  // deserialize data based on telemetry code and data type
  switch (telemetry_code) {
    case SYSD_CPU_LOAD:
      telemetry->cpu_load = *(double *)(buffer + offset);
      offset += sizeof(double);
      break;
    case SYSD_CPU_TEMP:
      telemetry->cpu_temp = *(float *)(buffer + offset);
      offset += sizeof(float);
      break;
    case SYSD_PROC_COUNT:
      telemetry->proc_count = *(uint16_t *)(buffer + offset);
      offset += sizeof(uint16_t);
      break;
    case SYSD_VRAM_TOTAL:
      telemetry->ram_info.vram_total = *(float *)(buffer + offset);
      offset += sizeof(float);
      break;
    case SYSD_VRAM_USED:
      telemetry->ram_info.vram_used = *(float *)(buffer + offset);
      offset += sizeof(float);
      break;
    case SYSD_VRAM_FREE:
      telemetry->ram_info.vram_free = *(float *)(buffer + offset);
      offset += sizeof(float);
      break;
    case SYSD_PRAM_TOTAL:
      telemetry->ram_info.pram_total = *(float *)(buffer + offset);
      offset += sizeof(float);
      break;
    case SYSD_PRAM_USED:
      telemetry->ram_info.pram_used = *(float *)(buffer + offset);
      offset += sizeof(float);
      break;
    case SYSD_PRAM_FREE:
      telemetry->ram_info.pram_free = *(float *)(buffer + offset);
      offset += sizeof(float);
      break;
    case SYSD_STRG_TOTAL:
      telemetry->ssd_info.storage_total = *(float *)(buffer + offset);
      offset += sizeof(float);
      break;
    case SYSD_STRG_USED:
      telemetry->ssd_info.storage_used = *(float *)(buffer + offset);
      offset += sizeof(float);
      break;
    case SYSD_STRG_FREE:
      telemetry->ssd_info.storage_free = *(float *)(buffer + offset);
      offset += sizeof(float);
      break;
    default:
      // handle unknown telemetry code
      perror("Unknown telemetry code");
      exit(EXIT_FAILURE);
  }
  */
}
