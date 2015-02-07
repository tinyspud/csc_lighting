#include "Common.h"

#include "LEDControl.h"


volatile unsigned char led_pwm_amount = 0;
volatile unsigned char led_pwm_set = 100;




void init_LED_pwm_timer(){
	/* Sourced off of ACLK, 32768 Hz, 100 tick = 327.68 Hz signal */
//	TA1CCR0 = 1; /* Pulse width of 20 ms */
	TA0CCR0 = 255; /* Pulse width of 20 ms */

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

	led_pwm_amount = 0;
	led_pwm_set = 30;

//	TA1CCTL0 |= CCIE;
	/* Don't need CCIE since using timer output */
	TA0CCTL0 &= ~CCIE;



	/* Restart the timer */
	// TASSEL_1	| Select ACLK source
	// MC_1		| Count timer up to TA0CCR0
	// TACLR	| Clear TAR
	// TAIE		| Enable Timer A interrupt
//	TA1CTL = TASSEL_1 | MC_1 | TACLR;         // ACLK, upmode, clear TAR
	TA0CTL = TASSEL_2 | MC_1 | TACLR | ID_3;         // ACLK, upmode, clear TAR
//	TA1CTL = TASSEL_2 | MC_1 | TACLR | ID_3;         // ACLK, upmode, clear TAR

	set_LED_pwm_output(63);
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



