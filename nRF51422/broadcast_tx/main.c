/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/**@file
 * @defgroup nrf_ant_broadcast_tx_example ANT Broadcast TX Example
 * @{
 * @ingroup nrf_ant_broadcast
 *
 * @brief Example of basic ANT Broadcast TX.
 */

#include <stdbool.h>
#include <stdint.h>
#include "app_error.h"
#include "nrf.h"
#include "ant_interface.h"
#include "ant_parameters.h"
#include "nrf_soc.h"
#include "nrf_sdm.h"
#include "app_timer.h"
#include "nrf_delay.h"
#include "nrf_temp.h"
#include "nordic_common.h"
#include "custom_board.h"

#include "system_timer.h"
#include "LEDs.h"
#include "system_error_callbacks.h"
#include "S210_LL.h" 
#include "twi_master_config.h"
#include "twi_master.h"
#include "LightSensor.h"

/**@brief Function for application main entry. Does not return.
 */
int main(void)
{
	/* Very first thing to try to init are the LEDs so you can blink out error
	 * codes */
	init_LEDs();
/* S210 SOFTDEVICE_PRESENT */
#ifdef SOFTDEVICE_PRESENT
	/* Init the S210 low level drivers (soft device) */
	init_S210_LL();
#endif
	
	/* Init the light sensor */
	//	init_light_sensor();

	/* Init the timer */
	system_timer_init();

	/* Init the temperature */
	nrf_temp_init();
	
	uint32_t err_code = 0;

	/* Start the system timer */
	start_system_timer();
	
	/* Start the LED timer */
	start_LED_timer();

//	while(1){}

	LightShow();


#ifdef SOFTDEVICE_PRESENT
	/* Open the ANT channel 0 */
	open_channel_0();
#endif

// Main loop. 
	for (;;)
	{
		/* Sort and check function pointers */
		SortFunctionPointers();
		CheckAndServiceFunctionPointers();
		LED_TOGGLE(LED_YELLOW);
//		// Put CPU in sleep if possible. 
//		err_code = sd_app_evt_wait();
//		APP_ERROR_CHECK(err_code);
//		
		// Extract and process all pending ANT events as long as there are any left. 
		#ifdef SOFTDEVICE_PRESENT
		do
		{
			// Fetch the event. 
			err_code = handle_ANT_events();
		} while (err_code == NRF_SUCCESS);
		#endif
	}
}

/**
 *@}
 **/
