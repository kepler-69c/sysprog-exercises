#include "lib.h"

#include <assert.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void leb128_encode(const uint64_t* values, const size_t count, uint8_t* output,
                   size_t* output_size) {
  *output_size = 0;
  // for each number...
  for (size_t i = 0; i < count; ++i) {
    uint64_t tmp = values[i];
    do {
      uint8_t block = tmp & 0x7f;  // take 7 bits
      tmp >>= 7;
      if (tmp) block |= 1 << 7;  // set continuation flag if more bits follow
      output[(*output_size)++] = block;
    } while (tmp);
  }
}

void leb128_decode(const uint8_t* input, const size_t input_size,
                   uint64_t* values, size_t* values_count) {
  uint64_t tmp = 0;
  uint64_t offset = 0;
  *values_count = 0;

  // NOTE: theoretically, we should check at there are <= 10 blocks per value
  for (size_t i = 0; i < input_size; ++i) {
    uint8_t block = input[i];
    uint64_t data = block & 0x7f;  // first 7 bits
    uint8_t flag = (block >> 7);   // check continuation flag

    // last block
    if (flag == 0) {
      tmp += (data << offset);
      values[*values_count] = tmp;
      *values_count += 1;
      offset = 0;
      tmp = 0;
      continue;
    }

    // not last block
    tmp += (data << offset);
    offset += 7;
  }
}
