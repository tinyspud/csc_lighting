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

#define UI_DELAY_SECONDS	20

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

	while(!ready_for_first_one()){ };

	/* Flush 20 times */
	clear_scratch_screen();

	blackout_screen();
	render_disable_rectangle(0, 0, DISPLAY_WIDTH_PIXELS, DISPLAY_HEIGHT_PIXELS,
			scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
	for(i = 0; i < 20; i++){
		/* Write thatched screen */
		render_rectangle(0, 0, DISPLAY_WIDTH_PIXELS, DISPLAY_HEIGHT_PIXELS, DRAW_SET_TOGGLE,
				scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);

		try_upload_screen();
		vTaskDelay(5 * SYS_TICKS_IN_1_SEC);
	}

	for (;;){
		/* For now just put the tick up on the screen */
		curtick = xTaskGetTickCount();
		if((curtick > (last_tick_refreshed + (waittime * SYS_TICKS_IN_1_SEC))) && (can_write_to_display())){
			clear_scratch_screen();

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
						scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);

				/* Write the position to screen */
				break;
			case GPSLockSearching:
				render_smart_string("Searching...\0", 13, 0, 0, DRAW_SET_F | DRAW_ALIGN_LEFT,
						scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
				break;
			case GPSLockUnknown:
			default:
				render_smart_string("GPS Error\0", 9, 0, 0, DRAW_SET_F | DRAW_ALIGN_LEFT,
						scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
				break;
			}

			top = (LINES_ON_SCREEN / 2) + ((__char_bottom_97 - __char_top_97) / 2) - __char_bottom_97;
			top = render_smart_string(string_to_disp, LEN_STR_2_DISP, top, (DISPLAY_WIDTH_PIXELS / 2), DRAW_SET_F | DRAW_ALIGN_CENTER,
					scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);

			for(i = 0; i < LEN_STR_2_DISP; i++)
				string_to_disp[i] = 0x00;

			generate_gps_string_UTC(_ui_gps_dat, &string_to_disp[0], LEN_STR_2_DISP);
			string_to_disp[5] = ' ';
			generate_gps_string_DATE(_ui_gps_dat, &string_to_disp[6], (LEN_STR_2_DISP - 6));
			render_smart_string(string_to_disp, 14, top, (DISPLAY_WIDTH_PIXELS / 2), DRAW_SET_F | DRAW_ALIGN_CENTER,
					scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);

			try_upload_screen();

			/* Log the time */
			last_tick_refreshed = xTaskGetTickCount();
			alternate = !alternate;
		}

		/* Have this task run like it's a timer */
	}//for
}//func


