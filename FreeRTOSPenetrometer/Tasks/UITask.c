#include "DisplayTask.h"
#include "LogTask.h"
#include "uart_mux.h"
#include "mcs_time.h"
#include "UITask.h"
#include "common.h"

#include "gps.h"

#include "eink.h"
#include "vis.h"
#include "menu_display.h"
#include "render.h"


static einkstate_t displaystate = EinkUninitialized;

static volatile boolean l_is_emcy = false;
static volatile boolean l_can_press = true;



void ui_task( void* p_params )
{
	TickType_t curtick = 0;

	int top = 0;

	for (;;){
		/* For now just put the tick up on the screen */
		if((xTaskGetTickCount() > (curtick + (5 * SYS_TICKS_IN_1_SEC))) && (can_write_to_display())){
			/* Log the time */
			curtick = xTaskGetTickCount();

			clear_scratch_screen();

			top = (LINES_ON_SCREEN / 2) + ((__char_bottom_97 - __char_top_97) / 2) - __char_bottom_97;
			top = render_smart_string("Hi!\0", 4, top, (DISPLAY_WIDTH_PIXELS / 2), DRAW_SET_F | DRAW_ALIGN_CENTER,
					scratch_screen, LINES_ON_SCREEN, BYTES_IN_1_LINE);

			try_upload_screen();
		}

		/* Have this task run like it's a timer */
	}//for
}//func


