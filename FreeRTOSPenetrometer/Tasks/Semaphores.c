/*
 * Semaphores.c
 *
 *  Created on: Jan 26, 2014
 *      Author: Harvey Ku
 */
#include "FreeRTOS.h"
//#include "semaphore.h"
#include "os_semphr.h"
#include "Semaphores.h"

/* Array of semaphore handles */
static SemaphoreHandle_t bsemaphores[NUM_BSEMAPHORES] =
{
		/* UARTMUX binary semaphore */
		NULL,

		/* MCS CLOCK binary semaphore */
		NULL,

		/* COM binary semaphore */
		NULL,

		/* ALFAT read/write (access) binary semaphore */
		NULL,

};


/* Binary semaphores function */
portBASE_TYPE xCreateBinarySemaphores( void )
{
	portBASE_TYPE xCreateResult = pdFAIL;
	unsigned int bsemaphore_index = 0;

	for (bsemaphore_index=0; bsemaphore_index < NUM_BSEMAPHORES; ++bsemaphore_index)
	{
		bsemaphores[bsemaphore_index] = xSemaphoreCreateBinary();
		if(bsemaphores[bsemaphore_index] == NULL)
		{
			/* Sempahore creation failed */
			xCreateResult = pdFAIL;
		}
	}

	return xCreateResult;
}


/* Get RTOS queue handle for a specified queue index */
SemaphoreHandle_t bsemaphores_get_semaphore_handle( unsigned bsemaphore_id )
{
	if (bsemaphore_id < NUM_BSEMAPHORES)
	{
		return bsemaphores[bsemaphore_id];
	}

	return NULL;
}

portBASE_TYPE take_bsemaphores(unsigned bsemaphore_id, portTickType send_delay)
{
	SemaphoreHandle_t bsemaphore;


	bsemaphore = bsemaphores_get_semaphore_handle( bsemaphore_id );

	if (bsemaphore == NULL)
	{
		return pdFAIL;
	}


	return xSemaphoreTake(bsemaphore, send_delay);
}

portBASE_TYPE give_bsemaphores(unsigned bsemaphore_id)
{
	SemaphoreHandle_t bsemaphore;


	bsemaphore = bsemaphores_get_semaphore_handle( bsemaphore_id );

	if (bsemaphore == NULL)
	{
		return pdFAIL;
	}


	return xSemaphoreGive(bsemaphore);
}





