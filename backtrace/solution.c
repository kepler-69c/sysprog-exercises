#include "lib.h"

#include <assert.h>
#include <stdalign.h>
#include <stdint.h>
#include <stdio.h>

// ===== stack frame layout: =====
// lower memory (empty)
// -------------
// local variables
// saved rbp (of previous frame)
// return address
// function parameters (if passed on stack)
// -------------
// higher memory (function that called us)

void print_backtrace(FILE* file, void* current_rbp,
                     const struct debugging_info* dbg_info_array,
                     size_t dbg_info_count) {
  void** rbp = (void**)current_rbp;  // cast ptr-to-ptr for easy access
  while (rbp) {
    // NOTE: if this were a real stack and a function didn't push rbp,
    // rbp[0] would be garbage and the backtrace would stop prematurely.
    // in this simulated stack exercise, all frames have valid rbp.

    void* ret_addr = rbp[1];  // return address is immediately below rbp

    const char* file_name = "¿?";
    const char* func_name = "¿?";

    // search debug info for the return address
    // NOTE: this is O(m) for m entries, but we cannot use use e.g. binary
    // search because the entries are not sorted and I don't want to sort
    for (size_t i = 0; i < dbg_info_count; ++i) {
      if (ret_addr >= dbg_info_array[i].start_addr &&
          ret_addr <= dbg_info_array[i].end_addr) {
        file_name = dbg_info_array[i].file_name;
        func_name = dbg_info_array[i].function_name;
        break;
      }
    }

    fprintf(file, "%p %s:%s\n", ret_addr, file_name, func_name);
    rbp = (void**)rbp[0];  // previous rbp
  }
}
