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

	LED_TURN_ON(LED_YELLOW);
	/* Get the status */
	slld_StatusGet(&_status_register);

	slld_ClearStatusRegisterCmd();

	uint8_t target[5] = { 0, 0, 0, 0, 0 };
	slld_RDIDCmd(&target[0], 5);

	slld_StatusGet(&_status_register);

	// blink_out_value(_status_register);

	LED_TURN_OFF(LED_YELLOW);

	/* Look at the status */

}



