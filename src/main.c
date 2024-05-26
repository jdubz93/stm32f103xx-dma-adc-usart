/*
    Documentation:
        Reference Manual: www.st.com/resource/en/reference_manual/cd00171190-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf
        Datasheet: www.st.com/resource/en/datasheet/stm32f103xb.pdf 
        Datasheet: www.st.com/en/microcontrollers-microprocessors/stm32f103c8.html
        All: https://www.st.com/en/microcontrollers-microprocessors/stm32f103/documentation.html
*/

#include <stdint.h>
#include <stdio.h>
#include "dma.h"
#include "adc.h"
#include "led.h"
#include "usart.h"


uint16_t adc_buffer[ADC_BUFFER_SIZE];

void delay_ms(volatile unsigned int time) {
    SysTick->LOAD = (8000 * time) - 1;                                      /* 1ms per count */
    SysTick->VAL = 0;                                                       /* clear current value register */          
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;   /* enable systick */

    while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);              /* wait for the COUNTFLAG to be set */

    SysTick->CTRL = 0;                                                      /* disable systick */
}

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

