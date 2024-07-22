#include <gtest/gtest.h>

#if defined(__cplusplus)
extern "C" {
#endif

#include "../sysd/network/proto.h"
#include "../libsysd/system.h"

#if defined(__cplusplus)
};
#endif


TEST(SerializeTelemetryDataTest, ValidData) {
  uint8_t code = SYSD_CPU_TEMP;
  proto_datatypes_e dt = SYSD_TYPE_FLOAT;
  float temp = 32.3;
  const char* dest = "192.168.1.10";
  uint32_t len = SYSD_MAX_MESSAGE_SIZE;

  proto_frame_t *data;
  data = serialize(code,
                   dt,
                   &temp,
                   dest,
                   &len);

  ASSERT_NE(data, nullptr);
  ASSERT_NE(data->buffer, nullptr);
  ASSERT_GT(len, 0);
}

TEST(SerializeTelemetryDataTest, DissectFrame) {
  char valid_frame[] = {SYSD_START_BYTE_A, SYSD_START_BYTE_B, 
                        0xC0, 0xA8, 0x01, 0x0A,
                        SYSD_CPU_TEMP,
                        SYSD_TYPE_FLOAT,
                        0x42, 0x01, 0x33, 0x33,
                        0xA8, 0xFD, 0xD4,
                      };

  uint8_t code = SYSD_CPU_TEMP;
  proto_datatypes_e dt = SYSD_TYPE_FLOAT;
  float temp = 32.3;
  const char* dest = "192.168.1.10";
  uint32_t len = SYSD_MAX_MESSAGE_SIZE;

  proto_frame_t *data;
  data = serialize(code,
                   dt,
                   &temp,
                   dest,
                   &len);
  
  for (uint8_t i = 0; i < data->length; i++) {
    ASSERT_EQ(data->buffer[i], valid_frame[i]);
  }

  ASSERT_GT(len, 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

