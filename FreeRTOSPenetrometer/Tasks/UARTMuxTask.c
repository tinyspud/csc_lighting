/*
 * UARTMuxTask.c
 *
 *  Created on: Jan 23, 2014
 *      Author: rsnoo_000
 */
/* SafeRTOS Includes */
#include "FreeRTOS.h"
#include "UARTMuxTask.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>

void uart_mux_task( void* p_params )
{
	static uint32_t uart_mux_msg_received = 0;

	for (;;)
	{
		/* Send task check-in message */
		/* Wait for message response from other tasks */
	}
}





