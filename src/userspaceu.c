#include "types.h"
#include "asmu.h"
#include "alloc.h"
#include "interrupter.h"
#include "userspaceu.h"

#include "outputu.h"

#define MIN_TSS_LIMIT 0x67

extern segment_descriptor tss_descriptor;

u16 max(u16 a, u16 b) {
  return a > b ? a : b;
}

void setup_tss() {
  u16 tss_limit = max(sizeof(TSS) - 1, MIN_TSS_LIMIT);
  TSS* tss_base = (TSS*) calloc_immortal(tss_limit + 1, 16);

  tss_base->esp_0 = 0x7C00;
  tss_base->ss_0 = KERNEL_DATA_SEGMENT;

  tss_descriptor.limit_low  = tss_limit;
  tss_descriptor.base_low   = (u32) tss_base & 0xFFFF;
  tss_descriptor.base_mid   = ((u32) tss_base >> 16) & 0xFF;
  tss_descriptor.type       = 0b1001;
  tss_descriptor.s          = 0b0;
  tss_descriptor.dpl        = 0b00;
  tss_descriptor.p          = 0b1;
  tss_descriptor.limit_high = 0x0;
  tss_descriptor.avl        = 0b0;
  tss_descriptor.zero       = 0b0;
  tss_descriptor.d_b        = 0b0;
  tss_descriptor.g          = 0b0;
  tss_descriptor.base_high  = ((u32) tss_base >> 24) & 0xFF;

  ltr();
}
