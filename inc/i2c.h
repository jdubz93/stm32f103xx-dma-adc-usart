#ifndef I2C_H
#define I2C_H

#include "stm32f103xb.h"

void i2c_init(void);
void i2c_start(void);
void i2c_address(uint8_t addr, uint8_t direction);
void i2c_write(uint8_t data);
uint8_t i2c_read_ack(void);
uint8_t i2c_read_nack(void);
void i2c_stop(void);

#endif // I2C_H