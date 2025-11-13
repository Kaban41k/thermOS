#ifndef vgau
#define vgau

// VGA Utilities

#include "types.h"

typedef struct {
	void* ptr;
	u32 x_size;
	u32 y_size;
	u32 cursor_x;
	u32 cursor_y;
} window;

char* vga_get_char_ptr(u32 x, u32 y);
void vga_fill_screen(char c);
void vga_clear_screen();
void vga_print_char(char c, u32 x, u32 y);
void vga_scroll_down();
window create_window(u32 x_size, u32 y_size, u32 x_offset, u32 y_offset);
char* win_get_char_ptr(window win, u32 x, u32 y);
char* win_clear(window win);
void win_print_char(window win, char c, u32 x, u32 y);
void win_scroll_down(window win);
void win_select_color(window win, char color);

#endif
