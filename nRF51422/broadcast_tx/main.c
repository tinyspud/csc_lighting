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
#include "bsp.h"

#include "LEDs.h"
#include "slld_hal.h"
#include "FlashApp.h"
#include "system_error_callbacks.h"
#include "S210_LL.h"
#include "ADCSampling.h"

/**@brief Function for application main entry. Does not return.
 */
int main(void)
{
	/* Very first thing to try to init are the LEDs so you can blink out error
	 * codes */
	init_LEDs();

	/* Init the S210 low level drivers (soft device) */
	init_S210_LL();

	/* Init the SPI Master */
	spi_EEPROM_init();

	/* Start the flash app */
	init_flash_app();

	/* Init the ADC */
	init_strain_ADC();
	
	/* Init the temperature */
	int32_t volatile temp = 1;
	nrf_temp_init();
	
	system_ADC_LL_init();

	NRF_TEMP->TASKS_START = 1; /** Start the temperature measurement. */

	/* Busy wait while temperature measurement is not finished, you can skip waiting if you enable interrupt for DATARDY event and read the result in the interrupt. */
	/*lint -e{845} // A zero has been given as right argument to operator '|'" */
	while (NRF_TEMP->EVENTS_DATARDY == 0)
	{
		// Do nothing.
	}
	NRF_TEMP->EVENTS_DATARDY = 0;

	/**@note Workaround for PAN_028 rev2.0A anomaly 29 - TEMP: Stop task clears the TEMP register. */
	temp = (nrf_temp_read() / 4);

	/**@note Workaround for PAN_028 rev2.0A anomaly 30 - TEMP: Temp module analog front end does not power down when DATARDY event occurs. */
	NRF_TEMP->TASKS_STOP = 1; /** Stop the temperature measurement. */

	uint32_t err_code = 0;
	LED_TURN_ON(LED_GREEN);
	
	// Main loop. 
	for (;;)
	{
		// Put CPU in sleep if possible. 
		err_code = sd_app_evt_wait();
		APP_ERROR_CHECK(err_code);
		

		// Extract and process all pending ANT events as long as there are any left. 
		do
		{
			LED_TURN_ON(LED_ORANGE);
			// Fetch the event. 
			err_code = handle_ANT_events();
		} while (err_code == NRF_SUCCESS);
		LED_TURN_OFF(LED_ORANGE);
	}
}

/**
 *@}
 **/
