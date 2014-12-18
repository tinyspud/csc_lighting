/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
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

/** @file
*
* @defgroup blinky_example_main main.c
* @{
* @ingroup blinky_example
* @brief Blinky Example Application main file.
*
*/

#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "common.h"
#include "app_error.h"
#include "app_util_platform.h"
#include "boards.h"

#include "spi_master.h"
#include "slld_hal.h"




void init_LEDs(){
		// Configure LED-pins as outputs.
	nrf_gpio_cfg_output(LED_RED);
	nrf_gpio_cfg_output(LED_GREEN);
	nrf_gpio_cfg_output(LED_YELLOW);
	nrf_gpio_cfg_output(LED_ORANGE);

		LED_TURN_OFF(LED_RED);
	LED_TURN_OFF(LED_GREEN);
	LED_TURN_OFF(LED_YELLOW);
	LED_TURN_OFF(LED_ORANGE);

}



/**
* @brief Function for application main entry.
*/
int main(void)
{
	init_LEDs();
	
	spi_EEPROM_init();

	LED_TURN_ON(LED_GREEN);
	// LED 0 and LED 1 blink alternately.
	while (true)
	{

	}
}
/** @} */
