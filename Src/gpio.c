#include "gpio.h"

void gpio_init(void)
{
    // Habilitar reloj para GPIOA, GPIOB, y GPIOC
    RCC->AHB2ENR |= (1 << 0) | (1 << 1) | (1 << 2); // GPIOA, GPIOB, GPIOC

    // Configuración de PA5 (LED de latido)
    GPIOA->MODER &= ~(3 << (LED_HEARTBEAT_PIN * 2));  // Limpiar bits
    GPIOA->MODER |= (1 << (LED_HEARTBEAT_PIN * 2));   // Configurar como salida

    // Configuración de PB0 (LED de estado de la puerta)
    GPIOB->MODER &= ~(3 << (LED_DOOR_PIN * 2));  // Limpiar bits
    GPIOB->MODER |= (1 << (LED_DOOR_PIN * 2));   // Configurar como salida

    // Configuración de PC13 (Botón de abrir/cerrar puerta)
    GPIOC->MODER &= ~(3 << (BUTTON_PIN * 2));  // Configurar como entrada
}

void led_heartbeat_toggle(void)
{
    GPIOA->ODR ^= (1 << LED_HEARTBEAT_PIN);  // Alternar el LED de latido
}

void led_door_toggle(void)
{
    GPIOB->ODR ^= (1 << LED_DOOR_PIN);  // Alternar el LED de estado de la puerta
}

void led_door_on(void)
{
    GPIOB->ODR |= (1 << LED_DOOR_PIN);  // Enciende el LED de estado de la puerta
}

void led_door_off(void)
{
    GPIOB->ODR &= ~(1 << LED_DOOR_PIN);  // Apaga el LED de estado de la puerta
}
