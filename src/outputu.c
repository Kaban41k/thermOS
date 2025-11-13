#include <stdarg.h>
#include <stddef.h>
#include "types.h"
#include "assert.h"
#include "vgau.h"
#include "kernelpanic.h"

typedef struct {
	u32 x;
	u32 y;
} Cursor;

Cursor cursor = {.x = 1, .y = 1};
Window* cur_win = NULL;

void select_win(Window* win) {
  if (cur_win != NULL) {
    cur_win->cursor_x = cursor.x;
    cur_win->cursor_y = cursor.y;
  }

  cursor.x = win->cursor_x;
  cursor.y = win->cursor_y;
  cur_win = win;
}

void scroll_checker() {
  assert(cur_win != NULL);
  
  if (cursor.y > cur_win->y_size - 1) {
    cursor.y = cur_win->y_size - 1;
    win_scroll_down(*cur_win);
  }
}

void line_feed() {
  assert(cur_win != NULL);

  cursor.x = 1;
  cursor.y++;
  scroll_checker();
}

void line_feed_checker() {
  assert(cur_win != NULL);

  if (cursor.x > cur_win->x_size - 2) {
    line_feed();
  }
}

void cursor_move(u32 x, u32 y) {
  assert(cur_win != NULL);
  assert(x < cur_win->x_size);
  assert(y < cur_win->y_size);
  
  cursor.x = x;
  cursor.y = y;
}

void cursor_next() {
  assert(cur_win != NULL);

  cursor.x++;
  line_feed_checker();
}

void print_char(char c) {
  assert(cur_win != NULL);

  win_print_char(*cur_win, c, cursor.x, cursor.y);
  cursor_next();
}

void print_int(u32 n, u16 base) {
  assert(cur_win != NULL);
  assert(1 < base && base < 32)

  char c;
  char buf[32];

  u16 l = 0;

  if (n == 0) {
    print_char('0');
    return;
  }

  while (n > 0) {
    i64 res = n % base;

    if (res < 10) {
      c = '0' + res;
    } else {
      c = 'A' + (res - 10);
    }

    buf[l++] = c;
    n /= base;
  }
  
  for (u16 i = 0; i < l; i++)
    print_char(buf[l - i - 1]);
}

void print_d(i32 n) {
  assert(cur_win != NULL);

  if (n < 0) {
    print_char('-');
    n *= -1;
  }

  print_int(n, 10);
}

void print_s(const char* fmt) {
  assert(cur_win != NULL);

  if (*fmt == '\0') return;

  u64 offset = 0;
  
  do {
    if (*(fmt + offset) == '\n') {
      line_feed();
      continue;
    }

    if (*(fmt + offset) == '\r') {
      cursor.x = 1;
      continue;
    }
    
    print_char(*(fmt + offset));
  } while (*(fmt + ++offset) != '\0');
}

void print_reg(u32 n, u16 l) {
  assert(cur_win != NULL);

  u32 x = n;
  u16 n_l = 0;

  do {
    x /= 16;
    n_l++;
  } while (x != 0);

  print_s("0x");
  for (u32 i = 0; i < l - n_l; i++)
    print_s("0");

  print_int(n, 16);
}

void vprintf(const char* fmt, va_list args) {
  assert(cur_win != NULL);
  char c;
  if ((c = *fmt) == '\0') return;

  u64 offset = 0;
  
  do {
    if (c == '%') {
      c = *(fmt + ++offset);

      switch (c) {
        case 'R': {
          i32 n = va_arg(args, i32);
          print_reg(n, 8);
          continue;
        }
        
        case 'd': {
          i32 n = va_arg(args, i32); 
          print_d(n);
          continue;
        }
        
        case 'x': {
          i32 n = va_arg(args, i32); 
          print_int(n, 16);
          continue;
        }
        
        case 'c': {
          c = va_arg(args, i32);
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
    
    print_char(c);
  } while ((c = *(fmt + ++offset)) != '\0');
}

void printf(const char* fmt, ...) {
  assert(cur_win != NULL);
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
}
