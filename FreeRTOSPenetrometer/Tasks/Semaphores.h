/*
 * Sempahores.h
 *
 *  Created on: Jan 26, 2014
 *      Author: swoosh
 */

#ifndef SEMPAHORES_H_
#define SEMPAHORES_H_

/* Number of semaphores */
#define NUM_BSEMAPHORES						(4)

/* Binary Semaphore id */
#define UARTMUX_BSEMAPHORE_ID				(0)
#define MCS_CLOCK_BSEMAPHORE_ID				(1)
#define COM_BSEMAPHORE_ID					(2)
#define ALFAT_BSEMAPHORE_ID					(3)


#define DEFAULT_BSEMAPHORE_DELAY			portMAX_DELAY

//portBaseType xCreateBinarySemaphores( void );
//portBaseType take_bsemaphores(unsigned bsemaphore_id, portTickType send_delay);
//portBaseType give_bsemaphores(unsigned bsemaphore_id);
//

#endif /* SEMPAHORES_H_ */
