#ifndef output
#define output

// Output Utilities

#include <stdarg.h>
#include "types.h"
#include "vgau.h"

/*
  Saves the position in the current window 
    and set the cursor on win.

  Window* win : the window to be selected
*/
void select_win(Window* win);

/*
  Move the cursor to the beginning of the 
    next line in current window 
    with scroll, if necessary.
*/
void line_feed();

/*
  Move the cursor to the (x, y) 
    position in current window.

  u32 x : new x coordinate 
  u32 y : new y coordinate
*/
void cursor_move(u32 x, u32 y);

/*
  Move the cursor to the next character 
    in current window, if it goes beyond 
    the end of the line, the line_feed() 
    called.
*/
void cursor_next();

/*
  Places the c character at the current 
  cursor position in current window and
  call cursor_next().

  char c : char to be placed
*/
void print_char(char c);

/*
  Formats and prints a string with variable arguments 
    to the current window.
  
  const char* fmt : format string (may contain format specifiers)
  va_list args    : variable arguments list
  
  Supported format specifiers:
    %R - register
    %d - integer
    %x - hexadecimal
    %c - character
    %s - string
*/
void vprintf(const char* fmt, va_list args);

/*
  Formats and prints a string with variable arguments 
    to the current window.
  
  const char* fmt : format string (contains format specifiers)
  ...             : variable arguments to format
  
  This function wraps vprintf() and handles 
    variable argument list initialization.
*/
void printf(const char* fmt, ...);

#endif
