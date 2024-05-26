/*
    Documentation:
        Reference Manual: www.st.com/resource/en/reference_manual/cd00171190-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf
        Datasheet: www.st.com/resource/en/datasheet/stm32f103xb.pdf 
        Datasheet: www.st.com/en/microcontrollers-microprocessors/stm32f103c8.html
        All: https://www.st.com/en/microcontrollers-microprocessors/stm32f103/documentation.html
*/

// #include "stm32f103xb.h"
#include <stdint.h>
#include <stdio.h>
#include "dma.h"
#include "adc.h"
#include "led.h"
#include "usart.h"


// #define ADC_BUFFER_SIZE 100 
uint16_t adc_buffer[ADC_BUFFER_SIZE];

void delay_ms(volatile unsigned int time)
{
    SysTick->LOAD = (8000 * time) - 1;                                      /* 1ms per count */
    SysTick->VAL = 0;                                                       /* clear current value register */          
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;   /* enable systick */

    while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);              /* wait for the COUNTFLAG to be set */

    SysTick->CTRL = 0;                                                      /* disable systick */
}

// void usart1_init(void)
// {
//     RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;                                     /* enable gpioa clock */
//     RCC->APB2ENR |= RCC_APB2ENR_USART1EN;                                   /* enable usart1 clock */

//     GPIOA->CRH &= ~GPIO_CRH_CNF9;
//     GPIOA->CRH &= ~GPIO_CRH_MODE9;
//     GPIOA->CRH |= GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9_0;                       /* PA9 TX: alt func | output push-pull | 10 MHz */

//     GPIOA->CRH &= ~GPIO_CRH_CNF10;
//     GPIOA->CRH &= ~GPIO_CRH_MODE10;
//     GPIOA->CRH |= GPIO_CRH_CNF10_0;                                         /* PA10 RX: input floating */

//     /*  baudrate calculation:
//             BRR = 9600 / 8000000 (​833.33)
//             baudrate = desired_baudrate / clock_speed (8mhz or 8000000)
//     */

//     // USART1->BRR = 833;                                                   /* baudrate 9600 */
//     USART1->BRR = 69;                                                       /* baudrate 115200 (rounded to nearest int) */

//     USART1->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;               /* enable usart1 both TX / RX */
// }

// void usart1_sendchar(char c)
// {
//     while (!(USART1->SR & USART_SR_TXE));                                   /* waits for TXE (empty) bit to be set */
//     USART1->DR = c;
// }

// void usart1_sendstring(const char* str)
// {
//     while (*str)
//     {
//         usart1_sendchar(*str++);
//     }
// }

// void adc_init(void)
// {
//     RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;                                     /* Enable adc1 clock */
//     RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;                                     /* Enable gpioa clock */

//     /* Configure PA0 as analog input */
//     GPIOA->CRL &= ~GPIO_CRL_CNF0;                                           /* Clear cnf bits for PA0 */
//     GPIOA->CRL &= ~GPIO_CRL_MODE0;                                          /* Clear mode bits for PA0 */

//     /* ADC1 configuration */
//     ADC1->SQR3 = 0;                                                         /* select channel 0 */
//     ADC1->SMPR2 = ADC_SMPR2_SMP0;                                           /* something to do with sampling time */
//     ADC1->CR2 |= ADC_CR2_ADON;                                              /* enable adc1 */
//     ADC1->CR2 |= ADC_CR2_CONT;                                              /* enable continuous conversion mode */
//     ADC1->CR2 |= ADC_CR2_CAL;                                               /* start calibration */

//     while (ADC1->CR2 & ADC_CR2_CAL);                                        /* wait for calibration to complete */

//     ADC1->CR2 |= ADC_CR2_ADON;                                              /* starts first conversion. 
//                                                                             note: in continuous conversion mode, the rest will trigger this automatically so we dont need to in adc_read */
// }

// uint16_t adc_read(void)
// {
//     while (!(ADC1->SR & ADC_SR_EOC));                                       /* wait for end of conversion */
//     return ADC1->DR;                                                        /* return read adc value */
// }

// void led_init(void)
// {
//     RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;                                     /* enable gpio c clock */

//     /* Configure PC13 as output */
//     GPIOC->CRH &= ~GPIO_CRH_MODE13;
//     GPIOC->CRH |= GPIO_CRH_MODE13_0;                                        /* output mode max speed 10mhz */
//     GPIOC->CRH &= ~GPIO_CRH_CNF13;                                          /* clears the CNF13 bits. Thus sets PC13 as general purpose out push pull */
// }

// void led_toggle(void)
// {
//     GPIOC->ODR ^= GPIO_ODR_ODR13;
// }

// void dma_init(void)
// {
//     RCC->AHBENR |= RCC_AHBENR_DMA1EN;                                       /* enable dma1 clock */
//     DMA1_Channel1->CPAR = (uint32_t)&ADC1->DR;                              /* attach adc1 to dma1 */
//     DMA1_Channel1->CMAR = (uint32_t)adc_buffer;                             /* mem addr to store adc data */
//     DMA1_Channel1->CNDTR = ADC_BUFFER_SIZE;                                 /* number of data bits to transfer */
//     DMA1_Channel1->CCR = DMA_CCR_PL | DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_0 | DMA_CCR_MINC | DMA_CCR_CIRC | DMA_CCR_EN;
//     ADC1->CR2 |= ADC_CR2_DMA;                                               /* enables dma mode */
// }

int main(void) {
    char buffer[10];
    usart1_init();
    adc_init();
    dma_init();

    delay_ms(1000);

    usart1_sendstring("app started\r\n");
    
    while (1) {
        for (int i = 0; i < ADC_BUFFER_SIZE; i++) {
            snprintf(buffer, sizeof(buffer), "%d\r\n", adc_buffer[i]);
            usart1_sendstring(buffer);
        }
        // delay_ms(100);
    }

    return 0;
}

