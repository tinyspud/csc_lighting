#ifndef __ADC_SAMPLING_H
#define __ADC_SAMPLING_H


#include <stdbool.h>
#include <stdint.h>


void init_strain_ADC(void);

uint32_t GetADCVal(void);

#endif
