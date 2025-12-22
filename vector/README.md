# Dynamic Vector

## Goal

Implement a robust **dynamic array (vector)** library in C. This exercise focuses on manual memory management, heap allocation, and the logic behind resizing contiguous memory blocks.

You must implement the core logic for managing `vector_t` in `lib.c`. This includes:
1. **Memory Management**: Initializing the vector, resizing the underlying buffer (reserve/shrink), and ensuring no memory leaks occur.
2. **Access and Information**: Retrieving elements by index and monitoring the size versus the total capacity.
3. **Modifications**: Adding and removing elements at any position, which requires shifting existing data and potentially triggering a capacity expansion.
4. **Search and Sort**: Implementing basic algorithms to find elements or reorder the data.

---

## The Data Structure

The vector is a structure that manages a pointer to a dynamically allocated array and tracks its usage:

```c
typedef struct {
  size_t size;      // Number of elements currently in the vector
  size_t capacity;  // Total space currently allocated in memory
  int *data;        // Pointer to the heap-allocated array
} vector_t;
```

A properly maintained vector must always ensure that:
* `size` is always less than or equal to `capacity`.
* If `capacity` is greater than 0, `data` must point to a valid memory block.
* When `size` reaches `capacity` during an insertion, the `capacity` must be increased (typically doubled) to accommodate new elements.

You will have to implement a growth and shrink strategy to manage the `data` buffer in order to get an amortized constant time complexity for insertions at the end of the vector.

---

## Testing Your Code

Run the test suite:

```bash
make
./test
```

or

```bash
make run
```

The tests will verify that your vector handles growth correctly, maintains the correct order of elements after insertions or deletions, and manages memory safely.

---

## Files You'll Modify

* **`lib.c`**: Implement all the functions declared in `lib.h`. This includes handling `malloc`, `realloc`, and `free` to manage the lifecycle of the internal `data` buffer.

## Files Provided

* **`lib.h`**: Data structures, inline helper functions for printing and empty creation, and function prototypes.
* **`test.c`**: The testing suite with unit tests for each operation.
* **`Makefile`**: Build instructions.
