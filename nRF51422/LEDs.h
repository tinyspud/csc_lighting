#ifndef __LEDS_H_
#define __LEDS_H_



#define LED_TURN_ON(x)	nrf_gpio_pin_clear(x)
#define LED_TURN_OFF(x)	nrf_gpio_pin_set(x)


void init_LEDs(void);

#endif
