/*
 * UITask.h
 *
 *  Created on: Dec 15, '14
 *      Author: C Chock
 *
 *
 * This task is responsible for writing things to the screen, working with user
 * input and ...LEDs?
 */

#ifndef UITASK_H_
#define UITASK_H_

/* FreeRTOS Includes */
#include "FreeRTOS.h"
#include "hal_stdtypes.h"
#include <stdint.h>

/* UI task callback */
void ui_task(void*);

#endif /* UITASK_H_ */

