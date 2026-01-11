#include "lib.h"

#include <assert.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// You might find a struct `hashmap_node` useful
struct hashmap_node {
  void* key;
  void* value;
  struct hashmap_node* next;
};

typedef struct hashmap_node hashmap_node_t;

struct hashmap {
  hashmap_node_t** buckets;
  size_t num_buckets;
  size_t key_size;
  size_t value_size;
  size_t size;  // current number of elements
};

struct hashmap_iterator {
  const hashmap_t* map;
  size_t bucket_idx;        // current bucket
  hashmap_node_t* current;  // current node in bucket
};

void hashmap_free_bucket(hashmap_t* map, size_t bucket_idx) {
  hashmap_node_t* curr = map->buckets[bucket_idx];
  while (curr) {
    hashmap_node_t* next = curr->next;
    free(curr->value);
    free(curr->key);
    free(curr);
    --map->size;
    curr = next;
  }
  map->buckets[bucket_idx] = NULL;
}

hashmap_t* hashmap_create(size_t num_buckets, size_t key_size,
                          size_t value_size) {
  // special case: zero buckets
  if (num_buckets == 0) return NULL;

  hashmap_t* map = malloc(sizeof(hashmap_t));
  if (!map) return NULL;

  map->buckets = calloc(num_buckets, sizeof(hashmap_node_t*));
  if (!map->buckets) {
    free(map);
    return NULL;
  }

  map->num_buckets = num_buckets;
  map->key_size = key_size;
  map->value_size = value_size;
  map->size = 0;
  return map;
}

void hashmap_free(hashmap_t* map) {
  if (!map) return;

  for (size_t i = 0; i < map->num_buckets; i++) hashmap_free_bucket(map, i);

  free(map->buckets);
  free(map);
}

bool hashmap_put(hashmap_t* map, void* key, void* value) {
  hash_t hash_key = hash(key, map->key_size);
  size_t index = hash_key % map->num_buckets;

  // if key already exists, replace value
  hashmap_node_t* curr = map->buckets[index];
  while (curr) {
    if (memcmp(curr->key, key, map->key_size) == 0) {
      memcpy(curr->value, value, map->value_size);
      return true;
    }
    curr = curr->next;
  }

  // else, generate new node and put at head of bucket
  hashmap_node_t* node = NULL;

  node = malloc(sizeof(hashmap_node_t));
  if (!node) goto fail;

  node->key = malloc(map->key_size);
  if (!node->key) goto fail;

  node->value = malloc(map->value_size);
  if (!node->value) goto fail;

  memcpy(node->key, key, map->key_size);
  memcpy(node->value, value, map->value_size);

  node->next = map->buckets[index];
  map->buckets[index] = node;
  ++map->size;
  return true;

fail:
  if (node) {
    free(node->value);
    free(node->key);
    free(node);
  }
  return false;
}

bool hashmap_get(const hashmap_t* map, const void* key, void** out_value) {
  hash_t hash_key = hash(key, map->key_size);
  size_t index = hash_key % map->num_buckets;

  hashmap_node_t* curr = map->buckets[index];
  while (curr) {
    if (memcmp(curr->key, key, map->key_size) == 0) {
      *out_value = curr->value;
      return true;
    }
    curr = curr->next;
  }
  return false;
}

bool hashmap_contains(const hashmap_t* map, const void* key) {
  void* tmp;
  return hashmap_get(map, key, &tmp);
}

void hashmap_remove(hashmap_t* map, const void* key) {
  hash_t hash_key = hash(key, map->key_size);
  size_t index = hash_key % map->num_buckets;

  // case: bucket empty
  if (!map->buckets[index]) return;

  // case: node is head
  if (memcmp(map->buckets[index]->key, key, map->key_size) == 0) {
    hashmap_node_t* curr = map->buckets[index];
    map->buckets[index] = curr->next;
    free(curr->value);
    free(curr->key);
    free(curr);
    --map->size;
    return;
  }

  // else, search node in bucket
  hashmap_node_t* prev = map->buckets[index];
  while (prev->next && memcmp(prev->next->key, key, map->key_size) != 0)
    prev = prev->next;

  if (!prev->next) return;
  hashmap_node_t* curr = prev->next;
  prev->next = curr->next;
  free(curr->value);
  free(curr->key);
  free(curr);
  --map->size;
}

size_t hashmap_size(const hashmap_t* map) {
  return map->size;
}

void hashmap_clear(hashmap_t* map) {
  for (size_t i = 0; i < map->num_buckets; i++) hashmap_free_bucket(map, i);
  assert(map->size == 0 && "hashmap not empty");
}

float hashmap_load_factor(const hashmap_t* map) {
  return (float)map->size / map->num_buckets;
}

hashmap_iterator_t* hashmap_iterator_create(const hashmap_t* map) {
  hashmap_iterator_t* iter = malloc(sizeof(hashmap_iterator_t));
  if (!iter) return NULL;

  iter->map = map;
  iter->bucket_idx = 0;
  iter->current = NULL;  // start before first bucket
  return iter;
}
void hashmap_iterator_free(hashmap_iterator_t* iter) {
  free(iter);
}

bool hashmap_iterator_next(hashmap_iterator_t* iter) {
  // no current node
  if (!iter->current) {
    // iterator not yet started, find first node
    while (iter->bucket_idx < iter->map->num_buckets &&
           !iter->map->buckets[iter->bucket_idx])
      ++iter->bucket_idx;
    if (iter->bucket_idx == iter->map->num_buckets) return false;

    iter->current = iter->map->buckets[iter->bucket_idx];
    return true;
  }
  // next node in bucket exists
  if (iter->current->next) {
    iter->current = iter->current->next;
    return true;
  }
  // move to next bucket, if exists
  ++iter->bucket_idx;
  while (iter->bucket_idx < iter->map->num_buckets &&
         !iter->map->buckets[iter->bucket_idx]) {
    ++iter->bucket_idx;
  }
  if (iter->bucket_idx < iter->map->num_buckets) {
    iter->current = iter->map->buckets[iter->bucket_idx];
    return true;
  }
  return false;
}

const void* hashmap_iterator_key(const hashmap_iterator_t* iter) {
  if (!iter->current) return NULL;
  return iter->current->key;
}

void* hashmap_iterator_value(const hashmap_iterator_t* iter) {
  if (!iter->current) return NULL;
  return iter->current->value;
}
