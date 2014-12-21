#include "nrf_gpio.h"
#include "boards.h"
#include "LEDs.h"


void init_LEDs(void){
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


