#include "i2c.h"

// by default i2c interface operates in slave mode
void i2c_init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;                /* enables port B gpio on apb2 bus */
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;                                     /* enables i2c1 on apb1 bus */

    // pins we need to use are PB6 and PB7
    // must be Alternate function open drain 
    // CRL for Low pins, CRH for High pins
    // CNF sets Analog mode, Floating input, Input with pullup/pulldown
    // MODE sets input, output
    GPIOB->CRL &= ~((GPIO_CRL_MODE6 | GPIO_CRL_CNF6) | (GPIO_CRL_MODE7 | GPIO_CRL_CNF7));
    GPIOB->CRL |= (GPIO_CRL_MODE6_1 | GPIO_CRL_CNF6_1 | GPIO_CRL_CNF6_0);   /* scl: 10mhz, alt func open drain. MODE6 (01) output mode */
    GPIOB->CRL |= (GPIO_CRL_MODE7_1 | GPIO_CRL_CNF7_1 | GPIO_CRL_CNF7_0);   /* sda: 10mhz, alt func open drain. MODE7 (01) output mode */

    RCC->APB1RSTR |= RCC_APB1RSTR_I2C1RST;                                  /* resets i2c1 */
    RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C1RST;

    I2C1->CR2 |= I2C_CR2_FREQ_3;                                            /* 8mhz */
    I2C1->CCR |= I2C_CCR_CCR;                                               /* I2C_CCR_FS sets fast mode however we want standard mode. I2C_CCR_DUTY is also used for fast mode */
    I2C1->TRISE |= I2C_TRISE_TRISE;

    I2C1->CR1 |= I2C_CR1_PE;                                                /* peripherial enabled */
}

void i2c_start(void) {
    I2C1->CR1 |= I2C_CR1_START;
    while(!(I2C1->SR1 & I2C_SR1_SB));
}

void i2c_stop(void) {
    I2C1->CR1 |= I2C_CR1_STOP;
}

void i2c_address(uint8_t addr, uint8_t write_en) {
    I2C1->DR = (addr << 1) | write_en;                                      /* read or write */
    while (!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2;
}

void i2c_write(uint8_t data) {
    I2C1->DR = data;
    while(!(I2C1->SR1 & I2C_SR1_TXE));                                      /* tx empty */
}

uint8_t i2c_read_ack(void) {
    I2C1->CR1 |= I2C_CR1_ACK;                                               /* enable ack */
    while (!(I2C1->SR1 & I2C_SR1_RXNE));
    return I2C1->DR;
}

uint8_t i2c_read_nack(void) {
    I2C1->CR1 &= ~I2C_CR1_ACK;                                              /* nack */
    I2C1->CR1 |= I2C_CR1_STOP;                                              /* stop condition */
    while (!(I2C1->SR1 & I2C_SR1_RXNE));                                    /* rx empty */
    return I2C1->DR;
}
