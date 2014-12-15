/** @file sys_main.c 
*   @brief Application main file
*   @date 17.Nov.2014
*   @version 04.02.00
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* 
* Copyright (C) 2009-2014 Texas Instruments Incorporated - http://www.ti.com/ 
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Include Files */

#include "sys_common.h"

/* USER CODE BEGIN (1) */
#include "FreeRTOS.h"
#include "os_task.h"

#include "os_timer.h"

#include "hal_stdtypes.h"
#include "adc.h"
#include "sci.h"
#include "gio.h"
#include "spi.h"
#include "het.h"
#include "eink.h"
#include "sys_vim.h"

#include "uart_mux.h"
#include "gps.h"
#include "mcs_time.h"

#include "Tasks.h"
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
/* Forward prototype */
void init_hardware();

/* USER CODE END */

void main(void)
{
/* USER CODE BEGIN (3) */
	/* Init the RTOS */

	/* Init the hardware involved */
	init_hardware();

	/* Init the clock */
	init_system_clock();

	/* Init UART Mux */
	uartmux_sci_init();

	/* Create queues */
	xCreateBinarySemaphores( );

	/* Create Task 1 */
    if (xTaskCreate(
    		display_task,	/* Task code */
    		"DisplayTask",	/* Name */
    		2048,			/* Stack Depth */
    		NULL,			/* Parameters */
    		1 | portPRIVILEGE_BIT ,	/* Priority */
    		&xDisplayTaskHandle	/* Created task */
    		) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }

    /* Create Task 1 */
    if (xTaskCreate(
    		gps_task,
    		"GPSTask",
    		256,
    		NULL,
    		1 | portPRIVILEGE_BIT,
    		&xGPSTaskHandle
    		) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }

    /* Start Scheduler */
    vTaskStartScheduler();

    /* Run forever */
    while(1);
	/* USER CODE END */
}

/* USER CODE BEGIN (4) */
void init_hardware(){

	/* Init GIO */
	gioInit();

	/* Init SPI */
	spiInit();

	/* Init HET */
	hetInit();

	/* Init SCI */
	sciInit();

	/* Init VIM */
	vimInit();

	/* Init ADC */
	adcInit();

	/* Enable interrupts */
//	_enable_IRQ();
}

/* USER CODE END */