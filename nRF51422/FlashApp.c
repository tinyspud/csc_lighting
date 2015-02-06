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

static ADDRESS _active_addr;

static uint8_t _status_reg_1;
static uint8_t _status_reg_2;


#define TARGETLEN	81
void init_flash_app(){
	_status_register = dev_status_unknown;
	int i = 0;
	uint8_t target[TARGETLEN] = { 0 };
	SLLD_STATUS _status = SLLD_ERROR;

	for(i = 0; i < TARGETLEN; i++)
		target[i] = 0;
	
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
	}
	slld_RDIDCmd(&target[0], TARGETLEN);
	
	/* Clear the status register */
	slld_ClearStatusRegisterCmd();

	/* Read status reg 1 */
	slld_RDSRCmd(&_status_reg_1);
	
	/* Read status reg 2 */
	slld_RDSR2Cmd(&_status_reg_2);
	
	/* Look at the status register values */
	
	/* Test write */
	
	/* test read */
	_active_addr = 0;
	slld_ReadCmd(_active_addr, &target[0], TARGETLEN);
	
	
	
	
	/* wait for 100 ms just to make sure the flash is done... */
	nrf_delay_ms(100);

	/* Look at the status */

}



/* Write ADC data to flash */

/* Read ADC data from flash to transmit to network */

