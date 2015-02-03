#ifndef __LEDS_H_
#define __LEDS_H_

#include <stdint.h>
#include "nrf_gpio.h"

#define LED_TURN_ON(x)	nrf_gpio_pin_clear(x)
#define LED_TURN_OFF(x)	nrf_gpio_pin_set(x)
#define LED_TOGGLE(x)	nrf_gpio_pin_toggle(x)


void init_LEDs(void);

void blink_out_value(uint32_t value);

#endif
