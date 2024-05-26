#ifndef DMA_H
#define DMA_H

#include "stm32f103xb.h"
#include "adc.h"

#define ADC_BUFFER_SIZE 100

extern uint16_t adc_buffer[ADC_BUFFER_SIZE];

void dma_init(void);

#endif // DMA_H