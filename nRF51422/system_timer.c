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
#include "ADCSampling.h"

void TIMER1_IRQHandler(void){
	/* Look at what kind of interrupt is being generated */
    if(NRF_TIMER1->EVENTS_COMPARE[0] != 0)
	{
		/* Toggle the red LED */
		LED_TURN_ON(ADC_INDICATOR_LED);

		if(NRF_ADC->BUSY == 0)
			NRF_ADC->TASKS_START = 1;							//Start ADC sampling

		NRF_TIMER1->EVENTS_COMPARE[0] = 0;
		NRF_TIMER1->TASKS_START = 1; // Start timer.
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
    NRF_TIMER1->PRESCALER   = 8;                           // Prescaler: 16 MHz/2^(prescaler) Hz
    NRF_TIMER1->BITMODE     = TIMER_BITMODE_BITMODE_16Bit; // 16 bit mode.
	NRF_TIMER1->SHORTS      |= (TIMER_SHORTS_COMPARE0_STOP_Enabled << TIMER_SHORTS_COMPARE0_STOP_Pos) |
								(TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos);
}

void start_system_timer(){
	NRF_TIMER1->TASKS_CLEAR = 1;                           // clear the task first to be usable for later.

	/* Enable the compare 0 interrupt */
	NRF_TIMER1->INTENSET |= TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos;

    // With 32 us ticks, we need to multiply by 31.25 to get milliseconds.
	//    NRF_TIMER1->CC[0]       = (ADC_SAMPLING_PERIOD * 31);
	//    NRF_TIMER1->CC[0]      += (ADC_SAMPLING_PERIOD / 4);
	NRF_TIMER1->CC[0] = 16000000/(1 << (NRF_TIMER1->PRESCALER)) / ADC_SAMPLING_FREQ;
	
#ifdef SOFTDEVICE_PRESENT
	sd_nvic_SetPriority(TIMER1_IRQn, NRF_APP_PRIORITY_LOW);  
	sd_nvic_EnableIRQ(TIMER1_IRQn);
#else
	NVIC_EnableIRQ(TIMER1_IRQn);
#endif

    NRF_TIMER1->TASKS_START = 1; // Start timer.

}

