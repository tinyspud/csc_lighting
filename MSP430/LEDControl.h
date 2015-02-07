#ifndef __LED_CONTROL_H
#define __LED_CONTROL_H

#define LED_1_PORT_DIR	P1DIR
#define LED_1_PORT_SEL	P1SEL
#define LED_1_PORT_OUT	P1OUT
#define LED_1_PIN	BIT2

#define LED_2_PORT_DIR	P1DIR
#define LED_2_PORT_SEL	P1SEL
#define LED_2_PORT_OUT	P1OUT
#define LED_2_PIN	BIT3

#define LED_3_PORT_DIR	P1DIR
#define LED_3_PORT_SEL	P1SEL
#define LED_3_PORT_OUT	P1OUT
#define LED_3_PIN	BIT4

#define LED_4_PORT_DIR	P1DIR
#define LED_4_PORT_SEL	P1SEL
#define LED_4_PORT_OUT	P1OUT
#define LED_4_PIN	BIT5

#define LED_1_CCTL	TA0CCTL1
#define LED_2_CCTL	TA0CCTL2
#define LED_3_CCTL	TA0CCTL3
#define LED_4_CCTL	TA0CCTL4

#define LED_1_CCR	TA0CCR1
#define LED_2_CCR	TA0CCR2
#define LED_3_CCR	TA0CCR3
#define LED_4_CCR	TA0CCR4


void init_LED_pwm_timer();

void set_LED_pwm_output(char);

uart_command_process_status_t LEDCommand(unsigned char*, unsigned char*);

#endif

