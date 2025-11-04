#ifndef vgau
#define vgau

// VGA Utilities

#include "types.h"

/*
  Structure of the window on the screen.

	void* ptr    : pointer to the upper left corner 
                  of the window in VGA memory
	u32 x_size   : window width in characters
  u32 y_size   : window height in characters
  
  --- Used only if there was a cursor in the window ---

  u32 cursor_x : last cursor x position, 
                  when a different window selected
	u32 cursor_y : last cursor y position, 
                  when a different window selected
  
  -----------------------------------------------------
  */
typedef struct {
	void* ptr;
	u32 x_size;
	u32 y_size;
	u32 cursor_x;
	u32 cursor_y;
} Window;

/*
  Get a pointer to a character in VGA memory 
    with position (x, y).
  
  u32 x : character x position on screen
  u32 y : character y position on screen

  return char* : pointer to a character
*/
char* vga_get_char_ptr(u32 x, u32 y);

/*
  Fill the entire screen with the c character.

  char c : character to fill the screen
*/
void vga_fill_screen(char c);

/*
  Removes all characters from the screen.
*/
void vga_clear_screen();

/*
  Put the character in position on the screen.

  char c : character to put
  u32 x  : character x position on screen
  u32 y  : character y position on screen
*/
void vga_print_char(char c, u32 x, u32 y);

/*
  Scroll down the entire screen by one line.
*/
void vga_scroll_down();

/*
  Create window :)

  u32 x_size     : window width in characters
  u32 y_size     : window height in characters
  u32 x_offset   : x position of window upper left corner
  u32 y_offset   : y position of window upper left corner
*/
Window create_window(u32 x_size, u32 y_size, u32 x_offset, u32 y_offset);

/*
  Get a pointer to a character in VGA memory 
    with position (x, y) relative to the win window.
  
  Window win : window
  u32 x      : character x position on window
  u32 y      : character y position on window

  return char* : pointer to a character
*/
char* win_get_char_ptr(Window win, u32 x, u32 y);

/*
  Removes all characters from the window.
*/
char* win_clear(Window win);

/*
  Put the character in position on the window.

  Window win : window
  char c     : character to put
  u32 x      : character x position on window
  u32 y      : character y position on window
*/
void win_print_char(Window win, char c, u32 x, u32 y);

/*
  Scroll down the window by one line.

  Window win : window
*/
void win_scroll_down(Window win);

/*
  Set a color to all chars and background in the window.

  Window win : window
  char color : 4 bits color of char
               4 bits color of background
*/
void win_select_color(Window win, char color);

#endif