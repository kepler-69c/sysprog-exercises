#ifndef LIB_H
#define LIB_H

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef uint64_t hash_t;

static inline hash_t hash(const void *data, size_t size) {
  const unsigned char *p = (const unsigned char *)data;
  uint64_t h = 14695981039346656037ULL;

  for (size_t i = 0; i < size; i++) {
    h ^= p[i];
    h *= 1099511628211ULL;
  }

  return h;
}

typedef struct hashmap hashmap_t;
typedef struct hashmap_iterator hashmap_iterator_t;

hashmap_t *hashmap_create(size_t num_buckets, size_t key_size,
                          size_t value_size);
#define HASHMAP_CREATE(num_buckets, key_type, value_type) \
  hashmap_create(num_buckets, sizeof(key_type), sizeof(value_type))

void hashmap_free(hashmap_t *map);

bool hashmap_put(hashmap_t *map, void *key, void *value);
bool hashmap_get(const hashmap_t *map, const void *key, void **out_value);
bool hashmap_contains(const hashmap_t *map, const void *key);
void hashmap_remove(hashmap_t *map, const void *key);
size_t hashmap_size(const hashmap_t *map);
void hashmap_clear(hashmap_t *map);
float hashmap_load_factor(const hashmap_t *map);

hashmap_iterator_t *hashmap_iterator_create(const hashmap_t *map);
void hashmap_iterator_free(hashmap_iterator_t *iter);
bool hashmap_iterator_next(hashmap_iterator_t *iter);
const void *hashmap_iterator_key(const hashmap_iterator_t *iter);
void *hashmap_iterator_value(const hashmap_iterator_t *iter);

#endif  // LIB_H
