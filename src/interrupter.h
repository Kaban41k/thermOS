#ifndef interrupter
#define interrupter

typedef enum {
  SYSTEM_TIMER, 
  KEYBOARD
} Device;

void init_interrupter();
void init_pic8259_master(bool auto_eoi);
void init_pic8259_slave();
void pic8259_enable_device(Device device);
void pic8259_send_EOI();
void delay(u16 n);
void global_plus();

#endif
