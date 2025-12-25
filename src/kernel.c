#include <stddef.h>
#include "vgau.h"
#include "outputu.h"
#include "assert.h"
#include "alloc.h"
#include "memu.h"

extern void inf_loop();

void test();
void test2();
void kernel_main();

void kernel_entry() {
  vga_clear_screen();
  
  kernel_main();

  inf_loop();
}

void kernel_main() {
  vga_fill_screen('#');
  Window main = create_window(60, 23, 2, 1);

  win_select_color(main, 0x2);
  
  select_win(&main);

  for (size_t i = 1; i; i++) {
    void* obj = malloc_immortal(i*3, i);
    printf("%d byte object, alignment: %d, address: %x\n", i*10, i, (size_t) obj);
  }
}

void test2() {
  memzero((void*) 0xB8000, 0xFA0);

  vga_fill_screen('#');
  Window main = create_window(70, 20, 2, 2);

  win_select_color(main, 0x2);


  vga_fill_screen('#');
  select_win(&main);
  printf("Hello!");
}

void test() {
  vga_fill_screen('}');

  Window main = create_window(20, 21, 2, 2);
  Window win1 = create_window(30, 10, 24, 2);
  Window win2 = create_window(20, 10, 24, 13);
  Window win3 = create_window(20, 10, 45, 13);

  win_clear(main);
  win_clear(win1);
  win_clear(win2);
  win_clear(win3);

  win_select_color(main, 0x2);
  win_select_color(win1, 0x4);
  win_select_color(win2, 0x5);
  win_select_color(win3, 0x1);

  select_win(&main);  

  printf("a");

  for (int i = 0; i < 1000; i++) {
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