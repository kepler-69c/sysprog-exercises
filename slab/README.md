# Slab Allocator

## Goal

Implement a **Slab Allocator**, a memory management mechanism designed to handle the allocation of numerous objects of the same size efficiently. This exercise focuses on reducing internal fragmentation, improving cache locality, and minimizing the overhead of frequent `malloc` and `free` calls by managing memory in page-sized "slabs."

In this exercise, you are provided with the public interface in `lib.h`, but you must define the internal structures for `slab_cache` and `slab_allocator` yourself.

For more information on how slab allocators work, you can refer to the [Slab Allocator Wikipedia page](https://en.wikipedia.org/wiki/Slab_allocation).

---

## The Data Structures

You must define the following types in your implementation:

1.  **`slab_allocator_t`**: The top-level structure that tracks all active caches.
2.  **`slab_cache_t`**: The structure that manages slabs for a specific object size and alignment.
3.  You might find it useful to define a **`slab_t`** structure to represent individual slabs within a cache.

---

## Functions to Implement

You are responsible for implementing the following API defined in `lib.h`:

* `slab_allocator_create()`: Initialize the global allocator state.
* `slab_cache_create()`: Create a new cache for a specific object size and alignment.
* `slab_alloc()`: Allocate a single object from the cache, respecting alignment.
* `slab_free()`: Return an object to the cache and update the slab's internal state.
* `slab_cache_free()`: Destroy a specific cache and free all associated slabs.
* `slab_allocator_free()`: Perform a deep free of the entire allocator and all its associated caches.

### Alignment Requirements
The `alignment` parameter refers to **individual element alignment**. You must ensure that the address of every returned object is a multiple of the requested alignment.

---

## Testing Your Code

Run the test suite provided to verify your implementation:

```bash
make
./test
```
or

```bash
make run
```

You should see:

```text
* Suite slab_suite:
..............................

30 tests - 30 pass, 0 fail, 0 skipped
```

You can also add custom logic during testing by modifying the `custom_tests.c` file. Your custom tests will be run after the provided tests.

---

## Files You'll Modify

* **`lib.c`**: Define your structures and implement the slab management logic.

## Files Provided

* **`lib.h`**: Public struct declarations, constants (like `PAGE_SIZE`), and function prototypes.
* **`test.c`**: Comprehensive testing suite with 30 test cases covering edge cases and stress tests.
* **`Makefile`**: Build instructions.
