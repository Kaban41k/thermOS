#ifndef interrupter
#define interrupter

struct interrupt_context;

void setup_interrupter();
void universal_handler(struct interrupt_context* context);

#endif