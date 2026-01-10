#include "lib.h"

void swap(int* a, int* b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

vector_t* vector_create(size_t initial_capacity) {
  vector_t* vec = malloc(sizeof(vector_t));
  if (!vec) return NULL;

  vec->capacity = initial_capacity;
  vec->size = 0;
  vec->data = malloc(sizeof(int) * vec->capacity);
  if (!vec->data) {
    free(vec);
    return NULL;
  }
  return vec;
}

void vector_free(vector_t* vec) {
  free(vec->data);  // defined for NULL, no need to check
  free(vec);
}

int vector_at(const vector_t* vec, size_t index) {
  // no bounds checking
  return vec->data[index];
}

int vector_front(const vector_t* vec) {
  return vec->data[0];
}

int vector_back(const vector_t* vec) {
  return vec->data[vec->size - 1];
}

int* vector_data(const vector_t* vec) {
  return vec->data;
}

bool vector_empty(const vector_t* vec) {
  return vec->size == 0;
}

size_t vector_size(const vector_t* vec) {
  return vec->size;
}

size_t vector_capacity(const vector_t* vec) {
  return vec->capacity;
}

void vector_shrink_to_fit(vector_t* vec) {
  if(vec->size == vec->capacity) return;

  if(vec->size == 0) {
    free(vec->data);
    vec->data = NULL;
    vec->capacity = 0;
    return;
  }

  int* new_data = realloc(vec->data, sizeof(int) * vec->size);
  if (!new_data) return;

  vec->data = new_data;
  vec->capacity = vec->size;
}

void vector_reserve(vector_t* vec, size_t new_capacity) {
  if (new_capacity <= vec->capacity) return;

  int* new_data = realloc(vec->data, sizeof(int) * new_capacity);
  if (!new_data) return;

  vec->data = new_data;
  vec->capacity = new_capacity;
}

void vector_clear(vector_t* vec) {
  vec->size = 0;
}

void vector_insert_before(vector_t* vec, size_t index, int value) {
  if (vec->size == vec->capacity) {
    // NOTE: if initial capacity is 0 -> 2*0=0 -> possible heap buffer overflow
    // set capacity to 1, if initial capacity is 0
    size_t capacity = vec->capacity == 0 ? 1 : vec->capacity * 2;
    vector_reserve(vec, capacity);
  }
  for (size_t i = vec->size; i > index; i--) {
    vec->data[i] = vec->data[i - 1];
  }
  vec->data[index] = value;
  vec->size++;
}

void vector_erase(vector_t* vec, size_t index) {
  for (size_t i = index + 1; i < vec->size; i++) {
    vec->data[i - 1] = vec->data[i];
  }
  vec->size--;
}

void vector_push_back(vector_t* vec, int value) {
  vector_insert_before(vec, vec->size, value);
}

void vector_pop_back(vector_t* vec) {
  vector_erase(vec, vec->size - 1);
}

void vector_resize(vector_t* vec, size_t new_size, int default_value) {
  if (new_size > vec->capacity) {
    vector_reserve(vec, new_size);
  }
  for (size_t i = vec->size; i < new_size; i++) {
    vec->data[i] = default_value;
  }
  vec->size = new_size;
}

ssize_t vector_find(const vector_t* vec, int value) {
  for (size_t i = 0; i < vec->size; i++) {
    if (vec->data[i] == value) return i;
  }
  return -1;
}

bool vector_contains(const vector_t* vec, int value) {
  for (size_t i = 0; i < vec->size; i++) {
    if (vec->data[i] == value) return true;
  }
  return false;
}

void vector_reverse(vector_t* vec) {
  for (size_t i = 0; i < vec->size / 2; i++) {
    swap(&vec->data[i], &vec->data[vec->size - i - 1]);
  }
}

void vector_sort(vector_t* vec) {
  // bubble sort
  for (size_t i = 0; i < vec->size; i++) {
    for (size_t j = i + 1; j < vec->size; j++) {
      if (vec->data[i] > vec->data[j]) {
        swap(&vec->data[i], &vec->data[j]);
      }
    }
  }
}
