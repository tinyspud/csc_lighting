/*
 * Semaphores.c
 *
 *  Created on: Jan 26, 2014
 *      Author: Harvey Ku
 */
#include "FreeRTOS.h"
//#include "semaphore.h"
#include "Semaphores.h"

#pragma DATA_ALIGN( cUartMuxBSemaphoreBuffer,8)
portInt8Type cUartMuxBSemaphoreBuffer[portQUEUE_OVERHEAD_BYTES];
#pragma DATA_ALIGN( cMCSClockBSemaphoreBuffer,8)
portInt8Type cMCSClockBSemaphoreBuffer[portQUEUE_OVERHEAD_BYTES];
#pragma DATA_ALIGN( cCOMBSemaphoreBuffer,8)
portInt8Type cCOMBSemaphoreBuffer[portQUEUE_OVERHEAD_BYTES];
#pragma DATA_ALIGN( cALFATBSemaphoreBuffer,8)
portInt8Type cALFATBSemaphoreBuffer[portQUEUE_OVERHEAD_BYTES];

/* Binary Semaphore creation parameter structure */
typedef struct bsemaphores_parameters
{
	portCharType* 			pcbSemaphoreName;
	portInt8Type* 			pcbSemaphoreMemory;
	xSemaphoreHandle		handle;
} bsemaphores_parameters_t;


/* Array of queue creation parameter structures */
static bsemaphores_parameters_t bsemaphores_creation_parameters[NUM_BSEMAPHORES] =
{
		/* UARTMUX binary semaphore */
		{
			"UARTMUX_BSEMAPHORE",
			cUartMuxBSemaphoreBuffer,
			{0}
		},

		/* MCS CLOCK binary semaphore */
		{
			"MCS_CLOCK_BSEMAPHORE",
			cMCSClockBSemaphoreBuffer,
			{0}
		},

		/* COM binary semaphore */
		{
			"COM_BSEMAPHORE",
			cCOMBSemaphoreBuffer,
			{0}
		},

		/* ALFAT read/write (access) binary semaphore */
		{
			"ALFAT_BSEMAPHORE",
			cALFATBSemaphoreBuffer,
			{0}
		},

};


/* Binary semaphores function */
portBaseType xCreateBinarySemaphores( void )
{
	portBaseType xCreateResult;
	bsemaphores_parameters_t* p_bsemaphore = 0;
	unsigned int bsemaphore_index = 0;

	for (bsemaphore_index=0; bsemaphore_index < NUM_BSEMAPHORES; ++bsemaphore_index)
	{
		/* Pointer to queue creation data */
		p_bsemaphore = &(bsemaphores_creation_parameters[bsemaphore_index]);

		/* Binary Semaphore creation data validation */
		if (!p_bsemaphore)
		{
			xCreateResult = pdFAIL;
		}

		/* Create each semaphore */
		if ( xCreateResult = xSemaphoreCreateBinary( p_bsemaphore->pcbSemaphoreMemory,
									 	 	 	 	 &(p_bsemaphore->handle) ))
		{
			if (xCreateResult != pdPASS)
			{
				break;
			}
		}
	}

	return xCreateResult;
}


/* Get RTOS queue handle for a specified queue index */
xSemaphoreHandle bsemaphores_get_semaphore_handle( unsigned bsemaphore_id )
{
	if (bsemaphore_id < NUM_BSEMAPHORES)
	{
		return bsemaphores_creation_parameters[bsemaphore_id].handle;
	}

	return NULL;
}

portBaseType take_bsemaphores(unsigned bsemaphore_id, portTickType send_delay)
{
	xSemaphoreHandle bsemaphore;


	bsemaphore = bsemaphores_get_semaphore_handle( bsemaphore_id );

	if (bsemaphore == NULL)
	{
		return pdFAIL;
	}


	return xSemaphoreTake(bsemaphore, send_delay);
}

portBaseType give_bsemaphores(unsigned bsemaphore_id)
{
	xSemaphoreHandle bsemaphore;


	bsemaphore = bsemaphores_get_semaphore_handle( bsemaphore_id );

	if (bsemaphore == NULL)
	{
		return pdFAIL;
	}


	return xSemaphoreGive(bsemaphore);
}





