#include "types.h"
#include "asmu.h"
#include "vgau.h"
#include "outputu.h"
#include "assert.h"
#include "alloc.h"
#include "interrupter.h"
#include "userspaceu.h"

#define USERSPACE_STACK_SIZE 1024

void setup_screen();
void setup_interrupter(InterruptType type, EoiMode mode);

Window main_window;

void user_main() {
  printf("I'M USER OH YEAH");
  inf_loop();
}

void kernel_main() {
  win_clear(main_window);

  void* userspace_stack = malloc_immortal(USERSPACE_STACK_SIZE, 16) + USERSPACE_STACK_SIZE;

  userspace_process(user_main, userspace_stack);

  printf("the end");
}

void kernel_entry() {
  setup_screen();
  setup_interrupter(INTERRUPT_GATE, AUTO_EOI);
  setup_tss();

  sti();

  kernel_main();

  inf_loop();
}

void setup_screen() {
  vga_clear_screen();
  vga_fill_screen('#');
  main_window = create_window(76, 21, 2, 2);
  win_select_color(main_window, 0x2);
  win_clear(main_window);
  select_win(&main_window);
}

void setup_interrupter(InterruptType type, EoiMode mode) {
  init_interrupter(type);
  init_pic8259_master(mode);
  init_pic8259_slave();
}