/*
 * mcs_time.c
 *
 *  Created on: Jan 29, 2014
 *      Author: swoosh
 */


#include "FreeRTOS.h"
#include <stdbool.h>
#include "mcs_time.h"
#include "Semaphores.h"
#include "GitParams.h"


static clock_struct_t g_mcs_clock;
//static timerHandleType xSystemClockTimer;
//static timerControlBlockType xSystemClockTimerBuffer;

uint32_t ticks_since_last_sync = 0;

///* GPS timer callback function */
//static void prvSystemClockTimerCallback(timerHandleType xExpiredTimer){
//	g_mcs_clock.secs++;
//
//	if(g_mcs_clock.secs >= 60){
//		g_mcs_clock.secs = 0;
//		g_mcs_clock.mins++;
//		if(g_mcs_clock.mins >= 60){
//			g_mcs_clock.mins = 0;
//			g_mcs_clock.hours++;
//			if(g_mcs_clock.hours >= 24){
//				g_mcs_clock.hours = 0;
//			}
//		}
//	}
//}

void get_system_clock(clock_struct_t* mcs_clock)
{
//	if (take_bsemaphores(MCS_CLOCK_BSEMAPHORE_ID, DEFAULT_BSEMAPHORE_DELAY) == pdPASS){
		mcs_clock->years = g_mcs_clock.years;
		mcs_clock->months = g_mcs_clock.months;
		mcs_clock->days = g_mcs_clock.days;
		mcs_clock->hours = g_mcs_clock.hours;
		mcs_clock->mins = g_mcs_clock.mins;
		mcs_clock->secs = g_mcs_clock.secs;

//		if(give_bsemaphores(MCS_CLOCK_BSEMAPHORE_ID)!= pdPASS){
//			printf_ui_uart("Can't give MCS clock BSEMAPHORE\r\n");
//		}
//	}
//	else{
//		printf_ui_uart("Can't take MCS clock BSEMAPHORE failed\r\n");
//	}

}

uint64_t GendotNETDateTime(clock_struct_t mcs_clock){
	uint64_t rtnval = DATETIME_MIN;

	rtnval = DATETIME_1984_01_01_MIDNIGHT +
			(DATETIME_1_DAY * ((uint64_t)get_days_since_jan_1_1984(mcs_clock))) +
			(DATETIME_TICKS_IN_1_MSEC * ((uint64_t)get_miliseconds_since_midnight(mcs_clock)));

	return ((rtnval & DATETIME_TICK_MASK) > DATETIME_MAX) ?
			(DATETIME_MIN) :
			(rtnval);
}

void set_system_clock(clock_struct_t mcs_clock)
{
//	if (take_bsemaphores(MCS_CLOCK_BSEMAPHORE_ID, DEFAULT_BSEMAPHORE_DELAY) == pdPASS){
		g_mcs_clock.years = mcs_clock.years;
		g_mcs_clock.months = mcs_clock.months;
		g_mcs_clock.days = mcs_clock.days;
		g_mcs_clock.hours = mcs_clock.hours;
		g_mcs_clock.mins = mcs_clock.mins;
		g_mcs_clock.secs = mcs_clock.secs;

//		if(give_bsemaphores(MCS_CLOCK_BSEMAPHORE_ID)!= pdPASS){
//			printf_ui_uart("Can't give MCS clock BSEMAPHORE\r\n");
//		}
//	}
//	else{
//		printf_ui_uart("Can't take MCS clock BSEMAPHORE failed\r\n");
//	}
}

uint32_t get_miliseconds_since_midnight(clock_struct_t mcs_clock){
	return mcs_clock.hours * 3600000 + mcs_clock.mins * 60000 + mcs_clock.secs * 1000;
}

bool _is_leap_year(uint16_t year){
	return (((year % 4) == 0) && (((year % 100) != 0))) || ((year % 400) == 0);
}

uint16_t _days_in_month(uint8_t month){
	switch(month){
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		return 31;
	case 2:
		return 28;
	case 4:
	case 6:
	case 9:
	case 11:
		return 30;
	default:
		return 0;
	}
}

uint16_t get_days_since_jan_1_1984(clock_struct_t mcs_clock){
	uint16_t rtnval = 0;
	uint8_t i_8 = 0;
	uint16_t i_16 = 0;

	for(i_16 = 1984; i_16 < mcs_clock.years; i_16++)
		rtnval += 365 + (_is_leap_year(i_16) ? 1 : 0);

	for(i_8 = 0; i_8 < mcs_clock.months; i_8++)
		rtnval += _days_in_month(i_8);

	rtnval += (mcs_clock.days - 1);

	return rtnval;
}

void vApplicationTickHook(void){

}

uint32_t init_system_clock(void)
{
	/* Create generic GPS timer */
//	if( xTimerCreate(
//			"System Clock Timer",			/* Unique name */
//			1000,							/* Period (# ticks) */
//			pdTRUE,							/* Auto reload timer? */
//			1,								/* Unique ID (so callback can link to several timers) */
//			&xSystemClockTimerBuffer,		/* Timer structure buffer (not sure yet) */
//			prvSystemClockTimerCallback,	/* Callback function (see above) */
//			&xSystemClockTimer,				/* timer handle */
//			NULL							/* pointer of timer instance to which this will belong - set to null for default */
//	) != pdPASS ){
//		return pdFAIL;
//	}
//
//	if (xTimerStart(xSystemClockTimer, portMAX_DELAY) == pdFAIL ) {
//		return pdFAIL;
//	}

	/* Init to compile time for now */
	g_mcs_clock.years = COMPILE_TIME_UTC_YYYY;
	g_mcs_clock.months = COMPILE_TIME_UTC_MM;
	g_mcs_clock.days = COMPILE_TIME_UTC_DD;
	g_mcs_clock.hours = COMPILE_TIME_UTC_HH;
	g_mcs_clock.mins = COMPILE_TIME_UTC_mm;
	g_mcs_clock.secs = COMPILE_TIME_UTC_ss;


	return pdPASS;
}


