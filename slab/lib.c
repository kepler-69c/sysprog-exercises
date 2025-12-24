#include <assert.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"

// You'll probably need a `struct slab`
// struct slab {};

struct slab_cache {
  // Implement internal state
};

struct slab_allocator {
  // Implement internal state
};

slab_allocator_t *slab_allocator_create(void) {
  return NULL;
}

void slab_cache_free(slab_cache_t *cache) {}

void slab_allocator_free(slab_allocator_t *allocator) {}

slab_cache_t *slab_cache_create(slab_allocator_t *allocator, size_t obj_size,
                                size_t alignment) {
  return NULL;
}

void *slab_alloc(slab_cache_t *cache) {
  return NULL;
}

void slab_free(slab_cache_t *cache, void *obj) {}
