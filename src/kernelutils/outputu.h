#ifndef output
#define output

// Output Utilities

#include <stddef.h>
#include <stdarg.h>
#include "vgau.h"

typedef struct {
	size_t x;
	size_t y;
} vcursor;

vcursor cursor = {.x = 1, .y = 1};
window* cur_win;

void init_printer(window* win) {
  cur_win = win;

  cursor.x = 1;
  cursor.y = 1;
}

void select_win(window* win) {
  cur_win->cursor_x = cursor.x;
  cur_win->cursor_y = cursor.y;
  cursor.x = win->cursor_x;
  cursor.y = win->cursor_y;
  cur_win = win;
}

void scroll_checker() {
  if (cursor.y > cur_win->y_size - 1) {
    cursor.y = cur_win->y_size - 1;
    win_scroll_down(*cur_win);
  }
}

void line_feed() {
  cursor.x = 1;
  cursor.y++;
  scroll_checker();
}

void line_feed_checker() {
  if (cursor.x > cur_win->x_size - 2) {
    line_feed();
  }
}

void cursor_next() {
  cursor.x++;
  line_feed_checker();
}

void printf(const char* fmt) {
  size_t offset = 0;
  
  do {
    if (*(fmt + offset) == '\n') {
      line_feed();
      continue;
    }

    if (*(fmt + offset) == '\r') {
      cursor.x = 1;
      continue;
    }
    
    win_print_char(*cur_win, *(fmt + offset), cursor.x, cursor.y);
    cursor_next();
  } while (*(fmt + ++offset) != '\0');
}

void vprintf(const char* fmt, va_list args) {
  size_t offset = 0;
  
  do {
    if (*(fmt + offset) == '\n') {
      line_feed();
      continue;
    }

    if (*(fmt + offset) == '\r') {
      cursor.x = 1;
      continue;
    }
    
    win_print_char(*cur_win, cursor.x, cursor.y, *(fmt + offset));
    cursor_next();
  } while (*(fmt + ++offset) != '\0');
}

#endif