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

#include "system_timer.h"
#include "LEDs.h"
#include "system_error_callbacks.h"
#include "S210_LL.h" 
#include "ADCSampling.h"
#include "twi_master_config.h"
#include "twi_master.h"
#include "LightSensor.h"

uint8_t scratch = 0;

void init_light_sensor(){
	/* Init the /INT pin to input */
	nrf_gpio_cfg_input(MAX_44008_INT, NRF_GPIO_PIN_NOPULL);

	/* Init TWI master driver */
	if(twi_master_init())
		LED_TURN_ON(LED_GREEN);
	
	/*  */
	read_int_register();

}

bool read_int_register(){
	bool transfer_succeeded = true;
	scratch = REG_INT_STATUS;
	/* Read the INT register */
    transfer_succeeded &= twi_master_transfer(MAX_44008_ADDR, &scratch, 1, TWI_DONT_ISSUE_STOP);
    if (transfer_succeeded) 
    {
	/* Read the INT register */
		transfer_succeeded &= twi_master_transfer(MAX_44008_ADDR | TWI_READ_BIT, &scratch, 1, TWI_ISSUE_STOP);
    }
    return transfer_succeeded;

}

