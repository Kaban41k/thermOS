#ifndef vgau
#define vgau

// VGA Utilities

#include <stddef.h>
#include "memu.h"

typedef struct {
	void* ptr;
	size_t x_size;
	size_t y_size;
} window;

void* vga_buf = (void*) 0xB8000;
const size_t vga_x_size = 80;
const size_t vga_y_size = 25;

// -----VGA-----
char* vga_get_char_ptr(size_t x, size_t y) {
	return (char*) vga_buf + 2 * (y * 80 + x);
}

void vga_fill_screen(char c) {
  for (size_t y = 0; y < vga_y_size; y++) {
	  for (size_t x = 0; x < vga_x_size; x++) {
			*vga_get_char_ptr(x, y) = c;
  	}
  }
}

void vga_clear_screen() {
	vga_fill_screen(0);
}

void vga_print_char(char c, size_t x, size_t y) {
	*vga_get_char_ptr(x, y) = c;
}

void vga_scroll_down() {
	memmove(vga_buf, vga_buf + vga_x_size * 2, 2 * vga_x_size * (vga_y_size - 1));
	for (size_t i = 0; i < vga_x_size; i++) {
		vga_print_char(0, i, vga_y_size - 1);
	}
}


// -----WIN-----
window create_window(size_t x_size, size_t y_size, size_t x_offset, size_t y_offset) {
	window win = {.ptr = (void*) vga_get_char_ptr(x_offset, y_offset), .x_size = x_size, .y_size = y_size};
	return win;
}

char* win_get_char_ptr(window win, size_t x, size_t y) {
	return (char*) win.ptr + 2 * (y * 80 + x);
}

char* win_clear(window win) {
	for (size_t y = 0; y < win.y_size; y++) {
	  for (size_t x = 0; x < win.x_size; x++) {
			*win_get_char_ptr(win, x, y) = 0;
  	}
  }
}

void win_print_char(window win, char c, size_t x, size_t y) {
	*win_get_char_ptr(win, x, y) = c;
}

void win_scroll_down(window win) {
	for (size_t i = 0; i < win.y_size - 1; i++) {
		memmove(win.ptr + 2 * vga_x_size * i, win.ptr + 2 * vga_x_size * (i + 1), win.x_size);
	}

	for (size_t i = 0; i < win.x_size; i++) {
		win_print_char(win, 0, i, win.y_size - 1);
	}
}

#endif