#ifndef LIB_H
#define LIB_H

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
  size_t bits;
  uint8_t *data;
} bitset_t;

bitset_t *bitset_create(size_t bits);
void bitset_free(bitset_t *bs);
size_t bitset_size(const bitset_t *bs);

bool bitset_get(const bitset_t *bs, size_t index);
void bitset_set(bitset_t *bs, size_t index, bool value);
void bitset_clear(bitset_t *bs);

typedef struct {
  bitset_t *bitset;
} bloom_filter_t;

#define NUM_HASHES 8

typedef uint64_t hash_t;

static inline hash_t hash(const void *data, size_t size, size_t index) {
  assert(index < NUM_HASHES);
  const unsigned char *p = (const unsigned char *)data;

  uint64_t h1 = 14695981039346656037ULL;
  for (size_t i = 0; i < size; i++) {
    h1 ^= p[i];
    h1 *= 1099511628211ULL;
  }

  uint64_t h2 = h1;
  h2 ^= h2 >> 33;
  h2 *= 0xff51afd7ed558ccdULL;
  h2 ^= h2 >> 33;
  h2 *= 0xc4ceb9fe1a85ec53ULL;
  h2 ^= h2 >> 33;

  return h1 + (uint64_t)index * h2;
}

bloom_filter_t *bloom_filter_create(size_t bits);
void bloom_filter_free(bloom_filter_t *bf);
void bloom_filter_add(bloom_filter_t *bf, const void *data, size_t size);
bool bloom_filter_contains(const bloom_filter_t *bf, const void *data,
                           size_t size);

#endif  // LIB_H
