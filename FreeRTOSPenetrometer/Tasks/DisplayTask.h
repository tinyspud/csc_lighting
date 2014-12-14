/*
 * DisplayTask.h
 *
 *  Created on: Feb 3, '14
 *      Author: C Chock
 *
 *
 * This task is responsible for handling the screen resource.
 */

#ifndef DISPTASK_H_
#define DISPTASK_H_

/* SafeRTOS Includes */
#include "FreeRTOS.h"
#include "hal_stdtypes.h"
#include <stdint.h>

/* UI task callback */
void display_task(void*);

boolean can_write_to_display(void);

boolean can_recieve_input(void);

int __display_load_state;
int __display_load_attempts;
boolean __display_is_confirmed;
boolean __display_is_initialized;


static volatile boolean gc_is_display_busy;

void set_emcy_msg(boolean, char*);


#endif /* UITASK_H_ */

