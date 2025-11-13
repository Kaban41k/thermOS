#ifndef output
#define output

// Output Utilities

#include <stdarg.h>
#include "types.h"
#include "vgau.h"

typedef struct {
	u32 x;
	u32 y;
} vcursor;

void init_printer(window* win);
void select_win(window* win);
void scroll_checker();
void line_feed();
void line_feed_checker();
void cursor_next();
void cursor_print(char c);
void vprintf(const char* fmt, va_list args);
void printf(const char* fmt, ...);

#endif
