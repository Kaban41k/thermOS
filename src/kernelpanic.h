#ifndef kernelpanic
#define kernelpanic

// Kernel Panic Utilities

#include <stdarg.h>
#include "types.h"

/*
  Is called when a critical error is detected.
  The function clears the screen, displays text,
  and goes into an eternal loop.

  
  const char* fmt : format string (contains format specifiers)
  ...             : variable arguments to format
*/

void kernel_panic(const char* fmt, ...);

#endif
