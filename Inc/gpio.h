#ifndef GPIO_H
#define GPIO_H

#include "stm32l4xx.h"

// Definiciones de pines
#define LED_HEARTBEAT_PIN    5   // PA5
#define LED_DOOR_PIN          0   // PB0
#define BUTTON_PIN            13  // PC13

// Funciones públicas
void gpio_init(void);
void led_heartbeat_toggle(void);
void led_door_toggle(void);
void led_door_on(void);
void led_door_off(void);

#endif // GPIO_H

