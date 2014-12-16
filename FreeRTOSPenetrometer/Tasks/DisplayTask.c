#include "DisplayTask.h"
#include "LogTask.h"
#include "uart_mux.h"
#include "mcs_time.h"

#include "gps.h"

//#ifdef UI_DISPLAY
#include "eink.h"
#include "vis.h"
#include "menu_display.h"
#include "render.h"
//#endif


static char l_emcy_msg[EMCY_MSG_MAX_LENGTH];
static int l_emcy_msg_len = 0;

static einkstate_t displaystate = EinkUninitialized;

static volatile boolean l_is_emcy = false;
static volatile boolean l_can_press = true;


void display_task( void* p_params )
{
	/* Do a broken start for the display */
	/* Display will be updated by the timer in terms of resetting display
	 * pointer, loading rows then updating the screen.  The init will be used
	 *  */

	/* Init the display buffers (they're very large) */
	gc_is_display_busy = false;

	l_can_press = false;

	displaystate = EinkUninitialized;

//	/* White out the screen */
//	whiteout_screen();


	/* Someone else comes in and tells you the display is busy */
	/* First time around, it's you */
	gc_is_display_busy = true;

	for (;;){
		/* Have this task run like it's a timer */
		if(gc_is_display_busy){
			displaystate = manage_eink(displaystate);

			/* Stop conditions for the display timer */
			switch(displaystate){
			case EinkIdle:
				gc_is_display_busy = false;
				l_can_press = true;
				break;
			default:
				l_can_press = false;
				break;
			}
		}
	}//for
}//func

void try_upload_screen(){
	if(!gc_is_display_busy)
		gc_is_display_busy = true;
}

/* Return flag that says whether or not it is safe to write to scratch_screen
 * or do any associated operations */
boolean can_write_to_display(){
	return (!gc_is_display_busy);
}

boolean can_recieve_input(){
	/* TODO implement */
	return l_can_press;
}

