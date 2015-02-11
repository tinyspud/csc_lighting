#include "Common.h"

#include "LEDControl.h"

#include "math.h"

typedef enum LED_state{
	OSCILLATE_SINE,
	ABS
}LED_state_t;


#define LED_STEPS		265
volatile unsigned int t_buffer[LED_STEPS];
volatile unsigned int a_buffer[LED_STEPS];
unsigned int num_steps;
volatile unsigned int cur_step_number;
LED_state_t curr_led_state = ABS;

volatile unsigned int cycle_counter;
volatile unsigned int cycle_width;
volatile unsigned int first_half;

void init_LED_pwm_timer(){
	/* Sourced off of ACLK, 32768 Hz, 100 tick = 327.68 Hz signal */
//	TA1CCR0 = 1; /* Pulse width of 20 ms */
//	TA0CCR0 = 1 << 14; /* Pulse width of 20 ms */
//	TA0CCR0 = LED_STEPS;
	TA0CCR0 = 255;

	/* Set up LEDs */
//	LED_PORT_DIR |= LED_PIN;
//	LED_PORT_OUT |= LED_PIN;
	LED_1_PORT_DIR |= LED_1_PIN;
	LED_1_PORT_OUT |= LED_1_PIN;
	LED_1_PORT_SEL |= LED_1_PIN;

	LED_2_PORT_DIR |= LED_2_PIN;
	LED_2_PORT_OUT |= LED_2_PIN;
	LED_2_PORT_SEL |= LED_2_PIN;

	LED_3_PORT_DIR |= LED_3_PIN;
	LED_3_PORT_OUT |= LED_3_PIN;
	LED_3_PORT_SEL |= LED_3_PIN;

	LED_4_PORT_DIR |= LED_4_PIN;
	LED_4_PORT_OUT |= LED_4_PIN;
	LED_4_PORT_SEL |= LED_4_PIN;

	LED_1_CCTL = OUTMOD_6 | OUT;
	LED_2_CCTL = OUTMOD_6 | OUT;
	LED_3_CCTL = OUTMOD_6 | OUT;
	LED_4_CCTL = OUTMOD_6 | OUT;

	TA0CCTL1 |= CCIE;

	/* Restart the timer */
	// TASSEL_1	| Select ACLK source
	// MC_1		| Count timer up to TA0CCR0
	// TACLR	| Clear TAR
	// TAIE		| Enable Timer A interrupt
	TA0CTL = TASSEL_2 | MC_1 | TACLR | ID_0 | TAIE;         // ACLK, upmode, clear TAR

//	set_LED_R(1);
//	set_sinusoid_R(150, 20);
}

void set_sinusoid_R(unsigned int amp, unsigned int period_in_sec){
	num_steps = 0;
	curr_led_state = ABS;
	unsigned int i = 0;
	float x = 0;
	float y = 0;

	cur_step_number = 0;
	for(i = 0; i < LED_STEPS; i++){
		t_buffer[i] = 0x00;
		a_buffer[i] = 0x00;
	}

	/* Calculate the oscillation waveform */
	cycle_counter = 0;
	cycle_width = (unsigned int)(period_in_sec * 50);
	first_half = cycle_width >> 1;

	for(i = 0; i <= amp; i++)
	{
		/* Calculate the scaled acos (scale in x and y) */
		x = ((((float)i) - (((float)amp) / 2)) * 2) / ((float)amp);
		y = roundf((((float)period_in_sec) * 25 / 3.14159) * (acosf(x)));
		t_buffer[amp - i] = (unsigned int)y;
		a_buffer[i] = i;
	}

	/* Set the current step counter to 0 */
	cur_step_number = 0;

	curr_led_state = ABS;
	TA0CCTL1 |= CCIE;

	TA0CTL = TASSEL_2 | MC_1 | TACLR | ID_0 | TAIE;         // ACLK, upmode, clear TAR
}

void set_LED_R(unsigned int pulse_width){
//	return;
//	LED_1_CCR = pulse_width;
}

void set_LED_G(unsigned int pulse_width){
	LED_2_CCR = pulse_width;
}

void set_LED_B(unsigned int pulse_width){
	LED_3_CCR = pulse_width;
}

void set_LED_pwm_output(char pulse_percent){
//	led_pwm_set = (unsigned)pulse_percent;
	LED_1_CCR = (unsigned)pulse_percent;
	LED_2_CCR = (unsigned)pulse_percent;
	LED_3_CCR = (unsigned)pulse_percent;
	LED_4_CCR = (unsigned)pulse_percent;
}

uart_command_process_status_t LEDCommand(unsigned char* ptr2cmdstr, unsigned char* ptr2end){
	/* It's always known that the command starts at the first pointer */
	unsigned int len = 0;
	len = (ptr2end - ptr2cmdstr) + 1;
	if(len < (UART_LED_LEN + UART_LED_CMD_LEN))
		return CMD_NOT_COMPLETE;

	if(len > (UART_LED_LEN + UART_LED_CMD_LEN))
		return CMD_SYNTAX_ERROR;

	//	set_LED_pwm_output(*(ptr2cmdstr + 1));
	switch(*(ptr2cmdstr + 1)){
	case 0x00:
		LED_1_CCR = (unsigned int)(*(ptr2cmdstr + 2));
		LED_2_CCR = (unsigned int)(*(ptr2cmdstr + 2));
		LED_3_CCR = (unsigned int)(*(ptr2cmdstr + 2));
		LED_4_CCR = (unsigned int)(*(ptr2cmdstr + 2));
		break;
	case 0x01:
		LED_1_CCR = (unsigned int)(*(ptr2cmdstr + 2));
		break;
	case 0x02:
		LED_2_CCR = (unsigned int)(*(ptr2cmdstr + 2));
		break;
	case 0x03:
		LED_3_CCR = (unsigned int)(*(ptr2cmdstr + 2));
		break;
	case 0x04:
		LED_4_CCR = (unsigned int)(*(ptr2cmdstr + 2));
		break;
	}

	return CMD_SUCCESS;
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void LED_ISR(){
	switch(__even_in_range(TA0IV, 14)){

	case 0:
		break;
	case 2:                 // CCR1
		/* Red */
//		turn_debug_led_on();
		DEBUG_LED_PORT |= DEBUG_LED_PIN;

//		if(curr_led_state == OSCILLATE_SINE){
//			/* Look to see if you need to adjust the width of the pulse, for now it's a sawtooth */
//			/* First half = going up, 2nd half = going down */
//			if(cycle_counter < first_half){
//				/* Go up the sinusoidal counter */
//				if(cycle_counter >= t_buffer[cur_step_number]){
//					cur_step_number++;
//					LED_1_CCR = a_buffer[cur_step_number];
//				}
//			}
//			else{
//				/* Go down the sinusoidal counter */
//				if((cycle_counter) >= (cycle_width - t_buffer[cur_step_number])){
//					cur_step_number--;
//					LED_1_CCR = a_buffer[cur_step_number];
//				}
//			}
//
//			/* Index counter, look at value to see if it needs to repeat */
//			cycle_counter++;
//		}
		break;
	case 4:                // CCR2
		break;
	case 6:                           // CCR3
		break;
	case 8:                           // CCR4
		break;
	case 10:                          // CCR5
		break;
	case 12:                          // Reserved not used
		break;
	case 14:                 // Timer overflows
		/* Reset all to 0 */
//		turn_debug_led_off();
		DEBUG_LED_PORT &= ~DEBUG_LED_PIN;

//		if((curr_led_state == OSCILLATE_SINE) && (cycle_counter >= cycle_width)){
//			cycle_counter = 0;
//			cur_step_number = 0;
//		}

		break;
	default: break;

	}

}


