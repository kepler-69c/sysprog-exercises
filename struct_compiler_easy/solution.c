#include "lib.h"

#include <stdalign.h>

size_t compile(size_t num_fields, struct field* fields) {
  size_t offset = 0;
  size_t largest = 1;

  for (size_t i = 0; i < num_fields; ++i) {
    struct field* entry = &fields[i];
    size_t padding;

    switch (entry->type) {
      case FIELD_TYPE_CHAR:
        entry->offset = offset;
        offset += 1;
        // largest is size of largest element in struct
        largest = (largest > 1) ? largest : 1;
        break;
      case FIELD_TYPE_INT:
      case FIELD_TYPE_FLOAT:
        // how much is needed to be 4-byte aligned
        padding = (4 - (offset % 4)) % 4;
        entry->offset = offset + padding;
        offset += padding + 4;
        largest = (largest > 4) ? largest : 4;
        break;
      default:  // double, pointer
        padding = (8 - (offset % 8)) % 8;
        entry->offset = offset + padding;
        offset += padding + 8;
        largest = 8;
        break;
    }
  }

  // struct size must be multiple of largest element
  size_t padding = (largest - (offset % largest)) % largest;
  return offset + padding;
}
