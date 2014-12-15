/*
 * Sempahores.h
 *
 *  Created on: Jan 26, 2014
 *      Author: swoosh
 */

#ifndef SEMPAHORES_H_
#define SEMPAHORES_H_

#include "FreeRTOS.h"
#include "os_semphr.h"

/* Number of semaphores */
#define NUM_BSEMAPHORES						(4)

/* Binary Semaphore id */
#define UARTMUX_BSEMAPHORE_ID				(0)
#define MCS_CLOCK_BSEMAPHORE_ID				(1)
#define COM_BSEMAPHORE_ID					(2)
#define ALFAT_BSEMAPHORE_ID					(3)

portBASE_TYPE xCreateBinarySemaphores( void );
SemaphoreHandle_t bsemaphores_get_semaphore_handle( unsigned bsemaphore_id );
portBASE_TYPE take_bsemaphores(unsigned bsemaphore_id, portTickType send_delay);
portBASE_TYPE give_bsemaphores(unsigned bsemaphore_id);
#endif /* SEMPAHORES_H_ */
