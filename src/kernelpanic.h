#ifndef kernelpanic
#define kernelpanic

// Kernel Panic Utilities

#include <stdarg.h>
#include "types.h"

void vkernel_panic(const char* format, va_list args);
void kernel_panic(const char* format, ...);

#endif
