/*
	SafeRTOS Copyright (C) Wittenstein High Integrity Systems.

	See projdefs.h for version number information.

	SafeRTOS is distributed exclusively by Wittenstein High Integrity Systems,
	and is subject to the terms of the License granted to your organization,
	including its warranties and limitations on distribution.  It cannot be
	copied or reproduced in any way except as permitted by the License.

	Licenses are issued for each concurrent user working on a specified product
	line.

	WITTENSTEIN high integrity systems is a trading name of WITTENSTEIN
	aerospace & simulation ltd, Registered Office: Brown's Court, Long Ashton
	Business Park, Yanley Lane, Long Ashton, Bristol, BS41 9LB, UK.
	Tel: +44 (0) 1275 395 600, fax: +44 (0) 1275 393 630.
	E-mail: info@HighIntegritySystems.com
	Registered in England No. 3711047; VAT No. GB 729 1583 15

	http://www.SafeRTOS.com
*/

/* SafeRTOS includes. */
#include "FreeRTOS.h"


/* Borrowed so that the vectors load here */

/*-----------------------------------------------------------*/
/* Public Function Declarations.							 */
/*-----------------------------------------------------------*/
/* Abort error handlers, these can be generated by general program exceptions
and MPU faults. */
void vPrefetchAbort( void );
void vDataAbort( void );
void vUndefAbort( void );


void vApplicationIdleHook(void);

/*-----------------------------------------------------------*/
/* External Functions (declared in abortsasm.s).			 */
/*-----------------------------------------------------------*/
extern uint32_t ulGetDataFaultStatusRegister( void );
extern uint32_t ulGetDataFaultAddressRegister( void );
extern uint32_t ulGetInstructionFaultStatusRegister( void );
extern uint32_t ulGetInstructionFaultAddressRegister( void );

/*-----------------------------------------------------------*/
/* Public Function Definitions.								 */
/*-----------------------------------------------------------*/

void vDataAbort( void )
{
volatile uint32_t ulDataFaultStatusReg = 0;
volatile uint32_t ulDataFaultAddressReg = 0;

	ulDataFaultStatusReg = ulGetDataFaultStatusRegister();
	ulDataFaultAddressReg = ulGetDataFaultAddressRegister();

	for( ;; );
}
/*-----------------------------------------------------------*/

void vPrefetchAbort( void )
{
volatile uint32_t ulInstrFaultStatusReg = 0;
volatile uint32_t ulInstrFaultAddressReg = 0;

	ulInstrFaultStatusReg = ulGetInstructionFaultStatusRegister();
	ulInstrFaultAddressReg = ulGetInstructionFaultAddressRegister();

	for( ;; );
}
/*-----------------------------------------------------------*/

void vUndefAbort( void )
{
	for( ;; );
}
/*-----------------------------------------------------------*/


void vApplicationIdleHook(void){

}
