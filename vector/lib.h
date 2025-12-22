#ifndef LIB_H
#define LIB_H

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

/**
 * A dynamic array (vector) of integers.
 * Size refers to the number of elements currently stored.
 * Capacity refers to the allocated storage size: capacity >= size. Usually
 * capacity is larger than size to accomodate for future growth without having
 * to reallocate on every insertion.
 */
typedef struct {
  size_t size;
  size_t capacity;
  int *data;
} vector_t;

/**
 * Initialize an empty vector with the given initial capacity.
 */
vector_t *vector_create(size_t initial_capacity);
static inline vector_t *vector_create_empty() {
  return vector_create(0);
}
/**
 * Free all memory associated with the vector.
 */
void vector_free(vector_t *vec);

/**
 * Retrieve the element at the given index.
 * No bounds checking is performed.
 */
int vector_at(const vector_t *vec, size_t index);
/**
 * Retrieve the first element
 */
int vector_front(const vector_t *vec);
/**
 * Retrieve the last element
 */
int vector_back(const vector_t *vec);
/**
 * Get a raw pointer to the underlying data array.
 */
int *vector_data(const vector_t *vec);

/**
 * Check if the vector is empty.
 */
bool vector_empty(const vector_t *vec);
/**
 * Get the current size (number of elements) of the vector.
 */
size_t vector_size(const vector_t *vec);
/**
 * Get the current capacity of the vector.
 */
size_t vector_capacity(const vector_t *vec);
/**
 * Ensure the vector has at least the given capacity.
 * If new_capacity is <= current capacity, no action is taken.
 */
void vector_reserve(vector_t *vec, size_t new_capacity);
/**
 * Reduce the capacity of the vector to fit its current size.
 */
void vector_shrink_to_fit(vector_t *vec);

/**
 * Clear all elements from the vector.
 * Capacity remains unchanged, size becomes zero.
 */
void vector_clear(vector_t *vec);
/**
 * Insert value before the given index.
 * Index must be in the range [0, size]. If index=size, the value is appended.
 */
void vector_insert_before(vector_t *vec, size_t index, int value);
/**
 * Erase the element at the given index. Every subsequent element is shifted
 * left.
 */
void vector_erase(vector_t *vec, size_t index);
/**
 * Append value to the end of the vector.
 */
void vector_push_back(vector_t *vec, int value);
/**
 * Remove the last element of the vector.
 */
void vector_pop_back(vector_t *vec);
/**
 * Resize the vector to new_size.
 * If new_size > current size, new elements are initialized to default_value.
 * If new_size < current size, elements are truncated.
 */
void vector_resize(vector_t *vec, size_t new_size, int default_value);

/**
 * Find the first occurrence of value in the vector.
 * Returns the index of the found element, or -1 if not found.
 */
ssize_t vector_find(const vector_t *vec, int value);
/**
 * Check if the vector contains the given value.
 */
bool vector_contains(const vector_t *vec, int value);
/**
 * Reverse the order of elements in the vector.
 */
void vector_reverse(vector_t *vec);
/**
 * Sort the vector in ascending order.
 */
void vector_sort(vector_t *vec);

static inline void vector_print(const vector_t *vec) {
  printf("vector(size=%zu, capacity=%zu, data=[", vec->size, vec->capacity);
  for (size_t i = 0; i < vec->size; i++) {
    printf("%d", vec->data[i]);
    if (i < vec->size - 1) {
      printf(", ");
    }
  }
  printf("])\n");
  fflush(stdout);
}

#endif  // LIB_H
