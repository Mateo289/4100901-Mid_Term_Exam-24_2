#ifndef UART_H
#define UART_H

#define UART_BAUD_RATE 115200  // Velocidad de UART

// Registros de UART2
#define USART2_CR1    (*(volatile uint32_t *)0x40004400)  // Control 1 de USART2
#define USART2_CR3    (*(volatile uint32_t *)0x4000440C)  // Control 3 de USART2
#define USART2_BRR    (*(volatile uint32_t *)0x40004408)  // Baud rate de USART2
#define USART2_ISR    (*(volatile uint32_t *)0x4000441C)  // Status de USART2
#define USART2_TDR    (*(volatile uint32_t *)0x40004428)  // Transmit Data Register
#define USART2_RDR    (*(volatile uint32_t *)0x40004424)  // Receive Data Register

// Funciones
void uart_init(void);
void uart_send(char *str);
char uart_receive(void);

#endif




