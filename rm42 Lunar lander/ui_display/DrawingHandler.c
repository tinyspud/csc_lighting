#include "eink.h"
#include "DrawingHandler.h"

int __display_load_state;
int __display_load_attempts;
boolean __display_is_confirmed;
boolean __display_is_initialized;

static int write_attempts;

void init_drawing_handler(){
	write_attempts = 0;

	__display_load_attempts = 0;
	__display_is_confirmed = false;
	__display_is_initialized = false;
	__display_load_state = EINK_UNINIT;
}

void write_to_screen(){
	gc_is_display_busy = true;
	write_attempts = 1;
}

void execute_drawing_handler(){
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
			break;
		default:
			break;
		}
	}
}
