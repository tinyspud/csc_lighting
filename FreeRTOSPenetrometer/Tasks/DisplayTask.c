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
/* Forward prototypes */
void clear_emergency();
void emcy();

static volatile boolean l_is_emcy = false;
static volatile boolean l_can_press = true;


void display_task( void* p_params )
{
	static uint32_t disp_msg_received = 0;

	static int write_attempts = 0;

	/* Do a broken start for the display */
	/* Display will be updated by the timer in terms of resetting display
	 * pointer, loading rows then updating the screen.  The init will be used
	 *  */
	__display_load_attempts = 0;
	__display_is_confirmed = false;
	__display_is_initialized = false;
	__display_load_state = EINK_UNINIT;

	/* Init the display buffers (they're very large) */
	init_display_buffers_and_pins();
	gc_is_display_busy = false;

	l_can_press = false;

	/* Wait from the time that the RTOS started 5 ms before you put the display into reset */
	epaper_power_on_sequence();

	/* Start the epaper COG driver */
	epaper_start_COG_driver();

	for (;;){
		/* Look at who's calling so you know where to give the response */

		/* TODO put lock in */
		/* Update the display */
		if(!gc_is_display_busy){
			gc_is_display_busy = true;
		}
		write_attempts++;
		/* Have this task run like it's a timer */
		if(gc_is_display_busy && (write_attempts > 0)){
			__display_load_state = manage_eink(__display_load_state);

			if((!__display_is_confirmed) || (!__display_is_initialized)){
				/* Confirm the display - check the state to see if it's
				 * passed the uninitialized state*/
				if(__display_load_state > EINK_UNINIT)
					__display_is_confirmed = true;
				else
				{
					if(__display_load_state > EINK_GETDEV){
						__display_is_initialized = true;
					}
					else{
						__display_load_attempts++;
					}
				}
			}

			/* Stop conditions for the display timer */
			switch(__display_load_state){
			case EINK_LOADED:
				gc_is_display_busy = false;
				write_attempts = 0;
				l_can_press = true;
				break;
			default:
				l_can_press = false;
				break;
			}
		}
	}//for
}//func

/* Return flag that says whether or not it is safe to write to scratch_screen
 * or do any associated operations */
boolean can_write_to_display(){
	return (!gc_is_display_busy) && (!l_is_emcy);
}

boolean can_recieve_input(){
	/* TODO implement */
	return can_write_to_display();
}

