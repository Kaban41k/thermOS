extern void inf_loop();

void kernel_entry() {
  *((short int*) 0xB8000) = (2 << 8) | 0x4B;
  *((short int*) 0xB8002) = (5 << 8) | 0x42;
  *((short int*) 0xB8004) = (4 << 8) | 0x4E;
  inf_loop();
}
