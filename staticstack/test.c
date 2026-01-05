#include <assert.h>
#include <float.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../greatest.h"
#include "custom_tests.h"
#include "lib.h"

typedef struct {
  int id;
  double value;
  char name[16];
} complex_item_t;

TEST test_init_state(void) {
  stack_t s;
  sstack_init(&s, 10, sizeof(int));
  ASSERT(sstack_is_empty(&s));
  ASSERT_FALSE(sstack_is_full(&s));
  ASSERT_EQ((size_t)0, sstack_size(&s));
  ASSERT_EQ((size_t)10, sstack_capacity(&s));
  sstack_destroy(&s);
  PASS();
}

TEST test_push_int(void) {
  stack_t s;
  sstack_init(&s, 5, sizeof(int));
  int val = 42;
  ASSERT(sstack_push(&s, &val));
  ASSERT_EQ((size_t)1, sstack_size(&s));
  ASSERT_FALSE(sstack_is_empty(&s));
  sstack_destroy(&s);
  PASS();
}

TEST test_pop_int(void) {
  stack_t s;
  sstack_init(&s, 5, sizeof(int));
  ASSERT(sstack_is_empty(&s));
  int in = 100, out = 0;
  sstack_push(&s, &in);
  ASSERT(sstack_pop(&s, &out));
  ASSERT_EQ(100, out);
  ASSERT(sstack_is_empty(&s));
  sstack_destroy(&s);
  PASS();
}

TEST test_lifo_order(void) {
  stack_t s;
  sstack_init(&s, 5, sizeof(int));
  int nums[] = {1, 2, 3};
  for (int i = 0; i < 3; i++) sstack_push(&s, &nums[i]);

  int val;
  sstack_pop(&s, &val);
  ASSERT_EQ(3, val);
  sstack_pop(&s, &val);
  ASSERT_EQ(2, val);
  sstack_pop(&s, &val);
  ASSERT_EQ(1, val);
  sstack_destroy(&s);
  PASS();
}

TEST test_overflow(void) {
  stack_t s;
  sstack_init(&s, 2, sizeof(int));
  int val = 1;
  ASSERT(sstack_push(&s, &val));
  ASSERT(sstack_push(&s, &val));
  ASSERT_FALSE(sstack_push(&s, &val));
  ASSERT_EQ((size_t)2, sstack_size(&s));
  sstack_destroy(&s);
  PASS();
}

TEST test_underflow(void) {
  stack_t s;
  sstack_init(&s, 5, sizeof(int));
  int val;
  ASSERT_FALSE(sstack_pop(&s, &val));
  sstack_destroy(&s);
  PASS();
}

TEST test_is_full(void) {
  stack_t s;
  sstack_init(&s, 1, sizeof(int));
  int val = 10;
  sstack_push(&s, &val);
  ASSERT(sstack_is_full(&s));
  sstack_destroy(&s);
  PASS();
}

TEST test_clear(void) {
  stack_t s;
  sstack_init(&s, 5, sizeof(int));
  int val = 1;
  sstack_push(&s, &val);
  sstack_push(&s, &val);
  sstack_clear(&s);
  ASSERT(sstack_is_empty(&s));
  ASSERT_EQ((size_t)0, sstack_size(&s));
  sstack_destroy(&s);
  PASS();
}

TEST test_double_type(void) {
  stack_t s;
  sstack_init(&s, 5, sizeof(double));
  double in = 3.14159, out = 0.0;
  sstack_push(&s, &in);
  sstack_pop(&s, &out);
  ASSERT_EQ(in, out);
  sstack_destroy(&s);
  PASS();
}

TEST test_char_type(void) {
  stack_t s;
  sstack_init(&s, 5, sizeof(char));
  char in = 'Z', out = ' ';
  sstack_push(&s, &in);
  sstack_pop(&s, &out);
  ASSERT_EQ('Z', out);
  sstack_destroy(&s);
  PASS();
}

TEST test_complex_struct(void) {
  stack_t s;
  sstack_init(&s, 2, sizeof(complex_item_t));
  complex_item_t item = {.id = 99, .value = 12.34};
  strcpy(item.name, "test");

  sstack_push(&s, &item);

  complex_item_t result;
  sstack_pop(&s, &result);

  ASSERT_EQ(99, result.id);
  ASSERT_EQ(12.34, result.value);
  ASSERT_STR_EQ("test", result.name);
  sstack_destroy(&s);
  PASS();
}

TEST test_zero_capacity(void) {
  stack_t s;
  sstack_init(&s, 0, sizeof(int));
  ASSERT(sstack_is_full(&s));
  int val = 1;
  ASSERT_FALSE(sstack_push(&s, &val));
  sstack_destroy(&s);
  PASS();
}

TEST test_mixed_operations(void) {
  stack_t s;
  sstack_init(&s, 5, sizeof(int));
  int v;
  int k = 10;
  sstack_push(&s, &k);
  k = 20;
  sstack_push(&s, &k);
  sstack_pop(&s, &v);
  ASSERT_EQ(20, v);
  k = 30;
  sstack_push(&s, &k);
  sstack_pop(&s, &v);
  ASSERT_EQ(30, v);
  sstack_pop(&s, &v);
  ASSERT_EQ(10, v);
  sstack_destroy(&s);
  PASS();
}

TEST test_fill_cycle(void) {
  stack_t s;
  sstack_init(&s, 3, sizeof(int));
  int val = 1;
  for (int i = 0; i < 3; i++) ASSERT(sstack_push(&s, &val));
  ASSERT(sstack_is_full(&s));
  for (int i = 0; i < 3; i++) ASSERT(sstack_pop(&s, &val));
  ASSERT(sstack_is_empty(&s));
  for (int i = 0; i < 3; i++) ASSERT(sstack_push(&s, &val));
  ASSERT(sstack_is_full(&s));
  sstack_destroy(&s);
  PASS();
}

TEST test_reinit(void) {
  stack_t s;
  sstack_init(&s, 5, sizeof(int));
  int val = 5;
  sstack_push(&s, &val);
  sstack_destroy(&s);

  sstack_init(&s, 10, sizeof(double));
  ASSERT(sstack_is_empty(&s));
  ASSERT_EQ((size_t)10, sstack_capacity(&s));
  sstack_destroy(&s);
  PASS();
}

TEST test_large_volume(void) {
  stack_t s;
  int count = 100000;
  sstack_init(&s, count, sizeof(int));
  for (int i = 0; i < count; i++) {
    if (!sstack_push(&s, &i)) FAIL();
  }
  ASSERT(sstack_is_full(&s));
  for (int i = count - 1; i >= 0; i--) {
    int val;
    sstack_pop(&s, &val);
    ASSERT_EQ(i, val);
  }
  sstack_destroy(&s);
  PASS();
}

TEST test_byte_correctness(void) {
  stack_t s;
  sstack_init(&s, 2, sizeof(uint8_t));
  uint8_t a = 0xAA, b = 0xBB, out;
  sstack_push(&s, &a);
  sstack_push(&s, &b);
  sstack_pop(&s, &out);
  ASSERT_EQ(0xBB, out);
  sstack_pop(&s, &out);
  ASSERT_EQ(0xAA, out);
  sstack_destroy(&s);
  PASS();
}

TEST test_capacity_persistence(void) {
  stack_t s;
  sstack_init(&s, 50, sizeof(int));
  int k = 1;
  sstack_push(&s, &k);
  ASSERT_EQ((size_t)50, sstack_capacity(&s));
  sstack_pop(&s, &k);
  ASSERT_EQ((size_t)50, sstack_capacity(&s));
  sstack_destroy(&s);
  PASS();
}

TEST test_item_size_large(void) {
  stack_t s;
  typedef struct {
    char data[100];
  } blob_t;
  sstack_init(&s, 2, sizeof(blob_t));
  blob_t b1;
  memset(b1.data, 'A', 100);
  sstack_push(&s, &b1);

  blob_t b2;
  sstack_pop(&s, &b2);
  ASSERT(memcmp(b1.data, b2.data, 100) == 0);
  sstack_destroy(&s);
  PASS();
}

TEST test_size_increments(void) {
  stack_t s;
  sstack_init(&s, 10, sizeof(int));
  int k = 0;
  for (size_t i = 1; i <= 5; i++) {
    sstack_push(&s, &k);
    ASSERT_EQ(i, sstack_size(&s));
  }
  for (size_t i = 4; i > 0; i--) {
    sstack_pop(&s, &k);
    ASSERT_EQ(i, sstack_size(&s));
  }
  sstack_destroy(&s);
  PASS();
}

SUITE(stack_suite) {
  RUN_TEST(test_init_state);
  RUN_TEST(test_push_int);
  RUN_TEST(test_pop_int);
  RUN_TEST(test_lifo_order);
  RUN_TEST(test_overflow);
  RUN_TEST(test_underflow);
  RUN_TEST(test_is_full);
  RUN_TEST(test_clear);
  RUN_TEST(test_double_type);
  RUN_TEST(test_char_type);
  RUN_TEST(test_complex_struct);
  RUN_TEST(test_zero_capacity);
  RUN_TEST(test_mixed_operations);
  RUN_TEST(test_fill_cycle);
  RUN_TEST(test_reinit);
  RUN_TEST(test_large_volume);
  RUN_TEST(test_byte_correctness);
  RUN_TEST(test_capacity_persistence);
  RUN_TEST(test_item_size_large);
  RUN_TEST(test_size_increments);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
  srand(42);
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(stack_suite);
  GREATEST_PRINT_REPORT();
  custom_tests();
  return greatest_all_passed() ? EXIT_SUCCESS : EXIT_FAILURE;
}
