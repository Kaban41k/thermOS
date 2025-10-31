#ifndef alloc
#define alloc

// Alloc Utilities

#include "types.h"

void* malloc_immortal(u32 size, u32 align);
void* calloc_immortal(u32 size, u32 align);

#endif
