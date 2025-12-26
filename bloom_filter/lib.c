#include <assert.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"

bitset_t *bitset_create(size_t bits) {
  return NULL;
}

void bitset_free(bitset_t *bs) {}

size_t bitset_size(const bitset_t *bs) {
  return 0;
}

bool bitset_get(const bitset_t *bs, size_t index) {
  return false;
}

void bitset_set(bitset_t *bs, size_t index, bool value) {}

void bitset_clear(bitset_t *bs) {}

bloom_filter_t *bloom_filter_create(size_t bits) {
  return NULL;
}

void bloom_filter_free(bloom_filter_t *bf) {}

void bloom_filter_add(bloom_filter_t *bf, const void *data, size_t size) {}

bool bloom_filter_contains(const bloom_filter_t *bf, const void *data,
                           size_t size) {
  return false;
}
