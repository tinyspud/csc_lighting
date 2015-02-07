#include "Common.h"

/* Include Drivers */
#include "LEDControl.h"
#include "UARTInterface.h"
#include "I2CInterface.h"
#include "LightSensor.h"

/* Include system level */
#include "SystemTimer.h"

/*
 * main.c
 */
int main(void) {
	/* Software initialization */
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	/* Set up the clocks */
	init_clocks_ucs();

	/* Init system timer */
	init_system_timer();

	/* Set up LED PWM timer (A0) */
	init_LED_pwm_timer();

	/* Set up the UART */
	init_uart_uca();

	/* Init the light sensor */
	init_light_sensor();

	/* Sleep while waiting; enable interrupts */
	__bis_SR_register(GIE);       // Enter LPM3, enable interrupts

	/* Once everything is initialized, set up the "general" configuration */

	/* Need to have 10 ms delay between init (power up) and when the auth co proc can talk */

	start_system_timer();

	/* General infinite loop */
	for(;;){
		/* Service UART */
		UARTServiceUCA1Rx();

		service_uart_UCA1_TX();

		//		if(ShouldSortFunctionPointers())
		SortFunctionPointers();
		/* Check the function queue and service the ones that have elapsed */
		CheckAndServiceFunctionPointers();

#ifdef READ_I2C_DATA_TO_UART
		if(i2c_rx_buff_has_stuff()){
			/* Read rx buff */
			disp_coproc_data();
		}
#endif
		/* Sleep while waiting; enable interrupts */
		__bis_SR_register(LPM3_bits + GIE);       // Enter LPM3, enable interrupts
	}
}

void init_clocks_ucs(){

	P5SEL |= BIT2+BIT3;                       // Port select XT2 (4.000 MHz)
	P5SEL |= BIT4+BIT5;                       // Port select XT1 (32768 Hz)

	UCSCTL6 &= ~XT2OFF;                       // Set XT2 On
	UCSCTL6 &= ~XT1OFF;                       // Set XT1 On
	UCSCTL6 |= XCAP_3;                        // Internal load cap

	// XT1 is by default on as it is used default reference for the FLL - internal load caps?
	// Loop until XT1,XT2 & DCO stabilizes
	do
	{
		UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
		// Clear XT2,XT1,DCO fault flags
		SFRIFG1 &= ~OFIFG;                      // Clear fault flags
	}while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

	UCSCTL6 &= ~XT2DRIVE0;                    // Decrease XT2 Drive according to
	// expected frequency
	UCSCTL4 |= SELA_0 + SELS_5;               // Select SMCLK, ACLK source and DCO source

}

void toggle_debug_led(){
	//	DEBUG_LED_PORT ^= DEBUG_LED_PIN;
}

void turn_debug_led_on(){
	//	DEBUG_LED_PORT |= DEBUG_LED_PIN;
}

void turn_debug_led_off(){
	//	DEBUG_LED_PORT &= ~DEBUG_LED_PIN;
}


