#include "types.h"
#include "asmu.h"
#include "vgau.h"
#include "outputu.h"
#include "assert.h"
#include "alloc.h"
#include "interrupter.h"
  
void kernel_entry() {
  init_interrupter();
  init_pic8259_master(true);
  init_pic8259_slave();

  vga_clear_screen();
  vga_fill_screen('#');
  Window main = create_window(76, 21, 2, 2);
  win_select_color(main, 0x2);
  win_clear(main);
  select_win(&main);


  pic8259_enable_device(SYSTEM_TIMER);
  pic8259_enable_device(KEYBOARD);

  sti();
 
  for (u64 i = 0; i < 1000000; i++) {
    //global_plus();
  }
 
  inf_loop();
}