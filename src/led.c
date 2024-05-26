#include "led.h"

void led_init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;                                     /* enable gpio c clock */

    /* Configure PC13 as output */
    GPIOC->CRH &= ~GPIO_CRH_MODE13;
    GPIOC->CRH |= GPIO_CRH_MODE13_0;                                        /* output mode max speed 10mhz */
    GPIOC->CRH &= ~GPIO_CRH_CNF13;                                          /* clears the CNF13 bits. Thus sets PC13 as general purpose out push pull */
}

void led_toggle(void) {
    GPIOC->ODR ^= GPIO_ODR_ODR13;
}