#include "gpio.h"

// Inicializar los pines GPIO
void gpio_init(void)
{
    // Habilitar el reloj para GPIOA y GPIOC en el registro RCC (Dirección: 0x40021000)
    *(volatile uint32_t *)0x40021000 |= (1 << 17);  // Habilitar el reloj de GPIOA (bit 17)
    *(volatile uint32_t *)0x40021000 |= (1 << 19);  // Habilitar el reloj de GPIOC (bit 19)

    // Configurar PA5 (LED de la puerta) como salida
    GPIOA_MODER &= ~(0x3 << (LED_PIN * 2));  // Limpiar bits
    GPIOA_MODER |= (0x1 << (LED_PIN * 2));   // Configurar como salida

    // Configurar PA6 (Timbre) como salida
    GPIOA_MODER &= ~(0x3 << (BELL_PIN * 2));  // Limpiar bits
    GPIOA_MODER |= (0x1 << (BELL_PIN * 2));   // Configurar como salida

    // Configurar PC13 (Botón) como entrada
    GPIOC_MODER &= ~(0x3 << (BUTTON_PIN * 2));  // Limpiar bits para modo entrada
}

// Encender el LED (PA5)
void led_door_on(void)
{
    GPIOA_ODR |= (1 << LED_PIN);  // Establecer el bit correspondiente al pin PA5 (LED encendido)
}

// Apagar el LED (PA5)
void led_door_off(void)
{
    GPIOA_ODR &= ~(1 << LED_PIN);  // Borrar el bit correspondiente al pin PA5 (LED apagado)
}

// Alternar el estado del LED de latido (PA5)
void heartbeat_led_toggle(void)
{
    GPIOA_ODR ^= (1 << LED_PIN);  // Cambiar el estado del pin PA5 (LED alterna)
}








