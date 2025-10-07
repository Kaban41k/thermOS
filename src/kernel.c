#include <stddef.h>
#include "kernelutils/vgau.h"

extern void inf_loop();

void kernel_entry() {
  vga_fill_screen('@');

  window main = create_window(40, 21, 20, 2);

  win_clear(main);
  win_print_char(main, '0', 1, 2);
  win_print_char(main, '1', 2, 5);
  win_print_char(main, '2', 3, 6);
  win_scroll_down(main);
  win_scroll_down(main);

  inf_loop();
}
