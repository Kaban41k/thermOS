#include "types.h"
#include "assert.h"
#include "kernelpanic.h"
#include "alloc.h"

#define TRAMPOLINE_SIZE 8
#define VECTORS_N 256

extern void inf_loop();
extern void collect_context();
extern void lidt(void* idt_ptr);

typedef struct {
  u16    offset_low;
  u16    seg_selector;
  uchar  reserved : 5;
  uchar  zeros    : 3;
  uchar  type     : 3;
  uchar  d        : 1;
  uchar  zero     : 1;
  uchar  dpl      : 2;
  uchar  present  : 1;
  u16    offset_high;
} interrupt_descriptor;

typedef struct interrupt_context {
  u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
  alignas(4) u16 gs, fs, es, ds;
  alignas(4) uchar vector;
  u32 error_code;
  u32 eip;
  alignas(4) u16 cs;
  u32 eflags;
} interrupt_context;

static_assert(sizeof(interrupt_descriptor) == 8);
static_assert(sizeof(interrupt_context) == 17 * sizeof(u32));

static bool has_error_code(uchar vector) {
  switch (vector) {
    case 0x8:
    case 0xA:
    case 0xB:
    case 0xC:
    case 0xD:
    case 0xE:
    case 0x11:
    case 0x15:
      return 1;
    default:
      return 0;
  }
}

static uchar* generate_trampolines() {
  uchar* trampolines = malloc_immortal(TRAMPOLINE_SIZE * VECTORS_N, 16);

  for (u16 vector = 0; vector < VECTORS_N; vector++) {
    uchar* trampoline = trampolines + vector * TRAMPOLINE_SIZE;
    u32 offset = 0;

    if (has_error_code(vector))
      trampoline[offset++] = 0x50; // push eax (if needed)
    trampoline[offset++] = 0x6A;   // push const
    trampoline[offset++] = vector; //      const = vector
    trampoline[offset++] = 0xE9;   // jmp ...
    u32 jmp_end = (u32) collect_context - (u32) (trampoline + offset + 4);
    *(u32*)(trampoline + offset) = jmp_end;
  }

  return trampolines;
}

static void* generate_idt(uchar* trampolines) {
  interrupt_descriptor* idt = 
    malloc_immortal(sizeof(interrupt_descriptor) * VECTORS_N, sizeof(interrupt_descriptor));

  for (u16 vector = 0; vector < VECTORS_N; vector++) {
    uchar* trampoline = trampolines + vector * TRAMPOLINE_SIZE;

    idt[vector].offset_low    = (u32)(trampoline) & 0xFFFF;
    idt[vector].seg_selector  = 0x8;
    idt[vector].reserved      = 0b0;
    idt[vector].zeros         = 0b000;
    idt[vector].type          = 0b110;
    idt[vector].d             = 0b1;
    idt[vector].zero          = 0b0;
    idt[vector].dpl           = 0b00;
    idt[vector].present       = 0b1;
    idt[vector].offset_high   = ((u32)(trampoline) >> 16) & 0xFFFF;
    assert(sizeof(idt[vector]) == 8);
  }

  return idt;
}

void setup_interrupter() {
  uchar* trampolines = generate_trampolines();
  void* idt = generate_idt(trampolines);
  u16 idt_limit = VECTORS_N * sizeof(interrupt_descriptor) - 1;
  u64 idt_ret = ((u64) idt << 16) | idt_limit; 
  lidt(&idt_ret);
}

void universal_handler(interrupt_context* context) {
  kernel_panic(
    "unhandled interrupt #%x at %R:%R\n\n"
    "Registers: \n"
    "  EAX: %R,  EBX: %R,  ECX: %R,  EDX: %R,\n"
    "  EDI: %R,  ESI: %R,  ESP: %R,  EBP: %R,\n"
    "  DS : %R,  ES : %R,  GS : %R,  FS : %R\n\n"
    "Error code: %R\n\n"
    "EFLAGS: %R\n",
    context->vector, context->cs, context->eip, context->eax, context->ebx, context->ecx, context->edx,
    context->edi, context->esi, context->esp, context->ebp, context->ds, context->es, context->gs, context->fs, 
    context->error_code,
    context->eflags
  );
}
