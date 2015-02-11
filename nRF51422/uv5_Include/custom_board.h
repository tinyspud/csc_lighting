/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */
#ifndef __BULLET_BREAD_BOARD_H
#define __BULLET_BREAD_BOARD_H

// LEDs definitions for PCA10031
#define LEDS_NUMBER    4

#define LED_0          8
#define LED_1          3
#define LED_2          15
#define LED_3          30

#define LED_RED				LED_0
#define LED_GREEN			LED_1
#define LED_YELLOW		LED_2
#define LED_ORANGE		LED_3

#define LED_START      LED_GREEN
#define LED_STOP       LED_RED

#define LED_RGB_RED				LED_RED
#define LED_RGB_GREEN			LED_GREEN
#define LED_RGB_YELLOW		LED_YELLOW
#define LED_RGB_ORANGE		LED_ORANGE

#define LED_RGB_RED_MASK    	(1<<LED_RED)
#define LED_RGB_GREEN_MASK  	(1<<LED_GREEN)
#define LED_RGB_ORANGE_MASK  	(1<<LED_ORANGE)

#define LEDS_LIST { LED_RED, LED_GREEN, LED_YELLOW, LED_ORANGE}
// defining RGB led as 3 single LEDs
#define BSP_LED_0 LED_RGB_RED
#define BSP_LED_1 LED_RGB_GREEN
#define BSP_LED_2 LED_RGB_ORANGE
#define BSP_LED_3 LED_RGB_YELLOW

#define BSP_LED_0_MASK    (1<<BSP_LED_0)
#define BSP_LED_1_MASK    (1<<BSP_LED_1)
#define BSP_LED_2_MASK    (1<<BSP_LED_2)
#define BSP_LED_3_MASK		(1<<BSP_LED_3)

#define LEDS_MASK      (BSP_LED_0_MASK | BSP_LED_1_MASK | BSP_LED_2_MASK)
//defines which LEDs are lit when signal is low
#define LEDS_INV_MASK  LEDS_MASK

// there are no user buttons
#define BUTTONS_NUMBER 0
#define BUTTONS_LIST {}
#define BUTTONS_MASK   0x00000000

//// UART connection with J-Link
//#define RX_PIN_NUMBER  11
//#define TX_PIN_NUMBER  9
//#define CTS_PIN_NUMBER 10
//#define RTS_PIN_NUMBER 8
//#define HWFC           true

#define MAX_44008_SCL	24U
#define MAX_44008_SDA	9U
//#define MAX_44008_A0	5
#define MAX_44008_INT	6U


#define STRAIN_GAUGE_PIN	ADC_CONFIG_PSEL_AnalogInput3
#define STRAIN_GAUGE_REF	0U



#endif
