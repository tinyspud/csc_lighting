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

#include "gps.h"

/* how often to poll GPS for coords in sec */
#define GPS_POLL_TIME_SEC 				(30)
#define GPS_RX_TIMEOUT_SEC				(5)



static GPSDATA_S most_recent_rmc_msg = {0};
static GPSDATA_S last_good_rmc_msg = {0};


void gps_task( void* p_params )
{
	/* Init GPS */
	init_gps();

	invalidate_GPSDATA_S_data(&most_recent_rmc_msg);
	invalidate_GPSDATA_S_data(&last_good_rmc_msg);

	/* TODO Start looking for a position */
	/* Query GPS for data */
	send_nmea_query_msg(RMC);
	receive_gps_response(&most_recent_rmc_msg, (GPS_RX_TIMEOUT_SEC * SYS_TICKS_IN_1_SEC));

	TickType_t _gps_now = xTaskGetTickCount();
	TickType_t _gps_delay_amount_sec = GPS_POLL_TIME_SEC;	// seconds

	for (;;){
		/* Every GPS_POLL_TIME_SEC, poll the GPS then update the UI */
		send_nmea_query_msg(RMC);
		receive_gps_response(&most_recent_rmc_msg, (GPS_RX_TIMEOUT_SEC * SYS_TICKS_IN_1_SEC));

		clean_up_lock();

		/* Check to see if the most recent RMC is valid */
		if(is_gps_good(most_recent_rmc_msg))
			GetCurrGPS(&last_good_rmc_msg);

		_gps_now = xTaskGetTickCount();
		vTaskDelayUntil(&_gps_now, (_gps_delay_amount_sec * SYS_TICKS_IN_1_SEC));
	}//for
}//func

void GetCurrGPS(GPSDATA_S* target){
	copy_GPSDATA_S_data(target, &most_recent_rmc_msg);
}


