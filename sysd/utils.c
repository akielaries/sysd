#include "../libsysd/system.h"
#include "../libsysd/util.h"

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
    // copy packed little endian hex sequence to output array
    memcpy(output_array, float_data, SYSD_REAL_SIZE_BYTES);
}

int sysd_error_handle(xcd_error_t *error, int8_t error_code) {
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
}
