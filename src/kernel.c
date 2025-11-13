#include "types.h"
#include "asmu.h"
#include "vgau.h"
#include "outputu.h"
#include "assert.h"
#include "alloc.h"
#include "interrupter.h"

extern void sti();
extern void inf_loop();
extern void set_regs();
extern void div_zero();
extern void int_n();

void collect_context();

void kernel_entry() {
  setup_interrupter();

  vga_clear_screen();
  window main = create_window(60, 40, 2, 2);

  win_select_color(main, 0x2);
  
  init_printer(&main);
  select_win(&main);

  printf("boom");

  set_regs();
  //collect_context();
  div_zero();
  //int_n();
  //sti();

  //inf_loop();
}

void test() {
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

  for (int i = 0; i < 100; i++) {
    select_win(&win1);
    char* s = "seg";
    printf("%s\n", s);

    select_win(&win2);
    int n = 19897;
    printf("%d\n", n);


    n = 158;
    select_win(&win3);
    printf("%x\n", n);


    select_win(&main);

    char p = i % 200;
    printf("%c\n", p);
  }

  assert('a' == 'b');

  printf("%f", &main);
  inf_loop();
}
