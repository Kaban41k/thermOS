#ifndef interrupter
#define interrupter

typedef enum {
  SYSTEM_TIMER, 
  KEYBOARD
} Device;

typedef enum {
  INTERRUPT_GATE = 0b110, 
  TRAP_GATE = 0b111
} InterruptType;

typedef enum {
  MANUAL_EOI, 
  AUTO_EOI
} EoiMode;

typedef struct interrupt_context {
  u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
  alignas(4) u16 gs, fs, es, ds;
  alignas(4) uchar vector;
  u32 error_code;
  u32 eip;
  alignas(4) u16 cs;
  u32 eflags;
} interrupt_context;

void init_interrupter(InterruptType type);
void init_pic8259_master(EoiMode auto_eoi);
void init_pic8259_slave();
void pic8259_enable_device(Device device);
void pic8259_disable_device(Device device);
void pic8259_send_EOI();
void delay(u16 n);
void global_plus();

#endif
