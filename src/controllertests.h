#ifndef controllertests
#define controllertests


#define EXPERIMENT_1                   \
  init_interrupter(INTERRUPT_GATE);    \
  init_pic8259_master(false);          \
  init_pic8259_slave();                \
  sti();
#define TIMER_1
#define KEYBOARD_1


#define EXPERIMENT_2                   \
  init_interrupter(INTERRUPT_GATE);    \
  init_pic8259_master(false);          \
  init_pic8259_slave();                \
                                       \
  pic8259_enable_device(SYSTEM_TIMER); \
                                       \
  sti();
#define TIMER_2 \
  kernel_panic_ctx(context);
#define KEYBOARD_2


#define EXPERIMENT_3                \
  init_interrupter(INTERRUPT_GATE); \
  init_pic8259_master(false);       \
  init_pic8259_slave();             \
                                    \
  pic8259_enable_device(KEYBOARD);  \
                                    \
  sti();
#define TIMER_3 \
  pic8259_send_EOI();
#define KEYBOARD_3 \
  kernel_panic_ctx(context);

#define EXPERIMENT_4                   \
  init_interrupter(INTERRUPT_GATE);    \
  init_pic8259_master(true);           \
  init_pic8259_slave();                \
                                       \
  pic8259_enable_device(SYSTEM_TIMER); \
                                       \
  sti();
#define TIMER_4 \
  printf("%d ", global++);
#define KEYBOARD_4


#define EXPERIMENT_5                   \
  init_interrupter(INTERRUPT_GATE);    \
  init_pic8259_master(true);           \
  init_pic8259_slave();                \
                                       \
  pic8259_enable_device(SYSTEM_TIMER); \
                                       \
  sti();                               \
  for (u64 i = 0; i < 1000000; i++) {  \
    global_plus();                     \
  }
#define TIMER_5 \
  global = 0;
#define KEYBOARD_5


#define EXPERIMENT_6                   \
  init_interrupter(INTERRUPT_GATE);    \
  init_pic8259_master(true);           \
  init_pic8259_slave();                \
                                       \
  pic8259_enable_device(SYSTEM_TIMER); \
                                       \
  sti();                               \
  for (u64 i = 0; i < 1000000; i++) {  \
    global_plus();                     \
  }
#define TIMER_6 \
  printf("%d ", global++);
#define KEYBOARD_6


#define EXPERIMENT_7                   \
  init_interrupter(INTERRUPT_GATE);    \
  init_pic8259_master(false);          \
  init_pic8259_slave();                \
                                       \
  pic8259_enable_device(SYSTEM_TIMER); \
                                       \
  sti();
#define TIMER_7 \
  printf("%d ", global++);
#define KEYBOARD_7


#define EXPERIMENT_8                   \
  init_interrupter(INTERRUPT_GATE);    \
  init_pic8259_master(false);          \
  init_pic8259_slave();                \
                                       \
  pic8259_enable_device(SYSTEM_TIMER); \
                                       \
  sti();
#define TIMER_8            \
  printf("%d ", global++); \
  pic8259_send_EOI();
#define KEYBOARD_8


#define EXPERIMENT_9                   \
  init_interrupter(INTERRUPT_GATE);    \
  init_pic8259_master(false);          \
  init_pic8259_slave();                \
                                       \
  pic8259_enable_device(SYSTEM_TIMER); \
                                       \
  sti();
#define TIMER_9            \
  printf("%d ", global++); \
  pic8259_send_EOI();      \
  sti();                   \
  inf_loop(); 
#define KEYBOARD_9


  #define EXPERIMENT_10                  \
    init_interrupter(INTERRUPT_GATE);    \
    init_pic8259_master(false);          \
    init_pic8259_slave();                \
                                         \
    pic8259_enable_device(SYSTEM_TIMER); \
                                         \
  sti();
#define TIMER_10           \
  printf("%d ", global++); \
  if (global < 100) {      \
    pic8259_send_EOI();    \
    sti();                 \
  }                        \
  inf_loop();
#define KEYBOARD_10


#define EXPERIMENT_11                  \
  init_interrupter(TRAP_GATE);         \
  init_pic8259_master(false);          \
  init_pic8259_slave();                \
                                       \
  pic8259_enable_device(SYSTEM_TIMER); \
                                       \
  sti();
#define TIMER_11           \
  printf("%d ", global++); \
  if (global < 100) {      \
    pic8259_send_EOI();    \
  }                        \
  inf_loop();
#define KEYBOARD_11


#define EXPERIMENT_12                  \
  init_interrupter(INTERRUPT_GATE);    \
  init_pic8259_master(true);           \
  init_pic8259_slave();                \
                                       \
  pic8259_enable_device(SYSTEM_TIMER); \
                                       \
  sti();
#define TIMER_12           \
  printf("%d ", global++); \
  if (global < 100) {      \
    sti();                 \
  }                        \
  inf_loop();
#define KEYBOARD_12


#define EXPERIMENT_13               \
  init_interrupter(INTERRUPT_GATE); \
  init_pic8259_master(true);        \
  init_pic8259_slave();             \
                                    \
  pic8259_enable_device(KEYBOARD);  \
                                    \
  sti();
#define TIMER_13
#define KEYBOARD_13 \
  printf("%c ", port_read(0x60));


#define EXPERIMENT_14                  \
  init_interrupter(INTERRUPT_GATE);    \
  init_pic8259_master(true);           \
  init_pic8259_slave();                \
                                       \
  pic8259_enable_device(KEYBOARD);     \
  pic8259_enable_device(SYSTEM_TIMER); \
                                       \
  sti();
#define TIMER_14 \
  printf("%d ", global++);
#define KEYBOARD_14 \
  printf("%c ", port_read(0x60));


#define EXPERIMENT_15                  \
  init_interrupter(INTERRUPT_GATE);    \
  init_pic8259_master(true);           \
  init_pic8259_slave();                \
                                       \
  pic8259_enable_device(KEYBOARD);     \
  pic8259_enable_device(SYSTEM_TIMER); \
                                       \
  sti();
#define TIMER_15         \
  printf("%d ", global); \
  inf_loop();
#define KEYBOARD_15 \
  printf("%c ", port_read(0x60));


#define EXPERIMENT_16                  \
  init_interrupter(INTERRUPT_GATE);    \
  init_pic8259_master(true);           \
  init_pic8259_slave();                \
                                       \
  pic8259_enable_device(KEYBOARD);     \
  pic8259_enable_device(SYSTEM_TIMER); \
                                       \
  sti();
#define TIMER_16          \
  printf("%d ", global++);
#define KEYBOARD_16               \
  printf("%c ", port_read(0x60)); \
  inf_loop();


#define EXPERIMENT_17                  \
  init_interrupter(INTERRUPT_GATE);    \
  init_pic8259_master(true);           \
  init_pic8259_slave();                \
                                       \
  pic8259_enable_device(KEYBOARD);     \
  pic8259_enable_device(SYSTEM_TIMER); \
                                       \
  sti();
#define TIMER_17          \
  printf("%d ", global++);
#define KEYBOARD_17               \
  printf("%c ", port_read(0x60)); \
  sti();                          \
  inf_loop();


#define EXPERIMENT_18                  \
  init_interrupter(TRAP_GATE);         \
  init_pic8259_master(false);          \
  init_pic8259_slave();                \
                                       \
  pic8259_enable_device(KEYBOARD);     \
  pic8259_enable_device(SYSTEM_TIMER); \
                                       \
  sti();
#define TIMER_18         \
  printf("%d ", global); \
  inf_loop();
#define KEYBOARD_18               \
  printf("%c ", port_read(0x60)); \
  pic8259_send_EOI();


#define EXPERIMENT_19                  \
  init_interrupter(TRAP_GATE);         \
  init_pic8259_master(false);          \
  init_pic8259_slave();                \
                                       \
  pic8259_enable_device(KEYBOARD);     \
  pic8259_enable_device(SYSTEM_TIMER); \
                                       \
  sti();
#define TIMER_19           \
  printf("%d ", global++); \
  pic8259_send_EOI();
#define KEYBOARD_19               \
  printf("%c ", port_read(0x60)); \
  inf_loop();


#define EXPERIMENT_20                  \
  init_interrupter(INTERRUPT_GATE);    \
  init_pic8259_master(true);           \
  init_pic8259_slave();                \
                                       \
  pic8259_enable_device(KEYBOARD);     \
  pic8259_enable_device(SYSTEM_TIMER); \
                                       \
  sti();
#define TIMER_20                    \
  port_write(MASTER_DATA_PORT, ~2); \
  printf("delay 1\n");              \
  delay(10000);                     \
  sti();                            \
  printf("delay 2\n");              \
  delay(10000);                     \
  printf("end :)");
#define KEYBOARD_20               \
  printf("%c ", port_read(0x60)); \
  inf_loop();

#endif