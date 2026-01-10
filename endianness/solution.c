#include "lib.h"

#include <assert.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void swap(uint8_t* a, uint8_t* b) {
  uint8_t tmp = *a;
  *a = *b;
  *b = tmp;
}

endianness_t get_system_endianness() {
  union {
    uint16_t i;
    uint8_t b[2];
  } bint = {0x0100};

  if (bint.b[0] == 0x01) {
    return BIG_ENDIAN;
  } else {
    return LITTLE_ENDIAN;
  }
}

uint16_t swap_u16(uint16_t val) {
  union {
    uint16_t i;
    uint8_t b[2];
  } bint = {val};

  swap(&bint.b[0], &bint.b[1]);
  return bint.i;
}

uint32_t swap_u32(uint32_t val) {
  union {
    uint32_t i;
    uint8_t b[4];
  } bint = {val};

  swap(&bint.b[0], &bint.b[3]);
  swap(&bint.b[1], &bint.b[2]);
  return bint.i;
}

uint64_t swap_u64(uint64_t val) {
  union {
    uint64_t i;
    uint8_t b[8];
  } bint = {val};

  swap(&bint.b[0], &bint.b[7]);
  swap(&bint.b[1], &bint.b[6]);
  swap(&bint.b[2], &bint.b[5]);
  swap(&bint.b[3], &bint.b[4]);
  return bint.i;
}
