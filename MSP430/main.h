

#ifndef __MAIN_H
#define __MAIN_H

#define SET_BIT_HIGH(port, pin)	port |= pin
#define SET_BIT_LOW(port, pin)	port &= ~pin

#define DEBUG_LED_PORT	PJOUT
#define DEBUG_LED_PIN	BIT3

#define USE_4_MHZ_SMCLK

void init_clocks_ucs();

void turn_debug_led_on();

void turn_debug_led_off();

void toggle_debug_led();

#endif
