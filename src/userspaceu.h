#ifndef userspaceu
#define userspaceu

// Userspace Utilities

#define KERNEL_DATA_SEGMENT 0X10

typedef struct {
  u16   limit_low  : 16;
  u16   base_low   : 16;
  uchar base_mid   : 8;
  uchar type       : 4;
  uchar s          : 1;
  uchar dpl        : 2;
  uchar p          : 1;
  uchar limit_high : 4;
  uchar avl        : 1;
  uchar zero       : 1;
  uchar d_b        : 1;
  uchar g          : 1;
  uchar base_high  : 8;
} segment_descriptor;

typedef struct {
  u16 previous_task_link;
  u16 r_0;
  u32 esp_0;
  u32 ss_0;
  u16 r_1;
  u64 esp_2_ss_2, esp_1_ss_1; 
  u32 eflags, eip, cr3;
  u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
  alignas(4) u16 ldtss, gs, fs, ds, ss, cs, es;
  u16 r_2;
  u16 io_map_base_address;
  u32 ssp;
} TSS;

void setup_tss();

#endif
