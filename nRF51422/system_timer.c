#include <stdbool.h>
#include <stdint.h>
#include "app_error.h"
#include "nrf.h"
#include "ant_interface.h"
#include "ant_parameters.h"
#include "nrf_soc.h"
#include "nrf_sdm.h"
#include "app_timer.h"
#include "nrf_delay.h"
#include "nrf_temp.h"
#include "nordic_common.h"
#include "bsp.h"

#include "system_timer.h"
#include "LEDs.h"

void TIMER1_IRQHandler(void){
	/* Look at what kind of interrupt is being generated */
//	/* Timer interrupt - start the ADC */
//	if(NRF_ADC->BUSY == 0)
//		NRF_ADC->TASKS_START = 1;							//Start ADC sampling
//    if ((NRF_TIMER1->EVENTS_COMPARE[0] != 0) && \
//        ((NRF_TIMER1->INTENSET & TIMER_INTENSET_COMPARE0_Msk) != 0))
    if(NRF_TIMER1->EVENTS_COMPARE[0] != 0)
	{
		/* Toggle the red LED */
		nrf_gpio_pin_toggle(LED_RED);
		NRF_TIMER1->EVENTS_COMPARE[0] = 0;
	}
}

void system_timer_init(void){
	
	// Start 16 MHz crystal oscillator.
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART    = 1;

    // Wait for the external oscillator to start up.
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)
    {
        // Do nothing.
    }
	
	/* Crystal is 16MHz */
    NRF_TIMER1->MODE        = TIMER_MODE_MODE_Timer;       // Set the timer in Timer Mode.
    NRF_TIMER1->PRESCALER   = 9;                           // Prescaler 10 produces n 16 MHz/2^(prescaler) = 15625 Hz
    NRF_TIMER1->BITMODE     = TIMER_BITMODE_BITMODE_16Bit; // 16 bit mode.
	NRF_TIMER1->SHORTS      = TIMER_SHORTS_COMPARE0_STOP_Disabled << TIMER_SHORTS_COMPARE0_STOP_Pos;
	
//	sd_nvic_SetPriority(TIMER1_IRQn, NRF_APP_PRIORITY_HIGH);  
//	sd_nvic_EnableIRQ(TIMER1_IRQn);
}

void start_system_timer(){
	NRF_TIMER1->TASKS_CLEAR = 1;                           // clear the task first to be usable for later.

	/* Enable the compare 0 interrupt */
//	NRF_TIMER1->INTENCLR |= TIMER_INTENCLR_COMPARE0_Clear << TIMER_INTENCLR_COMPARE0_Pos;;
	NRF_TIMER1->INTENSET |= TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos;

    // With 32 us ticks, we need to multiply by 31.25 to get milliseconds.
    NRF_TIMER1->CC[0]       = 1000 * 31;
    NRF_TIMER1->CC[0]      += 1000 / 4;

//	NVIC_EnableIRQ(TIMER1_IRQn);
	sd_nvic_SetPriority(TIMER1_IRQn, NRF_APP_PRIORITY_LOW);  
	sd_nvic_EnableIRQ(TIMER1_IRQn);
    NRF_TIMER1->TASKS_START = 1; // Start timer.

}

