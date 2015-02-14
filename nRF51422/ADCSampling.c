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
#include "custom_board.h"

#include "LEDs.h"
#include "ADCSampling.h"

uint32_t _adc_val = 0;

typedef enum active_adc_buffer{
	adc_buffer0 = 0,
	adc_buffer1 = 1,
}active_adc_buffer_t;

static volatile int active_buff_idx = 0;

static volatile active_adc_buffer_t active_buff_num;

uint32_t _adc_buffer_0[ADC_BUFF_LEN];
uint32_t _adc_buffer_1[ADC_BUFF_LEN];

void init_strain_ADC(void){
	/* Enable interrupt on ADC sample ready event*/		
	NRF_ADC->INTENSET = ADC_INTENSET_END_Msk;   
	sd_nvic_SetPriority(ADC_IRQn, NRF_APP_PRIORITY_LOW);  
	sd_nvic_EnableIRQ(ADC_IRQn);
	
	NRF_ADC->CONFIG	= (ADC_CONFIG_EXTREFSEL_AnalogReference0 << ADC_CONFIG_EXTREFSEL_Pos) /* Bits 17..16 : ADC external reference pin selection. */
					| (STRAIN_GAUGE_PIN << ADC_CONFIG_PSEL_Pos)					/*!< Use analog input 3 as analog input. */
					| (ADC_CONFIG_REFSEL_External << ADC_CONFIG_REFSEL_Pos)							/*!< Use external voltage at pin REF0 as reference for conversion. */
					| (ADC_CONFIG_INPSEL_AnalogInputNoPrescaling << ADC_CONFIG_INPSEL_Pos) /*!< Analog input specified by PSEL with no prescaling used as input for the conversion. */
					| (ADC_CONFIG_RES_10bit << ADC_CONFIG_RES_Pos);									/*!< 10bit ADC resolution. */ 
	
	/* Enable ADC*/
	NRF_ADC->ENABLE = ADC_ENABLE_ENABLE_Enabled;
	
	_adc_val = 0;
	
	int i = 0;
	for(i = 0; i < ADC_BUFF_LEN; i++){
		_adc_buffer_0[i] = 0;
		_adc_buffer_1[i] = 0;
	}

	active_buff_num = adc_buffer0;
	active_buff_idx = 0;
}


void start_ADC(void){
	
}

/* Interrupt handler for ADC data ready event */
void ADC_IRQHandler(void)
{
	/* Clear dataready event */
	NRF_ADC->EVENTS_END = 0;	

	/* Write ADC result to port 2 */
	_adc_val = NRF_ADC->RESULT;

	switch(active_buff_num){
		default:
			active_buff_num = adc_buffer0;
		case adc_buffer0:
			_adc_buffer_0[active_buff_idx] = _adc_val;
			break;
		case adc_buffer1:
			_adc_buffer_1[active_buff_idx] = _adc_val;
			break;
	}
	
	active_buff_idx++;
	if(active_buff_idx >= ADC_BUFF_LEN){
		active_buff_idx = 0;
		/* Switch buffers */
		active_buff_num = (active_buff_num == adc_buffer0) ? adc_buffer1 : adc_buffer0;
		if(active_buff_num == adc_buffer1)
			LED_TURN_ON(ADC_ACTIVE_BUFF_LED);
		else
			LED_TURN_OFF(ADC_ACTIVE_BUFF_LED);
	}
	
	LED_TURN_OFF(ADC_INDICATOR_LED);
	
	//Use the STOP task to save current. Workaround for PAN_028 rev1.5 anomaly 1.
	NRF_ADC->TASKS_STOP = 1;
	
}


uint32_t GetADCVal(void){
	return _adc_val;
}


