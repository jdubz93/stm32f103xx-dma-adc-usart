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
#include "i2c.h"

uint16_t adc_buffer[ADC_BUFFER_SIZE];

#define RELAY_OFF 0x00
#define RELAY_ON 0x01
#define RELAY_ADDR 0x18
#define TOGGLE_RELAY_ONE 0x01
#define TOGGLE_RELAY_TWO 0x02
#define TOGGLE_RELAY_THREE 0x03
#define TOGGLE_RELAY_FOUR 0x04
#define RELAY_ONE_STATUS 0x05
#define RELAY_TWO_STATUS 0x06
#define RELAY_THREE_STATUS 0x07
#define RELAY_FOUR_STATUS 0x08
#define TURN_ALL_OFF 0xA
#define TURN_ALL_ON 11
#define TOGGLE_ALL 12

void delay_ms(volatile unsigned int time) {
    SysTick->LOAD = (8000 * time) - 1;                                      /* 1ms per count */
    SysTick->VAL = 0;                                                       /* clear current value register */          
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;   /* enable systick */

    while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);              /* wait for the COUNTFLAG to be set */

    SysTick->CTRL = 0;                                                      /* disable systick */
}

void print(uint8_t *data, uint8_t size) {
    char buffer[size];
    for (int i = 0; i < size; i++) { 
        snprintf(buffer, sizeof(buffer), "%d\r\n", data[i]);
        usart1_sendstring(buffer);
    }
}

void turn_on_relay(void) {
    i2c_start();
    i2c_address(RELAY_ADDR, 0);
    i2c_write(RELAY_ON);
    i2c_stop();
}

void turn_off_relay(void) {
    i2c_start();
    i2c_address(RELAY_ADDR, 0);
    i2c_write(RELAY_OFF);
    i2c_stop();
}

void relay_status(void) {
    uint8_t data[4];
    i2c_start();
    i2c_address(RELAY_ADDR, 0);
    i2c_write(RELAY_ONE_STATUS);
    i2c_start();
    i2c_address(RELAY_ADDR, 1);
    for (int i = 0; i < 3; i++) {
        data[i] = i2c_read_ack();
    }
    data[3] = i2c_read_nack();
    i2c_stop();
    
    print(data, 4);
}

int main(void) {
    char buffer[10];
    usart1_init();
    adc_init();
    dma_init();
    i2c_init();

    delay_ms(1000);

    usart1_sendstring("app started\r\n");
    
    int toggle = 0;
    while (1) {
        if (toggle) {
            turn_on_relay();
        }
        else {
            turn_off_relay();
        }

        relay_status();
        
        for (int i = 0; i < ADC_BUFFER_SIZE; i++) {
            snprintf(buffer, sizeof(buffer), "%d\r\n", adc_buffer[i]);
            usart1_sendstring(buffer);
        }

        toggle = !toggle;
        delay_ms(1000);
    }

    return 0;
}

