#ifndef __ADC_SAMPLING_H
#define __ADC_SAMPLING_H


#include <stdbool.h>
#include <stdint.h>

#define ADC_SAMPLING_PERIOD		125

void init_strain_ADC(void);

uint32_t GetADCVal(void);

#endif
