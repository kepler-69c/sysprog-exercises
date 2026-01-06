#include <assert.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "lib.h"

// You might want to create a `struct merkle_tree_node` to represent nodes

struct merkle_tree {
  // Fill in
};

merkle_tree_t *merkle_tree_create(const void *data, size_t data_size) {
  return NULL;
}

void merkle_tree_free(merkle_tree_t *tree) {}

hash_t merkle_tree_root_hash(merkle_tree_t *tree) {
  return 0;
}

void merkle_tree_rehash(merkle_tree_t *tree) {}

size_t merkle_tree_size(const merkle_tree_t *tree) {
  return 0;
}

int64_t merkle_tree_first_diff(const merkle_tree_t *a, const merkle_tree_t *b) {
  return -1;
}
bool merkle_tree_verify(const merkle_tree_t *expected_tree, const void *data,
                        size_t data_size) {
  return true;
}
