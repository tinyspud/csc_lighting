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
#include "slld.h"
#include "FlashApp.h"

#include "LEDs.h"

DEVSTATUS _status_register;

void init_flash_app(){
	_status_register = dev_status_unknown;
	int i = 0;
	uint8_t target[5] = { 0, 0, 0, 0, 0 };
	SLLD_STATUS _status = SLLD_ERROR;

	LED_TURN_ON(LED_YELLOW);
	
	
	/* Set CS# high */
	nrf_gpio_pin_set(EEPROM_CS_PIN);
	/* Set RST# high */
	nrf_gpio_pin_set(EEPROM_RST_PIN);
	/* Set WP# high */
	nrf_gpio_pin_set(EEPROM_WP_PIN);

	/* Set RST# low */
	nrf_gpio_pin_clear(EEPROM_RST_PIN);
	
	/* wait for at least 200 ns */
	nrf_delay_ms(1);

	/* Set RST# high */
	nrf_gpio_pin_set(EEPROM_RST_PIN);

	
	/* Reset device */
	_status = slld_SRSTCmd();
	
	/* wait for 100 ms just to make sure the flash is done... */
	nrf_delay_ms(100);
	/* Get the status */
	slld_StatusGet(&_status_register);
	if(_status_register == dev_not_busy)
	{
		LED_TURN_ON(LED_GREEN);
	}
	else
	{
		slld_ClearStatusRegisterCmd();
	}
	slld_RDIDCmd(&target[0], 5);

	
	/* wait for 100 ms just to make sure the flash is done... */
	nrf_delay_ms(100);

	/* Look at the status */

}

/* Write ADC data to flash */

/* Read ADC data from flash to transmit to network */

