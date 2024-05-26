#include "dma.h"

void dma_init(void) {
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;                                       /* enable dma1 clock */
    DMA1_Channel1->CPAR = (uint32_t)&ADC1->DR;                              /* attach adc1 to dma1 */
    DMA1_Channel1->CMAR = (uint32_t)adc_buffer;                             /* mem addr to store adc data */
    DMA1_Channel1->CNDTR = ADC_BUFFER_SIZE;                                 /* number of data bits to transfer */
    DMA1_Channel1->CCR = DMA_CCR_PL | DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_0 | DMA_CCR_MINC | DMA_CCR_CIRC | DMA_CCR_EN;
    ADC1->CR2 |= ADC_CR2_DMA;                                               /* enables dma mode */
}