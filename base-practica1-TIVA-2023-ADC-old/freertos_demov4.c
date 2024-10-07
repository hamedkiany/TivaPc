//*****************************************************************************
//
// freertos_demo.c - Simple FreeRTOS example.v4
//
// Copyright (c) 2019 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.0.1.11577 of the EK-TM4C123GXL Firmware Package.
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"       // TIVA: Definiciones del mapa de memoria
#include "inc/hw_types.h"        // TIVA: Definiciones API
#include "inc/hw_ints.h"         // TIVA: Definiciones para configuracion de interrupciones
#include "driverlib/gpio.h"      // TIVA: Funciones API de GPIO
#include "driverlib/pin_map.h"   // TIVA: Mapa de pines del chip
#include "driverlib/rom.h"       // TIVA: Funciones API incluidas en ROM de micro (MAP_)
#include "driverlib/rom_map.h"   // TIVA: Mapeo automatico de funciones API incluidas en ROM de micro (MAP_)
#include "driverlib/sysctl.h"    // TIVA: Funciones API control del sistema
#include "driverlib/uart.h"      // TIVA: Funciones API manejo UART
#include "driverlib/interrupt.h" // TIVA: Funciones API manejo de interrupciones
#include "utils/uartstdio.h"     // TIVA: Funciones API UARTSTDIO (printf)
#include "drivers/buttons.h"     // TIVA: Funciones API manejo de botones
#include "FreeRTOS.h"            // FreeRTOS: definiciones generales
#include "task.h"                // FreeRTOS: definiciones relacionadas con tareas
#include <semphr.h>

// Definiciones de tareas
#define LED1TASKPRIO 1           // Prioridad para la tarea LED1TASK
#define LED1TASKSTACKSIZE 128    // Tamaño de pila para la tarea LED1TASK
#define LED2TASKPRIO 1           // Prioridad para la tarea LED2TASK
#define LED2TASKSTACKSIZE 128    // Tamaño de pila para la tarea LED2TASK
#define SW1TASKPRIO 1            // Prioridad para la tarea SW1TASK
#define SW1TASKSTACKSIZE 128     // Tamaño de pila para la tarea SW1TASK

// Variables globales "main"

//Importante: Los arrays con los parametros tengo que poner como globales !!!
// por que? se pasan por referencia a la funcion TaskCreate,
// como son variables locales automaticas, cuando se llega a ajecutar la tarea
// la tarea la memoria reservada para estas variables ya no existe (el compilador interpreta que ya no se usan tras llamar
// a task create  (falla incluso sin optimizacion y aunque se copien al comenzar la tarea).
SemaphoreHandle_t miSemaforo;
static uint32_t pui32Parametros1[2];
static uint32_t pui32Parametros2[2];

//Variables globales tarea Switch1Task + interrupcion boton
volatile int32_t i32Semaforo_malo1=0;

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
/*
#ifdef DEBUG
void __error__(char *pcFilename, unsigned long ulLine)
{
}

#endif
*/
//*****************************************************************************
//
// This hook is called by FreeRTOS when an stack overflow error is detected.
//
//*****************************************************************************
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    //
    // This function can not return, so loop forever.  Interrupts are disabled
    // on entry to this function, so no processor interrupts will interrupt
    // this loop.
    //
    while(1)
    {
    }
}


//#if 0
//NO PROBAR HASTA HABER PUESTO EL SEMAFORO Y LOS DELAYS bien!!!!
void vApplicationIdleHook( void )
{
    	SysCtlSleep();
    	//SysCtlDeepSleep();
}
//#endif

//*****************************************************************************
//
// This task toggles the user selected LED at a user selected frequency.
//
//*****************************************************************************

//static void LEDTask(void *pvParameters)
//Se pueden crear varias tareas de este tipo porque NO USA variables globales ni locales de tipo static
// ---> La funcion es "reentrante" --> Se puede llamar de forma recursiva o a varias instancias desde distintas tareas
static portTASK_FUNCTION(LEDTask,pvParameters)
{
    int32_t i32Estado_led=0;
    TickType_t ui32LastTime;
    //pvParameters es un puntero que se le pasa a la funcion vTaskCreate.
    uint32_t ui32Puerto=((uint32_t *)pvParameters)[1];
    uint32_t ui32Intervalo=((uint32_t *)pvParameters)[0];

    //
    // Get the current tick count.
    //
    ui32LastTime = xTaskGetTickCount();

    //
    // Loop forever.
    //
    while(1)
    {
        vTaskDelayUntil(&ui32LastTime, ui32Intervalo); //Espera del RTOS (eficiente, no gasta CPU)
        i32Estado_led=!i32Estado_led;
        if (i32Estado_led)
        	MAP_GPIOPinWrite(GPIO_PORTF_BASE, ui32Puerto , ui32Puerto);
        else
            MAP_GPIOPinWrite(GPIO_PORTF_BASE,  ui32Puerto,0);
    }
}

//*****************************************************************************
//
// This task waits for button pushed and then flash the green Led for 1seg
//
//*****************************************************************************

//static void Switch1Task(void *pvParameters)
static portTASK_FUNCTION(Switch1Task,pvParameters)
{
	i32Semaforo_malo1=0;
    //
    // Loop forever.
    //
    while(1)
    {
    	//Semaforo mal implementado (ineficiente)
  //  	i32Semaforo_malo1=0;
  //  	while (!i32Semaforo_malo1); // Espera hasta que alguien abra el semaforo



    	xSemaphoreTake(miSemaforo,portMAX_DELAY);
    	MAP_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3,GPIO_PIN_3);
    	vTaskDelay(1000); // Espera 1000 ticks encendido
    	MAP_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3,0);
    	vTaskDelay(500); // Espera 500 ticks apagado
    }
}

//*****************************************************************************
//
// Initialize FreeRTOS and start the initial set of tasks.
//
//*****************************************************************************
int main(void)
{

	//
	// Set the clocking to run at 40 MHz from the PLL.
	//
	MAP_SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
			SYSCTL_OSC_MAIN);

	//
	// Initialize the UART and configure it for 115,200, 8-N-1 operation.
	//
	// se usa para mandar mensajes por el puerto serie
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	MAP_GPIOPinConfigure(GPIO_PA0_U0RX);
	MAP_GPIOPinConfigure(GPIO_PA1_U0TX);
	MAP_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTStdioConfig(0, 115200, SysCtlClockGet());

	//Inicializa el puerto F (LEDs)
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	MAP_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);	//LEDS APAGADOS

    //Inicializa los botones (tambien en el puerto F) y habilita sus interrupciones
    ButtonsInit();
    MAP_GPIOIntTypeSet(GPIO_PORTF_BASE, ALL_BUTTONS,GPIO_FALLING_EDGE);
    MAP_IntPrioritySet(INT_GPIOF,configMAX_SYSCALL_INTERRUPT_PRIORITY);//para añadir prioridad by HAMED
    MAP_GPIOIntEnable(GPIO_PORTF_BASE,ALL_BUTTONS);
    MAP_IntEnable(INT_GPIOF);

    //
    // Print demo introduction.
    //
    UARTprintf("\n\nWelcome to the TIVA EK-TM4C123GXL FreeRTOS Demo!\n");
    miSemaforo = xSemaphoreCreateCounting(20,0);
    //
    // Create a LED task (only when #if & #endif directives are commented out)
    //

    pui32Parametros1[0]=0.5 * configTICK_RATE_HZ;  // 0.5 segundos
    pui32Parametros1[1]=GPIO_PIN_1;
//#if 0
    if((xTaskCreate(LEDTask, "Led1", LED1TASKSTACKSIZE, pui32Parametros1,tskIDLE_PRIORITY + LED1TASKPRIO, NULL) != pdTRUE))
        {
            while(1)
            {
            }
        }
//#endif

    //
    // Create another LED task (only when #if & #endif directives are commented out)
    //

//#if 0
    pui32Parametros2[0]=0.5 * configTICK_RATE_HZ;  // 0.5 segundos
    pui32Parametros2[1]=GPIO_PIN_2;
    if((xTaskCreate(LEDTask, "Led2", LED2TASKSTACKSIZE, pui32Parametros2,tskIDLE_PRIORITY + LED2TASKPRIO, NULL) != pdTRUE))
        {
            while(1)
            {
            }
        }
//#endif

    //
    // Create the switch1 task.
    //
//#if 0
    if((xTaskCreate(Switch1Task, "Sw1",SW1TASKSTACKSIZE, NULL, tskIDLE_PRIORITY + SW1TASKPRIO, NULL) != pdTRUE))
    {
        while(1)
        {
        }
    }
//#endif

    //
    // Start the scheduler.  This should not return.
    //
    vTaskStartScheduler();	//el RTOS habilita las interrupciones al entrar aqui, asi que no hace falta habilitarlas

    //
    // In case the scheduler returns for some reason, print an error and loop
    // forever.
    //

    while(1)
    {
    }
}


// Rutinas de interrupcion
void GPIOFIntHandler(void){
	//Lee el estado del puerto (activos a nivel bajo)

	int32_t i32PinStatus=MAP_GPIOPinRead(GPIO_PORTF_BASE,ALL_BUTTONS);
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if (!(i32PinStatus & LEFT_BUTTON))
	{
		//i32Semaforo_malo1=1;	//Activa la variable global semaforo
	    xSemaphoreGiveFromISR(miSemaforo,&xHigherPriorityTaskWoken);
	}
	MAP_GPIOIntClear(GPIO_PORTF_BASE,ALL_BUTTONS);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
