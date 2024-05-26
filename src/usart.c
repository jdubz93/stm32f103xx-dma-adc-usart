#include "usart.h"

void usart1_init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;                                     /* enable gpioa clock */
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;                                   /* enable usart1 clock */

    GPIOA->CRH &= ~GPIO_CRH_CNF9;
    GPIOA->CRH &= ~GPIO_CRH_MODE9;
    GPIOA->CRH |= GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9_0;                       /* PA9 TX: alt func | output push-pull | 10 MHz */

    GPIOA->CRH &= ~GPIO_CRH_CNF10;
    GPIOA->CRH &= ~GPIO_CRH_MODE10;
    GPIOA->CRH |= GPIO_CRH_CNF10_0;                                         /* PA10 RX: input floating */

    /*  baudrate calculation:
            BRR = 9600 / 8000000 (​833.33)
            baudrate = desired_baudrate / clock_speed (8mhz or 8000000)
    */

    // USART1->BRR = 833;                                                   /* baudrate 9600 */
    USART1->BRR = 69;                                                       /* baudrate 115200 (rounded to nearest int) */
    USART1->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;               /* enable usart1 both TX / RX */
}

void usart1_sendchar(char c) {
    while (!(USART1->SR & USART_SR_TXE));                                   /* waits for TXE (empty) bit to be set */
    USART1->DR = c;
}

void usart1_sendstring(const char* str) {
    while (*str) usart1_sendchar(*str++);
    // {
    //     usart1_sendchar(*str++);
    // }
}