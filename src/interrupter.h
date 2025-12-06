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

void init_interrupter(InterruptType type);
void init_pic8259_master(bool auto_eoi);
void init_pic8259_slave();
void pic8259_enable_device(Device device);
void pic8259_disable_device(Device device);
void pic8259_send_EOI();
void delay(u16 n);
void global_plus();

#endif
