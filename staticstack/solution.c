#include "lib.h"

#include <assert.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void sstack_init(stack_t* stack, size_t capacity, size_t item_size) {
  stack->data = malloc(capacity * item_size);
  // check whether malloc was successfull
  bool success = stack->data != NULL;

  stack->capacity = success ? capacity : 0;
  stack->item_size = success ? item_size : 0;
  stack->index = 0;
}

void sstack_destroy(stack_t* stack) {
  free(stack->data);  // free(NULL) is safe
  stack->data = NULL;
  stack->capacity = 0;
  stack->item_size = 0;
  stack->index = 0;
}

bool sstack_is_empty(const stack_t* stack) {
  return stack->index == 0;
}

bool sstack_is_full(const stack_t* stack) {
  return stack->index == stack->capacity;
}

bool sstack_push(stack_t* stack, const void* item) {
  if (sstack_is_full(stack)) return false;

  // copy item into stack->data at index position
  memcpy((char*)stack->data + stack->index * stack->item_size, item,
         stack->item_size);
  ++(stack->index);
  return true;
}

bool sstack_pop(stack_t* stack, void* item) {
  if (sstack_is_empty(stack)) return false;

  --(stack->index);
  memcpy(item, (char*)stack->data + stack->index * stack->item_size,
         stack->item_size);
  return true;
}

size_t sstack_size(const stack_t* stack) {
  return stack->index;
}

size_t sstack_capacity(const stack_t* stack) {
  return stack->capacity;
}

void sstack_clear(stack_t* stack) {
  stack->index = 0;
}
