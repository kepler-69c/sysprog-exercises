#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../greatest.h"
#include "custom_tests.h"
#include "lib.h"

TEST test_empty_tree() {
  merkle_tree_t *tree = merkle_tree_create(NULL, 0);
  ASSERT_EQ(NULL, tree);
  PASS();
}

TEST test_single_byte_tree() {
  uint8_t data = 0xAA;
  merkle_tree_t *tree = merkle_tree_create(&data, 1);
  ASSERT(tree != NULL);

  // Real-life test: Don't check internal 'is_leaf'.
  // Check if the tree reports the correct total size.
  ASSERT_EQ(1, merkle_tree_size(tree));

  // Real-life test: Check the hash correctness directly
  hash_t expected = hash(&data, 1);
  ASSERT_EQ(expected, merkle_tree_root_hash(tree));

  merkle_tree_free(tree);
  PASS();
}

TEST test_full_block_tree() {
  uint8_t data[BLOCK_SIZE];
  memset(data, 0xBB, BLOCK_SIZE);
  merkle_tree_t *tree = merkle_tree_create(data, BLOCK_SIZE);

  ASSERT(tree != NULL);
  ASSERT_EQ(BLOCK_SIZE, merkle_tree_size(tree));

  // Verify self-integrity
  ASSERT(merkle_tree_verify(tree, data, BLOCK_SIZE));

  merkle_tree_free(tree);
  PASS();
}

TEST test_two_blocks_even() {
  uint8_t data[BLOCK_SIZE * 2];
  memset(data, 0x01, BLOCK_SIZE);
  memset(data + BLOCK_SIZE, 0x02, BLOCK_SIZE);

  merkle_tree_t *tree = merkle_tree_create(data, BLOCK_SIZE * 2);

  // Calculate expected hash manually to verify tree structure logic
  hash_t h1 = hash(data, BLOCK_SIZE);
  hash_t h2 = hash(data + BLOCK_SIZE, BLOCK_SIZE);
  hash_t expected = combine_hash(h1, h2);

  ASSERT_EQ(expected, merkle_tree_root_hash(tree));
  merkle_tree_free(tree);
  PASS();
}

TEST test_three_blocks_odd() {
  uint8_t data[BLOCK_SIZE * 3];
  memset(data, 0xCC, sizeof(data));

  merkle_tree_t *tree = merkle_tree_create(data, sizeof(data));
  ASSERT_EQ(sizeof(data), merkle_tree_size(tree));

  merkle_tree_free(tree);
  PASS();
}

TEST test_verify_positive() {
  uint8_t data[100];
  for (int i = 0; i < 100; i++) data[i] = (uint8_t)i;

  merkle_tree_t *tree = merkle_tree_create(data, 100);
  ASSERT(merkle_tree_verify(tree, data, 100));

  merkle_tree_free(tree);
  PASS();
}

TEST test_verify_negative() {
  uint8_t data[100];
  uint8_t tampered[100];
  memset(data, 0x11, 100);
  memcpy(tampered, data, 100);
  tampered[50] ^= 0xFF;

  merkle_tree_t *tree = merkle_tree_create(data, 100);
  ASSERT(!merkle_tree_verify(tree, tampered, 100));

  merkle_tree_free(tree);
  PASS();
}

TEST test_update_and_rehash() {
  uint8_t data[BLOCK_SIZE * 2];
  memset(data, 0xAA, sizeof(data));

  merkle_tree_t *tree = merkle_tree_create(data, sizeof(data));
  hash_t original_root = merkle_tree_root_hash(tree);

  // Modify data
  data[BLOCK_SIZE + 10] = 0xBB;

  ASSERT_FALSE(merkle_tree_verify(tree, data, sizeof(data)));
  merkle_tree_rehash(tree);
  ASSERT(merkle_tree_verify(tree, data, sizeof(data)));

  hash_t new_root = merkle_tree_root_hash(tree);
  ASSERT_NEQ(original_root, new_root);

  merkle_tree_free(tree);
  PASS();
}

TEST test_diff_identical() {
  uint8_t data[500];
  memset(data, 0xEE, 500);
  merkle_tree_t *t1 = merkle_tree_create(data, 500);
  merkle_tree_t *t2 = merkle_tree_create(data, 500);

  ASSERT_EQ(-1, merkle_tree_first_diff(t1, t2));

  merkle_tree_free(t1);
  merkle_tree_free(t2);
  PASS();
}

TEST test_diff_first_block() {
  uint8_t d1[BLOCK_SIZE * 2], d2[BLOCK_SIZE * 2];
  memset(d1, 0xA, sizeof(d1));
  memcpy(d2, d1, sizeof(d2));
  d2[0] = 0xB;

  merkle_tree_t *t1 = merkle_tree_create(d1, sizeof(d1));
  merkle_tree_t *t2 = merkle_tree_create(d2, sizeof(d2));

  ASSERT_EQ(0, merkle_tree_first_diff(t1, t2));

  merkle_tree_free(t1);
  merkle_tree_free(t2);
  PASS();
}

TEST test_diff_second_block() {
  uint8_t d1[BLOCK_SIZE * 2], d2[BLOCK_SIZE * 2];
  memset(d1, 0xA, sizeof(d1));
  memcpy(d2, d1, sizeof(d2));
  d2[BLOCK_SIZE + 5] = 0xB;

  merkle_tree_t *t1 = merkle_tree_create(d1, sizeof(d1));
  merkle_tree_t *t2 = merkle_tree_create(d2, sizeof(d2));

  ASSERT_EQ(BLOCK_SIZE, merkle_tree_first_diff(t1, t2));

  merkle_tree_free(t1);
  merkle_tree_free(t2);
  PASS();
}

TEST test_large_tree_power_of_two() {
  size_t size = BLOCK_SIZE * 8;
  uint8_t *data = calloc(1, size);
  merkle_tree_t *tree = merkle_tree_create(data, size);

  ASSERT(tree != NULL);
  ASSERT_EQ(size, merkle_tree_size(tree));

  merkle_tree_free(tree);
  free(data);
  PASS();
}

TEST test_varying_bytes(size_t n_bytes) {
  uint8_t *data = malloc(n_bytes);
  memset(data, (uint8_t)n_bytes, n_bytes);

  merkle_tree_t *tree = merkle_tree_create(data, n_bytes);
  ASSERT(tree != NULL);
  ASSERT_EQ(n_bytes, merkle_tree_size(tree));

  ASSERT(merkle_tree_verify(tree, data, n_bytes));

  merkle_tree_free(tree);
  free(data);
  PASS();
}

TEST test_varying_blocks(int n_blocks) {
  size_t size = BLOCK_SIZE * n_blocks;
  uint8_t *data = malloc(size);
  memset(data, (uint8_t)n_blocks, size);

  merkle_tree_t *tree = merkle_tree_create(data, size);
  ASSERT(tree != NULL);
  ASSERT_EQ(size, merkle_tree_size(tree));

  ASSERT(merkle_tree_verify(tree, data, size));

  merkle_tree_free(tree);
  free(data);
  PASS();
}

TEST test_determinism() {
  uint8_t data[BLOCK_SIZE * 3];
  memset(data, 0x5A, sizeof(data));
  merkle_tree_t *tree1 = merkle_tree_create(data, sizeof(data));
  hash_t expected_hash = merkle_tree_root_hash(tree1);
  merkle_tree_free(tree1);

  for (int i = 0; i < 10; i++) {
    merkle_tree_t *treeN_on_original = merkle_tree_create(data, sizeof(data));
    ASSERT_EQ(expected_hash, merkle_tree_root_hash(treeN_on_original));
    uint8_t data_copy[BLOCK_SIZE * 3];
    memcpy(data_copy, data, sizeof(data));
    merkle_tree_t *treeN = merkle_tree_create(data_copy, sizeof(data_copy));
    ASSERT_EQ(expected_hash, merkle_tree_root_hash(treeN));
    merkle_tree_free(treeN_on_original);
    merkle_tree_free(treeN);
  }
  PASS();
}

SUITE(merkle_tree_suite) {
  RUN_TEST(test_empty_tree);
  RUN_TEST(test_single_byte_tree);
  RUN_TEST(test_full_block_tree);
  RUN_TEST(test_two_blocks_even);
  RUN_TEST(test_three_blocks_odd);
  RUN_TEST(test_verify_positive);
  RUN_TEST(test_verify_negative);
  RUN_TEST(test_update_and_rehash);
  RUN_TEST(test_diff_identical);
  RUN_TEST(test_diff_first_block);
  RUN_TEST(test_diff_second_block);
  RUN_TEST(test_large_tree_power_of_two);

  for (int i = 4; i <= 21; i++) {
    RUN_TESTp(test_varying_blocks, i);
  }

  for (int i = 200; i <= 10 * BLOCK_SIZE + 1; i += 64) {
    RUN_TESTp(test_varying_bytes, i);
  }

  RUN_TEST(test_determinism);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(merkle_tree_suite);
  GREATEST_PRINT_REPORT();
  custom_tests();
  return greatest_all_passed() ? EXIT_SUCCESS : EXIT_FAILURE;
}
