# Bloom Filter Implementation

## Goal

Implement a **Bloom Filter**, a space-efficient probabilistic data structure used to test whether an element is a member of a set. This exercise focuses on bit manipulation, hash function application, and understanding the trade-offs between space, speed, and accuracy in systems programming.

You must implement the library functions defined in `lib.h` for both a underlying `bitset_t` and the higher-level `bloom_filter_t`.

---

## The Problem

A [Bloom Filter](https://en.wikipedia.org/wiki/Bloom_filter) is used to determine if an item is "possibly in set" or "definitely not in set". It uses a bit array and multiple hash functions. When an item is added, several bits (determined by hash functions) are set to 1. To check for membership, you verify if all those bits are 1.

### Data Structures

* **`bitset_t`**: A structure representing a sequence of bits, packed into an array of `uint8_t`.
* **`bloom_filter_t`**: A wrapper that contains a `bitset_t` and logic to interface with the provided hash functions.

## Constraints and Requirements

* **Bit Manipulation**: In `bitset_t`, you must perform bitwise operations to read and set individual bits efficiently.
* **Hashing**: Use the provided `hash(const void *data, size_t size, int idx)` function. Note that `idx` must range from `0` to `NUM_HASHES - 1`: each index acts as a different hash function.
* **Memory Management**: Properly use `calloc` or `malloc/memset` to ensure that new bitsets are initialized to zero. Ensure `bitset_free` and `bloom_filter_free` clean up all allocated memory.

---

## Testing Your Code

The provided test suite includes over 40 test cases covering:
* Standard bitset set/get and byte-alignment boundaries.
* Large-scale bitset clearing.
* Bloom Filter false-negative verification (should always be 0%).
* False-positive rate testing (should be within expected probabilistic bounds).
* Handling of arbitrary binary data (null bytes, structs, large buffers).

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
* Suite bitset_suite:
....................
* Suite bloom_filter_suite:
....................

40 tests - 40 pass, 0 fail, 0 skipped
```

---

## Files You'll Modify

* **`lib.c`**: Implement the bitset and bloom filter logic.

## Files Provided

* **`lib.h`**: Header containing the data structures, the `hash` function, and function prototypes.
* **`greatest.h`**: The unit testing framework.
* **`Makefile`**: Build instructions.
