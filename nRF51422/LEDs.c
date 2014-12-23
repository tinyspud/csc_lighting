#include "nrf_gpio.h"
#include "boards.h"
#include "nrf_delay.h"
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


void blink_out_value(uint32_t value){
		/* Turn off the LEDs */
	LED_TURN_OFF(LED_RED);
	LED_TURN_OFF(LED_GREEN);
	LED_TURN_OFF(LED_ORANGE);
	LED_TURN_OFF(LED_YELLOW);

	int i = 0;
	/* Flash out the error code on the red LED, line number on orange */
	LED_TURN_OFF(LED_YELLOW);
	for(i = 0; i < 32; i++){
		LED_TURN_OFF(LED_YELLOW);
		(((value << i) & 0x80000000) == 0x80000000) ? LED_TURN_ON(LED_RED) : LED_TURN_OFF(LED_RED);
		nrf_delay_ms(100);
		LED_TURN_ON(LED_YELLOW);
		nrf_delay_ms(5);
	}
	LED_TURN_OFF(LED_RED);
	LED_TURN_OFF(LED_GREEN);
	LED_TURN_OFF(LED_ORANGE);
	LED_TURN_OFF(LED_YELLOW);	
}



