#ifndef GPIO_H
#define GPIO_H

// Definición de pines
#define BUTTON_PIN 13     // Botón en PC13
#define LED_PIN 5         // LED en PA5
#define BELL_PIN 6        // Timbre en PA6

// Registros para GPIO
#define GPIOA_MODER     (*(volatile uint32_t *)0x48000000)  // Modo de GPIOA
#define GPIOA_OTYPER    (*(volatile uint32_t *)0x48000004)  // Tipo de salida de GPIOA
#define GPIOA_ODR       (*(volatile uint32_t *)0x48000014)  // Datos de salida de GPIOA

#define GPIOC_MODER     (*(volatile uint32_t *)0x48000800)  // Modo de GPIOC
#define GPIOC_IDR       (*(volatile uint32_t *)0x48000810)  // Datos de entrada de GPIOC

// Funciones
void gpio_init(void);
void led_door_on(void);
void led_door_off(void);
void heartbeat_led_toggle(void);

#endif


