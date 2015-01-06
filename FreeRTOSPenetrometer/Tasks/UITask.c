#include "DisplayTask.h"
#include "LogTask.h"
#include "mcs_time.h"
#include "UITask.h"
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
static uint8_t string_to_disp[LEN_STR_2_DISP] = { 0 };

void ui_task( void* p_params )
{
	TickType_t curtick = 0,last_tick_refreshed = 0;

	int top = 0, i = 0;

	TickType_t waittime = UI_DELAY_SECONDS;

	for (;;){
		/* For now just put the tick up on the screen */
		curtick = xTaskGetTickCount();
		if(((curtick > (last_tick_refreshed + (waittime * SYS_TICKS_IN_1_SEC))) && (can_write_to_display())) ||
				(ready_for_first_one())){
			clear_scratch_screen();

			/* Clear out string to display */
			for(i = 0; i < LEN_STR_2_DISP; i++)
				string_to_disp[i] = 0x00;

			/* Convert the tick counter value to a seconds then a number */
			get_str_int((curtick / SYS_TICKS_IN_1_SEC), &string_to_disp[0], LEN_STR_2_DISP);


			top = (LINES_ON_SCREEN / 2) + ((__char_bottom_97 - __char_top_97) / 2) - __char_bottom_97;
			top = render_smart_string(string_to_disp, LEN_STR_2_DISP, top, (DISPLAY_WIDTH_PIXELS / 2), DRAW_SET_F | DRAW_ALIGN_CENTER,
					scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);

			/* Set note in top left about GPS */
			switch(GetCurrentGPSLockState()){
			case GPSLockLocked:
				top = render_smart_string("Locked\0", 13, 0, 0, DRAW_SET_F | DRAW_ALIGN_LEFT,
						scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
				break;
			case GPSLockSearching:
				top = render_smart_string("Searching...\0", 13, 0, 0, DRAW_SET_F | DRAW_ALIGN_LEFT,
						scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
				break;
			case GPSLockUnknown:
			default:
				top = render_smart_string("GPS Error\0", 13, 0, 0, DRAW_SET_F | DRAW_ALIGN_LEFT,
						scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);
				break;
			}

			try_upload_screen();

			/* Log the time */
			last_tick_refreshed = xTaskGetTickCount();
		}

		/* Have this task run like it's a timer */
	}//for
}//func


