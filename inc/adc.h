#ifndef ADC_H
#define ADC_H

#include "stm32f103xb.h"

void adc_init(void);
uint16_t adc_read(void);

#endif // ADC_H