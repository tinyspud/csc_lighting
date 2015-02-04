#ifndef __ADC_SAMPLING_H
#define __ADC_SAMPLING_H


#include <stdbool.h>
#include <stdint.h>

#define ADC_SAMPLING_FREQ		(512)
#define ADC_INDICATOR_LED		LED_RED
#define ADC_ACTIVE_BUFF_LED		LED_YELLOW

#define ADC_BUFF_LEN	256	/* Sampling at ~500 Hz means this fills up in 0.5s */


void init_strain_ADC(void);

uint32_t GetADCVal(void);

#endif
