#ifndef LIB_H
#define LIB_H

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
  void *data;
  size_t capacity;
  size_t item_size;

  size_t index;
} stack_t;

void sstack_init(stack_t *stack, size_t capacity, size_t item_size);
#define SSTACK_INIT(type, stack, cap) sstack_create(stack, cap, sizeof(type))
void sstack_destroy(stack_t *stack);
bool sstack_is_empty(const stack_t *stack);
bool sstack_is_full(const stack_t *stack);
bool sstack_push(stack_t *stack, const void *item);
bool sstack_pop(stack_t *stack, void *item);
size_t sstack_size(const stack_t *stack);
size_t sstack_capacity(const stack_t *stack);
void sstack_clear(stack_t *stack);

#endif  // LIB_H
