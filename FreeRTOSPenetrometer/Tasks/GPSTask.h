/*
 * GPSTask.h
 *
 *  Created on: Jan 17, 2014
 *      Author: rsnoo_000
 */

#ifndef GPSTASK_H_
#define GPSTASK_H_

/* SafeRTOS Includes */
#include "FreeRTOS.h"
#include <stdint.h>
#include "gps.h"



/* GPS task callback */
void gps_task( void* p_params );

void GetCurrGPS(GPSDATA_S* target);

#endif /* GPSTASK_H_ */
