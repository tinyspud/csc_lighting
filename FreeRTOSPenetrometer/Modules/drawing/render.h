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

#define RENDER_ENDING_3_ARGUMENTS	scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE

#define MAX_POINTS_IN_BEZIER	(BYTES_IN_1_LINE)

/* Create the ellipsis width (the ... character) */
#define RENDER_ELLIPSIS_WID ((__char_wid_46 + RENDER_KERN_PIXELS) * 3)


/* Do not touch bits 0-3, 12 active bits of line properties */
#define LINE_DRAWING_BIT_MASK		0xFFF0
#define LINE_DATA_BIT_MASK			0x000F
#define LINE_NUM_DRAW_BITS			12
#define LINE_NUM_DATA_BITS			4


//typedef enum LineDrawingStyle{
//	ToggleLineBit0 = (1 << 4),
//	ToggleLineBit1 = (1 << 5),
//	ToggleLineBit2 = (1 << 6),
//	ToggleLineBit3 = (1 << 7),
//	ToggleLineBit4 = (1 << 8),
//	ToggleLineBit5 = (1 << 9),
//	ToggleLineBit6 = (1 << 10),
//	ToggleLineBit7 = (1 << 11),
//	ToggleLineBit8 = (1 << 12),
//	ToggleLineBit9 = (1 << 13),
//	ToggleLineBitA = (1 << 14),
//	ToggleLineBitB = (1 << 15),
//}LineDrawingStyle_t;
//


typedef uint16_t LineDrawingStyle_t;
#define		TOGGLELINEBIT0  (1 << 4)
#define		TOGGLELINEBIT1  (1 << 5)
#define		TOGGLELINEBIT2  (1 << 6)
#define		TOGGLELINEBIT3  (1 << 7)
#define		TOGGLELINEBIT4  (1 << 8)
#define		TOGGLELINEBIT5  (1 << 9)
#define		TOGGLELINEBIT6  (1 << 10)
#define		TOGGLELINEBIT7  (1 << 11)
#define		TOGGLELINEBIT8  (1 << 12)
#define		TOGGLELINEBIT9  (1 << 13)
#define		TOGGLELINEBITA  (1 << 14)
#define		TOGGLELINEBITB  (1 << 15)

/* Dot line:	". . . . . . " */
#define LINE_DRAWING_STYLE_DOT		(TOGGLELINEBIT0 | TOGGLELINEBIT2 | TOGGLELINEBIT4 | TOGGLELINEBIT6 | TOGGLELINEBIT8 | TOGGLELINEBITA)
/* Dash line:	".  ..  ..  ." */
#define LINE_DRAWING_STYLE_DASH		(TOGGLELINEBIT0 | TOGGLELINEBIT3 | TOGGLELINEBIT4 | TOGGLELINEBIT7 | TOGGLELINEBIT8 | TOGGLELINEBITB)
/* Long Dash:	".. ..... ..." */
#define LINE_DRAWING_STYLE_LONG		(TOGGLELINEBIT0 | TOGGLELINEBIT1 | TOGGLELINEBIT3 | TOGGLELINEBIT4 | TOGGLELINEBIT5 | TOGGLELINEBIT6 | TOGGLELINEBIT7 | TOGGLELINEBIT9 | TOGGLELINEBITA | TOGGLELINEBITB)
/* Dot/Dash:	".. . ... . ." */
#define LINE_DRAWING_STYLE_DOT_DASH	(TOGGLELINEBIT0 | TOGGLELINEBIT1 | TOGGLELINEBIT3 | TOGGLELINEBIT5 | TOGGLELINEBIT6 | TOGGLELINEBIT7 | TOGGLELINEBIT9 | TOGGLELINEBITB)
/* Solid:		"............" */
#define LINE_DRAWING_STYLE_SOLID	(TOGGLELINEBIT0 | TOGGLELINEBIT1 | TOGGLELINEBIT2 | TOGGLELINEBIT3 | TOGGLELINEBIT4 | TOGGLELINEBIT5 | TOGGLELINEBIT6 | TOGGLELINEBIT7 | TOGGLELINEBIT8 | TOGGLELINEBIT9 | TOGGLELINEBITA | TOGGLELINEBITB)
#define RESET_LINE_DRAWING_STYLE(x)		(x &= LINE_DRAWING_BIT_MASK)



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

int get_line_length(char*, int);

void render_ellipsis(int, int, uint8, uint8 [][RENDER_MAX_WIDTH], int, int);

void render_disable_rectangle(int, int, int, int, uint8 [][RENDER_MAX_WIDTH], int, int);

void render_disable_rectangle_OR(int, int, int, int, uint8 [][RENDER_MAX_WIDTH], int, int);

void render_copy_line(uint8 [][RENDER_MAX_WIDTH], uint8 source[][RENDER_MAX_WIDTH], int, int, int, int);

/* Line rendering */
void render_line(int, int, int, int, uint8, LineDrawingStyle_t *, uint8 [][RENDER_MAX_WIDTH], int, int);
void render_line_solid(int, int, int, int, uint8, uint8 [][RENDER_MAX_WIDTH], int, int);

void render_bezier(int, int, int, int, int, int, int, int, uint8, LineDrawingStyle_t *, uint8 [][RENDER_MAX_WIDTH], int, int);
void render_bezier_solid(int, int, int, int, int, int, int, int, uint8, uint8 [][RENDER_MAX_WIDTH], int, int);

void render_bezier_with_control_lines(int, int, int, int, int, int, int, int, uint8, LineDrawingStyle_t *,uint8 [][RENDER_MAX_WIDTH], int, int);

#endif

