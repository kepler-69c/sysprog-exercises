#include "lib.h"

vector_t *vector_create(size_t initial_capacity) {
  return NULL;
}
void vector_free(vector_t *vec) {}

int vector_at(const vector_t *vec, size_t index) {
  return 0;
}
int vector_front(const vector_t *vec) {
  return 0;
}
int vector_back(const vector_t *vec) {
  return 0;
}
int *vector_data(const vector_t *vec) {
  return NULL;
}

bool vector_empty(const vector_t *vec) {
  return true;
}
size_t vector_size(const vector_t *vec) {
  return 0;
}
size_t vector_capacity(const vector_t *vec) {
  return 0;
}
void vector_shrink_to_fit(vector_t *vec) {}
void vector_reserve(vector_t *vec, size_t new_capacity) {}

void vector_clear(vector_t *vec) {}
void vector_insert_before(vector_t *vec, size_t index, int value) {}
void vector_erase(vector_t *vec, size_t index) {}
void vector_push_back(vector_t *vec, int value) {}
void vector_pop_back(vector_t *vec) {}
void vector_resize(vector_t *vec, size_t new_size, int default_value) {}

ssize_t vector_find(const vector_t *vec, int value) {
  return -1;
}
bool vector_contains(const vector_t *vec, int value) {
  return false;
}
void vector_reverse(vector_t *vec) {}
void vector_sort(vector_t *vec) {}
