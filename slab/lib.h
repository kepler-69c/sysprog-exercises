#ifndef LIB_H
#define LIB_H

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct slab_allocator slab_allocator_t;
typedef struct slab_cache slab_cache_t;

/**
 * Slabs should be allocated in PAGE_SIZE chunks.
 */
static inline const size_t PAGE_SIZE = 4096;

slab_allocator_t *slab_allocator_create(void);
void slab_allocator_free(slab_allocator_t *allocator);

slab_cache_t *slab_cache_create(slab_allocator_t *allocator, size_t obj_size,
                                size_t alignment);
void slab_cache_free(slab_cache_t *cache);

void *slab_alloc(slab_cache_t *cache);
void slab_free(slab_cache_t *cache, void *obj);

#endif  // LIB_H
