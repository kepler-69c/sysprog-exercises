# Fixed-Size Generic Stack

## Goal

Implement a **generic stack** data structure with a fixed capacity.

You must implement the library defined in `lib.h`. The stack must be able to store elements of any data type (integers, doubles, structs) by handling memory as raw bytes (`void *`) and managing a pre-allocated buffer.

---

## Constraints and Requirements

* **Generic Storage**: The stack is agnostic to the data type it stores. You must use `void *` pointers and `memcpy` to move data in and out of the internal storage.
* **Fixed Capacity**: The capacity is determined at initialization and does not change. If the stack is full, `sstack_push` must return `false` rather than resizing.
* **Memory Management**:
    * `sstack_init` must allocate the internal buffer on the heap based on the `capacity` and `item_size`.
    * `sstack_destroy` must free any allocated memory.
    * The `stack_t` struct itself is allocated by the caller (on the stack or heap), but you manage its internal `data` pointer.

---

## Core API

### Lifecycle
* **`sstack_init`**: Allocates the internal buffer for `capacity` items of `item_size` bytes each.
* **`sstack_destroy`**: Frees the internal buffer.

### Operations
* **`sstack_push`**: Copies an item onto the top of the stack. Returns `true` if successful, or `false` if the stack is full.
* **`sstack_pop`**: Copies the item at the top of the stack into the provided `item` pointer and removes it. Returns `true` if successful, or `false` if the stack is empty.
* **`sstack_clear`**: Resets the stack index to 0 effectively emptying it, without freeing the internal memory.

### State
* **`sstack_is_empty` / `sstack_is_full`**: Boolean checks for stack state.
* **`sstack_size`**: Returns the current number of elements.
* **`sstack_capacity`**: Returns the maximum number of elements the stack can hold.

---

## Testing Your Code

The provided test suite covers:
* Push/Pop logic for various types (`int`, `double`, `struct`).
* Overflow (pushing to full) and Underflow (popping from empty) protection.
* Memory integrity and correct LIFO (Last-In-First-Out) ordering.

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
* Suite stack_suite:
....................

20 tests - 20 pass, 0 fail, 0 skipped
```

---

## Files You'll Modify

* **`lib.c`**: Implement the `stack_t` logic.

## Files Provided

* **`lib.h`**: Struct definitions and function prototypes.
* **`test.c`**: The unit testing suite.
* **`Makefile`**: Build instructions.
