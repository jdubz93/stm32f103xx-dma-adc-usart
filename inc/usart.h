#ifndef USART_H
#define USART_H

#include "stm32f103xb.h"

void usart1_init(void);
void usart1_sendchar(char c);
void usart1_sendstring(const char* str);

#endif // USART_H