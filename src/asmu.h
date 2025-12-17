#ifndef asmu
#define asmu

#include "types.h"

void inf_loop();
void cli();
void sti();
void lidt(void* idt_ptr);
void ltr();
void set_regs();
void div_zero();
void int_n();
void collect_context();

uchar port_read(u16 port);
void port_write(u16 port, uchar data);

void userspace_process(void (*entry)(), void* stack);

#endif
