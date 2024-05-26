#include "adc.h"

void adc_init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;                                     /* Enable adc1 clock */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;                                     /* Enable gpioa clock */

    /* Configure PA0 as analog input */
    GPIOA->CRL &= ~GPIO_CRL_CNF0;                                           /* Clear cnf bits for PA0 */
    GPIOA->CRL &= ~GPIO_CRL_MODE0;                                          /* Clear mode bits for PA0 */

    /* ADC1 configuration */
    ADC1->SQR3 = 0;                                                         /* select channel 0 */
    ADC1->SMPR2 = ADC_SMPR2_SMP0;                                           /* something to do with sampling time */
    ADC1->CR2 |= ADC_CR2_ADON;                                              /* enable adc1 */
    ADC1->CR2 |= ADC_CR2_CONT;                                              /* enable continuous conversion mode */
    ADC1->CR2 |= ADC_CR2_CAL;                                               /* start calibration */

    while (ADC1->CR2 & ADC_CR2_CAL);                                        /* wait for calibration to complete */

    ADC1->CR2 |= ADC_CR2_ADON;                                              /* starts first conversion. 
                                                                            note: in continuous conversion mode, the rest will trigger this automatically so we dont need to in adc_read */
}

uint16_t adc_read(void) {
    while (!(ADC1->SR & ADC_SR_EOC));                                       /* wait for end of conversion */
    return ADC1->DR;                                                        /* return read adc value */
}