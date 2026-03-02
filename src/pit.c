#include "pit.h"
#include "asmu.h"
#include "interrupter.h"

#define HZ                     100
#define MAGIC_DREQUENCY_NUMBER 1193182

static_assert(18 <= MAGIC_DREQUENCY_NUMBER / HZ &&
                    MAGIC_DREQUENCY_NUMBER / HZ <= 1193183);

void init_pit8254() {
    port_write(PIT_MCR_PORT, 0b00110100);
    //                         /\/\/ \^=binary_coded_decimal
    //                         ||||{{{==mode
    //                         |||{=====least_byte
    //                         ||{======most_byte
    //                         {{=======counter_i(if 11 then Read-Back Command)

    u32 frequency = MAGIC_DREQUENCY_NUMBER / HZ;

    port_write(PIT_CHANNEL_0_PORT, frequency & 0xFF);
    port_write(PIT_CHANNEL_0_PORT, (frequency >> 8) & 0xFF);
}