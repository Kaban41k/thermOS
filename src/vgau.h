#ifndef vgau
#define vgau

// VGA Utilities

#include <stddef.h>


typedef struct {
	void* ptr;
	size_t x_size;
	size_t y_size;
	size_t cursor_x;
	size_t cursor_y;
} window;

char* vga_get_char_ptr(size_t x, size_t y);
void vga_fill_screen(char c);
void vga_clear_screen();
void vga_print_char(char c, size_t x, size_t y);
void vga_scroll_down();
window create_window(size_t x_size, size_t y_size, size_t x_offset, size_t y_offset);
char* win_get_char_ptr(window win, size_t x, size_t y);
char* win_clear(window win);
void win_print_char(window win, char c, size_t x, size_t y);
void win_scroll_down(window win);
void win_select_color(window win, char color);

#endif