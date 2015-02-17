#include <stdbool.h>
#include <stdint.h>
#include "app_error.h"
#include "nrf.h"
#include "ant_interface.h"
#include "ant_parameters.h"
#include "nrf_soc.h"
#include "nrf_sdm.h"
#include "nrf_gpiote.h"
#include "app_timer.h"
#include "nrf_delay.h"
#include "nrf_temp.h"
#include "nordic_common.h"
#include "custom_board.h"

#include "nrf_gpio.h"
#include "boards.h"
#include "nrf_delay.h"
#include "LEDs.h"

#define RESET_INDEX	0
#define A_INDEX		5
#define R_INDEX		3
#define G_INDEX		2
#define B_INDEX		1

static volatile uint32_t pwm_val_A = 1;
static volatile uint32_t pwm_val_R = 64;
static volatile uint32_t pwm_val_G = 128;
static volatile uint32_t pwm_val_B = 1;

void set_A_PWM_val(uint32_t val){
	pwm_val_A = (val >= MAX_LED_PWM_VAL) ? (MAX_LED_PWM_VAL - 1) : val;
}

void set_R_PWM_val(uint32_t val){
	pwm_val_R = (val >= MAX_LED_PWM_VAL) ? (MAX_LED_PWM_VAL - 1) : val;
}

void set_G_PWM_val(uint32_t val){
	pwm_val_G = (val >= MAX_LED_PWM_VAL) ? (MAX_LED_PWM_VAL - 1) : val;
}

void set_B_PWM_val(uint32_t val){
	pwm_val_B = (val >= MAX_LED_PWM_VAL) ? (MAX_LED_PWM_VAL - 1) : val;
}

/* Refresh PWM1-3 values */
void TIMER1_IRQHandler(void){
	/* Look at what kind of interrupt is being generated */
//	if((NRF_TIMER1->EVENTS_COMPARE[RESET_INDEX] != 0) &&
//		((NRF_TIMER1->INTENSET & TIMER_INTENSET_COMPARE1_Msk) != 0))
	if((NRF_TIMER1->EVENTS_COMPARE[RESET_INDEX]) != 0)
	{
//	LED_TURN_ON(LED_RED);
//	LED_TOGGLE(LED_RED);
//	LED_TOGGLE(LED_GREEN);
		/* TODO For now just set to lowest until can figure out how to disable the event that makes it switch */
		NRF_TIMER1->CC[R_INDEX] = (pwm_val_R == 0) ? 1 : pwm_val_R;	// red
		NRF_TIMER1->CC[G_INDEX] = (pwm_val_G == 0) ? 1 : pwm_val_G;	// green
		NRF_TIMER1->CC[B_INDEX] = (pwm_val_B == 0) ? 1 : pwm_val_B;	// yellow
		NRF_TIMER1->EVENTS_COMPARE[RESET_INDEX] = 0;
		
		/* TODO need fix to see if this still fires if all LEDs are off */
//		if(pwm_val_R > 0)
//			NRF_TIMER1->EVENTS_COMPARE[R_INDEX] = 0;
//		if(pwm_val_G > 0)
//			NRF_TIMER1->EVENTS_COMPARE[G_INDEX] = 0;
//		if(pwm_val_B > 0)
//			NRF_TIMER1->EVENTS_COMPARE[B_INDEX] = 0;
		
			NRF_TIMER1->EVENTS_COMPARE[R_INDEX] = 0;
			NRF_TIMER1->EVENTS_COMPARE[G_INDEX] = 0;
			NRF_TIMER1->EVENTS_COMPARE[B_INDEX] = 0;
		/* Do sanity check to see if system's still running */
		NRF_TIMER1->TASKS_START = 1;
	}
}

void TIMER2_IRQHandler(void){
	/* Look at what kind of interrupt is being generated */
//	if((NRF_TIMER2->EVENTS_COMPARE[RESET_INDEX] != 0) &&
//		((NRF_TIMER2->INTENSET & TIMER_INTENSET_COMPARE1_Msk) != 0))
	if((NRF_TIMER2->EVENTS_COMPARE[RESET_INDEX]) != 0)
	{
		NRF_TIMER2->CC[(A_INDEX % 4)] = (pwm_val_A == 0) ? 1 : pwm_val_A;	// orange
		NRF_TIMER2->EVENTS_COMPARE[RESET_INDEX] = 0;
		if(pwm_val_A > 0)
			NRF_TIMER2->EVENTS_COMPARE[(A_INDEX % 4)] = 0;
		NRF_TIMER2->TASKS_START = 1;
	}
}

void init_LEDs(void){
	// Configure LED-pins as outputs.
	nrf_gpio_cfg_output(LED_RED);
	nrf_gpio_cfg_output(LED_GREEN);
	nrf_gpio_cfg_output(LED_YELLOW);
	nrf_gpio_cfg_output(LED_ORANGE);

	LED_TURN_OFF(LED_RED);
	LED_TURN_OFF(LED_GREEN);
	LED_TURN_OFF(LED_YELLOW);
	LED_TURN_OFF(LED_ORANGE);

	// Start 16 MHz crystal oscillator.
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART    = 1;

    // Wait for the external oscillator to start up.
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)
    {
        /* Wait for HF crystal to start */
    }

	/* Crystal is 16MHz */
	NRF_TIMER1->MODE        = TIMER_MODE_MODE_Timer;       // Set the timer in Timer Mode.
	NRF_TIMER1->PRESCALER   = 0;                           // Prescaler: 16 MHz/2^(prescaler) Hz - highest frequency possible
	NRF_TIMER1->BITMODE     = TIMER_BITMODE_BITMODE_16Bit;

	NRF_TIMER1->SHORTS      = (TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos);
	
	/* Crystal is 16MHz */
	NRF_TIMER2->MODE        = TIMER_MODE_MODE_Timer;       // Set the timer in Timer Mode.
	NRF_TIMER2->PRESCALER   = 0;                           // Prescaler: 16 MHz/2^(prescaler) Hz - highest frequency possible
	NRF_TIMER2->BITMODE     = TIMER_BITMODE_BITMODE_16Bit;

	NRF_TIMER2->SHORTS      = (TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos);

	/* GPIOTE config */
	nrf_gpiote_task_config(R_INDEX, LED_RED, NRF_GPIOTE_POLARITY_TOGGLE, NRF_GPIOTE_INITIAL_VALUE_LOW);
	nrf_gpiote_task_config(G_INDEX, LED_GREEN, NRF_GPIOTE_POLARITY_TOGGLE, NRF_GPIOTE_INITIAL_VALUE_LOW);
	nrf_gpiote_task_config(B_INDEX, LED_ORANGE, NRF_GPIOTE_POLARITY_TOGGLE, NRF_GPIOTE_INITIAL_VALUE_LOW);
	nrf_gpiote_task_config(0, LED_YELLOW, NRF_GPIOTE_POLARITY_TOGGLE, NRF_GPIOTE_INITIAL_VALUE_LOW);

	/* Configure PPI to toggle the LED on CC[x] and CC[0] */
	/* Red */
	/* RED at CH0 gets blown out by TWI driver */
//    NRF_PPI->CH[0].EEP = (uint32_t)&NRF_TIMER1->EVENTS_COMPARE[R_INDEX];
//    NRF_PPI->CH[0].TEP = (uint32_t)&NRF_GPIOTE->TASKS_OUT[R_INDEX];

//    NRF_PPI->CH[1].EEP = (uint32_t)&NRF_TIMER1->EVENTS_COMPARE[RESET_INDEX];
//    NRF_PPI->CH[1].TEP = (uint32_t)&NRF_GPIOTE->TASKS_OUT[R_INDEX];

    NRF_PPI->CH[1].EEP = (uint32_t)&NRF_TIMER1->EVENTS_COMPARE[R_INDEX];
    NRF_PPI->CH[1].TEP = (uint32_t)&NRF_GPIOTE->TASKS_OUT[R_INDEX];

	/* Green */
    NRF_PPI->CH[2].EEP = (uint32_t)&NRF_TIMER1->EVENTS_COMPARE[G_INDEX];
    NRF_PPI->CH[2].TEP = (uint32_t)&NRF_GPIOTE->TASKS_OUT[G_INDEX];

    NRF_PPI->CH[3].EEP = (uint32_t)&NRF_TIMER1->EVENTS_COMPARE[RESET_INDEX];
    NRF_PPI->CH[3].TEP = (uint32_t)&NRF_GPIOTE->TASKS_OUT[G_INDEX];

	/* Blue */
    NRF_PPI->CH[4].EEP = (uint32_t)&NRF_TIMER1->EVENTS_COMPARE[B_INDEX];
    NRF_PPI->CH[4].TEP = (uint32_t)&NRF_GPIOTE->TASKS_OUT[B_INDEX];

    NRF_PPI->CH[5].EEP = (uint32_t)&NRF_TIMER1->EVENTS_COMPARE[RESET_INDEX];
    NRF_PPI->CH[5].TEP = (uint32_t)&NRF_GPIOTE->TASKS_OUT[B_INDEX];

	/* Amber */
    NRF_PPI->CH[6].EEP = (uint32_t)&NRF_TIMER2->EVENTS_COMPARE[(A_INDEX % 4)];
    NRF_PPI->CH[6].TEP = (uint32_t)&NRF_GPIOTE->TASKS_OUT[0];

    NRF_PPI->CH[7].EEP = (uint32_t)&NRF_TIMER2->EVENTS_COMPARE[RESET_INDEX];
    NRF_PPI->CH[7].TEP = (uint32_t)&NRF_GPIOTE->TASKS_OUT[0];

	// Enable PPI channels 0-7
    NRF_PPI->CHEN = 
					(PPI_CHEN_CH0_Enabled << PPI_CHEN_CH0_Pos) |
                    (PPI_CHEN_CH1_Enabled << PPI_CHEN_CH1_Pos) |
                    (PPI_CHEN_CH2_Enabled << PPI_CHEN_CH2_Pos) |
                    (PPI_CHEN_CH3_Enabled << PPI_CHEN_CH3_Pos) |
                    (PPI_CHEN_CH4_Enabled << PPI_CHEN_CH4_Pos) |
					(PPI_CHEN_CH5_Enabled << PPI_CHEN_CH5_Pos) |
					(PPI_CHEN_CH6_Enabled << PPI_CHEN_CH6_Pos) |
                    (PPI_CHEN_CH7_Enabled << PPI_CHEN_CH7_Pos)
//					(PPI_CHEN_CH8_Enabled << PPI_CHEN_CH8_Pos)
					;
	
	NRF_TIMER1->CC[RESET_INDEX] = MAX_LED_PWM_VAL;
	NRF_TIMER2->CC[RESET_INDEX] = MAX_LED_PWM_VAL;

	NRF_TIMER1->CC[R_INDEX] = pwm_val_R;	// red
	NRF_TIMER1->CC[G_INDEX] = pwm_val_G;	// green
	NRF_TIMER1->CC[B_INDEX] = pwm_val_B;	// orange
	NRF_TIMER2->CC[(A_INDEX % 4)] = pwm_val_A;	// orange
}

void start_LED_timer(){
	NRF_TIMER1->TASKS_CLEAR = 1;                           // clear the task first to be usable for later.
	NRF_TIMER2->TASKS_CLEAR = 1;                           // clear the task first to be usable for later.

	/* Enable the compare 0 interrupt */
	NRF_TIMER1->INTENSET |= (TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos) |
							(TIMER_INTENSET_COMPARE1_Enabled << TIMER_INTENSET_COMPARE1_Pos) |
							(TIMER_INTENSET_COMPARE2_Enabled << TIMER_INTENSET_COMPARE2_Pos) |
							(TIMER_INTENSET_COMPARE3_Enabled << TIMER_INTENSET_COMPARE3_Pos);

	NRF_TIMER2->INTENSET |= (TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos) |
							(TIMER_INTENSET_COMPARE1_Enabled << TIMER_INTENSET_COMPARE1_Pos);
#ifdef SOFTDEVICE_PRESENT
	sd_nvic_SetPriority(TIMER1_IRQn, NRF_APP_PRIORITY_LOW);
	sd_nvic_EnableIRQ(TIMER1_IRQn);
	sd_nvic_SetPriority(TIMER2_IRQn, NRF_APP_PRIORITY_LOW);
	sd_nvic_EnableIRQ(TIMER2_IRQn);
#else
	NVIC_EnableIRQ(TIMER1_IRQn);
	NVIC_EnableIRQ(TIMER2_IRQn);
#endif
    NRF_TIMER1->TASKS_START = 1; // Start timer.
    NRF_TIMER2->TASKS_START = 1; // Start timer.
}








void blink_out_value(uint32_t value){
		/* Turn off the LEDs */
	LED_TURN_OFF(LED_RED);
	LED_TURN_OFF(LED_GREEN);
	LED_TURN_OFF(LED_ORANGE);
	LED_TURN_OFF(LED_YELLOW);

	int i = 0;
	/* Flash out the error code on the red LED, line number on orange */
	LED_TURN_OFF(LED_YELLOW);
	for(i = 0; i < 32; i++){
		LED_TURN_OFF(LED_YELLOW);
		(((value << i) & 0x80000000) == 0x80000000) ? LED_TURN_ON(LED_RED) : LED_TURN_OFF(LED_RED);
		nrf_delay_ms(100);
		LED_TURN_ON(LED_YELLOW);
		nrf_delay_ms(5);
	}
	LED_TURN_OFF(LED_RED);
	LED_TURN_OFF(LED_GREEN);
	LED_TURN_OFF(LED_ORANGE);
	LED_TURN_OFF(LED_YELLOW);	
}



