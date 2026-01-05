#include <assert.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"

void sstack_init(stack_t *stack, size_t capacity, size_t item_size) {}

void sstack_destroy(stack_t *stack) {}

bool sstack_is_empty(const stack_t *stack) {
  return true;
}

bool sstack_is_full(const stack_t *stack) {
  return true;
}

bool sstack_push(stack_t *stack, const void *item) {
  return false;
}

bool sstack_pop(stack_t *stack, void *item) {
  return false;
}

size_t sstack_size(const stack_t *stack) {
  return 0;
}

size_t sstack_capacity(const stack_t *stack) {
  return 0;
}

void sstack_clear(stack_t *stack) {}
