#ifndef userspacetests
#define userspacetests

#define USER_PROCESS_1 \
  inf_loop();
#define TIMER_1

#define USER_PROCESS_2 \
  printf("I'M USER");  \
  inf_loop();
#define TIMER_2

#define USER_PROCESS_3       \
  for (;;)                   \
    printf("%d", global_plus());
#define TIMER_3

#define USER_PROCESS_4    \
  printf("%R ", get_esp); \
  inf_loop();
#define TIMER_4

#define USER_PROCESS_5  \
  cli();                \
  inf_loop();
#define TIMER_5

#define USER_PROCESS_6  \
  USER_PROCESS_3
#define TIMER_6             \
  printf("%R ", get_esp()); \
  inf_loop();

#define USER_PROCESS_7  \
  USER_PROCESS_3
#define TIMER_7 \
  global = 0;

#define USER_PROCESS_8  \
  USER_PROCESS_3
#define TIMER_8 \
  TIMER_7

#define USER_PROCESS_9  \
  kernel_code_segment_descriptor.p = 0;
#define TIMER_9

#define USER_PROCESS_10      \
  for (;;)                   \
    userspace_syscall(global_plus());
#define TIMER_10 \
  TIMER_7

#define USER_PROCESS_HACK                              \
  memmove((void*) vga_clear_screen, (void*) hack, 20); \
  inf_loop();
#define TIMER_HACK \
  vga_clear_screen();

#endif
