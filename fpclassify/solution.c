#include "lib.h"

#include <assert.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

fp_class_t fpclassify(float value) {
  union {
    float f;
    uint32_t u;
  } bint = {value};
  
  // NOTE: will only work on little-endian architecture
  uint32_t exp = (bint.u >> 23) & 0xff;
  uint32_t mantissa = bint.u & 0x7fffff;

  // Infinity (+/-)
  if(exp == 0xff && mantissa == 0x0)
    return FP_INFINITE;
  // Zero
  if(exp == 0x0 && mantissa == 0x0)
    return FP_ZERO;
  // NaN
  if(exp == 0xff && mantissa != 0x0)
    return FP_NAN;
  // denormalized (close to 0)
  if(exp == 0x0)
    return FP_SUBNORMAL;
  // else, it is a normalized number
  return FP_NORMAL;
}
