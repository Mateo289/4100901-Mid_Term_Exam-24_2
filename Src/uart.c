#include "uart.h"
#include "stm32l4xx.h"

void uart_init(void)
{
    // Habilitar reloj para USART2
    RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;

    // Configuración de USART2: 115200 baudios
    USART2->BRR = 0x8B;  // 115200 baudios (16 MHz / 115200)

    // Configuración de USART2: Habilitar transmisión y recepción
    USART2->CR1 |= USART_CR1_RE | USART_CR1_TE;  // Habilitar RX y TX
    USART2->CR1 |= USART_CR1_UE;  // Habilitar USART2

    // Habilitar interrupciones por recepción
    USART2->CR1 |= USART_CR1_RXNEIE;  // Habilitar interrupción por recepción de datos
    NVIC_EnableIRQ(USART2_IRQn);      // Habilitar interrupción USART2 en el NVIC
}

void uart_send(const char* data)
{
    while (*data)
    {
        while (!(USART2->ISR & USART_ISR_TXE)) { }  // Esperar a que TXE esté listo
        USART2->TDR = *data++;  // Enviar carácter
    }
}
