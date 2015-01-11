#include "DisplayTask.h"
#include "LogTask.h"
#include "mcs_time.h"
#include "UITask.h"
#include "GPSTask.h"
#include "common.h"

#include "gps.h"

#include "eink.h"
#include "vis.h"
#include "menu_display.h"
#include "render.h"

static volatile boolean l_is_emcy = false;
static volatile boolean l_can_press = true;

#define UI_DELAY_SECONDS	30

#define LEN_STR_2_DISP	30
static char string_to_disp[LEN_STR_2_DISP] = { 0 };

boolean alternate = false;

void ui_task( void* p_params )
{
	TickType_t curtick = 0,last_tick_refreshed = 0;

	int top = 0, i = 0;

	TickType_t waittime = UI_DELAY_SECONDS;
	GPSDATA_S _ui_gps_dat = {0};
	invalidate_GPSDATA_S_data(&_ui_gps_dat);

	clear_scratch_screen();
	//	render_disable_rectangle_OR(0,0,DISPLAY_WIDTH_PIXELS, DISPLAY_HEIGHT_PIXELS, RENDER_ENDING_3_ARGUMENTS);

	//	render_line(1, 101, 1, 120, DRAW_SET_F, RENDER_ENDING_3_ARGUMENTS);
	//	render_line(2, 101, 2, 120, DRAW_SET_F, RENDER_ENDING_3_ARGUMENTS);
	//	render_line(3, 101, 3, 120, DRAW_SET_F, RENDER_ENDING_3_ARGUMENTS);
	//	render_rectangle(0, 0, 5, 21, DRAW_SET_TOGGLE, RENDER_ENDING_3_ARGUMENTS);
	//
	//	render_line(10, 20, 10, 1, DRAW_SET_F, RENDER_ENDING_3_ARGUMENTS);
	//	render_line(11, 20, 11, 1, DRAW_SET_F, RENDER_ENDING_3_ARGUMENTS);
	//	render_line(12, 20, 12, 1, DRAW_SET_F, RENDER_ENDING_3_ARGUMENTS);
	//
	//	render_line(0, 25, 20, 25, DRAW_SET_F, RENDER_ENDING_3_ARGUMENTS);
	//	render_line(0, 27, 20, 27, DRAW_SET_F, RENDER_ENDING_3_ARGUMENTS);
	//
	render_line(40, 26, 20, 40, DRAW_SET_F, RENDER_ENDING_3_ARGUMENTS);
	//
	//		render_rectangle(0,0,50,50,DRAW_SET_TOGGLE, RENDER_ENDING_3_ARGUMENTS);

	//	render_line(2, 1, 4, 20, DRAW_SET_F, RENDER_ENDING_3_ARGUMENTS);
	//	render_line(50, 20, 1, 20, DRAW_SET_F, RENDER_ENDING_3_ARGUMENTS);
	render_line(0, 0, 90, 90, DRAW_SET_F, RENDER_ENDING_3_ARGUMENTS);
	render_line(0, 1, 90, 91, DRAW_SET_F, RENDER_ENDING_3_ARGUMENTS);
	render_line(1, 0, 91, 90, DRAW_SET_F, RENDER_ENDING_3_ARGUMENTS);

	render_bezier(10, 10, 40, 10, 40, 40, 70, 40, DRAW_SET_F, RENDER_ENDING_3_ARGUMENTS);

	boolean upload = false;

	for (;;){
		/* For now just put the tick up on the screen */
		curtick = xTaskGetTickCount();
		if((curtick > (last_tick_refreshed + (waittime * SYS_TICKS_IN_1_SEC))) && (can_write_to_display())){
			clear_scratch_screen();
			upload = false;

			/* Clear out string to display */
			for(i = 0; i < LEN_STR_2_DISP; i++)
				string_to_disp[i] = 0x00;

			/* Convert the tick counter value to a seconds then a number */
			get_str_int((curtick / SYS_TICKS_IN_1_SEC), &string_to_disp[0], LEN_STR_2_DISP);
			invalidate_GPSDATA_S_data(&_ui_gps_dat);
			GetCurrGPS(&_ui_gps_dat);
			/* Set note in top left about GPS */
			switch(GetCurrentGPSLockState()){
			case GPSLockLocked:
				render_smart_string("Locked\0", 6, 0, 0, DRAW_SET_F | DRAW_ALIGN_LEFT,
						RENDER_ENDING_3_ARGUMENTS);

				/* Write the position to screen */
				upload = true;
				break;
			case GPSLockSearching:
				render_smart_string("Searching...\0", 13, 0, 0, DRAW_SET_F | DRAW_ALIGN_LEFT,
						RENDER_ENDING_3_ARGUMENTS);
				break;
			case GPSLockUnknown:
			default:
				render_smart_string("GPS Error\0", 9, 0, 0, DRAW_SET_F | DRAW_ALIGN_LEFT,
						RENDER_ENDING_3_ARGUMENTS);
				break;
			}

			top = (LINES_ON_SCREEN / 2) + ((__char_bottom_97 - __char_top_97) / 2) - __char_bottom_97;
			top = render_smart_string(string_to_disp, LEN_STR_2_DISP, top, (DISPLAY_WIDTH_PIXELS / 2), DRAW_SET_F | DRAW_ALIGN_CENTER,
					RENDER_ENDING_3_ARGUMENTS);

			for(i = 0; i < LEN_STR_2_DISP; i++)
				string_to_disp[i] = 0x00;

			generate_gps_string_UTC(_ui_gps_dat, &string_to_disp[0], LEN_STR_2_DISP);
			string_to_disp[5] = ' ';
			generate_gps_string_DATE(_ui_gps_dat, &string_to_disp[6], (LEN_STR_2_DISP - 6));
			render_smart_string(string_to_disp, 14, top, (DISPLAY_WIDTH_PIXELS / 2), DRAW_SET_F | DRAW_ALIGN_CENTER,
					RENDER_ENDING_3_ARGUMENTS);

			if(upload)
				try_upload_screen();
			else{
				render_disable_rectangle_OR(0,0,DISPLAY_WIDTH_PIXELS, DISPLAY_HEIGHT_PIXELS, RENDER_ENDING_3_ARGUMENTS);

				if(alternate)
					render_rectangle(0, 0, DISPLAY_WIDTH_PIXELS, DISPLAY_HEIGHT_PIXELS, DRAW_SET_TOGGLE, RENDER_ENDING_3_ARGUMENTS);
				try_upload_screen();
			}

			/* Log the time */
			last_tick_refreshed = xTaskGetTickCount();
			alternate = !alternate;
		}

		/* Have this task run like it's a timer */
	}//for
}//func


