#include <stddef.h>
#include <stdarg.h>
#include "vgau.h"
#include "kernelpanic.h"

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

void cursor_print(char c) {
  win_print_char(*cur_win, c, cursor.x, cursor.y);
  cursor_next();
}

void print_int(unsigned int n, unsigned short int base) {
  char c;
  char buf[32];

  int l = 0;
  while (n > 0) {
    int res = n % base;

    if (res < 10) {
      c = '0' + res;
    } else {
      c = 'A' + (res - 10);
    }

    buf[l++] = c;
    n /= base;
  }

  for (size_t i = 0; i < l; i++) {
    cursor_print(buf[l - i - 1]);
  }
}

void print_d(int n) {
  if (n < 0) {
    cursor_print('-');
    n *= -1;
  }

  print_int(n, 10);
}

void print_s(const char* fmt) {
  if (*fmt == '\0') return;

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
    
    cursor_print(*(fmt + offset));
  } while (*(fmt + ++offset) != '\0');
}

void vprintf(const char* fmt, va_list args) {
  char c;
  if ((c = *fmt) == '\0') return;

  size_t offset = 0;
  
  do {
    if (c == '%') {
      c = *(fmt + ++offset);

      switch (c) {
        case 'd': {
          int n = va_arg(args, int); 
          print_d(n);
          continue;
        }
        
        case 'x': {
          int n = va_arg(args, int); 
          print_int(n, 16);
          continue;
        }
        
        case 'c': {
          c = va_arg(args, int);
          break;
        }
        
        case 's': {
          const char* s = va_arg(args, const char*);
          print_s(s);
          continue;
        }
        
        default:
          kernel_panic("Unknown type specifier: %c", c);
          return;
        }
      
    }

    if (c == '\n') {
      line_feed();
      continue;
    }

    if (c == '\r') {
      cursor.x = 1;
      continue;
    }
    
    cursor_print(c);
  } while ((c = *(fmt + ++offset)) != '\0');
}

void printf(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
}
