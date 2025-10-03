extern void inf_loop();

void usless_print(char* str, int n) {
  char* display_buf = (char*) 0xB8000;
  
  for (int i = 0; i < n; i++) {
    *display_buf = str[i];
    display_buf += 2;
  }
}

void kernel_entry() {
  char output[] = "Hello :)                               ";

  usless_print(output, 39);

  inf_loop();
}
