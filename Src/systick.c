#include "systick.h"

// Definición de la estructura SysTick_t para acceder a los registros
typedef struct {
    volatile uint32_t CTRL;   // Control register
    volatile uint32_t LOAD;   // Reload register
    volatile uint32_t VAL;    // Current value register
    volatile uint32_t CALIB;  // Calibration value (opcional)
} SysTick_t;

#define SysTick ((SysTick_t *)0xE000E010) // Dirección base de SysTick

// Definición de variable global para el contador de milisegundos
volatile uint32_t ms_counter = 0;

// Configuración y habilitación de SysTick
void configure_systick_and_start(void) {
    // Deshabilitar SysTick para configuración
    SysTick->CTRL = 0;

    // Configurar el valor de recarga para 1 ms (suponiendo que HCLK es de 80 MHz)
    // El valor de recarga es calculado como: (HCLK / 1000) - 1
    // Si HCLK = 80 MHz, entonces: (80,000,000 / 1000) - 1 = 79999
    SysTick->LOAD = 79999;   // Esto se configura para 1 ms de interrupción

    // Limpiar el contador del SysTick (inicializar a cero)
    SysTick->VAL = 0;

    // Habilitar SysTick con el reloj del procesador (HCLK) y habilitar interrupciones
    SysTick->CTRL = 0x07;  // 0x07 = Habilitar SysTick, habilitar interrupción
}

// Función para obtener el contador de milisegundos
uint32_t systick_GetTick(void) {
    return ms_counter;
}

// Función para reiniciar el contador de milisegundos
void systick_reset(void) {
    ms_counter = 0;
}

// Interrupción de SysTick que se ejecuta cada vez que el temporizador llega a cero
void SysTick_Handler(void) {
    ms_counter++;  // Incrementar el contador de milisegundos
}
