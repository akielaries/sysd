#include "util.h"

/** @brief breakup input_val into least and most significant bits */
void sysd_lsbmsb(const uint16_t input_val, uint8_t output_array[SYSD_ID_SIZE]) {
  output_array[SYSD_OFFSET_LSB] = input_val & 0xFF;
  output_array[SYSD_OFFSET_MSB] = (input_val >> 8) & 0xFF;
}

/** @brief represent floating point value as 4 byte hex values */
void sysd_pack_float(float input_val, uint8_t *output_array) {
  uint8_t *float_data = (uint8_t *)&input_val;
  // copy packed little endian hex sequence to output array
  memcpy(output_array, float_data, SYSD_REAL_SIZE_BYTES);
}

