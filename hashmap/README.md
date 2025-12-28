# Generic Hash Map Implementation

## Goal

Implement a generic **Hash Map** (Hash Table) data structure in C. This exercise focuses on memory management, the use of `void *` for generic data handling, and the implementation of an **Iterator** pattern.

You must create a map that can store keys and values of arbitrary sizes, handling memory allocation internally to ensure the map owns its data.

---

## Constraints and Requirements

* **Generic Storage**: Your hash map must support any data type for both keys and values. This is achieved by storing the `key_size` and `value_size` at creation time and using `memcpy` to store data.
* **Collision Handling**: You must implement a collision resolution strategy. The suggested method is **Separate Chaining** (linked lists at each bucket).
* **Memory Ownership**: When a user "puts" a key-value pair into the map, you must allocate memory and copy the data. When an item is removed or the map is cleared, you must free that memory.
* **Iterator Pattern**: You must implement a heap-allocated iterator that allows users to traverse all elements currently in the map.
* **Fixed Bucket Size**: For this exercise, you do not need to implement dynamic resizing (rehashing). The number of buckets is fixed at the time of creation.

---

## Core API

### Map Management
* **`hashmap_create`**: Allocates and initializes the map metadata and the bucket array. Returns `NULL` if `num_buckets` is 0.
* **`hashmap_put`**: Inserts a key-value pair. If the key already exists, update the value. 
* **`hashmap_get`**: Retrieves a pointer to the value associated with a key.
* **`hashmap_remove`**: Removes a specific key and its associated value from the map.
* **`hashmap_clear`**: Removes all entries without destroying the map itself.
* **`hashmap_free`**: Deallocates all entries and the map structure.

Example user usage:
```c
struct hashmap *map = HASHMAP_CREATE(1024, int, double);
int key = 42;
double value = 3.14;
hashmap_put(map, &key, &value);
double retrieved;
if (hashmap_get(map, &key, &retrieved)) {
    // retrieved now holds 3.14
}
hashmap_free(map);
```

### The Iterator

The iterator provides sequential access to the elements in the hashmap.

* **`hashmap_iterator_create`**: Allocates and initializes an iterator for the given map. The iterator is initially positioned *before* the first element.
* **`hashmap_iterator_next`**: Advances the iterator to the next element and returns `true` if a valid element exists, `false` otherwise.
* **`hashmap_iterator_key`**: Returns a pointer to the current key. Valid only after `hashmap_iterator_next` has returned `true`.
* **`hashmap_iterator_value`**: Returns a pointer to the current value. Valid only after `hashmap_iterator_next` has returned `true`.
* **`hashmap_iterator_free`**: Deallocates the iterator.

Example usage:
```c
hashmap_iterator_t *it = hashmap_iterator_create(map);

while (hashmap_iterator_next(it)) {
    void *key = hashmap_iterator_key(it);
    void *value = hashmap_iterator_value(it);
    /* use key and value */
}

hashmap_iterator_free(it);
```

Notes:
- There is no separate `has_next` function; `hashmap_iterator_next` both advances the iterator and reports completion.
- Accessing the key or value before the first successful call to `hashmap_iterator_next` is undefined behavior.

---

## Testing Your Code

The provided test suite includes 39 test cases covering:
* **Basic Operations**: Put, get, contains, and remove functionality.
* **Collisions**: Handling multiple keys mapping to the same bucket.
* **Memory**: Overwriting existing keys and clearing the map.
* **Iterators**: Stability, multiple concurrent iterators, and full traversal.

To run the tests:

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
* Suite hashmap_suite:
............................
* Suite hashmap_iterator_suite:
...........

39 tests - 39 pass, 0 fail, 0 skipped
```

---

## Files You'll Modify

* **`lib.c`**: You must define the internal structures `struct hashmap`, `struct hashmap_node`, and `struct hashmap_iterator` here, along with all the required logic.

## Files Provided

* **`lib.h`**: Header containing the hash function, the `HASHMAP_CREATE` macro, and function prototypes.
* **`greatest.h`**: The unit testing framework.
* **`Makefile`**: Build instructions.
