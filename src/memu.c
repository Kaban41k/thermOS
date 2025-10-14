#include <stddef.h>


void* memmove(void* dst, const void* src, size_t n) {
  if (src >= dst) {
    for (size_t offset = 0; offset < n; offset++) {
      *((char*) dst + offset) = *((char*) src + offset);
    }
  } else {
    size_t offset;
    for (size_t i = 0; i < n; i++) {
      offset = n - i - 1;
      *((char*) dst + offset) = *((char*) src + offset);
    }
  }

  return dst;
}

void* memzero(void* dst, size_t n) {
  for (size_t offset = 0; offset < n; offset++) {
    *((char*) dst + offset) = 0;
	}

  return dst;
}
