#ifndef __H_DRAWINGhANDLER
#define __H_DRAWINGhANDLER

boolean can_write_to_display(void);

boolean can_recieve_input(void);

static volatile boolean gc_is_display_busy;


void init_drawing_handler();
void write_to_screen();
void execute_drawing_handler();

#endif
