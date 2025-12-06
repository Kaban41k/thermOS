#include "types.h"
#include "asmu.h"
#include "vgau.h"
#include "outputu.h"
#include "assert.h"
#include "alloc.h"
#include "interrupter.h"
#include "controllertests.h"
  
void kernel_entry() {
  vga_clear_screen();
  vga_fill_screen('#');
  Window main = create_window(76, 21, 2, 2);
  win_select_color(main, 0x2);
  win_clear(main);
  select_win(&main);

  EXPERIMENT_20;
 
  inf_loop();
}