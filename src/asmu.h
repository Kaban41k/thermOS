#ifndef asmu
#define asmu

void inf_loop();
void cli();
void sti();
void lidt(void* idt_ptr);
void set_regs();
void div_zero();
void int_n();
void collect_context();

#endif
