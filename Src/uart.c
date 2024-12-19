#include "uart.h"

// Inicializar UART2 con una velocidad de 115200 baudios
void uart_init(void)
{
    // Habilitar el reloj para USART2 (RCC_APB1ENR1, Dirección: 0x40021000)
    *(volatile uint32_t *)0x40021000 |= (1 << 17);  // Habilitar USART2

    // Configurar baud rate
    uint32_t uart_baud_rate_value = 80000000 / UART_BAUD_RATE;  // Suponiendo reloj de 80 MHz
    USART2_BRR = uart_baud_rate_value;

    // Habilitar TX y RX, y USART
    USART2_CR1 |= (1 << 2);  // Habilitar receptor (RE)
    USART2_CR1 |= (1 << 3);  // Habilitar transmisor (TE)
    USART2_CR1 |= (1 << 13); // Habilitar USART (UE)
}

// Enviar un string por UART
void uart_send(char *str)
{
    while (*str)
    {
        // Esperar hasta que el buffer de transmisión esté vacío
        while (!(USART2_ISR & (1 << 7)))  // Verificar si TXE está establecido
            ;
        USART2_TDR = *str++;  // Enviar el siguiente carácter
    }
}

// Recibir un carácter por UART
char uart_receive(void)
{
    // Esperar hasta que haya datos disponibles
    while (!(USART2_ISR & (1 << 5)))  // Verificar si RXNE está establecido
        ;
    return USART2_RDR;  // Leer el carácter recibido
}

