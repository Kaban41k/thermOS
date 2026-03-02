#ifndef picconf
#define picconf

#include "types.h"

#define MASTER_COMMAND_PORT  0x20
#define MASTER_DATA_PORT     0x21

#define SLAVE_COMMAND_PORT   0xA0
#define SLAVE_DATA_PORT      0xA1

#define DELAY_PORT           0x80

typedef enum {
  MASTER, 
  SLAVE
} Controller;

//                                              mask_of_cascade
//                                              \/\/\/\/\/
const uchar MASTER_WORDS[] = {0b00010001, 0x20, 0b00000100};
const uchar SLAVE_WORDS[]  = {0b00010001, 0x28, 0b00000010};
//                                 ^^ ^^          ^^^^^^^^=num_of_IR_contact
//                                 || ||
//                                 || ||=secondary_flags
//                                 || |==cascade
//                                 ||====mode
//                                 |=====start_setup

#endif
