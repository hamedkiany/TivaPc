//*****************************************************************************
//
// freertos_demo.c - Simple FreeRTOS Timers demo
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
// This is part of revision 9453 of the EK-LM4F120XL Firmware Package.
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
#include "timers.h"              // FreeRTOS: definiciones relacionadas con timers

// Variables globales

TimerHandle_t xTimer;

/* An array to hold a count of the number of times each timer expires. */
uint32_t ui32ExpireCounters =  0 ;
int32_t i32Estado_led=0;
//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}

#endif

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
void vApplicationIdleHook( void )
{
    	SysCtlSleep();
    	//SysCtlDeepSleep();
}
//#endif


//*****************************************************************************
//
// This hook is called by FreeRTOS when a timer SW is used
//
//*****************************************************************************

void vTimerCallback( TimerHandle_t pxTimer )
{
const uint32_t xMaxExpiryCountBeforeStopping = 100;

    /* Optionally do something if the pxTimer parameter is NULL. */
    configASSERT( pxTimer );

    /* Change led state */
    i32Estado_led=!i32Estado_led;
    /* update LED status */
    if (i32Estado_led)
    	MAP_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 ,  GPIO_PIN_1);
    else
    	MAP_GPIOPinWrite(GPIO_PORTF_BASE,   GPIO_PIN_1,0);

    /* Increment the number of times that pxTimer has expired. */
    ui32ExpireCounters += 1;

    /* If the timer has expired 10 times then stop it from running. */
    if( ui32ExpireCounters == xMaxExpiryCountBeforeStopping )
    {
        /* Do not use a block time if calling a timer API function from a
        timer callback function, as doing so could cause a deadlock! */
        xTimerStop( pxTimer, 0 );
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
	MAP_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);	//LEDS APAGAOS

    //
    // Print demo introduction.
    //
    UARTprintf("\n\nWelcome to the TIVA EK-TM4C123GXL FreeRTOS Demo!\n");

    // Create and start timer SW
    xTimer = xTimerCreate("TimerSW",0.333 * configTICK_RATE_HZ, pdTRUE,NULL,vTimerCallback);
    if( xTimer == NULL )
             {
                 /* The timer was not created. */
    			while(1);
             }
    else{
        /* Start the timer.  No block time is specified, and even if one was
        it would be ignored because the RTOS scheduler has not yet been
        started. */
        if( xTimerStart( xTimer, 0 ) != pdPASS )
        {
            /* The timer could not be set into the Active state. */
        	while(1);
        }
    }
    //
    // Start the scheduler.  This should not return.
    //
    vTaskStartScheduler();	//el RTOS habilita las interrupciones al entrar aqui, asa que no hace falta habilitarlas

    //
    // In case the scheduler returns for some reason, print an error and loop
    // forever.
    //

    while(1)
    {
    }
}
