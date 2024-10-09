#include <FreeRTOS.h>
#include <task.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"


void vTaskPWMControl(void *pvParameters) {
    while (1) {
        // Controlar el PWM u otras tareas aquí
    }
}

int main(void) {
    // Configurar el PWM antes de iniciar el scheduler
    setupPWM();

    // Crear tareas de FreeRTOS
    xTaskCreate(vTaskPWMControl, "PWMTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    // Iniciar el scheduler de FreeRTOS
    vTaskStartScheduler();

    // No debería llegar aquí
    while (1) {
    }
}
