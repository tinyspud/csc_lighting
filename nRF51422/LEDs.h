#ifndef __LEDS_H_
#define __LEDS_H_

#include <stdint.h>
#include "nrf_gpio.h"

#define LED_TURN_ON(x)	nrf_gpio_pin_clear(x)
#define LED_TURN_OFF(x)	nrf_gpio_pin_set(x)
#define LED_TOGGLE(x)	nrf_gpio_pin_toggle(x)


#define MAX_LED_PWM_VAL		2048
//#define MAX_LED_PWM_VAL		256


/* Values to set ARGB LED values */
void set_A_PWM_val(uint32_t);
void set_R_PWM_val(uint32_t);
void set_G_PWM_val(uint32_t);
void set_B_PWM_val(uint32_t);

void start_LED_timer(void);

void init_LEDs(void);

void LightShow(void);

void blink_out_value(uint32_t value);

#endif
