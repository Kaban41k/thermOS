#include <stdarg.h>
#include "vgau.h"
#include "outputu.h"


extern void inf_loop();
extern void cli();


void vkernel_panic(const char* fmt, va_list args) {
  cli();
  vga_clear_screen();

  window screen = create_window(80, 25, 0, 0);
  select_win(&screen);
  win_select_color(screen, 5);

  printf("Kernel panic!!!\n\n");
  vprintf(fmt, args);
  inf_loop();
}

void kernel_panic(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vkernel_panic(fmt, args);
  va_end(args);
}
