#include "gpio.h"
#include "uart.h"
#include "stm32l4xx.h"

#define TEMP_UNLOCK_TIME 5000  // 5 segundos para desbloqueo temporal

// Estados de la puerta
typedef enum {
    DOOR_LOCKED,
    DOOR_TEMP_UNLOCKED,
    DOOR_PERM_UNLOCKED
} DoorState;

// Variables globales
volatile DoorState doorState = DOOR_LOCKED;
volatile uint32_t doorUnlockTime = 0;
volatile uint8_t uartCommandReceived = 0;
volatile char uartCommand = '\0';

// Prototipos de funciones
void handle_button_press(void);
void uart_receive_handler(void);

int main(void)
{
    // Inicialización de periféricos
    gpio_init();
    uart_init();

    // Bucle principal
    while (1)
    {
        // Si recibimos un comando UART
        if (uartCommandReceived)
        {
            uartCommandReceived = 0;
            if (uartCommand == 'O')  // Comando de abrir puerta
            {
                if (doorState == DOOR_LOCKED)
                {
                    doorState = DOOR_TEMP_UNLOCKED;
                    doorUnlockTime = TEMP_UNLOCK_TIME;  // 5 segundos de desbloqueo
                    uart_send("DOOR_UNLOCKED\n");
                    led_door_on();
                }
            }
            else if (uartCommand == 'C')  // Comando de cerrar puerta
            {
                if (doorState != DOOR_LOCKED)
                {
                    doorState = DOOR_LOCKED;
                    uart_send("DOOR_LOCKED\n");
                    led_door_off();
                }
            }
        }

        handle_button_press();  // Revisar los botones

        // Gestionar el estado de la puerta
        if (doorState == DOOR_TEMP_UNLOCKED)
        {
            if (doorUnlockTime > 0)
            {
                doorUnlockTime--;
            }
            else
            {
                doorState = DOOR_LOCKED;
                uart_send("DOOR_LOCKED\n");
                led_door_off();
            }
        }
    }
}

void handle_button_press(void)
{
    static uint32_t buttonPressTime = 0;
    static uint8_t buttonState = 0;  // 0: liberado, 1: presionado

    if ((GPIOC->IDR & (1 << BUTTON_PIN)) == 0)  // Botón presionado
    {
        if (buttonState == 0)
        {
            buttonState = 1;
            buttonPressTime = HAL_GetTick();  // Capturar el tiempo de la primera pulsación
        }
        else if (buttonState == 1 && (HAL_GetTick() - buttonPressTime) > 500)  // Pulsación larga (más de 500ms)
        {
            uart_send("BUTTON_DOUBLE_PRESS\n");
            if (doorState == DOOR_TEMP_UNLOCKED)
            {
                doorState = DOOR_PERM_UNLOCKED;
                uart_send("DOOR_PERM_UNLOCKED\n");
                led_door_on();
            }
        }
    }
    else  // Botón liberado
    {
        buttonState = 0;
    }
}

// Función de recepción UART (en la interrupción de UART)
void uart_receive_handler(void)
{
    if (USART2->ISR & USART_ISR_RXNE)  // Si hay datos recibidos
    {
        uartCommand = USART2->RDR;  // Leer el dato recibido
        uartCommandReceived = 1;    // Marcar como recibido
    }
}

