#include <stddef.h>

#include "kernelpanic.h"
#include "memu.h"

#define ARENA_START 0x7C00 + KERNEL_SIZE
#define ARENA_END 0x80000

char* ptr = (char*) ARENA_START;

void* malloc_immortal(size_t size, size_t align) {
  if (size == 0) return NULL;
  if (align == 0) align = 1;

  if ((size_t) ptr % align != 0)
    ptr += align - (size_t) ptr % align;

  if (ptr + size > (char*) ARENA_END)
    kernel_panic("ARENA OVERFLOW, last obj size %d bytes with %d align", size, align);

  void* result = (void*) ptr;
  ptr += size;
  return result;
}

void* calloc_immortal(size_t size, size_t align) {
  void* result = malloc_immortal(size, align);
  
  if (result == NULL) return NULL;
  
  memzero(result, size);
  return result;
}