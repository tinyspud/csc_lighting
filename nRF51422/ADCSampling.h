#ifndef __ADC_SAMPLING_H
#define __ADC_SAMPLING_H


#include <stdbool.h>
#include <stdint.h>

#define ADC_SAMPLING_PERIOD		125
#define ADC_INDICATOR_LED		LED_RED

#define ADC_BUFF_LEN	16


void init_strain_ADC(void);

uint32_t GetADCVal(void);

#endif
