/******************************************
 *
 * Menu display screen graphics and
 * navigation engine
 * Sets up the menus that are to be shown
 * on the screen.  The contents are
 * controlled by files outside of this
 *
 *****************************************/

#include "FreeRTOS.h"
#include "mcs_time.h"
#include "gps.h"

#include "eink.h"
#include "menu_display.h"
#include "render.h"
#include "DisplayTask.h"

#include "Semaphores.h"


/* States of the screen */
typedef enum display_mode{
	MENU_MODE,
	ITEM_MODE
}display_mode_t;

static display_mode_t l_curr_disp_mode = MENU_MODE;

static char l_curr_screen_title[MENU_TITLE_MAX_STRING_LENGTH];
static int l_curr_menu_title_length;
static int l_curr_menu_active_range;
/* Index of last menu item + 1 (-1 if menu is empty) */
static uint16_t l_curr_menu_last_item_index;

/* to disable a menu item, set its length to 0 */
static char l_curr_menu_item_list[MAX_NUM_MENU_ITEMS][MENU_TITLE_MAX_STRING_LENGTH];
static int l_curr_menu_item_lengths[MAX_NUM_MENU_ITEMS];
static uint8 l_curr_menu_item_flags[MAX_NUM_MENU_ITEMS];
static menuItemCallbackFunctionPtrType l_curr_menu_item_callback[MAX_NUM_MENU_ITEMS];

static menuItemRenderFunctionPtrType l_curr_menu_item_render_functions[MAX_NUM_ITEM_RENDERERS];
static uint8 l_highest_item_callback_index = 0;

static boolean l_item_needs_complete_keypad_control = false;
static menuItemKeyPressFunctionPtrType l_curr_item_key_press_handler;

uint8 g_item_render_space[ITEM_RENDER_SPACE_HEIGHT][ITEM_RENDER_SPACE_WIDTH];
static int l_rightmost_item_pixel = 0;
static int l_bottommost_item_pixel = 0;
static int l_top_item_pixel = 0;

boolean g_gps_is_set;
boolean g_gsm_is_set;
boolean g_is_home;
boolean g_can_go_back;





/* Functions */
void menu_driver_init(){
	l_curr_disp_mode = MENU_MODE;
	clear_menu();
	g_gsm_is_set = false;
	g_gps_is_set = false;
	g_is_home = true;
	g_can_go_back = false;
}

int get_last_index_value(){
	return l_curr_menu_last_item_index;
}


void draw_top_info(){
	if(can_write_to_display()){
		int i = 0;
		int j = 0;
		/* Place GPS icon */
		for(i = 0; i < GPS_ICON_HEIGHT; i++){
			for(j = 0; j < GPS_ICON_WIDTH; j++){
				/* Put the GPS icon (globe) 4 pixels down from the top */
				render_put_byte((BYTES_IN_1_LINE - GPS_ICON_WIDTH) + j, i + 4,
						GPS_ICON[i][j], DRAW_TOGGLE, scratch_screen, LINES_ON_SCREEN,
						BYTES_IN_1_LINE);
			}
		}

		/* See if GPS is working - if not X it out */
		if(!g_gps_is_set){
			render_disable_rectangle((BYTES_IN_1_LINE - GPS_ICON_WIDTH) * 8, 4,
					GPS_ICON_WIDTH * 8, GPS_ICON_HEIGHT, scratch_screen,
					LINES_ON_SCREEN, BYTES_IN_1_LINE);
		}

		/* Place GSM icon */
		for(i = 0; i < GSM_ICON_HEIGHT; i++){
			for(j = 0; j < GSM_ICON_WIDTH; j++){
				render_put_byte(j, i, GSM_ICON[i][j], DRAW_TOGGLE, scratch_screen,
						LINES_ON_SCREEN, BYTES_IN_1_LINE);
			}
		}

		/* See if GSM is working - if not X it out */
		if(!g_gsm_is_set){
			render_disable_rectangle(0, 0, GSM_ICON_WIDTH * 8, GSM_ICON_HEIGHT,
					scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
		}
		else{
			/* Draw the waves */
			for(i = 0; i < GSM_ICON_HEIGHT; i++){
				for(j = 0; j < GSM_ICON_WIDTH; j++){
					render_put_byte(j, i, GSM_ICON_WAVES[i][j], DRAW_TOGGLE,
							scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
				}
			}
		}

		/* Draw the top box */
		render_rectangle(0, 0,
				(8 * BYTES_IN_1_LINE), MENU_TITLE_HEIGHT, DRAW_TOGGLE,
				scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
	}
}

void draw_home_icon(){
	if(can_write_to_display()){
		/* ICON IN POSITION B */
		/* Draw the home icon */
		draw_home((POSITION_B - ((HOME_WIDTH * 8) / 2)),
				((LINES_ON_SCREEN - MENU_BOTTOM_HEIGHT) + ((MENU_BOTTOM_HEIGHT - HOME_HEIGHT) / 2)),
				DRAW_TOGGLE);

		/* Disable the home icon if you're already at home */
		if(g_is_home)
			render_disable_rectangle((POSITION_B - ((HOME_WIDTH * 8) / 2)),
					((LINES_ON_SCREEN - MENU_BOTTOM_HEIGHT) + ((MENU_BOTTOM_HEIGHT - HOME_HEIGHT) / 2)),
					HOME_WIDTH * 8, HOME_HEIGHT, scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
	}
}

void draw_back_icon(){
	if(can_write_to_display()){
		/* ICON IN POSITION C */
		/* Draw the back space icon */
		draw_return_arrow((POSITION_C - ((RETURN_ARROW_WIDTH * 8) / 2)),
				((LINES_ON_SCREEN - MENU_BOTTOM_HEIGHT) + ((MENU_BOTTOM_HEIGHT - RETURN_ARROW_HEIGHT) / 2)),
				DRAW_TOGGLE);

		if(!g_can_go_back)
			render_disable_rectangle((POSITION_C - ((RETURN_ARROW_WIDTH * 8) / 2)),
					((LINES_ON_SCREEN - MENU_BOTTOM_HEIGHT) + ((MENU_BOTTOM_HEIGHT - RETURN_ARROW_HEIGHT) / 2)),
					RETURN_ARROW_WIDTH * 8, RETURN_ARROW_HEIGHT, scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
	}
}

/* Draw the bottom bar at the bottom of the screen - designed for use with menu
 * (not icons)
 * */
void draw_bottom_info_MENU(){
	if(can_write_to_display()){
		int arrow_centerline = ((LINES_ON_SCREEN - MENU_BOTTOM_HEIGHT) + ((MENU_BOTTOM_HEIGHT - UP_ARROW_HEIGHT) / 2));
		/* MAGIC NUMBER */
		int arrow_vert_center = (UP_ARROW_WIDTH * 8) - 6;

		/* ICON IN POSITION A */
		/* Draw the up icon */
		draw_arrow(POSITION_A - (arrow_vert_center / 2), arrow_centerline, DRAW_TOGGLE, true);

		switch(l_curr_disp_mode){
		case MENU_MODE:
			if(l_curr_menu_active_range == 0)
				render_disable_rectangle(POSITION_A - (arrow_vert_center / 2),
						arrow_centerline, UP_ARROW_WIDTH * 8, UP_ARROW_HEIGHT, scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
			break;
		case ITEM_MODE:
			if(l_top_item_pixel == 0)
				render_disable_rectangle(POSITION_A - (arrow_vert_center / 2),
						arrow_centerline, UP_ARROW_WIDTH * 8, UP_ARROW_HEIGHT, scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
			break;
		default:
			break;
		}

		if(!l_item_needs_complete_keypad_control)
			draw_home_icon();

		draw_back_icon();

		/* ICON IN POSITION D */
		/* Draw the down icon */
		draw_arrow(POSITION_D - (arrow_vert_center / 2), arrow_centerline, DRAW_TOGGLE, false);

		switch(l_curr_disp_mode){
		case MENU_MODE:
			if((l_curr_menu_active_range + NUM_ITEMS_FULLY_VISIBLE) >= (l_curr_menu_last_item_index))
				render_disable_rectangle(POSITION_D - (arrow_vert_center / 2),
						arrow_centerline, UP_ARROW_WIDTH * 8, UP_ARROW_HEIGHT, scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
			break;
		case ITEM_MODE:
			if((l_top_item_pixel + MENU_ACTIVE_HEIGHT) >= l_bottommost_item_pixel)
				render_disable_rectangle(POSITION_D - (arrow_vert_center / 2),
						arrow_centerline, UP_ARROW_WIDTH * 8, UP_ARROW_HEIGHT, scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
			break;
		default:
			break;
		}

		/* Draw the box */
		render_rectangle(0, (LINES_ON_SCREEN - MENU_BOTTOM_HEIGHT),
				(8 * BYTES_IN_1_LINE), MENU_BOTTOM_HEIGHT, DRAW_TOGGLE, scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
	}
}

/* Render the menu */
void render_menu(){
	if(can_write_to_display()){
		clear_scratch_screen();

		int i = 0;
		/* Clear the buffer */
		char item_string[2] = {'X', '.'};

		draw_top_info();
		/* Render the menu header */
		render_smart_string(l_curr_screen_title, l_curr_menu_title_length, MENU_TOP_BUFF,
				(4 * BYTES_IN_1_LINE), DRAW_ALIGN_CENTER | DRAW_TOGGLE, scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);

		switch(l_curr_disp_mode){
		case MENU_MODE:
		{
			uint8_t draw_flag = 0;
			for(i = 0; i < (NUM_ITEMS_FULLY_VISIBLE + 1); i++){
				/* Render each item on the screen then deal with its flags but only if
				 * its length is longer than 0 (to disable a menu item, set its length to 0)*/
				if(((l_curr_menu_active_range + i) < MAX_NUM_MENU_ITEMS) && (l_curr_menu_item_lengths[i + l_curr_menu_active_range] > 0)){
					item_string[0] = i + 49 + l_curr_menu_active_range;

					draw_flag = (l_curr_menu_item_flags[i + l_curr_menu_active_range] & MENU_FLAG_ITEM_DISABLE) ? DRAW_LETTER_DISABLED : 0;
					/* Draw the index (i. e. 1., 2. etc.) */
					render_smart_string_within_bounds(item_string,
							2,
							get_menu_item_vert_offset_for_rendering(i),
							(MENU_HORIZ_OFFET / 2),
							DRAW_ALIGN_CENTER | DRAW_TOGGLE | draw_flag,
							(LINES_ON_SCREEN - MENU_BOTTOM_HEIGHT - 1),
							MENU_RIGHT_BAR_LINE, scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);

					/* Draw text to be displayed at index */
					render_smart_string_within_bounds(l_curr_menu_item_list[i + l_curr_menu_active_range],
							l_curr_menu_item_lengths[i + l_curr_menu_active_range],
							get_menu_item_vert_offset_for_rendering(i),
							(MENU_HORIZ_OFFET + RENDER_SPACE_PIXELS),
							DRAW_ALIGN_LEFT | DRAW_TOGGLE | draw_flag,
							(LINES_ON_SCREEN - MENU_BOTTOM_HEIGHT - 1),
							MENU_RIGHT_BAR_LINE, scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);

				}
			}
		}
		break;
		case ITEM_MODE:
		{
			/* Render the item from the l_top_item_pixel to the bottom */
			/* TODO - write rendering routine */
			if(l_top_item_pixel < 0)
				l_top_item_pixel = 0;

			/* Force rerender (update data as necessary) */
			pre_render_menu_item();

			int y = MENU_TITLE_HEIGHT;
			int x = 0;
			for(x = l_top_item_pixel; (x < l_bottommost_item_pixel) && (x < l_top_item_pixel + MENU_ACTIVE_HEIGHT); x++, y++){
				render_copy_line(scratch_screen, g_item_render_space, y, ITEM_RENDER_SPACE_WIDTH, x, ITEM_RENDER_SPACE_WIDTH);
			}
		}
		break;
		default:
		{
		}
		break;
		}

		/* Render the right bar */
		/* TODO - create right scroll bar */

		/* Render in the bottom menu items */
		draw_bottom_info_MENU();
	}
}

void set_gps(boolean have_gps){
	g_gps_is_set = have_gps;
}

void set_gsm(boolean have_gsm){
	g_gsm_is_set = have_gsm;
}

void set_home(boolean _is_home){
	g_is_home = _is_home;
}

void set_back_low_level(boolean _can_go_back){
	g_can_go_back = _can_go_back;
}

int get_menu_item_vert_offset(int item_number){
	/* Menu items */
	return (item_number * ITEM_HEIGHT) + MENU_TITLE_HEIGHT;
}

int get_menu_item_vert_offset_for_rendering(int item_number){
	return get_menu_item_vert_offset(item_number) + MENU_TOP_BUFF;
}

void clear_menu(){
	/* Clear the menu so you can load another */
	int i = 0;
	int j = 0;

	for(i = 0; i < MENU_TITLE_MAX_STRING_LENGTH; i++)
		l_curr_screen_title[i] = 0;

	l_curr_menu_title_length = 0;
	l_curr_menu_active_range = 0;
	l_curr_menu_last_item_index = 0;

	for(i = 0; i < MAX_NUM_MENU_ITEMS; i++){
		for(j = 0; j < MENU_TITLE_MAX_STRING_LENGTH; j++)
			l_curr_menu_item_list[i][j] = 0;

		l_curr_menu_item_lengths[i] = 0;
		l_curr_menu_item_flags[i] = 0;
	}

	clear_render_space();

	for(i = 0; i < MAX_NUM_ITEM_RENDERERS; i++)
		l_curr_menu_item_render_functions[i] = NULL;

	l_highest_item_callback_index = 0;

	l_rightmost_item_pixel = 0;
	l_bottommost_item_pixel = 0;
	l_top_item_pixel = 0;

	l_curr_item_key_press_handler = NULL;
	l_item_needs_complete_keypad_control = false;

}

void clear_render_space(){
	int i = 0;
	int j = 0;

	for(i = 0; i < MENU_ITEM_MAX_HEIGHT; i++){
		for(j = 0; j < BYTES_IN_1_LINE; j++)
			g_item_render_space[i][j] = 0x00;
	}
}

void set_screen_title(char* nul_terminated_string_menu_title){
	int max_string_length = MENU_TITLE_MAX_STRING_LENGTH;
	int i = 0;
	while(max_string_length >= 0){
		if(nul_terminated_string_menu_title[i] != 0)
			l_curr_screen_title[i] = nul_terminated_string_menu_title[i];
		else
			break;

		i++;
		max_string_length--;
	}
	l_curr_menu_title_length = i;
}

/* Add a menu item:
 * Item's name (as displayed on the screen)
 * Display options
 * callback that is executed when item is pressed (enqueue actions, draw items, etc.)
 *
 * */
void add_menu_item(char* nul_terminated_string_menu_item, uint8 flags,
		menuItemCallbackFunctionPtrType item_callback){
	if(l_curr_menu_last_item_index < (MAX_NUM_MENU_ITEMS)){
		/* TODO clean up how menus/items are registerred */
		l_curr_disp_mode = MENU_MODE;

		int max_string_length = MENU_ITEM_MAX_STRING_LENGTH;
		int i = 0;
		while(max_string_length >= 0){
			if(nul_terminated_string_menu_item[i] != 0)
				l_curr_menu_item_list[l_curr_menu_last_item_index][i] = nul_terminated_string_menu_item[i];
			else
				break;

			i++;
			max_string_length--;
		}

		l_curr_menu_item_flags[l_curr_menu_last_item_index] = flags;

		l_curr_menu_item_lengths[l_curr_menu_last_item_index] = i;

		l_curr_menu_item_callback[l_curr_menu_last_item_index] = item_callback;

		l_curr_menu_last_item_index++;
	}
}

/*
 * Returns if the screen needs to be refreshed, if it does then
 * it will be refreshed to the active value
 * */
boolean menu_up(){
	int origval = l_curr_menu_active_range;
	if(l_curr_menu_active_range > 0){
		l_curr_menu_active_range -= NUM_ITEMS_FULLY_VISIBLE;
		if(l_curr_menu_active_range < 0)
			l_curr_menu_active_range = 0;
	}

	return !(origval == l_curr_menu_active_range);
}

/*
 * Returns if the screen needs to be refreshed, if it does then
 * it will be refreshed to the active value
 * */
boolean menu_down(){
	int origval = l_curr_menu_active_range;
	if(l_curr_menu_active_range < l_curr_menu_last_item_index){
		l_curr_menu_active_range += NUM_ITEMS_FULLY_VISIBLE;
		if(l_curr_menu_active_range >= (l_curr_menu_last_item_index - NUM_ITEMS_FULLY_VISIBLE)){
			l_curr_menu_active_range = (l_curr_menu_last_item_index - NUM_ITEMS_FULLY_VISIBLE);
			if(l_curr_menu_active_range < 0)
				l_curr_menu_active_range = 0;
		}
	}

	return !(origval == l_curr_menu_active_range);
}

/*
 * Go to home menu - return TRUE if you can go home, then execute
 * */
boolean menu_home(){
	boolean rtnval = false;
	if(!g_is_home){
		g_current_menu = MAIN_MENU;
		switch_menu();

		rtnval = true;
	}
	return rtnval;
}

/*
 * Go back - return TRUE if you can go back, then execute going back
 *  */
boolean menu_back(){
	return g_can_go_back;
}

boolean item_up(){
	int origval = l_top_item_pixel;
	l_top_item_pixel -= MENU_ITEM_MOVE_SPACE;
	if(l_top_item_pixel < 0)
		l_top_item_pixel = 0;

	return !(origval == l_top_item_pixel);
}

boolean item_down(){
	int origval = l_top_item_pixel;
	l_top_item_pixel += MENU_ITEM_MOVE_SPACE;

	if((l_top_item_pixel + MENU_ACTIVE_HEIGHT) > l_bottommost_item_pixel)
		l_top_item_pixel = l_bottommost_item_pixel - MENU_ACTIVE_HEIGHT;

	if(l_top_item_pixel < 0)
		l_top_item_pixel = 0;

	return !(origval == l_top_item_pixel);
}

/* Items can ALWAYS go home (since home is never an item) */
boolean item_home(){
	g_current_menu = MAIN_MENU;
	switch_menu();

	return true;
}

/* Items can ALWAYS go back (since they're always children) */
boolean item_back(){
	return true;
}

/* The key pressed state contains an alpha character */
boolean is_key_pressed_navigation(uint16 key_pressed){
	boolean rtnval = false;
	if((key_pressed & KEY_ALPHA) != 0)
		rtnval = true;

	return rtnval;
}


/* Pass in the key press value and this will return true if the key
 * that was pressed corresponds to a real menu option that's
 * available, will also set menu option flag
 */
boolean select_menu_item(uint16_t key_pressed){
	/* Calculate what the actual value (in index values) the item
	 * that was selected is */
	boolean rtnval = false;
	int key_val;

	key_val = (keypad_get_numeric(key_pressed) + 9) % 10;

	/* If there's a callback that can handle itself, call it (this funciton will
	 * return true if that's the case, otherwise it returns false at which point it
	 * goes to the staticly linked items below) */
	rtnval = generic_menu_item_selector(key_val);

	/* TODO remove the below sections - everything should be handled by callbacks */
	if(!rtnval){
		/* make sure that there is an item to select for the keypress */
		if(key_val >= l_curr_menu_last_item_index){
			rtnval = false;
		}
		else{
			/*go to the right menu for processing */
			switch(g_current_menu){
			case MAIN_MENU:
				main_menu_selector(key_pressed);
				rtnval = true;
				break;
			case ADMIN_MENU:
			case SYSTEM_STATUS_ITEM:
			case START_PUMPS_MENU:
				break;
			default:
				/* Unsure what's going on - set to main menu and redraw */
				break;
			}
		}
	}
	return rtnval;

}

/* Interpret the key that was pressed and take appropriate action */
void process_keypress(uint16_t key_pressed){

	uint8_t status_flag = FALSE_FLAG;
	switch(l_curr_disp_mode){
	case MENU_MODE:
	{
		switch(key_pressed){
		case KEY_1:
		case KEY_2:
		case KEY_3:
		case KEY_4:
		case KEY_5:
		case KEY_6:
		case KEY_7:
		case KEY_8:
		case KEY_9:
		case KEY_0:
			/* Choose item 1-[1]0 */
			if(select_menu_item(key_pressed)){
				status_flag = TRUE_FLAG;
			}
			break;
		case KEY_A:
			/* UP */
			if(menu_up()){
				status_flag = TRUE_FLAG;
			}
			break;
		case KEY_B:
			/* HOME */
			if(menu_home()){
				status_flag = TRUE_FLAG;
			}
			break;
		case KEY_C:
			/* BACK */
			if(menu_back()){
				go_to_prev();
				status_flag = TRUE_FLAG;
			}
			break;
		case KEY_D:
			/* DOWN */
			if(menu_down()){
				status_flag = TRUE_FLAG;
			}
			break;
		case KEY_STAR:
		case KEY_POUND:
		default:
			/* default */
			break;
		}
	}
	break;
	/* TODO fix up handling of keys A-D (universal controls) */
	case ITEM_MODE:
	{
		if(l_item_needs_complete_keypad_control){
			status_flag = l_curr_item_key_press_handler(key_pressed);
		}
		else{
			if((l_curr_item_key_press_handler != NULL) && !is_key_pressed_navigation(key_pressed))
				status_flag = l_curr_item_key_press_handler(key_pressed);
			else{
				switch(key_pressed){
				case KEY_A:
					/* UP */
					if(item_up()){
						status_flag = TRUE_FLAG;
					}
					break;
				case KEY_B:
					/* HOME */
					if(item_home()){
						status_flag = TRUE_FLAG;
					}
					break;
				case KEY_1:
				case KEY_2:
				case KEY_3:
				case KEY_4:
				case KEY_5:
				case KEY_6:
				case KEY_7:
				case KEY_8:
				case KEY_9:
				case KEY_0:
				case KEY_STAR:
				case KEY_POUND:
				case KEY_C:
					/* BACK */
					if(item_back()){
						go_to_prev();
						status_flag = TRUE_FLAG;
					}
					break;
				case KEY_D:
					/* DOWN */
					if(item_down()){
						status_flag = TRUE_FLAG;
					}
					break;
				default:
					/* default */
					break;
				}
			}
		}
	}
	break;
	default:
		break;
	}

	/*render display and notify displaytask to update display */
	if(status_flag == TRUE_FLAG){
		try_draw_menu_now();
	}
}

void try_draw_menu_now(){
	if(can_write_to_display()){
		static queues_command_input_t msg = {0};
		msg.task_id = TASKS_APP_UI;
		render_menu();
		msg.command = QUEUE_CMD_DISPLAY_LOAD_AND_GO;
		queues_send_command_message(QUEUES_APP_DISPLAY_INPUT, &msg, 0);
	}
}

/* Draw the up (or down) arrow:
 * x_pixel
 * y_pixel
 * draw_type - standard draw flags
 * isup - TRUE = draw up arrow, FALSE = draw down arrow
 *  */
void draw_arrow(int x_pixel, int y_pixel, uint8_t draw_type, boolean isup){
	if(can_write_to_display()){
		/* Draw the up arrow ^ */
		/* Column (in bytes) - need to multiply by 8 to get pixel */
		int _B_i = 0;
		/* Pixel of row you're in */
		int _p_j = 0;

		/* The left offset */
		uint8 shift_p_set = x_pixel % 8;
		uint8 byte2go;

		/* Get the first byte that's actually got data (indexed by the __HELV_b_get_cur_lstart */
		uint8 plotxmin = x_pixel / 8;

		/* Make sure this is on the screen before doing anything */
		if((x_pixel < (BYTES_IN_1_LINE * 8)) && (y_pixel < LINES_ON_SCREEN)){
			/* Go through each row */
			for(_p_j = 0; _p_j < UP_ARROW_HEIGHT; _p_j++){
				for(_B_i = 0; _B_i < UP_ARROW_WIDTH; _B_i++){
					/* safeguard against the case where the width is the entire width of the byte[] */
					/* If the left of the character is actually right on the beginning of a bit,
					 * don't bother with doing bit shifting */
					byte2go = UP_ARROW[isup ? _p_j : (UP_ARROW_HEIGHT - _p_j - 1)][_B_i];
					/* only bother to write if the byte has data (aka not 0x00) */
					if(byte2go != 0x00){
						/* Now that we have the byte that we're going to place on there, we need to place it on the
						 * right byte in the scratch screen */
						switch(draw_type & DRAW_MODE_MASK){
						case DRAW_SET_B:
							/* Since we want the background to have an imprint of this, not it in */
							render_set_background_byte(plotxmin + _B_i, y_pixel + _p_j, ~(byte2go >> shift_p_set), scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
							if ((plotxmin + _B_i + 1) < BYTES_IN_1_LINE)
								render_set_background_byte(plotxmin + _B_i + 1, y_pixel + _p_j, ~(byte2go << (8 - shift_p_set)), scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
							break;
						case DRAW_SET_F:
							/* Place the character onto the screen queued */
							render_set_foreground_byte(plotxmin + _B_i, y_pixel + _p_j, byte2go >> shift_p_set, scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
							if ((plotxmin + _B_i + 1) < BYTES_IN_1_LINE)
								render_set_foreground_byte(plotxmin + _B_i + 1, y_pixel + _p_j, byte2go << (8 - shift_p_set), scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
							break;
						default:
						case DRAW_TOGGLE:
							render_XOR_byte(plotxmin + _B_i, y_pixel + _p_j, byte2go >> shift_p_set, scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
							if ((plotxmin + _B_i + 1) < BYTES_IN_1_LINE)
								render_XOR_byte(plotxmin + _B_i + 1, y_pixel + _p_j, byte2go << (8 - shift_p_set), scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
							break;
						}
					}
				}
			}
		}
	}
}

void draw_home(int x_pixel, int y_pixel, uint8_t draw_type){
	if(can_write_to_display()){
		/* Draw the up arrow ^ */
		/* Column (in bytes) - need to multiply by 8 to get pixel */
		int _B_i = 0;
		/* Pixel of row you're in */
		int _p_j = 0;

		/* The left offset */
		uint8 shift_p_set = x_pixel % 8;
		uint8 byte2go;

		/* Get the first byte that's actually got data (indexed by the __HELV_b_get_cur_lstart */
		uint8 plotxmin = x_pixel / 8;

		/* Make sure this is on the screen before doing anything */
		if((x_pixel < (BYTES_IN_1_LINE * 8)) && (y_pixel < LINES_ON_SCREEN)){
			/* Go through each row */
			for(_p_j = 0; _p_j < HOME_HEIGHT; _p_j++){
				for(_B_i = 0; _B_i < HOME_WIDTH; _B_i++){
					/* safeguard against the case where the width is the entire width of the byte[] */
					/* If the left of the character is actually right on the beginning of a bit,
					 * don't bother with doing bit shifting */
					byte2go = HOME_ICON[_p_j][_B_i];
					/* only bother to write if the byte has data (aka not 0x00) */
					if(byte2go != 0x00){
						/* Now that we have the byte that we're going to place on there, we need to place it on the
						 * right byte in the scratch screen */
						switch(draw_type & DRAW_MODE_MASK){
						case DRAW_SET_B:
							/* Since we want the background to have an imprint of this, not it in */
							render_set_background_byte(plotxmin + _B_i, y_pixel + _p_j, ~(byte2go >> shift_p_set), scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
							if ((plotxmin + _B_i + 1) < BYTES_IN_1_LINE)
								render_set_background_byte(plotxmin + _B_i + 1, y_pixel + _p_j, ~(byte2go << (8 - shift_p_set)), scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
							break;
						case DRAW_SET_F:
							/* Place the character onto the screen queued */
							render_set_foreground_byte(plotxmin + _B_i, y_pixel + _p_j, byte2go >> shift_p_set, scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
							if ((plotxmin + _B_i + 1) < BYTES_IN_1_LINE)
								render_set_foreground_byte(plotxmin + _B_i + 1, y_pixel + _p_j, byte2go << (8 - shift_p_set), scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
							break;
						default:
						case DRAW_TOGGLE:
							render_XOR_byte(plotxmin + _B_i, y_pixel + _p_j, byte2go >> shift_p_set, scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
							if ((plotxmin + _B_i + 1) < BYTES_IN_1_LINE)
								render_XOR_byte(plotxmin + _B_i + 1, y_pixel + _p_j, byte2go << (8 - shift_p_set), scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
							break;
						}
					}
				}
			}
		}
	}
}

void draw_return_arrow(int x_pixel, int y_pixel, uint8_t draw_type){
	if(can_write_to_display()){
		/* Draw the up arrow ^ */
		/* Column (in bytes) - need to multiply by 8 to get pixel */
		int _B_i = 0;
		/* Pixel of row you're in */
		int _p_j = 0;

		/* The left offset */
		uint8 shift_p_set = x_pixel % 8;
		uint8 byte2go;

		/* Get the first byte that's actually got data (indexed by the __HELV_b_get_cur_lstart */
		uint8 plotxmin = x_pixel / 8;

		/* Make sure this is on the screen before doing anything */
		if((x_pixel < (BYTES_IN_1_LINE * 8)) && (y_pixel < LINES_ON_SCREEN)){
			/* Go through each row */
			for(_p_j = 0; _p_j < RETURN_ARROW_HEIGHT; _p_j++){
				for(_B_i = 0; _B_i < RETURN_ARROW_WIDTH; _B_i++){
					/* safeguard against the case where the width is the entire width of the byte[] */
					/* If the left of the character is actually right on the beginning of a bit,
					 * don't bother with doing bit shifting */
					byte2go = RETURN_ARROW[_p_j][_B_i];
					/* only bother to write if the byte has data (aka not 0x00) */
					if(byte2go != 0x00){
						/* Now that we have the byte that we're going to place on there, we need to place it on the
						 * right byte in the scratch screen */
						switch(draw_type & DRAW_MODE_MASK){
						case DRAW_SET_B:
							/* Since we want the background to have an imprint of this, not it in */
							render_set_background_byte(plotxmin + _B_i, y_pixel + _p_j, ~(byte2go >> shift_p_set), scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
							if ((plotxmin + _B_i + 1) < BYTES_IN_1_LINE)
								render_set_background_byte(plotxmin + _B_i + 1, y_pixel + _p_j, ~(byte2go << (8 - shift_p_set)), scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
							break;
						case DRAW_SET_F:
							/* Place the character onto the screen queued */
							render_set_foreground_byte(plotxmin + _B_i, y_pixel + _p_j, byte2go >> shift_p_set, scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
							if ((plotxmin + _B_i + 1) < BYTES_IN_1_LINE)
								render_set_foreground_byte(plotxmin + _B_i + 1, y_pixel + _p_j, byte2go << (8 - shift_p_set), scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
							break;
						default:
						case DRAW_TOGGLE:
							render_XOR_byte(plotxmin + _B_i, y_pixel + _p_j, byte2go >> shift_p_set, scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
							if ((plotxmin + _B_i + 1) < BYTES_IN_1_LINE)
								render_XOR_byte(plotxmin + _B_i + 1, y_pixel + _p_j, byte2go << (8 - shift_p_set), scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
							break;
						}
					}
				}
			}
		}
	}
}

/* Wire in callbacks */
boolean generic_menu_item_selector(int argument){
	boolean rtnval = false;
	if(argument >= 0 && argument < l_curr_menu_last_item_index){
		/* See if the item's disabled */
		if((l_curr_menu_item_flags[argument] & MENU_FLAG_ITEM_DISABLE) == MENU_FLAG_ITEM_DISABLE){
			/* Return false indicating that it's been handled */
		}
		else if(l_curr_menu_item_callback[argument] != NULL){
			/* There's a callback registered (this takes precedence over static linking) */
			rtnval = l_curr_menu_item_callback[argument]();
		}
	}
	else{
		/* Do nothing */
		// current_menu = MAIN_MENU;
		// switch_menu();
	}
	return rtnval;
}

void load_item_render_function(menuItemRenderFunctionPtrType fcn){
	l_curr_disp_mode = ITEM_MODE;
	l_top_item_pixel = 0;

	if(l_highest_item_callback_index < MAX_NUM_ITEM_RENDERERS)
		l_curr_menu_item_render_functions[l_highest_item_callback_index] = fcn;

	l_highest_item_callback_index++;
}

void load_item_key_press_handler(menuItemKeyPressFunctionPtrType fcn, boolean need_exclusive_keypad_control){
	l_curr_item_key_press_handler = fcn;
	if(fcn != NULL)
		l_item_needs_complete_keypad_control = need_exclusive_keypad_control;
	else
		l_item_needs_complete_keypad_control = false;
}

/* Render selected menu item in scratch space (to be later renderred to screen)
 *  - also sets mode to ITEM display mode */
void pre_render_menu_item(){

	/* Clear out the rendering screen */
	clear_render_space();

	/* Since this is a derived item, it can always go home, just make sure it
	 * isn't requiring exclusive control */
	if(l_curr_item_key_press_handler != NULL)
		g_is_home = false;
	//	g_can_go_back = true; /* Can go back value is set by loading function */

	int i = 0;
	int btm = 0;
	l_bottommost_item_pixel = 0;

	for(i = 0; i < MAX_NUM_ITEM_RENDERERS; i++){
		if(l_curr_menu_item_render_functions[i] != NULL){
			btm = l_curr_menu_item_render_functions[i]();

			if(btm > l_bottommost_item_pixel)
				l_bottommost_item_pixel = btm;
		}
	}


	/* Get rightmost and bottommost bounds if they aren't calculated within the
	 * render functions */
	int j = 0;
	if(l_bottommost_item_pixel <= 0){
		/* Find the bottommost row with a pixel triggered */
		for(j = MENU_ITEM_MAX_HEIGHT - 1; j >= 0; j--){
			if(l_bottommost_item_pixel != 0)
				break;

			for(i = 0; i < BYTES_IN_1_LINE; i++){
				if(g_item_render_space[j][i] != 0x00){
					l_bottommost_item_pixel = j + 1;
					break;
				}
			}
		}

		if(l_bottommost_item_pixel > ITEM_RENDER_SPACE_HEIGHT)
			l_bottommost_item_pixel = ITEM_RENDER_SPACE_HEIGHT;
	}

	/* TODO possibly put something in to calculate the rightmost pixel */
	/* TODO implement rightmost pixel */
	l_rightmost_item_pixel = 0;
	for(i = BYTES_IN_1_LINE - 1; i >= 0; i--){
		if(l_rightmost_item_pixel != 0)
			break;

		for(j = MENU_ITEM_MAX_HEIGHT - 1; j >= 0; j--){
			if(g_item_render_space[j][i] != 0x00){
				l_rightmost_item_pixel = i;
				break;
			}
		}
	}

	/* Now that we have the rightmost byte, get the bit in there */
	uint8 scratch = 0x00;
	for(i = 0; i < MENU_ITEM_MAX_HEIGHT; i++)
		scratch |= g_item_render_space[i][l_rightmost_item_pixel];

	i = 8;
	if(scratch != 0x00){
		for(i = 0; i < 8; i++){
			if((scratch & 0x01) == 0x01)
				break;

			scratch >>= 1;
		}
	}

	l_rightmost_item_pixel = ((l_rightmost_item_pixel + 1) * 8) - i;
}










