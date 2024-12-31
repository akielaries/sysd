#include "../libsysd/utils.h"
#include "../libsysd/system.h"
#include <stdint.h>

/** @brief breakup input_val into least and most significant bits */
/*
void sysd_lsbmsb(const uint16_t input_val, uint8_t output_array[SYSD_ID_SIZE]) {
  output_array[SYSD_OFFSET_LSB] = input_val & 0xFF;
  output_array[SYSD_OFFSET_MSB] = (input_val >> 8) & 0xFF;
}
*/

/** @brief represent floating point value as 4 byte hex values */
void sysd_pack_float(float input_val, uint8_t *output_array) {
  uint8_t *float_data = (uint8_t *)&input_val;
  // Copy packed big-endian hex sequence to output array
#ifdef HEX_DUMP
  printf("\nINPUT FLOAT: %f\nPACKED FLOAT: \n", input_val);
#endif
  for (int i = 0; i < 4; i++) {
    output_array[i] = float_data[3 - i];
#ifdef HEX_DUMP
    printf("0x%02X ", output_array[i]);
#endif
  }
}

/** @brief represent double value as 8 byte hex values in big-endian */
void sysd_pack_double(double input_val, uint8_t *output_array) {
  uint8_t *double_data = (uint8_t *)&input_val;
  // Copy packed big-endian hex sequence to output array
#ifdef HEX_DUMP
  printf("\nINPUT DOUBLE: %f\nPACKED DOUBLE: \n", input_val);
#endif
  for (int i = 0; i < 8; i++) {
    output_array[i] = double_data[7 - i];
#ifdef HEX_DUMP
    printf("0x%02X ", output_array[i]);
#endif
  }
}

void sysd_unpack_float(const uint8_t *input_array, float *output_val) {
  uint8_t *float_data = (uint8_t *)output_val;
  for (int i = 0; i < 4; i++) {
    float_data[3 - i] = input_array[i];
  }
}

void sysd_unpack_double(const uint8_t *input_array, double *output_val) {
  uint8_t *double_data = (uint8_t *)output_val;
  for (int i = 0; i < 8; i++) {
    double_data[7 - i] = input_array[i];
  }
}

/*int sysd_error_handle(xcd_error_t *error, int8_t error_code) {
    error->last_error_code = error_code; // most recent error code
    // if code is not successful
    if (error_code != 0) {
        error->error_count++; // increment error count
        // if there was a previous error
        if (error->last_error) {
            error->consecutive_error_count++; // increment consecutive error
                                              // count
        }
        error->last_error = true; // set last_error flag to true
    }

    // if code is successful
    else {
        error->consecutive_error_count = 0;     // no consecutive errors
        error->last_error              = false; // set last_error flag to false
    }
    // return the error code
    return error_code;
}*/
