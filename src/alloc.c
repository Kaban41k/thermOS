#include <stdarg.h>
#include <stddef.h>
#include "types.h"
#include "kernelpanic.h"
#include "memu.h"

#define ARENA_START 0x10000
#define ARENA_END 0x40000

char* ptr = (char*) ARENA_START;

void* malloc_immortal(u32 size, u32 align) {
  if (size == 0) return NULL;
  if (align == 0) align = 1;

  if ((u32) ptr % align != 0)
    ptr += align - (u32) ptr % align;

  if (ptr + size > (char*) ARENA_END)
    kernel_panic("ARENA OVERFLOW, last obj size %d bytes with %d align", size, align);

  void* result = (void*) ptr;
  ptr += size;
  return result;
}

void* calloc_immortal(u32 size, u32 align) {
  void* result = malloc_immortal(size, align);
  
  if (result == NULL) return NULL;
  
  memzero(result, size);
  return result;
}