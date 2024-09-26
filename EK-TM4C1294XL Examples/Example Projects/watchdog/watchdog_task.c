/*
 * watchdog_task
 *
 * Copyright (C) 2022 Texas Instruments Incorporated
 * 
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/

/******************************************************************************
 *
 * The application's main function creates a watchdog task which will
 * continuously clear the watchdog's interrupt flag depending on the state of
 * a flag to prevent the watchdog from expiring.  The state of the flag is
 * changed in the ISR when either the SW1 or SW2 switch is pressed.  If the
 * flag is set, the task will continuously clear the watchdog interrupt flag
 * and set LED0 ON permanently.  If the flag is not set, then the timer will
 * expire and trigger an interrupt to toggle LED0.
 *
 * This example uses UARTprintf for output of UART messages.  UARTprintf is not
 * a thread-safe API and is only being used for simplicity of the demonstration
 * and in a controlled manner.
 *
 */

/* Standard includes. */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Hardware includes. */
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/watchdog.h"
#include "drivers/rtos_hw_drivers.h"
#include "utils/uartstdio.h"
/*-----------------------------------------------------------*/

/*
 * The system clock frequency.
 */
extern uint32_t g_ui32SysClock;

/*
 * Flag to tell the vWatchdogTask whether or not to clear the interrupt.
 * This is commonly referred to as 'feeding the watchdog'.
 */
volatile bool g_bFeedWatchdog = false;

/*
 * The tasks as described in the comments at the top of this file.
 */
static void prvWatchdogTask( void *pvParameters );

/*
 * Called by main() to create the Watchdog task.
 */
void vWatchdogTask( void );

/*
 * Configure a button to generate interrupts.
 */
static void prvConfigureButton( void );
/*-----------------------------------------------------------*/

void vWatchdogTask( void )
{
    /* Create the task as described in the comments at the top of this file.
     *
     * The xTaskCreate parameters in order are:
     *  - The function that implements the task.
     *  - The text name Hello processing task - for debug only as it is
     *    not used by the kernel.
     *  - The size of the stack to allocate to the task.
     *  - No parameter passed to the task
     *  - The priority assigned to the task.
     *  - The task handle is NULL */
    xTaskCreate( prvWatchdogTask,
                 "Watchdog",
                 configMINIMAL_STACK_SIZE,
                 NULL,
                 tskIDLE_PRIORITY + 1,
                 NULL );
}
/*-----------------------------------------------------------*/

void prvWatchdogTask( void *pvParameters )
{
    /* Configure a button to generate interrupts. */
    prvConfigureButton();

    /* Enable the peripherals used by this example. */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG0);

    /* Enable the watchdog interrupt. */
    IntEnable(INT_WATCHDOG);

    /* Enable processor interrupts. */
    IntMasterEnable();

    /* Set the period of the watchdog timer to 1 second. */
    WatchdogReloadSet(WATCHDOG0_BASE, g_ui32SysClock);

    /* Enable interrupt generation from the watchdog timer. */
    WatchdogIntEnable(WATCHDOG0_BASE);

    /* Enable the watchdog timer. */
    WatchdogEnable(WATCHDOG0_BASE);

    for (;;)
    {
        /* Prevent Watchdog ISR trigger if button was pushed */
        if (g_bFeedWatchdog)
        {
            WatchdogIntClear(WATCHDOG0_BASE);
        }
    }
}
/*-----------------------------------------------------------*/

static void prvConfigureButton( void )
{
    /* Initialize the LaunchPad Buttons. */
    ButtonsInit();

    /* Configure both switches to trigger an interrupt on a falling edge. */
    GPIOIntTypeSet(BUTTONS_GPIO_BASE, ALL_BUTTONS, GPIO_FALLING_EDGE);

    /* Enable the interrupt for LaunchPad GPIO Port in the GPIO peripheral. */
    GPIOIntEnable(BUTTONS_GPIO_BASE, ALL_BUTTONS);

    /* Enable the Port J interrupt in the NVIC. */
    IntEnable(INT_GPIOJ);
}
/*-----------------------------------------------------------*/

void xWatchdogHandler( void )
{
uint32_t ui32LEDState;

    /* Clear the watchdog interrupt. */
    WatchdogIntClear(WATCHDOG0_BASE);

    /* Toggle LED1 on each time through the loop. */
    LEDRead(&ui32LEDState);
    LEDWrite(LED_D1, (ui32LEDState ^ LED_D1));
}
/*-----------------------------------------------------------*/

void xButtonsHandler( void )
{
uint32_t ui32Status;

    /* Read the interrupt status to find the cause of the interrupt. */
    ui32Status = GPIOIntStatus(BUTTONS_GPIO_BASE, true);

    /* Clear the interrupt. */
    GPIOIntClear(BUTTONS_GPIO_BASE, ui32Status);

    /* Toggle the flag.  When the flag is true, it will prevent watchdog from
     * expiring. */
    g_bFeedWatchdog ^= true;

    /* Keep the LED on. */
    LEDWrite(LED_D1, LED_D1);
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
    /* This function will be called by each tick interrupt if
        configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
        added here, but the tick hook is called from an interrupt context, so
        code must not attempt to block, and only the interrupt safe FreeRTOS API
        functions can be used (those that end in FromISR()). */

    /* Only the full demo uses the tick hook so there is no code is
        executed here. */
}


