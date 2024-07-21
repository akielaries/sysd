#ifndef PROTO_PUBSUB_H
#define PROTO_PUBSUB_H

#include "../../libsysd/system.h"

int sysd_publish_telemetry(sysd_telemetry_t *telemetry);

int sysd_subscribe_telemetry(sysd_telemetry_t *telemetry);

#endif
