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

#include "LEDs.h"
#include "system_ADC_LL.h"

void system_ADC_LL_init(){
	NRF_ADC->CONFIG = (ADC_CONFIG_RES_10bit << ADC_CONFIG_RES_Pos) |
		(ADC_CONFIG_INPSEL_SupplyOneThirdPrescaling << ADC_CONFIG_INPSEL_Pos) |
		(ADC_CONFIG_REFSEL_VBG << ADC_CONFIG_REFSEL_Pos) |
		(ADC_CONFIG_PSEL_AnalogInput3 << ADC_CONFIG_PSEL_Pos) |
		(ADC_CONFIG_EXTREFSEL_None << ADC_CONFIG_EXTREFSEL_Pos);
	NRF_ADC->EVENTS_END = 0;
	NRF_ADC->ENABLE = ADC_ENABLE_ENABLE_Enabled;

	NRF_ADC->EVENTS_END = 0;    // Stop any running conversions.
	NRF_ADC->TASKS_START = 1;

	while (!NRF_ADC->EVENTS_END)
	{
	}

	uint16_t vbg_in_mv = 1200;
	uint8_t adc_max = 255;
	uint16_t vbat_current_in_mv = (NRF_ADC->RESULT * 3 * vbg_in_mv) / adc_max;

	NRF_ADC->EVENTS_END = 0;
	NRF_ADC->TASKS_STOP = 1;


}



