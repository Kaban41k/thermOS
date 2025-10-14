#ifndef alloc
#define alloc

// Alloc Utilities

#include <stddef.h>

void* malloc_immortal(size_t size, size_t align);
void* calloc_immortal(size_t size, size_t align);

#endif
