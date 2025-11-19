#include "types.h"
#include "asmu.h"
#include "assert.h"
#include "outputu.h"
#include "kernelpanic.h"
#include "alloc.h"
#include "interrupter.h"

#define TRAMPOLINE_SIZE     8
#define VECTORS_N           256

#define MASTER_COMMAND_PORT 0x20
#define MASTER_DATA_PORT    0x21
#define SLAVE_COMMAND_PORT  0xA0
#define SLAVE_DATA_PORT     0xA1
#define DELAY_PORT          0x80

u32 global = 0;

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
  uchar* trampolines = (uchar*) malloc_immortal(TRAMPOLINE_SIZE * VECTORS_N, 8);

  for (u16 vector = 0; vector < VECTORS_N; vector++) {
    uchar* trampoline = trampolines + vector * TRAMPOLINE_SIZE;
    u32 offset = 0;

    if (!has_error_code(vector)) {
      trampoline[offset++] = 0x50; // push eax (if needed)
    }
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
    (interrupt_descriptor*) malloc_immortal(sizeof(interrupt_descriptor) * VECTORS_N, sizeof(interrupt_descriptor));

  for (u16 vector = 0; vector < VECTORS_N; vector++) {
    uchar* trampoline = trampolines + vector * TRAMPOLINE_SIZE;

    idt[vector].offset_low    = (u32)(trampoline) & 0xFFFF;
    idt[vector].seg_selector  = 0x8;
    idt[vector].reserved      = 0b0;
    idt[vector].zeros         = 0b000;
    idt[vector].type          = 0b110;

    // №111819 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    // idt[vector].type          = 0b111;

    idt[vector].d             = 0b1;
    idt[vector].zero          = 0b0;
    idt[vector].dpl           = 0b00;
    idt[vector].present       = 0b1;
    idt[vector].offset_high   = ((u32)(trampoline) >> 16) & 0xFFFF;
    assert(sizeof(idt[vector]) == 8);
  }

  return idt;
}

void init_interrupter() {
  uchar* trampolines = generate_trampolines();
  void* idt = generate_idt(trampolines);
  u16 idt_limit = VECTORS_N * sizeof(interrupt_descriptor) - 1;
  u64 idt_ret = ((u64) idt << 16) | idt_limit; 
  lidt(&idt_ret);
}

bool auto_eoi_defined = false;
bool auto_eoi;

void init_pic8259_master(bool aeoi) {
  auto_eoi_defined = true;
  auto_eoi = aeoi;

  port_write(MASTER_DATA_PORT, 0b11111111);

  port_write(MASTER_COMMAND_PORT, 0b00010001);
  port_write(MASTER_DATA_PORT, 0x20);
  port_write(MASTER_DATA_PORT, 0b00000100);
  port_write(MASTER_DATA_PORT, (uchar) auto_eoi << 1 | 1);
}

void init_pic8259_slave() {
  assert(auto_eoi_defined == true);

  port_write(SLAVE_DATA_PORT,  0b11111111);

  port_write(SLAVE_COMMAND_PORT,  0b00010001);
  port_write(SLAVE_DATA_PORT,  0x28);
  port_write(SLAVE_DATA_PORT,  0b00000010);
  port_write(SLAVE_DATA_PORT,  (uchar) auto_eoi << 1 | 1);
}

void pic8259_enable_device(Device device) {
    uchar mask = port_read(MASTER_DATA_PORT);
    port_write(MASTER_DATA_PORT, ~(1 << device) & mask);
}

void pic8259_send_EOI() {
  assert(!auto_eoi);
  port_write(MASTER_COMMAND_PORT, 0x20);
};

void delay(u16 n) {
  for (u32 i = 0; i < n; i++) {
    for (u32 j = 0; j < 100; j++) {
      port_write(DELAY_PORT, DELAY_PORT);
    }
  }
}

void global_plus() {
  delay(10);
  printf("%d ", global++);
}

void kernel_panic_ctx(interrupt_context* context) {
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

void timer_handler(struct interrupt_context* context) {
  // №2 -----------------------
  // kernel_panic_ctx(context);
  
  // №4678141617 ##############
  // printf("%d ", global++);
 
  // №5 -----------------------
  // global = 0;

  // №9 -----------------------
  /*
  printf("%d ", global++);
  pic8259_send_EOI();
  sti();
  inf_loop();
  */

  // №10 ----------------------
  /*
  printf("%d ", global++);
  if (global < 100) {
    pic8259_send_EOI();
    sti();
  }
  inf_loop();
  */

  // №11 ----------------------
  /*
  printf("%d ", global++);
  if (global < 100){
    pic8259_send_EOI();
  }
  inf_loop();
  */

  // №12 ----------------------
  /*
  printf("%d ", global++);
  if (global < 100){
    sti();
  }
  inf_loop();
  */

  // №1518 ------------------
  // printf("%d ", global++);
  // inf_loop();

  // №19 ----------------------
  // printf("%d ", global++);
  // pic8259_send_EOI();

  // №20 ----------------------
  port_write(MASTER_DATA_PORT, ~2); // disable timer
  printf("delay 1\n");
  delay(10000);
  sti();
  printf("delay 2\n");
  delay(10000);
  printf("end :)");

  return;
}

void keyboard_handler(struct interrupt_context* context) {
  // №3 -----------------------
  // kernel_panic_ctx(context);

  // №1314
  // printf("%c ", port_read(0x60));

  // №1620
  printf("%c ", port_read(0x60));
  inf_loop();

  // №17
  /*
  printf("%c ", port_read(0x60));
  sti();
  inf_loop();
  */

  // №18
  // printf("%c ", port_read(0x60));
  // pic8259_send_EOI();

  // №19
  // printf("%c ", port_read(0x60));
  // inf_loop();

  return;
}

void universal_handler(interrupt_context* context) {
  switch (context->vector) {
    case 0x20:
      timer_handler(context);
      break;
    case 0x21:
      keyboard_handler(context);
      break;
    default:
      kernel_panic_ctx(context);
      break;
  }
}
