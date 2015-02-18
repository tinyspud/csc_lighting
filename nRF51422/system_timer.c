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

#include "system_timer.h"
#include "LEDs.h"
#include "ADCSampling.h"

typedef struct _fcn_pair{
	uint32_t exe_offset_from_req_time;
	uint32_t req_time;
	queueableFunctionPointer fcn_ptr;
}_fcn_pair_t;

static volatile uint8_t _should_sort;

static volatile uint32_t _cur_small_tick;
static volatile uint32_t _cur_big_tick;
static volatile int8_t _fcn_list_updated;
static volatile _fcn_pair_t _fcn_pntr_buffer[MAX_NUM_FCNS];

void SortFunctionPointers(void);
void store_a_to_b(volatile _fcn_pair_t *, volatile _fcn_pair_t *);
void clear_out_fcn_pair(volatile _fcn_pair_t *);




void RTC1_IRQHandler()
{
    if ((NRF_RTC1->EVENTS_TICK != 0) &&
        ((NRF_RTC1->INTENSET & RTC_INTENSET_TICK_Msk) != 0))
    {
		_cur_small_tick++;

		/* Check to see if the current small tick increments */
		if(_cur_small_tick == 0x0000FFFF){
			_cur_big_tick++;
			_cur_small_tick = 0;
		}
		LED_TOGGLE(LED_ORANGE);

		/* Clear tick event */
        NRF_RTC1->EVENTS_TICK = 0;
    }

    if ((NRF_RTC1->EVENTS_COMPARE[0] != 0) &&
        ((NRF_RTC1->INTENSET & RTC_INTENSET_COMPARE0_Msk) != 0))
    {
//		_cur_small_tick++;

//		/* Check to see if the current small tick increments */
//		if(_cur_small_tick == 0x0000FFFF){
//			_cur_big_tick++;
//			_cur_small_tick = 0;
//			LED_TOGGLE(LED_ORANGE);
//		}

//        NRF_RTC1->EVENTS_COMPARE[0] = 0;
//		NRF_RTC1->TASKS_START = 1;
    }
}

void system_timer_init(void){
	/* Create the function pointer list */
	_fcn_list_updated = -1;

	_should_sort = 0;

	uint16_t i = 0;
	for(i = 0; i < MAX_NUM_FCNS; i++){
		clear_out_fcn_pair(&_fcn_pntr_buffer[i]);
	}

	/* Start the low frequency crystal (LFXTAL) */
	NRF_CLOCK->LFCLKSRC            = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos);
    NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_LFCLKSTART    = 1;

    while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0)
    {
        // Do nothing.
    }
    NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;

	/* Start the RTC */
//    NRF_RTC1->PRESCALER = COUNTER_PRESCALER;                    // Set prescaler to a TICK of RTC_FREQUENCY.
	NRF_RTC1->PRESCALER = 0;                    // Set prescaler to a TICK of RTC_FREQUENCY.
//    NRF_RTC1->CC[0]     = COMPARE_COUNTERTIME * RTC_FREQUENCY;  // Compare0 after approx COMPARE_COUNTERTIME seconds.
	
	NRF_RTC1->CC[0] = 1;
    // Enable TICK event and TICK interrupt:
    NRF_RTC1->EVTENSET = RTC_EVTENSET_TICK_Msk;
    NRF_RTC1->INTENSET = RTC_INTENSET_TICK_Msk;

//    // Enable COMPARE0 event and COMPARE0 interrupt:
//    NRF_RTC1->EVTENSET = RTC_EVTENSET_COMPARE0_Msk;
//    NRF_RTC1->INTENSET = RTC_INTENSET_COMPARE0_Msk;
	
	_cur_small_tick = 0;
}

void start_system_timer(){
#ifdef SOFTDEVICE_PRESENT
	sd_nvic_SetPriority(RTC1_IRQn, NRF_APP_PRIORITY_HIGH);
	sd_nvic_EnableIRQ(RTC1_IRQn);
#else
    NVIC_EnableIRQ(RTC1_IRQn);                                  // Enable Interrupt for the RTC in the core.
#endif
	NRF_RTC1->TASKS_START = 1;  // start the RTC
}

uint32_t TickToSec(uint32_t tickval){
	return (tickval / SYS_TIMER_ONE_SEC);
}

uint16_t get_current_tick(){
//	return _cur_small_tick; /* TODO give value of cur tick */
	return NRF_RTC1->COUNTER;
}

uint32_t GetCurrentLongTick(){
	return (uint32_t)((((uint32_t)_cur_big_tick) << 16) | get_current_tick());
}

void TryRegisterWithSysTimer(queueableFunctionPointer fcn_ptr, uint32_t ticks_from_now_execution){
	if((_fcn_list_updated + 1) < MAX_NUM_FCNS){
		while(_fcn_list_updated < MAX_NUM_FCNS){
			_fcn_list_updated++;
			if(_fcn_pntr_buffer[_fcn_list_updated].fcn_ptr == 0){
				_fcn_pntr_buffer[_fcn_list_updated].req_time = GetCurrentLongTick();
				_fcn_pntr_buffer[_fcn_list_updated].exe_offset_from_req_time = ticks_from_now_execution;
				_fcn_pntr_buffer[_fcn_list_updated].fcn_ptr = fcn_ptr;

				break;
			}
		}
		/* Sort if it's more than 0 (i. e. 1) in the queue */
		if(_fcn_list_updated > 0)
			_should_sort++;
	}
}

uint8_t _does_a_execute_before_b(volatile _fcn_pair_t * a, volatile _fcn_pair_t *b, uint32_t cur_long){
	/* Temporary variable */
	uint32_t scratch = 0;

	/* How far from now is a? */
	uint32_t a_from_now = 0;
	uint32_t b_from_now = 0;

	if((a->req_time + a->exe_offset_from_req_time) < (a->req_time)){
		/* execution time wraps */
		scratch = ~a->req_time;
	}
	a_from_now = scratch + a->exe_offset_from_req_time;

	scratch = 0;
	if((b->req_time + b->exe_offset_from_req_time) < (b->req_time)){
		scratch = ~b->req_time;
	}
	b_from_now = scratch + b->exe_offset_from_req_time;


	return a_from_now < b_from_now;
}

uint8_t _should_fcn_have_executed(volatile _fcn_pair_t * a, uint32_t cur_long){
	uint8_t _fcn_should_execute = false;

	if((((a->req_time + a->exe_offset_from_req_time) <= cur_long) && (a->req_time <= cur_long))){
		_fcn_should_execute = true;
	}else{
		/* This is used to detect if the counter has wrapped */
		if((a->req_time >= cur_long) && (cur_long >= (a->req_time + a->exe_offset_from_req_time))){
			/* Wraps when the numbers are added */
			_fcn_should_execute = true;
		}
	}
	return _fcn_should_execute;
}

void __sort(){
	/* Sort fcns chronologically */
	volatile _fcn_pair_t scratch = { 0 };
	int a = 0;
	int b = 0;

	int8_t this_fcn_list_cnt = 0;
	this_fcn_list_cnt = _fcn_list_updated;
	//	this_fcn_list_cnt = MAX_NUM_FCNS;

	/* Simple bubble sort for now (can optimize later but this isn't running in
	 * an ISR and the processor load's light at the moment) */
	/* Get now */
	uint32_t cur_long = 0;
	cur_long = GetCurrentLongTick();

	/* Note during all of the _fcn_pair_t executions they are flagged as
	 * volatile but they should be treated as static in this loop since ISRs
	 * shouldn't be updating this list */
	for(a = 0; a <= this_fcn_list_cnt; a++){
		for(b = 0; b <= this_fcn_list_cnt; b++){
			if(a == b)
				continue;

			/* The soonest function for execution goes into the highest value in
			 * the queue */
			if(!_does_a_execute_before_b(&_fcn_pntr_buffer[a], &_fcn_pntr_buffer[b], cur_long)){
				/* Switch positions if something with sooner execution is in a lower
				 * numeric position */
				/* If a executes before b, put a to the b position */
				/* Place b into the scratch variable */
				store_a_to_b(&_fcn_pntr_buffer[b], &scratch);

				store_a_to_b(&_fcn_pntr_buffer[a], &_fcn_pntr_buffer[b]);

				store_a_to_b(&scratch, &_fcn_pntr_buffer[a]);
			}
		}
	}

	/* WIP - double and tripple check this logic */
	/* This is to deal with the volatility of ISRs adding functions to the queue
	 * during the execution of this function */
	if(this_fcn_list_cnt != _fcn_list_updated)
		__sort();
}

void SortFunctionPointers(){
//	if(_fcn_list_updated <= 0)
//		return;

	__sort();
	int a = 0;
	_fcn_list_updated = -1;
	for(a = (MAX_NUM_FCNS - 1); a >= 0; a--){
		if(_fcn_pntr_buffer[a].fcn_ptr != 0){
			_fcn_list_updated = a;
			break;
		}
	}

	_should_sort = 0;
}

void store_a_to_b(volatile _fcn_pair_t * a, volatile _fcn_pair_t * b){
	b->exe_offset_from_req_time = a->exe_offset_from_req_time;
	b->fcn_ptr = a->fcn_ptr;
	b->req_time = a->req_time;
}

void clear_out_fcn_pair(volatile _fcn_pair_t * a){
	a->exe_offset_from_req_time = 0;
	a->req_time = 0;
	a->fcn_ptr = NULL;
}

void CheckAndServiceFunctionPointers(){
	/* See if anyone needs to be executed */
	if(_fcn_list_updated >= 0)
	{
		/* Get now */
		uint32_t cur_long = 0;
		int i = 0;
		cur_long = GetCurrentLongTick();

		bool all_serviced = false;

		do{
			/* Sort the list */
			SortFunctionPointers();

			i = MAX_NUM_FCNS - 1;
			for(i = MAX_NUM_FCNS - 1; i >= 0; i--){
				if((_fcn_pntr_buffer[i].fcn_ptr != NULL) && _should_fcn_have_executed(&_fcn_pntr_buffer[i], cur_long)){
					/* Start iterating through from the beginning */
					if(_fcn_pntr_buffer[i].fcn_ptr != 0){
						_fcn_pntr_buffer[i].fcn_ptr();
					}

					clear_out_fcn_pair(&_fcn_pntr_buffer[i]);

					/* Sort the list */
					SortFunctionPointers();

					/* Decrement the function list */
					_fcn_list_updated--;
					break;
				}
			}
			if(i == -1)
				all_serviced = true;

		}
		while(!all_serviced);
	}
}

uint8_t ShouldSortFunctionPointers(){
	return (uint8_t)_should_sort;
}

