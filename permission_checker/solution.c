#include "lib.h"

#include <assert.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>

// NOTE: will only work on little-endian architecture (x86)

union {
  page_table_entry_t entry;
  struct {
    uint16_t ppn : PPN_BITS;
    uint16_t valid : 1;
    uint16_t read : 1;
    uint16_t write : 1;
    uint16_t exec : 1;  // bit 15, msb
  } bits;
} u_entry;

result_t access_memory(struct page_table* pt, va_t virtual_address,
                       action_type_t action, pa_t* physical_address) {
  // in a virtual address,
  uint32_t v_offset = virtual_address & 0xff;  // lower 8 bits are offset
  uint32_t v_vpn = (virtual_address >> PAGE_OFFSET_BITS) &
                   ((1 << VPN_BITS) - 1);  // next 14 bits are vpn

  // check the entry
  u_entry.entry = pt->entries[v_vpn];

  if (!u_entry.bits.valid) return PAGE_FAULT;
  if ((action == READ && !u_entry.bits.read) ||
      (action == WRITE && !u_entry.bits.write) ||
      (action == EXECUTE && !u_entry.bits.exec))
    return PERMISSION_DENIED;

  // combine physical address
  *physical_address = u_entry.bits.ppn << PAGE_OFFSET_BITS | v_offset;
  return SUCCESS;
}
