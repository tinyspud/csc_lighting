/* Rendering code to place pixels onto an array of dimensions:
 * array[height][row/8]
 *
 */


#ifndef __RENDER_H
#define __RENDER_H

/* Draw types */
/* Set the image add mode */
#define DRAW_SET_B	0x01
#define DRAW_SET_F	0x02
#define DRAW_SET_TOGGLE	0x03
#define DRAW_MODE_MASK	0x03

/* Text alignment */
#define DRAW_ALIGN_LEFT		0x04
#define DRAW_ALIGN_CENTER	0x0C
#define DRAW_ALIGN_RIGHT	0x08
#define DRAW_ALIGN_MASK		0x0C

/* Word wrapping */
#define DRAW_WORD_WRAP		0x10
#define DRAW_WORD_WRAP_MASK	0x10

/* Draw the word in gray */
#define DRAW_LETTER_DISABLED		0x20
#define DRAW_LETTER_DISABLE_MASK	0x20

#define RENDER_OFF_AREA_VIOLATION 3
#define RENDER_PIXEL_SET 2
#define RENDER_FOREGROUND 1
#define RENDER_BACKGROUND 0

/* Typography values (very crude - will have to do until a real font is
 * implemented in this software) */
#define RENDER_KERN_PIXELS (2)
#define RENDER_SPACE_PIXELS (10)
#define RENDER_TAB_PIXELS (4 * RENDER_SPACE_PIXELS)
#define RENDER_LINE_HEIGHT (__HELV_HEIGHT + RENDER_KERN_PIXELS)

#define RENDER_MAX_WIDTH	BYTES_IN_1_LINE

/* Create the ellipsis width (the ... character) */
#define RENDER_ELLIPSIS_WID ((__char_wid_46 + RENDER_KERN_PIXELS) * 3)

uint8 render_set_pixel(int, int, uint8, uint8 [][RENDER_MAX_WIDTH], int, int);

uint8 render_set_byte(int, int, uint8, uint8 [][RENDER_MAX_WIDTH], int, int);

uint8 render_get_pixel(int, int, uint8 [][RENDER_MAX_WIDTH], int, int);

uint8 render_set_pixel_foreground(int, int, uint8 [][RENDER_MAX_WIDTH], int, int);

uint8 render_set_foreground_byte(int, int, uint8, uint8 [][RENDER_MAX_WIDTH], int, int);

uint8 render_set_pixel_background(int, int, uint8 [][RENDER_MAX_WIDTH], int, int);

uint8 render_set_background_byte(int, int, uint8, uint8 [][RENDER_MAX_WIDTH], int, int);

uint8 render_XOR_byte(int, int, uint8, uint8 [][RENDER_MAX_WIDTH], int, int);

uint8 render_put_byte(int, int, uint8, uint8, uint8 [][RENDER_MAX_WIDTH], int, int);

void render_char(char, int, int, uint8, uint8 [][RENDER_MAX_WIDTH], int, int);

/* Text drawing */
void render_char_w_ptr(uint8*, int, int, uint8, uint8 [][RENDER_MAX_WIDTH], int, int);

void render_char_w_ptr_adv(uint8*, int, int, uint8, int, int, int, int, uint8 [][RENDER_MAX_WIDTH], int, int);

int render_smart_string_within_bounds(char*, int, int, int, uint8, int, int, uint8 [][RENDER_MAX_WIDTH], int, int);

int render_smart_string(char*, int, int, int, uint8, uint8 [][RENDER_MAX_WIDTH], int, int);

/* Shape drawing */
void render_rectangle(int, int, int, int, uint8, uint8 [][RENDER_MAX_WIDTH], int, int);

void render_line(int, int, int, int, uint8, uint8 [][RENDER_MAX_WIDTH], int, int);

int get_line_length(char*, int);

void render_ellipsis(int, int, uint8, uint8 [][RENDER_MAX_WIDTH], int, int);

void render_disable_rectangle(int, int, int, int, uint8 [][RENDER_MAX_WIDTH], int, int);

void render_disable_rectangle_OR(int, int, int, int, uint8 [][RENDER_MAX_WIDTH], int, int);

void render_copy_line(uint8 [][RENDER_MAX_WIDTH], uint8 source[][RENDER_MAX_WIDTH], int, int, int, int);

#endif

