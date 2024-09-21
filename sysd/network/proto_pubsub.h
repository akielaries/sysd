#ifndef PROTO_PUBSUB_H
#define PROTO_PUBSUB_H

#include "../../libsysd/system.h"
#include <stdint.h>

int sysd_publish_telemetry(sysd_telemetry_t *telemetry, char *dest_ip, uint16_t port);

int sysd_subscribe_telemetry(sysd_telemetry_t *telemetry, uint16_t port);

#endif
