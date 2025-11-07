#ifndef alloc
#define alloc

// Alloc Utilities

#include "types.h"

/*
  Memory allocation for an immortal object.
  All memory is filled with undefined values.

  u32 size  : size of object in bytes
  u32 align : align in memory
*/
void* malloc_immortal(u32 size, u32 align);

/*
  Memory allocation for an immortal object.
  All memory is filled with zeros.

  u32 size  : size of object in bytes
  u32 align : align in memory
*/
void* calloc_immortal(u32 size, u32 align);

#endif
