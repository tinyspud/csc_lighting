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

void system_timer_init(void){
	// Start 16 MHz crystal oscillator.
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART    = 1;

    // Wait for the external oscillator to start up.
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)
    {
        // Do nothing.
    }
	
	/* Crystal is 15MHz */
    NRF_TIMER0->MODE        = TIMER_MODE_MODE_Timer;       // Set the timer in Timer Mode.
    NRF_TIMER0->PRESCALER   = 9;                           // Prescaler 9 produces 31250 Hz timer frequency => 1 tick = 32 us.
    NRF_TIMER0->BITMODE     = TIMER_BITMODE_BITMODE_16Bit; // 16 bit mode.
	
	NVIC_EnableIRQ(TIMER0_IRQn);
	

}

void start_system_timer(){
	    NRF_TIMER0->TASKS_CLEAR = 1;                           // clear the task first to be usable for later.

    // With 32 us ticks, we need to multiply by 31.25 to get milliseconds.
//    NRF_TIMER0->CC[0]       = number_of_ms * 31;
//    NRF_TIMER0->CC[0]      += number_of_ms / 4;
    NRF_TIMER0->TASKS_START = 1; // Start timer.

    while (NRF_TIMER0->EVENTS_COMPARE[0] == 0)
    {
        // Do nothing.
    }

    NRF_TIMER0->EVENTS_COMPARE[0] = 0;
    NRF_TIMER0->TASKS_STOP        = 1; // Stop timer.
	
	NRF_TIMER0->INTENSET |= TIMER_INTENSET_COMPARE0_Enabled;
	

}

void TIMER0_IRQHandler(void){

}

