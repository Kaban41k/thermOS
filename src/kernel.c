#include "types.h"
#include "memu.h"
#include "asmu.h"
#include "vgau.h"
#include "outputu.h"
#include "assert.h"
#include "alloc.h"
#include "interrupter.h"
#include "pit.h"
#include "userspaceu.h"
#include "userspacetests.h"

#define USERSPACE_STACK_SIZE 1024

void setup_screen();
void setup_interrupter(InterruptType type, EoiMode mode);

Window main_window;

extern segment_descriptor kernel_code_segment_descriptor;
extern segment_descriptor kernel_data_segment_descriptor;
extern segment_descriptor userspace_code_segment_descriptor;
extern segment_descriptor userspace_data_segment_descriptor;

void p() {
  for (;;) {
    printf("Hello!!!");
  }
}

void user_main() {
  //memmove((void*) vga_clear_screen, (void*) hack, 20);
  //inf_loop();
}

typedef struct {
  int b;
  short a;
  short d;
  int c;
} Test;

void kernel_main() {
  printf("%d", sizeof(Test));
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
  init_pit8254();
}
