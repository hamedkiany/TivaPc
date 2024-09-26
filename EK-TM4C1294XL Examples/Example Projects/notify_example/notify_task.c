/*
 * notify_task
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
 * This project demonstrates how to configure the TM4C1294NCPDT to toggle a LED
 * using FreeRTOS notification.  One task is created to toggle a LED. The task
 * waits for for the notification emitted in the ISR when either SW1 or SW2
 * button is pressed.  Each press on either the SW1 or SW2 button will
 * sequence through different color of LED on the board.
 *
 */

/* Standard includes. */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Hardware includes. */
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "drivers/rtos_hw_drivers.h"
#include "utils/uartstdio.h"
/*-----------------------------------------------------------*/

/*
 * Time stamp global variable.
 */
volatile uint32_t g_ui32TimeStamp = 0;

/*
 * The notification used by the task.
 */
TaskHandle_t xTaskLEDOnHandle = NULL;

/*
 * The tasks as described in the comments at the top of this file.
 */
static void prvTaskLed( void *pvParameters );

/*
 * Called by main() to create the simple blinky style application.
 */
void vBlinkyTask( void );

/*
 * Hardware configuration for the LEDs.
 */
static void prvConfigureLED( void );

/*
 * Hardware configuration for the buttons SW1 and SW2 to generate interrupts.
 */
static void prvConfigureButton( void );
/*-----------------------------------------------------------*/

void vBlinkyTask( void )
{
    /* Set an initial LED. */
    prvConfigureLED();

    /* Configure a button to generate interrupts. */
    prvConfigureButton();

    /* Create the task as described in the comments at the top of this file.
     *
     * The xTaskCreate parameters in order are:
     *  - The function that implements the task.
     *  - The text name for LED processing task - for debug only as it is
     *    not used by the kernel.
     *  - The size of the stack to allocate to the task.
     *  - The parameter passed to the task - just to check the functionality.
     *  - The priority assigned to the task.
     *  - The task handle used for the task notify. */
    xTaskCreate( prvTaskLed,
                 "LED",
                 configMINIMAL_STACK_SIZE,
                 NULL,
                 tskIDLE_PRIORITY + 1,
                 &xTaskLEDOnHandle );
}
/*-----------------------------------------------------------*/

static void prvTaskLed( void *pvParameters )
{
uint32_t ui32LEDState;

    while (1)
    {
        /* Block until the task notification is received. */
        if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) == pdPASS)
        {
            /* Toggle LED1 on each time through the loop. */
            LEDRead(&ui32LEDState);
            LEDWrite(LED_D1, (ui32LEDState ^ LED_D1));
            if (ui32LEDState == 0)
            {
                UARTprintf("LED ON\n");
            }
            else
            {
                UARTprintf("LED OFF\n");
            }
        }
    }
}
/*-----------------------------------------------------------*/

static void prvConfigureLED( void )
{
    /* Configure the initial LED output.  PinoutSet() has already
     * configured LED I/O. */
    LEDWrite(LED_D1, LED_D1);

    /* Provide initial instructions. */
    UARTprintf("Press either SW1 or SW2 to toggle the LED.\n");
}
/*-----------------------------------------------------------*/

static void prvConfigureButton( void )
{
    /* Initialize the LaunchPad Buttons. */
    ButtonsInit();

    /* Configure both switches to trigger an interrupt on a falling edge. */
    GPIOIntTypeSet(BUTTONS_GPIO_BASE, ALL_BUTTONS, GPIO_FALLING_EDGE);

    /* Enable the interrupt for Port F in the GPIO peripheral. */
    GPIOIntEnable(BUTTONS_GPIO_BASE, ALL_BUTTONS);

    /* Enable the Port J interrupt in the NVIC. */
    IntEnable(INT_GPIOJ);

    /* Enable global interrupts in the NVIC. */
    IntMasterEnable();
}
/*-----------------------------------------------------------*/

void xButtonsHandler( void )
{
BaseType_t xLEDTaskWoken;
uint32_t ui32Status;

    /* Initialize the xLEDTaskWoken as pdFALSE.  This is required as the
     * FreeRTOS interrupt safe API will change it if needed should a
     * context switch be required. */
    xLEDTaskWoken = pdFALSE;

    /* Read the buttons interrupt status to find the cause of the interrupt. */
    ui32Status = GPIOIntStatus(BUTTONS_GPIO_BASE, true);

    /* Clear the interrupt. */
    GPIOIntClear(BUTTONS_GPIO_BASE, ui32Status);

    /* Debounce the input with 200ms filter */
    if ((xTaskGetTickCount() - g_ui32TimeStamp ) > 200)
    {
        vTaskNotifyGiveFromISR(xTaskLEDOnHandle, &xLEDTaskWoken);

        /* This FreeRTOS API call will handle the context switch if it is
        * required or have no effect if that is not needed. */
        portYIELD_FROM_ISR( xLEDTaskWoken );
    }

    /* Update the time stamp. */
    g_ui32TimeStamp = xTaskGetTickCount();
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

