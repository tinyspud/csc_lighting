
#include "hal_stdtypes.h"
#include "common.h"
#include "Scheduling.h"



static uint32_t ms_count;
void InitScheduler(){
	/* Initialize the unit that counts the miliseconds since the device has been turend on */
	ms_count = 0;

}

void TickScheduler(){
	ms_count++;
}


portBaseType xTimerChangePeriod( timerHandleType xTimer,
		portTickType xNewPeriod,
		portTickType xBlockTime )
{
	return 0;
}


portBaseType xTimerStart( timerHandleType xTimer, portTickType xBlockTime ){
	return 0;
}

portBaseType xTimerReset( timerHandleType xTimer, portTickType xBlockTime )
{
	//portBaseType xReturn;
	//portBaseType xRunningPrivileged;
	//
	//	xRunningPrivileged = portRAISE_PRIVILEGE( );
	//
	//	xReturn = xTimerReset( xTimer, xBlockTime );
	//
	//	/* Was Privilege Level Raised for Wrapper Function? */
	//	if( xRunningPrivileged != pdTRUE )
	//	{
	//		/* Yes, Lower Privilege Level Back to Original Level */
	//		portLOWER_PRIVILEGE( );
	//	}
	//
	//	return xReturn;
	return 0;
}

portBaseType xTimerStop( timerHandleType xTimer, portTickType xBlockTime )
{
	//portBaseType xReturn;
	//portBaseType xRunningPrivileged;
	//
	//	xRunningPrivileged = portRAISE_PRIVILEGE( );
	//
	//	xReturn = xTimerStop( xTimer, xBlockTime );
	//
	//	/* Was Privilege Level Raised for Wrapper Function? */
	//	if( xRunningPrivileged != pdTRUE )
	//	{
	//		/* Yes, Lower Privilege Level Back to Original Level */
	//		portLOWER_PRIVILEGE( );
	//	}
	//
	//	return xReturn;
	return 0;
}


portBaseType xTimerCreate( const portCharType *pcTimerName,
		portTickType xTimerPeriodInTicks,
		portBaseType xIsPeriodic,
		portBaseType xTimerID,
		timerControlBlockType *pxNewTimer,
		timerCallbackFunctionPtrType pxCallbackFunction,
		timerHandleType *pxTimerHandle,
		timerInstanceParametersType *pxTimerInstance )
{
	return 0;
}
