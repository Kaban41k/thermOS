#include <stddef.h>
#include "kernelutils/vgau.h"
#include "kernelutils/outputu.h"

extern void inf_loop();

void kernel_entry() {
  vga_fill_screen('}');

  window main = create_window(20, 21, 2, 2);
  window win1 = create_window(30, 10, 24, 2);
  window win2 = create_window(20, 10, 24, 13);
  window win3 = create_window(20, 10, 45, 13);

  win_clear(main);
  win_clear(win1);
  win_clear(win2);
  win_clear(win3);

  win_select_color(main, 0x2);
  win_select_color(win1, 0x4);
  win_select_color(win2, 0x5);
  win_select_color(win3, 0x1);
  



  init_printer(&main);
  select_win(&main);  

  printf("a");
  win_print_char(*cur_win, 'f', 1, 1);

  for (int i = 0; i < 1000; i++) {
    select_win(&win1);
    printf("Hello\n"
      "I'm WIN1\n"
      "a\n"
      "b\n"
      "c\n"
      "d\n"
      "e\n");

    select_win(&win2);
    printf("Hello\n"
      "I'm WIN2\n"
      "a\n"
      "b\n"
      "c\n"
      "d\n"
      "e\n");


    select_win(&win3);
    printf("Hello\n"
      "I'm WIN3\n"
      "a\n"
      "b\n"
      "c\n"
      "d\n"
      "e\n");


    select_win(&main);
    printf("Hello\n"
      "I'm MAIN\n"
      "a\n"
      "b\n"
      "c\n"
      "d\n"
      "e\n");

    }

  inf_loop();
}
