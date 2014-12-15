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
static SemaphoreHandle_t bsemaphores[NUM_BSEMAPHORES];

/* Binary semaphores function */
BaseType_t xCreateBinarySemaphores( void )
{
	BaseType_t xCreateResult = pdFAIL;
	unsigned int bsemaphore_index = 0;

	for (bsemaphore_index=0; bsemaphore_index < NUM_BSEMAPHORES; ++bsemaphore_index)
	{
		/* Initialize value to 0 */
		bsemaphores[bsemaphore_index] = NULL;

		/* Create the semaphore */
		bsemaphores[bsemaphore_index] = xSemaphoreCreateBinary();

		/* Check to see if semaphore was created */
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
	return (bsemaphore_id < NUM_BSEMAPHORES) ? bsemaphores[bsemaphore_id] : NULL;
}

BaseType_t take_bsemaphores(unsigned bsemaphore_id, portTickType send_delay)
{
	return (bsemaphores_get_semaphore_handle( bsemaphore_id ) == NULL) ? pdFAIL : xSemaphoreTake(bsemaphores_get_semaphore_handle( bsemaphore_id ), send_delay);
}

BaseType_t give_bsemaphores(unsigned bsemaphore_id)
{
	return (bsemaphores_get_semaphore_handle( bsemaphore_id ) == NULL) ? pdFAIL : xSemaphoreGive(bsemaphores_get_semaphore_handle( bsemaphore_id ));
}





