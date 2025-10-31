#include "types.h"
#include "memu.h"

#define VGA_X_SIZE 80
#define VGA_Y_SIZE 25

typedef struct {
	void* ptr;
	u32 x_size;
	u32 y_size;
	u32 cursor_x;
	u32 cursor_y;
} window;

void* VGA_BUF = (void*) 0xB8000;

// -----VGA-----
char* vga_get_char_ptr(u32 x, u32 y) {
	return (char*) VGA_BUF + 2 * (y * 80 + x);
}

void vga_fill_screen(char c) {
  for (u32 y = 0; y < VGA_Y_SIZE; y++) {
	  for (u32 x = 0; x < VGA_X_SIZE; x++) {
			*vga_get_char_ptr(x, y) = c;
  	}
  }
}

void vga_clear_screen() {
	vga_fill_screen(0);
}

void vga_print_char(char c, u32 x, u32 y) {
	*vga_get_char_ptr(x, y) = c;
}

void vga_scroll_down() {
	memmove(VGA_BUF, VGA_BUF + VGA_X_SIZE * 2, 2 * VGA_X_SIZE * (VGA_Y_SIZE - 1));
	for (u32 i = 0; i < VGA_X_SIZE; i++) {
		vga_print_char(0, i, VGA_Y_SIZE - 1);
	}
}


// -----WIN-----
window create_window(u32 x_size, u32 y_size, u32 x_offset, u32 y_offset) {
	window win = {.ptr = (void*) vga_get_char_ptr(x_offset, y_offset), 
				        .x_size = x_size, .y_size = y_size,
				        .cursor_x = 1, .cursor_y = 1};
	return win;
}

char* win_get_char_ptr(window win, u32 x, u32 y) {
	return (char*) win.ptr + 2 * (y * 80 + x);
}

char* win_clear(window win) {
	for (u32 y = 0; y < win.y_size; y++) {
	  for (u32 x = 0; x < win.x_size; x++) {
			*win_get_char_ptr(win, x, y) = 0;
  	}
  }
}

void win_print_char(window win, char c, u32 x, u32 y) {
	*win_get_char_ptr(win, x, y) = c;
}

void win_scroll_down(window win) {
	for (u32 i = 0; i < win.y_size - 1; i++) {
		memmove(win.ptr + 2 * VGA_X_SIZE * i, win.ptr + 2 * VGA_X_SIZE * (i + 1), 2 * win.x_size);
	}

	for (u32 i = 0; i < win.x_size; i++) {
		win_print_char(win, 0, i, win.y_size - 1);
	}
}

void win_select_color(window win, char color) {
  for (u32 y = 0; y < win.y_size; y++) {
    for (u32 x = 0; x < win.x_size; x++) {
      *(win_get_char_ptr(win, x, y) + 1) = color;
    }
  }
}
