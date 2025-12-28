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

GREATEST_MAIN_DEFS();

TEST test_hashmap_create_and_free() {
  hashmap_t *map = HASHMAP_CREATE(16, int, int);
  ASSERT(map != NULL);
  ASSERT_EQ(0, hashmap_size(map));
  hashmap_free(map);
  PASS();
}

TEST test_hashmap_put_get_basic() {
  hashmap_t *map = HASHMAP_CREATE(16, int, int);
  int k = 10, v = 100;
  void *out = NULL;

  ASSERT(hashmap_put(map, &k, &v));
  ASSERT_EQ(1, hashmap_size(map));
  ASSERT(hashmap_get(map, &k, &out));
  ASSERT_EQ(100, *(int *)out);

  hashmap_free(map);
  PASS();
}

TEST test_hashmap_overwrite() {
  hashmap_t *map = HASHMAP_CREATE(16, int, int);
  int k = 1, v1 = 100, v2 = 200;
  void *out = NULL;

  hashmap_put(map, &k, &v1);
  hashmap_put(map, &k, &v2);

  ASSERT_EQ(1, hashmap_size(map));
  hashmap_get(map, &k, &out);
  ASSERT_EQ(200, *(int *)out);

  hashmap_free(map);
  PASS();
}

TEST test_hashmap_contains() {
  hashmap_t *map = HASHMAP_CREATE(16, int, int);
  int k1 = 1, k2 = 2;
  int v = 10;

  hashmap_put(map, &k1, &v);
  ASSERT(hashmap_contains(map, &k1));
  ASSERT_FALSE(hashmap_contains(map, &k2));

  hashmap_free(map);
  PASS();
}

TEST test_hashmap_remove() {
  hashmap_t *map = HASHMAP_CREATE(16, int, int);
  int k = 5, v = 50;

  hashmap_put(map, &k, &v);
  ASSERT_EQ(1, hashmap_size(map));
  hashmap_remove(map, &k);
  ASSERT_EQ(0, hashmap_size(map));
  ASSERT_FALSE(hashmap_contains(map, &k));

  hashmap_free(map);
  PASS();
}

TEST test_hashmap_clear() {
  hashmap_t *map = HASHMAP_CREATE(16, int, int);
  for (int i = 0; i < 10; i++) {
    hashmap_put(map, &i, &i);
  }
  ASSERT_EQ(10, hashmap_size(map));
  hashmap_clear(map);
  ASSERT_EQ(0, hashmap_size(map));
  hashmap_free(map);
  PASS();
}

TEST test_hashmap_load_factor() {
  hashmap_t *map = HASHMAP_CREATE(10, int, int);
  int v = 0;
  for (int i = 0; i < 5; i++) hashmap_put(map, &i, &v);

  float lf = hashmap_load_factor(map);
  ASSERT(lf > 0.49f && lf < 0.51f);

  hashmap_free(map);
  PASS();
}

TEST test_hashmap_collisions() {
  hashmap_t *map = HASHMAP_CREATE(1, int, int);
  int k1 = 10, k2 = 20, v = 1;

  hashmap_put(map, &k1, &v);
  hashmap_put(map, &k2, &v);

  ASSERT_EQ(2, hashmap_size(map));
  ASSERT(hashmap_contains(map, &k1));
  ASSERT(hashmap_contains(map, &k2));

  hashmap_free(map);
  PASS();
}

TEST test_hashmap_large_insert() {
  hashmap_t *map = HASHMAP_CREATE(100, int, int);
  for (int i = 0; i < 500; i++) {
    hashmap_put(map, &i, &i);
  }
  ASSERT_EQ(500, hashmap_size(map));
  for (int i = 0; i < 500; i++) {
    void *out;
    ASSERT(hashmap_get(map, &i, &out));
    ASSERT_EQ(i, *(int *)out);
  }
  hashmap_free(map);
  PASS();
}

TEST test_hashmap_get_nonexistent() {
  hashmap_t *map = HASHMAP_CREATE(16, int, int);
  int k = 1;
  void *out;
  ASSERT_FALSE(hashmap_get(map, &k, &out));
  hashmap_free(map);
  PASS();
}

TEST test_hashmap_remove_nonexistent() {
  hashmap_t *map = HASHMAP_CREATE(16, int, int);
  int k = 1;
  hashmap_remove(map, &k);
  ASSERT_EQ(0, hashmap_size(map));
  hashmap_free(map);
  PASS();
}

TEST test_hashmap_string_keys() {
  hashmap_t *map = hashmap_create(16, 10, sizeof(int));
  char k1[10] = "hello";
  char k2[10] = "world";
  int v1 = 1, v2 = 2;

  hashmap_put(map, k1, &v1);
  hashmap_put(map, k2, &v2);

  void *out;
  hashmap_get(map, k1, &out);
  ASSERT_EQ(1, *(int *)out);

  hashmap_free(map);
  PASS();
}

TEST test_hashmap_zero_buckets() {
  hashmap_t *map = HASHMAP_CREATE(0, int, int);
  ASSERT(map == NULL);
  PASS();
}

TEST test_hashmap_struct_values() {
  typedef struct {
    int x;
    int y;
  } Point;
  hashmap_t *map = HASHMAP_CREATE(16, int, Point);
  int k = 1;
  Point p = {10, 20};
  hashmap_put(map, &k, &p);

  void *out;
  hashmap_get(map, &k, &out);
  Point *res = (Point *)out;
  ASSERT_EQ(10, res->x);
  ASSERT_EQ(20, res->y);

  hashmap_free(map);
  PASS();
}

TEST test_hashmap_remove_middle_of_chain() {
  hashmap_t *map = HASHMAP_CREATE(1, int, int);
  int k1 = 1, k2 = 2, k3 = 3, v = 0;
  hashmap_put(map, &k1, &v);
  hashmap_put(map, &k2, &v);
  hashmap_put(map, &k3, &v);

  hashmap_remove(map, &k2);
  ASSERT_EQ(2, hashmap_size(map));
  ASSERT(hashmap_contains(map, &k1));
  ASSERT(hashmap_contains(map, &k3));
  ASSERT_FALSE(hashmap_contains(map, &k2));

  hashmap_free(map);
  PASS();
}

TEST test_hashmap_remove_head_of_chain() {
  hashmap_t *map = HASHMAP_CREATE(1, int, int);
  int k1 = 1, k2 = 2, v = 0;
  hashmap_put(map, &k1, &v);
  hashmap_put(map, &k2, &v);

  hashmap_remove(map, &k1);
  ASSERT_EQ(1, hashmap_size(map));
  ASSERT_FALSE(hashmap_contains(map, &k1));
  ASSERT(hashmap_contains(map, &k2));

  hashmap_free(map);
  PASS();
}

TEST test_hashmap_remove_tail_of_chain() {
  hashmap_t *map = HASHMAP_CREATE(1, int, int);
  int k1 = 1, k2 = 2, v = 0;
  hashmap_put(map, &k1, &v);
  hashmap_put(map, &k2, &v);

  hashmap_remove(map, &k2);
  ASSERT_EQ(1, hashmap_size(map));
  ASSERT(hashmap_contains(map, &k1));
  ASSERT_FALSE(hashmap_contains(map, &k2));

  hashmap_free(map);
  PASS();
}

TEST test_hash_function_consistency() {
  char *data = "test";
  hash_t h1 = hash(data, 4);
  hash_t h2 = hash(data, 4);
  ASSERT_EQ(h1, h2);
  PASS();
}

TEST test_hashmap_size_after_failed_put() {
  hashmap_t *map = HASHMAP_CREATE(16, int, int);
  int k = 1;
  int v1 = 1;
  int v2 = 2;
  hashmap_put(map, &k, &v1);
  ASSERT_EQ(1, hashmap_size(map));
  hashmap_put(map, &k, &v2);
  ASSERT_EQ(1, hashmap_size(map));
  hashmap_remove(map, &k);
  ASSERT_EQ(0, hashmap_size(map));
  hashmap_free(map);
  PASS();
}

TEST test_hashmap_put_get_large_values() {
  typedef struct {
    uint8_t data[1024];
  } Big;
  hashmap_t *map = HASHMAP_CREATE(16, int, Big);
  int k = 1;
  Big b;
  memset(b.data, 0xAA, 1024);
  hashmap_put(map, &k, &b);

  void *out;
  hashmap_get(map, &k, &out);
  ASSERT_EQ(0xAA, ((Big *)out)->data[512]);
  hashmap_free(map);
  PASS();
}

TEST test_hashmap_reput_same_value() {
  hashmap_t *map = HASHMAP_CREATE(16, int, int);
  int k = 1, v = 100;
  hashmap_put(map, &k, &v);
  hashmap_put(map, &k, &v);
  ASSERT_EQ(1, hashmap_size(map));
  hashmap_free(map);
  PASS();
}

TEST test_hashmap_remove_all_one_by_one() {
  hashmap_t *map = HASHMAP_CREATE(16, int, int);
  for (int i = 0; i < 10; i++) hashmap_put(map, &i, &i);
  for (int i = 0; i < 10; i++) hashmap_remove(map, &i);
  ASSERT_EQ(0, hashmap_size(map));
  hashmap_free(map);
  PASS();
}

TEST test_hashmap_put_after_clear() {
  hashmap_t *map = HASHMAP_CREATE(16, int, int);
  int k = 1, v = 1;
  hashmap_put(map, &k, &v);
  hashmap_clear(map);
  hashmap_put(map, &k, &v);
  ASSERT_EQ(1, hashmap_size(map));
  hashmap_free(map);
  PASS();
}

TEST test_hashmap_contains_after_remove() {
  hashmap_t *map = HASHMAP_CREATE(16, int, int);
  int k = 1, v = 1;
  hashmap_put(map, &k, &v);
  hashmap_remove(map, &k);
  ASSERT_FALSE(hashmap_contains(map, &k));
  hashmap_free(map);
  PASS();
}

TEST test_hashmap_get_after_remove() {
  hashmap_t *map = HASHMAP_CREATE(16, int, int);
  int k = 1, v = 1;
  void *out;
  hashmap_put(map, &k, &v);
  hashmap_remove(map, &k);
  ASSERT_FALSE(hashmap_get(map, &k, &out));
  hashmap_free(map);
  PASS();
}

TEST test_hashmap_stress_put_remove() {
  hashmap_t *map = HASHMAP_CREATE(10, int, int);
  for (int i = 0; i < 100; i++) {
    hashmap_put(map, &i, &i);
    if (i % 2 == 0) hashmap_remove(map, &i);
  }
  ASSERT_EQ(50, hashmap_size(map));
  hashmap_free(map);
  PASS();
}

TEST test_hashmap_large_key_size() {
  uint8_t k[256];
  memset(k, 0xFF, 256);
  hashmap_t *map = hashmap_create(16, 256, sizeof(int));
  int v = 123;
  hashmap_put(map, k, &v);
  ASSERT(hashmap_contains(map, k));
  hashmap_free(map);
  PASS();
}

TEST test_hashmap_iterator_empty() {
  hashmap_t *map = HASHMAP_CREATE(16, int, int);
  hashmap_iterator_t *it = hashmap_iterator_create(map);
  ASSERT_FALSE(hashmap_iterator_next(it));
  hashmap_iterator_free(it);
  hashmap_free(map);
  PASS();
}

TEST test_hashmap_iterator_full_traversal() {
  hashmap_t *map = HASHMAP_CREATE(16, int, int);
  int count = 0;
  for (int i = 0; i < 10; i++) hashmap_put(map, &i, &i);

  hashmap_iterator_t *it = hashmap_iterator_create(map);
  while (hashmap_iterator_next(it)) {
    count++;
  }
  ASSERT_EQ(10, count);

  hashmap_iterator_free(it);
  hashmap_free(map);
  PASS();
}

TEST test_hashmap_iterator_values() {
  hashmap_t *map = HASHMAP_CREATE(16, int, int);
  int k = 42, v = 84;
  hashmap_put(map, &k, &v);

  hashmap_iterator_t *it = hashmap_iterator_create(map);
  ASSERT(hashmap_iterator_next(it));
  ASSERT_EQ(42, *(int *)hashmap_iterator_key(it));
  ASSERT_EQ(84, *(int *)hashmap_iterator_value(it));

  hashmap_iterator_free(it);
  hashmap_free(map);
  PASS();
}

TEST test_hashmap_iterator_after_clear() {
  hashmap_t *map = HASHMAP_CREATE(16, int, int);
  int k = 1, v = 1;
  hashmap_put(map, &k, &v);
  hashmap_clear(map);

  hashmap_iterator_t *it = hashmap_iterator_create(map);
  ASSERT_FALSE(hashmap_iterator_next(it));

  hashmap_iterator_free(it);
  hashmap_free(map);
  PASS();
}

TEST test_hashmap_iterator_stability() {
  hashmap_t *map = HASHMAP_CREATE(16, int, int);
  int k = 1, v = 1;
  hashmap_put(map, &k, &v);
  hashmap_iterator_t *it = hashmap_iterator_create(map);

  ASSERT(hashmap_iterator_next(it));
  ASSERT_FALSE(hashmap_iterator_next(it));

  hashmap_iterator_free(it);
  hashmap_free(map);
  PASS();
}

TEST test_hashmap_multiple_iterators() {
  hashmap_t *map = HASHMAP_CREATE(16, int, int);
  int k = 1, v = 1;
  hashmap_put(map, &k, &v);

  hashmap_iterator_t *it1 = hashmap_iterator_create(map);
  hashmap_iterator_t *it2 = hashmap_iterator_create(map);

  ASSERT(hashmap_iterator_next(it1));
  ASSERT(hashmap_iterator_next(it2));

  hashmap_iterator_free(it1);
  hashmap_iterator_free(it2);
  hashmap_free(map);
  PASS();
}

TEST test_hashmap_iterator_key_value_consistency() {
  hashmap_t *map = HASHMAP_CREATE(16, int, int);
  for (int i = 0; i < 5; i++) hashmap_put(map, &i, &i);

  hashmap_iterator_t *it = hashmap_iterator_create(map);
  while (hashmap_iterator_next(it)) {
    int key = *(int *)hashmap_iterator_key(it);
    int val = *(int *)hashmap_iterator_value(it);
    ASSERT_EQ(key, val);
  }
  hashmap_iterator_free(it);
  hashmap_free(map);
  PASS();
}

TEST test_hashmap_iterator_next_on_last() {
  hashmap_t *map = HASHMAP_CREATE(1, int, int);
  int k = 1, v = 1;
  hashmap_put(map, &k, &v);
  hashmap_iterator_t *it = hashmap_iterator_create(map);

  ASSERT(hashmap_iterator_next(it));
  ASSERT_FALSE(hashmap_iterator_next(it));

  hashmap_iterator_free(it);
  hashmap_free(map);
  PASS();
}

TEST test_hashmap_iterator_manual_advance() {
  hashmap_t *map = HASHMAP_CREATE(16, int, int);
  int k1 = 1, k2 = 2, v = 0;
  hashmap_put(map, &k1, &v);
  hashmap_put(map, &k2, &v);

  hashmap_iterator_t *it = hashmap_iterator_create(map);
  ASSERT(hashmap_iterator_next(it));
  ASSERT(hashmap_iterator_next(it));
  ASSERT_FALSE(hashmap_iterator_next(it));

  hashmap_iterator_free(it);
  hashmap_free(map);
  PASS();
}

TEST test_hashmap_iterator_value_ref() {
  hashmap_t *map = HASHMAP_CREATE(16, int, int);
  int k = 1, v = 500;
  hashmap_put(map, &k, &v);

  hashmap_iterator_t *it = hashmap_iterator_create(map);
  ASSERT(hashmap_iterator_next(it));
  int *val_ptr = (int *)hashmap_iterator_value(it);
  *val_ptr = 600;

  void *out;
  hashmap_get(map, &k, &out);
  ASSERT_EQ(600, *(int *)out);

  hashmap_iterator_free(it);
  hashmap_free(map);
  PASS();
}

TEST test_hashmap_iterator_non_destructive() {
  hashmap_t *map = HASHMAP_CREATE(16, int, int);
  int k = 1, v = 1;
  hashmap_put(map, &k, &v);

  hashmap_iterator_t *it = hashmap_iterator_create(map);
  while (hashmap_iterator_next(it)) {
  }
  hashmap_iterator_free(it);

  ASSERT_EQ(1, hashmap_size(map));
  hashmap_free(map);
  PASS();
}

SUITE(hashmap_suite) {
  RUN_TEST(test_hashmap_create_and_free);
  RUN_TEST(test_hashmap_put_get_basic);
  RUN_TEST(test_hashmap_overwrite);
  RUN_TEST(test_hashmap_contains);
  RUN_TEST(test_hashmap_remove);
  RUN_TEST(test_hashmap_clear);
  RUN_TEST(test_hashmap_load_factor);
  RUN_TEST(test_hashmap_collisions);
  RUN_TEST(test_hashmap_large_insert);
  RUN_TEST(test_hashmap_get_nonexistent);
  RUN_TEST(test_hashmap_remove_nonexistent);
  RUN_TEST(test_hashmap_string_keys);
  RUN_TEST(test_hashmap_zero_buckets);
  RUN_TEST(test_hashmap_struct_values);
  RUN_TEST(test_hashmap_remove_middle_of_chain);
  RUN_TEST(test_hashmap_remove_head_of_chain);
  RUN_TEST(test_hashmap_remove_tail_of_chain);
  RUN_TEST(test_hash_function_consistency);
  RUN_TEST(test_hashmap_size_after_failed_put);
  RUN_TEST(test_hashmap_put_get_large_values);
  RUN_TEST(test_hashmap_reput_same_value);
  RUN_TEST(test_hashmap_remove_all_one_by_one);
  RUN_TEST(test_hashmap_put_after_clear);
  RUN_TEST(test_hashmap_contains_after_remove);
  RUN_TEST(test_hashmap_get_after_remove);
  RUN_TEST(test_hashmap_stress_put_remove);
  RUN_TEST(test_hashmap_large_key_size);
}

SUITE(hashmap_iterator_suite) {
  RUN_TEST(test_hashmap_iterator_empty);
  RUN_TEST(test_hashmap_iterator_full_traversal);
  RUN_TEST(test_hashmap_iterator_values);
  RUN_TEST(test_hashmap_iterator_after_clear);
  RUN_TEST(test_hashmap_iterator_stability);
  RUN_TEST(test_hashmap_multiple_iterators);
  RUN_TEST(test_hashmap_iterator_key_value_consistency);
  RUN_TEST(test_hashmap_iterator_next_on_last);
  RUN_TEST(test_hashmap_iterator_manual_advance);
  RUN_TEST(test_hashmap_iterator_value_ref);
  RUN_TEST(test_hashmap_iterator_non_destructive);
}

int main(int argc, char **argv) {
  srand(42);
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(hashmap_suite);
  RUN_SUITE(hashmap_iterator_suite);
  GREATEST_PRINT_REPORT();
  custom_tests();
  return greatest_all_passed() ? EXIT_SUCCESS : EXIT_FAILURE;
}
