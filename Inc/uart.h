#ifndef UART_H
#define UART_H

#include "stm32l4xx.h"

// Funciones públicas
void uart_init(void);
void uart_send(const char* data);
void uart_receive(void);

#endif // UART_H
