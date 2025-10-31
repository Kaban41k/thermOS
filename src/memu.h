#ifndef memu
#define memu

// Memory Utilities

#include "types.h"

void* memmove(void* dst, const void* src, u32 n);
void* memzero(void* dst, u32 n);

#endif
