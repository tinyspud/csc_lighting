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
#include "GraphArea.h"

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

	RenderGraph();

	//	render_line_solid(40, 26, 20, 40, DRAW_SET_F, RENDER_ENDING_3_ARGUMENTS);
	//
	//	LineDrawingStyle_t style = LINE_DRAWING_STYLE_DOT;
	//	render_line(0, 0, 50, 5, DRAW_SET_F, &style, RENDER_ENDING_3_ARGUMENTS);
	//	style = LINE_DRAWING_STYLE_DOT;
	//	render_line(0, 1, 50, 6, DRAW_SET_F, &style, RENDER_ENDING_3_ARGUMENTS);
	//	style = LINE_DRAWING_STYLE_DOT;
	//	render_line(0, 2, 50, 7, DRAW_SET_F, &style, RENDER_ENDING_3_ARGUMENTS);
	//
	//	style = LINE_DRAWING_STYLE_DASH;
	//	render_line(0, 4, 50, 9, DRAW_SET_F, &style, RENDER_ENDING_3_ARGUMENTS);
	//	style = LINE_DRAWING_STYLE_DASH;
	//	render_line(0, 5, 50, 10, DRAW_SET_F, &style, RENDER_ENDING_3_ARGUMENTS);
	//	style = LINE_DRAWING_STYLE_DASH;
	//	render_line(0, 6, 50, 11, DRAW_SET_F, &style, RENDER_ENDING_3_ARGUMENTS);
	//
	//	style = LINE_DRAWING_STYLE_LONG;
	//	render_line(0, 8, 50, 13, DRAW_SET_F, &style, RENDER_ENDING_3_ARGUMENTS);
	//	style = LINE_DRAWING_STYLE_LONG;
	//	render_line(0, 9, 50, 14, DRAW_SET_F, &style, RENDER_ENDING_3_ARGUMENTS);
	//	style = LINE_DRAWING_STYLE_LONG;
	//	render_line(0, 10, 50, 15, DRAW_SET_F, &style, RENDER_ENDING_3_ARGUMENTS);
	//
	//	style = LINE_DRAWING_STYLE_SOLID;
	//	render_line(0, 12, 50, 17, DRAW_SET_F, &style, RENDER_ENDING_3_ARGUMENTS);
	//	style = LINE_DRAWING_STYLE_SOLID;
	//	render_line(0, 13, 50, 18, DRAW_SET_F, &style, RENDER_ENDING_3_ARGUMENTS);
	//	style = LINE_DRAWING_STYLE_SOLID;
	//	render_line(0, 14, 50, 19, DRAW_SET_F, &style, RENDER_ENDING_3_ARGUMENTS);

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

			if(!upload)
				render_disable_rectangle_OR(0,0,DISPLAY_WIDTH_PIXELS, DISPLAY_HEIGHT_PIXELS, RENDER_ENDING_3_ARGUMENTS);
			try_upload_screen();

			/* Log the time */
			last_tick_refreshed = xTaskGetTickCount();
			alternate = !alternate;
		}

		/* Have this task run like it's a timer */
	}//for
}//func


