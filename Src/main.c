#include "gpio.h"
#include "uart.h"
#include "stm32l476xx.h"

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
volatile uint32_t tickCount = 0;  // Contador de tiempo para el heartbeat y temporizador

// Prototipo de la función
void handle_button_press(void);
void heartbeat(void);

int main(void)
{
    // Inicialización de periféricos
    gpio_init();
    uart_init();

    // Habilitar el temporizador SysTick para crear un retraso de 1ms
    SysTick->LOAD = 7999;             // Configurar para 1 ms (con un reloj de 80 MHz)
    SysTick->VAL = 0;                 // Limpiar el contador
    SysTick->CTRL = 0x5;              // Habilitar SysTick con el reloj del sistema (80 MHz)

    // Bucle principal
    while (1)
    {
        heartbeat();  // Llamada para el LED de latido

        // Si hemos recibido un comando UART
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

// Manejar la pulsación del botón (sin funciones HAL)
void handle_button_press(void)
{
    static uint32_t buttonPressTime = 0;
    static uint8_t buttonState = 0;  // 0: liberado, 1: presionado

    // Leer el estado del botón (PC13)
    if ((GPIOC_IDR & (1 << BUTTON_PIN)) == 0)  // Botón presionado
    {
        if (buttonState == 0)
        {
            buttonState = 1;
            buttonPressTime = tickCount;  // Guardar el tiempo de la pulsación
        }
    }
    else  // Botón liberado
    {
        if (buttonState == 1)
        {
            buttonState = 0;

            // Calcular el tiempo que el botón estuvo presionado
            uint32_t pressDuration = tickCount - buttonPressTime;

            if (pressDuration < 1000)  // Pulsación corta (< 1 segundo)
            {
                if (doorState == DOOR_LOCKED)
                {
                    doorState = DOOR_TEMP_UNLOCKED;
                    doorUnlockTime = TEMP_UNLOCK_TIME;
                    uart_send("DOOR_UNLOCKED\n");
                    led_door_on();
                }
            }
            else  // Pulsación larga (> 1 segundo)
            {
                if (doorState == DOOR_LOCKED)
                {
                    doorState = DOOR_PERM_UNLOCKED;
                    uart_send("DOOR_UNLOCKED_PERM\n");
                    led_door_on();
                }
            }
        }
    }
}

// Función para manejar el parpadeo del LED de latido (heartbeat)
void heartbeat(void)
{
    static uint32_t lastTick = 0;

    if (tickCount - lastTick >= 500)  // Parpadeo cada 500 ms
    {
        lastTick = tickCount;
        heartbeat_led_toggle();  // Alternar el LED de latido
    }
}

// Interrupción del temporizador SysTick para manejar el tiempo de reloj
void SysTick_Handler(void)
{
    tickCount++;  // Incrementar el contador de tiempo
}

