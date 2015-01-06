/*
 * GPSTask.c
 *
 *  Created on: Jan 17, 2014
 *      Author: rsnoo_000
 */

/* SafeRTOS Includes */
#include "GPSTask.h"
#include "LogTask.h"
#include "mcs_time.h"
#include "common.h"

//#include <stdint.h>
//#include <string.h>
//#include <stdio.h>



//#ifdef GPS_MODULE
#include "gps.h"
//#endif

//static const uint8_t instr[69] ={'$','G','P','R','M','C',',',
//							'0','5','3','7','4','0','.','0','0','0',',',
//							'A',',',
//							'2','5','0','3','.','6','3','1','9',',',
//							'N',',',
//							'1','2','1','3','6','.','0','0','9','9',',',
//							'E',',',
//							'2','.','6','9',',',
//							'7','9','.','6','5',',',
//							'1','0','0','1','0','6',',',',',',',
//							'A','*','5','3'
//							};


/* how often to poll GPS for coords in sec */
#define GPS_POLL_TIME_SEC 				(30)
#define GPS_RX_TIMEOUT_SEC				(5)





void gps_task( void* p_params )
{
//	static uint32_t gps_msg_received = 0;
	static GPSDATA_S rmc_msg = {0};
//	GPSDATA_S rmc_msg = {0};
//
//	portTickType  xPollLast;
//	portTickType  xPollRate;
//

	/* Init GPS */
	init_gps();

	/* TODO Start looking for a position */
	/* Query GPS for data */
	send_nmea_query_msg(RMC);
	receive_gps_response(&rmc_msg, (GPS_RX_TIMEOUT_SEC * SYS_TICKS_IN_1_SEC), 0);

	TickType_t _gps_now = xTaskGetTickCount();
	TickType_t _gps_delay_amount_sec = GPS_POLL_TIME_SEC;	// seconds

	for (;;){
		/* Every GPS_POLL_TIME_SEC, poll the GPS then update the UI */
		send_nmea_query_msg(RMC);
		receive_gps_response(&rmc_msg, (GPS_RX_TIMEOUT_SEC * SYS_TICKS_IN_1_SEC), 0);

		clean_up_lock();

		_gps_now = xTaskGetTickCount();
		vTaskDelayUntil(&_gps_now, (_gps_delay_amount_sec * SYS_TICKS_IN_1_SEC));


	}//for
}//func



