#include "eink.h"
#include "vis.h"
#include "render.h"
#include <math.h>

/*
 * Set a pixel's value at (x, y) with origin being top left of screen
 */
uint8 render_set_pixel(int x, int y, uint8 value, uint8 target[][RENDER_MAX_WIDTH], int bottommost, int rightmost){
	/* Do value checking to make sure you're getting the right value */
	uint8 rtnval = RENDER_OFF_AREA_VIOLATION;
	if(((y < bottommost) && (y >= 0))
			&& ((x < (8 * rightmost)) && (x >= 0)))
	{
		target[y][x/8] &= ~((0x01 & value) << (7 - (x % 8)));
		rtnval = RENDER_PIXEL_SET;
	}
	return rtnval;
}

/* OR pixel to FOREGROUND (black in black on white, white in white on black mode) */
uint8 render_set_pixel_foreground(int x, int y, uint8 target[][RENDER_MAX_WIDTH], int bottommost, int rightmost){
	/* Do value checking to make sure you're getting the right value */
	uint8 rtnval = RENDER_OFF_AREA_VIOLATION;
	if(((y < bottommost) && (y >= 0))
			&& ((x < (8 * rightmost)) && (x >= 0)))
	{
		target[y][x/8] |= (0x01 << (7 - (x % 8)));
		rtnval = RENDER_PIXEL_SET;
	}
	return rtnval;
}

/* OR byte onto the display buffer
 * Note that x here is the number of bytes, NOT the pixel
 * */
uint8 render_set_foreground_byte(int x, int y, uint8 value, uint8 target[][RENDER_MAX_WIDTH], int bottommost, int rightmost){
	/* Do value checking to make sure you're getting the right value */
	uint8 rtnval = RENDER_OFF_AREA_VIOLATION;
	if(((y < bottommost) && (y >= 0))
			&& ((x < rightmost) && (x >= 0)))
	{
		target[y][x] |= value;
		rtnval = RENDER_PIXEL_SET;
	}
	return rtnval;
}

/* Set the byte on (no boolean logic, just set the byte to value) */
uint8 render_set_byte(int x, int y, uint8 value, uint8 target[][RENDER_MAX_WIDTH], int bottommost, int rightmost){
	/* Do value checking to make sure you're getting the right value */
	uint8 rtnval = RENDER_OFF_AREA_VIOLATION;
	if(((y < bottommost) && (y >= 0))
			&& ((x < rightmost) && (x >= 0)))
	{
		target[y][x] = value;
		rtnval = RENDER_PIXEL_SET;
	}
	return rtnval;
}

/* XOR byte onto the display buffer (toggle everyone in byte)
 * Note that x here is the number of bytes, NOT the pixel
 * */
uint8 render_XOR_byte(int x, int y, uint8 value, uint8 target[][RENDER_MAX_WIDTH], int bottommost, int rightmost){
	/* Do value checking to make sure you're getting the right value */
	uint8 rtnval = RENDER_OFF_AREA_VIOLATION;
	if(((y < bottommost) && (y >= 0))
			&& ((x < rightmost) && (x >= 0)))
	{
		target[y][x] ^= value;
		rtnval = RENDER_PIXEL_SET;
	}
	return rtnval;
}

uint8 render_put_byte(int x, int y, uint8 value, uint8 draw_type, uint8 target[][RENDER_MAX_WIDTH], int bottommost, int rightmost){
	uint8 rtnval = RENDER_OFF_AREA_VIOLATION;
	if(((y < bottommost) && (y >= 0))
			&& ((x < rightmost) && (x >= 0)))
	{
		switch(draw_type & DRAW_MODE_MASK){
		case DRAW_SET_B:
			/* Since we want the background to have an imprint of this, not it in */
			render_set_background_byte(x, y, ~value, target, bottommost, rightmost);
			break;
		case DRAW_SET_F:
			/* Place the character onto the screen queued */
			render_set_foreground_byte(x, y, value, target, bottommost, rightmost);
			break;
		default:
		case DRAW_SET_TOGGLE:
			render_XOR_byte(x, y, value, target, bottommost, rightmost);
			break;
		}
		rtnval = RENDER_PIXEL_SET;
	}
	return rtnval;
}

/* Set the bit to background value (0) */
uint8 render_set_pixel_background(int x, int y, uint8 target[][RENDER_MAX_WIDTH], int bottommost, int rightmost){
	/* Do value checking to make sure you're getting the right value */
	uint8 rtnval = RENDER_OFF_AREA_VIOLATION;
	if(((y < bottommost) && (y >= 0))
			&& ((x < (8 * rightmost)) && (x >= 0)))
	{
		target[y][x/8] &= ~(0x01 << (7 - (x % 8)));
		rtnval = RENDER_PIXEL_SET;
	}
	return rtnval;
}

/* Set the bit to background value (0) */
uint8 render_set_background_byte(int x, int y, uint8 value, uint8 target[][RENDER_MAX_WIDTH], int bottommost, int rightmost){
	/* Do value checking to make sure you're getting the right value */
	uint8 rtnval = RENDER_OFF_AREA_VIOLATION;
	if(((y < bottommost) && (y >= 0))
			&& ((x < rightmost) && (x >= 0)))
	{
		target[y][x] &= value;
		rtnval = RENDER_PIXEL_SET;
	}
	return rtnval;
}

/* Retrieve the pixel value at coordinates (x, y) in pixels from screen buffer target
 * */
uint8 render_get_pixel(int x, int y, uint8 target[][RENDER_MAX_WIDTH], int bottommost, int rightmost){
	uint8 rtnval = RENDER_OFF_AREA_VIOLATION;
	if(((y < bottommost) && (y >= 0))
			&& ((x < (8 * rightmost)) && (x >= 0)))
	{
		rtnval = (target[y][x/8] & (1 << (7 - (x % 8)))) >> (7 - (x % 8));
	}
	return rtnval;
}

/*
 * XOR Set a pixel's value at (x, y) with origin being top left of screen
 */
uint8 render_flip_pixel(int x, int y, uint8 target[][RENDER_MAX_WIDTH], int bottommost, int rightmost){
	/* Do value checking to make sure you're getting the right value */
	uint8 rtnval = RENDER_OFF_AREA_VIOLATION;
	if(((y < bottommost) && (y >= 0))
			&& ((x < (8 * rightmost)) && (x >= 0)))
	{
		target[y][x/8] ^= ((0x01) << (7 - (x % 8)));
		rtnval = RENDER_PIXEL_SET;
	}
	return rtnval;
}


/* Draw a rectangle from (x_pixel,y_pixel) of width (x_p_width) and height (y_p_height(
 * All units in pixels
 * Draw type defined in header as either SET_F, SET_B or TOGGLE */
void render_rectangle(int x_pixel, int y_pixel, int x_p_width, int y_p_height,
		uint8 draw_type, uint8 target[][RENDER_MAX_WIDTH], int bottommost, int rightmost){
	/* Draw the box in at x_pixel for x_p_width */
	int i = 0;
	int j = 0;

	int rtpixel = ((x_pixel + x_p_width)/8 + 1);

	/* Create the left and right masks */
	uint8 leftmask = (0xFF >> (x_pixel % 8)) & 0xFF;
	uint8 rightmask = (0xFF << (8 - ((x_pixel + x_p_width) % 8))) & 0xFF;
	for(j = y_pixel; (j < (y_pixel + y_p_height)) && (j < bottommost); j++){
		for(i = (x_pixel/8); (i < rtpixel) && (i < rightmost); i++){
			switch((draw_type & DRAW_MODE_MASK)){
			case DRAW_SET_B:
				render_set_background_byte(i, j, (i == (x_pixel/8) ? leftmask : 0xFF) & 0xFF & (i == (rtpixel - 1) ? rightmask : 0xFF), target, bottommost, rightmost);
				break;
			case DRAW_SET_F:
				render_set_foreground_byte(i, j, (i == (x_pixel/8) ? leftmask : 0xFF) & 0xFF & (i == (rtpixel - 1) ? rightmask : 0xFF), target, bottommost, rightmost);
				break;
			case DRAW_SET_TOGGLE:
				render_XOR_byte(i, j, (i == (x_pixel/8) ? leftmask : 0xFF) & 0xFF & (i == (rtpixel - 1) ? rightmask : 0xFF), target, bottommost, rightmost);
				break;
			default:
				break;
			}
		}
	}
}

/* Impose grayed out area over a rectangle (ANDed on)
 * To implement this, draw the item then if it's on a black background, toggle
 * the black background over it AFTER you set this disable rectangle
 * */
void render_disable_rectangle(int x_pixel, int y_pixel, int x_p_width, int y_p_height, uint8 target[][RENDER_MAX_WIDTH], int bottommost, int rightmost){
	/* Draw the box in at x_pixel for x_p_width */
	int i = 0;
	int j = 0;

	int rtpixel = ((x_pixel + x_p_width)/8 + 1);

	/* Create the left and right masks */
	uint8 even_mask = 0x55;

	uint8 leftmask = 0;
	uint8 rightmask = 0;

	for(j = y_pixel; (j < (y_pixel + y_p_height)) && (j < bottommost); j++){
		even_mask = ~even_mask;

		leftmask = (0xFF >> (x_pixel % 8)) & even_mask;
		rightmask = (0xFF << (8 - ((x_pixel + x_p_width) % 8))) & even_mask;
		for(i = (x_pixel/8); (i < rtpixel) && (i < rightmost); i++){
			render_set_background_byte(i, j, (i == (x_pixel/8) ? leftmask : even_mask) & even_mask & (i == (rtpixel - 1) ? rightmask : even_mask), target, bottommost, rightmost);
		}
	}
}

/* Impose grayed out area over a rectangle (ORed on)
 * To implement this, draw the item then if it's on a black background, toggle
 * the black background over it AFTER you set this disable rectangle
 * */
void render_disable_rectangle_OR(int x_pixel, int y_pixel, int x_p_width, int y_p_height, uint8 target[][RENDER_MAX_WIDTH], int bottommost, int rightmost){
	/* Draw the box in at x_pixel for x_p_width */
	int i = 0;
	int j = 0;

	int rtpixel = ((x_pixel + x_p_width)/8 + 1);

	/* Create the left and right masks */
	uint8 even_mask = 0x55;

	uint8 leftmask = 0;
	uint8 rightmask = 0;

	for(j = y_pixel; (j < (y_pixel + y_p_height)) && (j < bottommost); j++){
		even_mask = ~even_mask;

		leftmask = (0xFF >> (x_pixel % 8)) & even_mask;
		rightmask = (0xFF << (8 - ((x_pixel + x_p_width) % 8))) & even_mask;
		for(i = (x_pixel/8); (i < rtpixel) && (i < rightmost); i++){
			render_set_foreground_byte(i, j, (i == (x_pixel/8) ? leftmask : even_mask) & even_mask & (i == (rtpixel - 1) ? rightmask : even_mask), target, bottommost, rightmost);
		}
	}
}

/* Place char2place onto the screen, starting at x, y (pixels) using draw_type */
void render_char(char char2place, int x, int y, uint8 draw_type, uint8 target[][RENDER_MAX_WIDTH], int bottommost, int rightmost){
	render_char_w_ptr(get_char_graphic_pointer(char2place), x, y, draw_type, target, bottommost, rightmost);
}

/* Not fully implemented yet */
void render_char_w_ptr_adv(uint8* ptr2char, int x_pixel, int y_pixel, uint8 draw_type,
		int left_bound, int right_bound, int top_bound, int bottom_bound, uint8 target[][RENDER_MAX_WIDTH], int bottommost, int rightmost){
	/* Do quick sanity check to see if this will be printing off of the screen
	 * (if that's the case, don't bother) */

	/* Column (in bytes) - need to multiply by 8 to get pixel */
	int _B_i = 0;
	/* Pixel of row you're in */
	int _p_j = 0;

	/* The left offset */
	uint8 shift_p_get = __HELV_b_cur_lstart % 8;
	uint8 shift_p_set = x_pixel % 8;
	uint8 byte2go;

	uint8 leftmask = (0xFF >> (x_pixel % 8)) & 0xFF;
	uint8 rightmask = (0xFF << (8 - ((x_pixel + right_bound) % 8))) & 0xFF;
	int rtpixel = ((x_pixel + right_bound)/8 + 1);

	/* Get the first byte that's actually got data (indexed by the __HELV_b_get_cur_lstart */
	uint8 xmin = (__HELV_b_cur_lstart / 8);
	uint8 plotxmin = x_pixel / 8;

	int resolved_x_pixel;
	uint8 resolved_byte = 0;

	uint8 even_mask = 0x55;
	boolean use_even = (draw_type & DRAW_LETTER_DISABLE_MASK) == DRAW_LETTER_DISABLED;

	/* Make sure this is on the screen before doing anything */
	if((x_pixel < (rightmost * 8)) && (y_pixel < bottommost)){
		/* Go through each row */
		for(_p_j = __HELV_b_cur_top; _p_j < __HELV_b_cur_bottom; _p_j++){

			if((y_pixel + _p_j) > bottom_bound)
				break;

			if(use_even)
				even_mask = ~even_mask;

			if((y_pixel + _p_j) >= top_bound){
				for(_B_i = (int)xmin; _B_i < ((__HELV_b_cur_wid / 8) + 1) + (int)xmin; _B_i++){
					/* safeguard against the case where the width is the entire width of the byte[] */
					if(_B_i < __HELV_WID){
						/* If the left of the character is actually right on the beginning of a bit,
						 * don't bother with doing bit shifting */
						byte2go = (shift_p_get == 0) ? ptr2char[(_p_j * __HELV_WID) + _B_i] :
								(ptr2char[(_p_j * __HELV_WID) + _B_i] << shift_p_get) |
								(ptr2char[(_p_j * __HELV_WID) + _B_i + 1] >> (8 - shift_p_get));
						/* only bother to write if the byte has data (aka not 0x00) */
						if(byte2go != 0x00){
							/* Now that we have the byte that we're going to place on there, we need to place it on the
							 * right byte in the scratch screen */
							resolved_x_pixel = plotxmin + _B_i - xmin;
							switch(draw_type & DRAW_MODE_MASK){
							case DRAW_SET_B:
								/* Since we want the background to have an imprint of this, not it in */
								resolved_byte = ((~(byte2go >> shift_p_set)) & (use_even ? even_mask : 0xFF));
								render_set_background_byte(resolved_x_pixel, y_pixel + _p_j,
										(resolved_x_pixel == (x_pixel/8) ? leftmask : 0xFF) & resolved_byte &
										(resolved_x_pixel == (rtpixel - 1) ? rightmask : 0xFF), target, bottommost, rightmost
								);
								if ((resolved_x_pixel + 1) < rightmost){
									resolved_byte = ((~(byte2go << (8 - shift_p_set)) & (use_even ? even_mask : 0xFF)));
									render_set_background_byte(resolved_x_pixel + 1, y_pixel + _p_j,
											((resolved_x_pixel + 1) == (x_pixel/8) ? leftmask : 0xFF) & resolved_byte &
											((resolved_x_pixel + 1) == (rtpixel - 1) ? rightmask : 0xFF), target, bottommost, rightmost
									);
								}
								break;
							case DRAW_SET_F:
								/* Place the character onto the screen queued */
								resolved_byte = (byte2go >> shift_p_set) & (use_even ? even_mask : 0xFF);
								render_set_foreground_byte(resolved_x_pixel, y_pixel + _p_j,
										(resolved_x_pixel == (x_pixel/8) ? leftmask : 0xFF) & resolved_byte &
										(resolved_x_pixel == (rtpixel - 1) ? rightmask : 0xFF), target, bottommost, rightmost
								);
								if ((resolved_x_pixel + 1) < rightmost){
									resolved_byte = (byte2go << (8 - shift_p_set)) & (use_even ? even_mask : 0xFF);
									render_set_foreground_byte(resolved_x_pixel + 1, y_pixel + _p_j,
											((resolved_x_pixel + 1) == (x_pixel/8) ? leftmask : 0xFF) & resolved_byte &
											((resolved_x_pixel + 1) == (rtpixel - 1) ? rightmask : 0xFF), target, bottommost, rightmost
									);
								}
								break;
							default:
							case DRAW_SET_TOGGLE:
								resolved_byte = (byte2go >> shift_p_set) & (use_even ? even_mask : 0xFF);
								render_XOR_byte(resolved_x_pixel, y_pixel + _p_j,
										(resolved_x_pixel == (x_pixel/8) ? leftmask : 0xFF) & resolved_byte &
										(resolved_x_pixel == (rtpixel - 1) ? rightmask : 0xFF), target, bottommost, rightmost
								);
								if ((resolved_x_pixel + 1) < rightmost){
									resolved_byte = (byte2go << (8 - shift_p_set)) & (use_even ? even_mask : 0xFF);
									render_XOR_byte(resolved_x_pixel + 1, y_pixel + _p_j,
											((resolved_x_pixel + 1) == (x_pixel/8) ? leftmask : 0xFF) & resolved_byte &
											((resolved_x_pixel + 1) == (rtpixel - 1) ? rightmask : 0xFF), target, bottommost, rightmost
									);
								}
								break;
							}
						}
					}
				}
			}
		}
	}
}

/* Render a character on target with a pointer to the glyph's rasterized array
 * */
void render_char_w_ptr(uint8* ptr2char, int x_pixel, int y_pixel, uint8 draw_type, uint8 target[][RENDER_MAX_WIDTH], int bottommost, int rightmost){
	render_char_w_ptr_adv(ptr2char, x_pixel, y_pixel, draw_type, 0, rightmost * 8, 0, bottommost, target, bottommost, rightmost);
}

/*  TODO - fix center alignment
 * */
int render_smart_string_within_bounds(char* line, int linelen, int start_row, int alignment_line,
		uint8 draw_type, int bottommost_line, int rightmost_line, uint8 target[][RENDER_MAX_WIDTH], int bottommost, int rightmost){
	/* First thing to do is get the length of the string */
	int length_of_string = get_line_length(line, linelen);
	int i = 0;
	int j = 0;
	int _b_linewid_starting = 0;
	int _b_linewid_current = 0;
	int tempwid = 0;
	uint8* ptr2char;
	int numlines = 0;
	boolean _should_try_draw_ellipsis = false;
	int wordlen = 0;
	boolean checkword = false;
	boolean _dontdraw = false;

	switch(draw_type & DRAW_ALIGN_MASK){
	default:
	case DRAW_ALIGN_LEFT:
		_b_linewid_current = alignment_line;
		_b_linewid_starting = _b_linewid_current;
		break;
	case DRAW_ALIGN_CENTER:
		/* Calculate the middle of the string */
		_b_linewid_current = alignment_line - (length_of_string / 2);
		_b_linewid_starting = _b_linewid_current;
		break;
	case DRAW_ALIGN_RIGHT:
		_b_linewid_current = alignment_line - (length_of_string);
		_b_linewid_starting = _b_linewid_current;
		break;
	}

	for(i = 0; i < linelen; i++){
		if(((start_row + (numlines * RENDER_LINE_HEIGHT)) > bottommost_line) ||
				((start_row + (numlines * RENDER_LINE_HEIGHT)) > bottommost))
			break;

		switch(line[i]){
		case ' ':
			_b_linewid_current += RENDER_SPACE_PIXELS;
			checkword = true;
			break;
		case '\t':
			_b_linewid_current += RENDER_TAB_PIXELS;
			checkword = true;
			break;
		case '\r':
		case '\n':
			/* New line - just advance the numlines and reset linewid */
			_b_linewid_current = _b_linewid_starting;
			numlines++;
			/* Don't bother checking the next word since all it'll do
			 * is try to make the word wrap which if it needs to it's
			 * just too long*/
			checkword = false;
			break;
		default:
			if(checkword){
				/* See if the current word will fit on the screen */
				wordlen = 0;
				for(j = i; j < linelen; j++){
					/* If it's a space then we're done here */
					if(is_whitespace(line[j]))
						break;
					/* Place the word on a new line if it's longer than the current line allows. */
					wordlen += get_char_graphic_width(line[j]) + RENDER_KERN_PIXELS;
					if((wordlen + _b_linewid_current) > rightmost_line){

						if((draw_type & DRAW_WORD_WRAP_MASK) == DRAW_WORD_WRAP_MASK){
							_b_linewid_current = _b_linewid_starting;
							numlines++;
						}
						else{
							//							_dontdraw = true;
							/* Don't stop drawing, just be aware that this is
							 * the last line so draw the ellipsis instead */
							_should_try_draw_ellipsis = true;
						}

						checkword = false;
						break;
					}
				}
				checkword = false;
			}

			/* Calculate if you are at the last allowed line */
			if(((start_row + ((numlines - 1) * RENDER_LINE_HEIGHT)) >= bottommost_line) ||
					((start_row + ((numlines - 1) * RENDER_LINE_HEIGHT)) >= bottommost))
				_should_try_draw_ellipsis = (length_of_string + _b_linewid_current) > (rightmost_line);

			if(((start_row + (numlines * RENDER_LINE_HEIGHT)) > bottommost_line) ||
					((start_row + (numlines * RENDER_LINE_HEIGHT)) > bottommost))
				_dontdraw = true;

			if(!_dontdraw){
				ptr2char = get_char_graphic_pointer(line[i]);
				tempwid = __HELV_b_cur_wid;
				if(ptr2char != 0){
					if(_should_try_draw_ellipsis && ((_b_linewid_current + tempwid + RENDER_KERN_PIXELS) > ((rightmost_line) - RENDER_ELLIPSIS_WID))){
						/* Draw the ellipsis instead of finishing the line */
						render_ellipsis(_b_linewid_current, (start_row + (numlines * RENDER_LINE_HEIGHT)), draw_type, target, bottommost, rightmost);
						_dontdraw = true;
					}
					else{
						if(_b_linewid_current < (rightmost_line)){
							length_of_string -= tempwid + RENDER_KERN_PIXELS;
							_b_linewid_current += tempwid + RENDER_KERN_PIXELS;
							render_char_w_ptr_adv(ptr2char, (_b_linewid_current - tempwid ), (start_row + (numlines * RENDER_LINE_HEIGHT)),
									draw_type, 0, rightmost_line, start_row, bottommost_line, target, bottommost, rightmost);
						}
					}
				}
			}
			break;
		}
	}
	return start_row + ((numlines + 1) * RENDER_LINE_HEIGHT);
}


/* Use this to place a line onto the screen, top of the line is at start_row
 * alignment_line is the alignment line (if draw_type is
 * DRAW_ALIGN_LEFT - alignment_line is the leftmost pixel,
 * DRAW_ALIGN_CENTER - alignment_line is center pixel,
 * DRAW_ALIGN_RIGHT - alignment_line is rightmost pixel,
 * if the string won't fit onto the screen, an ellipsis is inserted at the end
 *  */
int render_smart_string(char* line, int linelen, int start_row, int alignment_line, uint8 draw_type, uint8 target[][RENDER_MAX_WIDTH], int bottommost, int rightmost){
	return render_smart_string_within_bounds(line, linelen, start_row, alignment_line, draw_type, bottommost, 8 * rightmost, target, bottommost, rightmost);
}

/* Place an ellipsis on the screen (...)
 * */
void render_ellipsis(int x, int y, uint8 draw_type, uint8 target[][RENDER_MAX_WIDTH], int bottommost, int rightmost){
	uint8* ptr = get_char_graphic_pointer('.');
	int i = 0;
	for(i = 0; i < 3; i++){
		x += RENDER_KERN_PIXELS + __HELV_b_cur_wid;
		render_char_w_ptr(ptr, x - __HELV_b_cur_wid, y, draw_type, target, bottommost, rightmost);
	}
}

/* Copy a line from one screen to another (anticipated use is for copying from the scratch area for
 * rendering items to the scratch screen that's then placed on the screen)
 * */
void render_copy_line(uint8 target[][RENDER_MAX_WIDTH], uint8 source[][RENDER_MAX_WIDTH],
		int target_row, int target_width, int source_row, int source_width){
	int i = 0;
	int j = 0;

	for(i = 0; (i < target_width) && (j < source_width); i++, j++){
		target[target_row][i] = source[source_row][j];
	}
}

/* Returns the length of the line passed to it (does not take screen size into account)
 * */
int get_line_length(char* line, int linelen){
	int tempwid = 0;
	int length_of_string = 0;
	int i = 0;
	for(i = 0; i < linelen; i++){
		switch(line[i]){
		case ' ':
			length_of_string += RENDER_SPACE_PIXELS;
			break;
		case '\t':
			length_of_string += RENDER_TAB_PIXELS;
			break;
		default:
			tempwid = get_char_graphic_width(line[i]);
			length_of_string += tempwid + (tempwid == 0 ? 0 : RENDER_KERN_PIXELS);
			break;
		}
	}
	return length_of_string;
}

#define INT_2_FLOAT(x)	((float)x)

inline float parametric_bezier(float t, float a0, float a1, float a2, float a3){
	return ((powf((1 - t), 3)) * a0) + 3 * (((powf((1 - t), 2)) * a1) * t) + 3 * (1 - t) * a2 * powf(t, 2) + powf(t, 3) * a3;
}

void render_bezier_with_control_lines(int x0_p, int y0_p, int x1_p, int y1_p, int x2_p, int y2_p, int x3_p, int y3_p, uint8 draw_type, LineDrawingStyle_t * Ptr2LineProperties, uint8 target[][RENDER_MAX_WIDTH], int bottommost, int rightmost){
	/* Render in the Bezier */
	render_bezier(x0_p, y0_p, x1_p, y1_p, x2_p, y2_p, x3_p, y3_p, draw_type, Ptr2LineProperties, target, bottommost, rightmost);
	/* Render in the two control lines */
	render_line_solid(x0_p, y0_p, x1_p, y1_p, draw_type, target, bottommost, rightmost);
	render_line_solid(x2_p, y2_p, x3_p, y3_p, draw_type, target, bottommost, rightmost);
}

void render_bezier(int x0_p, int y0_p, int x1_p, int y1_p, int x2_p, int y2_p, int x3_p, int y3_p, uint8 draw_type, LineDrawingStyle_t * Ptr2LineProperties, uint8 target[][RENDER_MAX_WIDTH], int bottommost, int rightmost){
	float x0, y0, x1, y1, x2, y2, x3, y3, t, minx, maxx, miny, maxy;
	int i, numsteps;
	float xvals[MAX_POINTS_IN_BEZIER], yvals[MAX_POINTS_IN_BEZIER];

	boolean goalongy = true;

	/* Init the x and y val arrays */
	for(i = 0; i < MAX_POINTS_IN_BEZIER; i++){
		xvals[i] = 0;
		yvals[i] = 0;
	}

	/* Convert to float */
	x0 = INT_2_FLOAT(x0_p);
	y0 = INT_2_FLOAT(y0_p);
	x1 = INT_2_FLOAT(x1_p);
	y1 = INT_2_FLOAT(y1_p);
	x2 = INT_2_FLOAT(x2_p);
	y2 = INT_2_FLOAT(y2_p);
	x3 = INT_2_FLOAT(x3_p);
	y3 = INT_2_FLOAT(y3_p);

	/* Get mins, maxes */
	minx = fminf(fminf(x0, x1), fminf(x2, x3));
	maxx = fmaxf(fmaxf(x0, x1), fmaxf(x2, x3));

	miny = fminf(fminf(y0, y1), fminf(y2, y3));
	maxy = fmaxf(fmaxf(y0, y1), fmaxf(y2, y3));


	/* Calculate the number of steps */
	goalongy = ((maxy - miny) > (maxx - minx));
	numsteps = (goalongy ? (int)(maxy - miny) : (int)(maxx - minx)) + 1;

	if(numsteps >= MAX_POINTS_IN_BEZIER)
		numsteps = MAX_POINTS_IN_BEZIER - 1;

	/* Go through line */
	for(i = 0; i < numsteps; i++){
		/* Calculate t */
		t = ((float)i) / ((float)numsteps);
		xvals[i] = roundf(parametric_bezier(t, x0, x1, x2, x3));
		yvals[i] = roundf(parametric_bezier(t, y0, y1, y2, y3));
	}

	/* Place on screen */
	for(i = 0; i < (numsteps - 1); i++){
		if((xvals[i] != xvals[i + 1]) || (yvals[i] != yvals[i + 1]))
			render_line((int)xvals[i], (int)yvals[i], (int)xvals[i + 1], (int)yvals[i + 1], draw_type, Ptr2LineProperties, target, bottommost, rightmost);
	}
}

void render_line_solid(int x1_p, int y1_p, int x2_p, int y2_p, uint8 draw_type, uint8 target[][RENDER_MAX_WIDTH], int bottommost, int rightmost){
	LineDrawingStyle_t linespec = LINE_DRAWING_STYLE_SOLID;
	render_line(x1_p, y1_p, x2_p, y2_p, draw_type, &linespec, target, bottommost, rightmost);
}


inline void increment_line(LineDrawingStyle_t * Ptr2LineProperties){
	*(Ptr2LineProperties) = ((*(Ptr2LineProperties)) & LineDrawingBitMask) | (((*(Ptr2LineProperties)) + 1) & LineDataBitMask);
}

inline bool needtodrawpixelinline(LineDrawingStyle_t * Ptr2LineProperties){
	return (((*(Ptr2LineProperties)) >> (LineNumDataBits + ((*(Ptr2LineProperties)) & LineDataBitMask))) & 1) == 1;
}

/* Draw a line from (x1, y1) to (x2, y2) */
void render_line(int x1_p, int y1_p, int x2_p, int y2_p, uint8 draw_type, LineDrawingStyle_t * Ptr2LineProperties, uint8 target[][RENDER_MAX_WIDTH], int bottommost, int rightmost){
	/* Convert to signed decimels */
	float y1 = 0, y2 = 0, slope = 0, tempfloat = 0;
	int runs = 0, i = 0, tempx = 0, tempy = 0, lastx = 0, lasty = 0, ydiff = 0, j = 0;
	uint8 curbit = 0;
	boolean halfway = false;
	y1 = INT_2_FLOAT(y1_p);
	y2 = INT_2_FLOAT(y2_p);

	/* Calculate in terms of y = mx + b */
	/* m = (y2 - y1) / (x2 - x1) */
	if(x2_p == x1_p){
		if(x1_p < 0 || x1_p > (rightmost * 8))
			return;

		/* Vertical line */
		runs = (y2_p - y1_p);
		tempx = x1_p;
		for(i = 0; (runs > 0) ? (i <= runs) : (i >= runs); (runs > 0) ? (i++) : (i--)){
			tempy = y1_p + i;
			if((tempy >= 0) && (tempy <= bottommost)){
				if(needtodrawpixelinline(Ptr2LineProperties)){
					switch(draw_type & DRAW_MODE_MASK){
					case DRAW_SET_B:
						render_set_pixel_background(tempx, tempy, target, bottommost, rightmost);
						break;
					case DRAW_SET_F:
						render_set_pixel_foreground(tempx, tempy, target, bottommost, rightmost);
						break;
					case DRAW_SET_TOGGLE:
						render_flip_pixel(tempx, tempy, target, bottommost, rightmost);
						break;
					default:
						break;
					}
				}
				increment_line(Ptr2LineProperties);
			}
		}
	}
	else{
		/* Calculate how many values of x */
		runs = x2_p - x1_p;

		slope = (y2 - y1) / INT_2_FLOAT(runs);
		lasty = y1_p;
		lastx = x1_p;

		for(i = 0; (runs > 0) ? (i <= runs) : (i >= runs); (runs > 0) ? (i++) : (i--)){
			tempx = i + x1_p;
			tempfloat = (slope * INT_2_FLOAT(i));

			/* Determine if to round up or leave as is (bottom will be truncated when cast back to int) */
			tempfloat += ((tempfloat - ((float32)((int)tempfloat))) >= 0.5) ? 1 : 0;

			tempy = ((int)tempfloat) + y1_p;

			/* Iterate through this for as many y values between tempy and lasty */
			ydiff = tempy - lasty;
			halfway = false;
			for(j = (ydiff > 0) ? 1 : 0; (ydiff > 0) ? (j <= ydiff) : (j >= ydiff); (ydiff > 0) ? (j++) : (j--)){
				/* Halfway through? */
				if((!halfway) && ((ydiff > 0) ? (j >= (ydiff/2)) : (j < (ydiff/2))))
					halfway = true;
				/* Draw in ydiff dots between tempy and lasty along tempx and lastx */
				if((((lasty + j) >= 0) && ((lasty + j) <= bottommost)) && (((halfway ? tempx : lastx) >= 0) && ((halfway ? tempx : lastx) <= (rightmost * 8)))){
					if(needtodrawpixelinline(Ptr2LineProperties)){
						switch(draw_type & DRAW_MODE_MASK){
						case DRAW_SET_B:
							render_set_pixel_background(halfway ? tempx : lastx, lasty + j, target, bottommost, rightmost);
							break;
						case DRAW_SET_F:
							render_set_pixel_foreground(halfway ? tempx : lastx, lasty + j, target, bottommost, rightmost);
							break;
						case DRAW_SET_TOGGLE:
							render_flip_pixel(tempx, tempy, target, bottommost, rightmost);
							break;
						default:
							break;
						}
					}
					increment_line(Ptr2LineProperties);
				}
			}
			/* Update value of lasty */
			lasty = tempy;
			lastx = tempx;
		}
	}
}













