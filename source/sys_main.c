/** @file sys_main.c
*   @brief Application main file
*   @date 25.July.2013
*   @version 03.06.00
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* (c) Texas Instruments 2009-2013, All rights reserved. */

/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Include Files */

#include "sys_common.h"
#include "system.h"

/* USER CODE BEGIN (1) */
#include "sci.h"
#include "gio.h"
#include "spi.h"
#include "het.h"
#include "eink.h"
#include "rti.h"
#include "sys_vim.h"


#include "Scheduling.h"
#include "DrawingHandler.h"
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
/* USER CODE END */

void main(void)
{
/* USER CODE BEGIN (3) */
	/* Init low level drivers */
	/* Init SPI */
	spiInit();

	/* Init GIO */
	gioInit();

	/* Init HET */
	hetInit();

	/* Init SCI */
	sciInit();

	/* Init RTI */
	rtiInit();

	/* Init VIM */
	vimInit();

	/* Initialize the scheduler */
	InitScheduler();

	/* Init the higher level items */
	/* Init EInk */
	init_display_buffers();

	/* Enable interrupts */
	_enable_IRQ();

	/* Enable notification and start 1ms timer */
	rtiEnableNotification(rtiNOTIFICATION_COMPARE0);
	rtiStartCounter(rtiCOUNTER_BLOCK0);

	/* Init the drawing handler (in lieu of display task) */
	init_drawing_handler();

	blackout_screen();

	write_to_screen();

	/* Execute main while loop */
	while(1){
		execute_drawing_handler();



	}

	/* USER CODE END */
}

/* USER CODE BEGIN (4) */
/* USER CODE END */
