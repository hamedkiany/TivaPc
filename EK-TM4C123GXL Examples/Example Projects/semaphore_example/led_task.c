/*
 * led_task
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
 * vLEDTask turns on the red LED, configurations the buttons, and creates
 * prvProcessSwitchInputTask which handles processing the ISR result to adjust
 * the LED per the button inputs.
 *
 * prvProcessSwitchInputTask uses semaphore take to wait until it receives a
 * semaphore from the button ISR.  Once it does, then it processes the button
 * pressed.  Each time the right button is pressed, the light toggles in the
 * order of red->blue->green.  If the left button is pressed, the light toggles
 * in the order of red->green->blue.
 *
 * When either user switch SW1 or SW2 on the EK-TM4C123GXL is pressed, an
 * interrupt is generated and the switch pressed is logged in the global
 * variable g_pui32ButtonPressed.  Then the binary semaphore is given to
 * prvProcessSwitchInputTask before yielding to it.  This is an example of
 * using binary semaphores to defer ISR processing to a task.
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
/*-----------------------------------------------------------*/

/*
 * Time stamp global variable.
 */
volatile uint32_t g_ui32TimeStamp = 0;

/*
 * Global variable to log the last GPIO button pressed.
 */
volatile static uint32_t g_pui32ButtonPressed = NULL;

/*
 * Array used to toggle between LED colors via index.
 */
static uint8_t g_pui8LEDArray[3] = {RED_LED, BLUE_LED, GREEN_LED};

/*
 * The binary semaphore used by the switch ISR & task.
 */
extern SemaphoreHandle_t xButtonSemaphore;

/*
 * The tasks as described in the comments at the top of this file.
 */
static void prvProcessSwitchInputTask( void *pvParameters );

/*
 * Called by main() to do example specific hardware configurations and to
 * create the Process Switch task.
 */
void vLEDTask( void );

/* 
 * Hardware configuration for the LEDs.
 */
static void prvConfigureLED( void );

/*
 * Hardware configuration for the buttons SW1 and SW2 to generate interrupts.
 */
static void prvConfigureButton( void );
/*-----------------------------------------------------------*/

void vLEDTask( void )
{
    /* Set the initial LED color to red. */
    prvConfigureLED();

    /* Configure the button to generate interrupts. */
    prvConfigureButton();

    /* Create the task as described in the comments at the top of this file.
     *
     * The xTaskCreate parameters in order are:
     *  - The function that implements the task.
     *  - The text name for the LED Task - for debug only as it is not used by
     *    the kernel.
     *  - The size of the stack to allocate to the task.
     *  - The parameter passed to the task - just to check the functionality.
     *  - The priority assigned to the task.
     *  - The task handle is not required, so NULL is passed. */
    xTaskCreate( prvProcessSwitchInputTask,
                 "LED",
                 configMINIMAL_STACK_SIZE,
                 NULL,
                 tskIDLE_PRIORITY + 1,
                 NULL );
}
/*-----------------------------------------------------------*/

static void prvProcessSwitchInputTask( void *pvParameters )
{
uint8_t ui8LEDIndex = 0;

    for( ;; )
    {
        /* Block until the ISR gives the semaphore. */
        if( xSemaphoreTake(xButtonSemaphore, portMAX_DELAY) == pdPASS)
        {
            /* If the right button is hit, either increment by 1 or reset the
             * index to 0 if it is at 2. */
            if (g_pui32ButtonPressed == RIGHT_BUTTON)
            {
                if (ui8LEDIndex == 2)
                {
                    ui8LEDIndex = 0;
                }
                else
                {
                    ui8LEDIndex++;
                }
            }
            /* If the left button is hit, either decrement by 1 or reset the
             * index to 2 if it is at 0. */
            else if (g_pui32ButtonPressed == LEFT_BUTTON)
            {
                if (ui8LEDIndex == 0)
                {
                    ui8LEDIndex = 2;
                }
                else
                {
                    ui8LEDIndex--;
                }
            }

            /* Set the LED based on the updated Index. */
            LEDWrite(BLUE_LED|RED_LED|GREEN_LED, g_pui8LEDArray[ui8LEDIndex]);
        }
    }
}
/*-----------------------------------------------------------*/

static void prvConfigureLED( void )
{
    /* Configure initial LED state to be red.  PinoutSet() has already
     * configured LED I/O. */
    LEDWrite(BLUE_LED|RED_LED|GREEN_LED, g_pui8LEDArray[0]);
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

    /* Enable the Port F interrupt in the NVIC. */
    IntEnable(INT_GPIOF);

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
        /* Log which button was pressed to trigger the ISR. */
        if ((ui32Status & RIGHT_BUTTON) == RIGHT_BUTTON)
        {
            g_pui32ButtonPressed = RIGHT_BUTTON;
        }
        else if ((ui32Status & LEFT_BUTTON) == LEFT_BUTTON)
        {
            g_pui32ButtonPressed = LEFT_BUTTON;
        }

        /* Give the semaphore to unblock prvProcessSwitchInputTask.  */
        xSemaphoreGiveFromISR( xButtonSemaphore, &xLEDTaskWoken );

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

