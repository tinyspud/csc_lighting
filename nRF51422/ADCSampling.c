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


uint32_t _adc_val = 0;

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
	
	//Use the STOP task to save current. Workaround for PAN_028 rev1.5 anomaly 1.
	NRF_ADC->TASKS_STOP = 1;
	
	//Release the external crystal
//	sd_clock_hfclk_release();
}


uint32_t GetADCVal(void){
	return _adc_val;
}


