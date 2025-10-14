#ifndef kernelpanic
#define kernelpanic

// Kernel Panic Utilities

#include <stdarg.h>


void vkernel_panic(const char* format, va_list args);
void kernel_panic(const char* format, ...);

#endif
