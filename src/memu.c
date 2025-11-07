#include <stddef.h>
#include "types.h"

void* memmove(void* dst, const void* src, u32 n) {
  if (src >= dst) {
    for (u32 offset = 0; offset < n; offset++) {
      *((char*) dst + offset) = *((char*) src + offset);
    }
  } else {
    u32 offset;
    for (u32 i = 0; i < n; i++) {
      offset = n - i - 1;
      *((char*) dst + offset) = *((char*) src + offset);
    }
  }

  return dst;
}

void* memzero(void* dst, u32 n) {
  for (u32 offset = 0; offset < n; offset++) {
    *((char*) dst + offset) = 0;
	}

  return dst;
}
