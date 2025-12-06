#include <stdarg.h>
#include "asmu.h"
#include "vgau.h"
#include "outputu.h"
#include "interrupter.h"

#define KERNEL_PANIC_CONTEXT_STRING  \
  "unhandled interrupt #%x at %R:%R\n\n"   \
  "Registers: \n" \
  "  EAX: %R,  EBX: %R,  ECX: %R,  EDX: %R,\n" \
  "  EDI: %R,  ESI: %R,  ESP: %R,  EBP: %R,\n" \
  "  DS : %R,  ES : %R,  GS : %R,  FS : %R\n\n" \
  "Error code: %R\n\n" \
  "EFLAGS: %R\n"

void vkernel_panic(const char* fmt, va_list args) {
  cli();
  vga_clear_screen();

  Window screen = create_window(80, 25, 0, 0);
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

void kernel_panic_ctx(interrupt_context* context) {
  kernel_panic(KERNEL_PANIC_CONTEXT_STRING,
        context->vector, context->cs, context->eip, context->eax, context->ebx, context->ecx, context->edx,
        context->edi, context->esi, context->esp, context->ebp, context->ds, context->es, context->gs, context->fs, 
        context->error_code,
        context->eflags
      );
}
