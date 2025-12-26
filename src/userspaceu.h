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
  alignas(4) u16 previous_task_link;
  
  // ----Ring-0-stack----
  alignas(4) u32 esp_0;
  alignas(4) u16 ss_0;
 
  // --Ring 1-and-2-stacks-- (not used)
  alignas(4) u64 esp_2_ss_2, esp_1_ss_1; 
  alignas(4) u32 cr3;

  // ----context----
  u32 eflags, eip;
  u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
  alignas(4) u16 gs, fs, ds, ss, cs, es;
  
  alignas(4) u16 ldtss;
  u16 _reserved;
  u16 io_map_base_address;
  u32 ssp;
} TSS;

static_assert(sizeof(TSS) >= 0x67);

void setup_tss();

#endif
