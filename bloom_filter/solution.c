#include "lib.h"

#include <assert.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

bitset_t* bitset_create(size_t bits) {
  bitset_t* bs = malloc(sizeof(bitset_t));
  if (bs == NULL) return NULL;

  bs->bits = bits;
  size_t bytes = (bits + 7) / 8; // round up to next byte
  bs->data = calloc(bytes, sizeof(uint8_t));
  if (bs->data == NULL) {
    free(bs);
    return NULL;
  }

  return bs;
}

void bitset_free(bitset_t* bs) {
  free(bs->data);
  free(bs);
}

size_t bitset_size(const bitset_t* bs) {
  return bs->bits;
}

bool bitset_get(const bitset_t* bs, size_t index) {
  assert(index < bs->bits);
  // each element is 8 bits -> get array element, then mask to get bit
  return bs->data[index / 8] & (1 << (index % 8));
}

void bitset_set(bitset_t* bs, size_t index, bool value) {
  assert(index < bs->bits);
  if (value)
    bs->data[index / 8] = bs->data[index / 8] | (1 << (index % 8));
  else
    bs->data[index / 8] = bs->data[index / 8] & ~(1 << (index % 8));
}

void bitset_clear(bitset_t* bs) {
  size_t bytes = (bs->bits + 7) / 8;
  memset(bs->data, 0, bytes);
}

bloom_filter_t* bloom_filter_create(size_t bits) {
  bloom_filter_t* bf = malloc(sizeof(bloom_filter_t));
  bitset_t* bs = bitset_create(bits);
  if (bf == NULL || bs == NULL) {
    free(bf);
    bitset_free(bs);
    return NULL;
  }

  bf->bitset = bs;
  return bf;
}

void bloom_filter_free(bloom_filter_t* bf) {
  bitset_free(bf->bitset);
  free(bf);
}

void bloom_filter_add(bloom_filter_t* bf, const void* data, size_t size) {
  // for each bitset index, use a different hash
  for (size_t i = 0; i < NUM_HASHES; i++) {
    size_t index = hash(data, size, i) % bitset_size(bf->bitset);
    bitset_set(bf->bitset, index, true);
  }
}

bool bloom_filter_contains(const bloom_filter_t* bf, const void* data,
                           size_t size) {
  for (size_t i = 0; i < NUM_HASHES; i++) {
    size_t index = hash(data, size, i) % bitset_size(bf->bitset);
    if (!bitset_get(bf->bitset, index)) return false;
  }
  return true;
}
