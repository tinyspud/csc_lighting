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
#define NUM_BSEMAPHORES						(5)

/* Binary Semaphore id */
#define UARTMUX_BSEMAPHORE_ID				(0)
#define MCS_CLOCK_BSEMAPHORE_ID				(1)
#define COM_BSEMAPHORE_ID					(2)
#define ALFAT_BSEMAPHORE_ID					(3)
#define SPI1_BSEMAPHORE_ID					(4)

BaseType_t xCreateBinarySemaphores( void );
SemaphoreHandle_t bsemaphores_get_semaphore_handle( unsigned bsemaphore_id );
BaseType_t take_bsemaphores(unsigned bsemaphore_id, portTickType send_delay);
BaseType_t give_bsemaphores(unsigned bsemaphore_id);
#endif /* SEMPAHORES_H_ */
