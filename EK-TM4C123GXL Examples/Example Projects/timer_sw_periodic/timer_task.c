/*
 * timer_task
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
 * The vSWTimerTask creates a software timer that runs every half-second and
 * calls the prvPeriodicTimer1Callback to toggle the EK-TM4C123GXL's blue LED.
 * The task also outputs a simple initial message over UART.
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
#include "timers.h"

/* Hardware includes. */
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "drivers/rtos_hw_drivers.h"
#include "utils/uartstdio.h"
/*-----------------------------------------------------------*/

/*
 * The period assigned to the periodic SW timer.
 */
#define SWTimer1PERIODIC_TIMER_PERIOD pdMS_TO_TICKS( 500 )

/*
 * Used to pass if the SW periodic timer started successfully to main.
 */
extern BaseType_t g_xTimer1Started;

/*
 * The tasks as described in the comments at the top of this file.
 */
static void prvPeriodicTimer1Callback( TimerHandle_t xTimer );

/*
 * Called by main() to create the software timer task.
 */
void vSWTimerTask( void );
/*-----------------------------------------------------------*/

void vSWTimerTask( void )
{
TimerHandle_t xPeriodicTimer1;

    /* Create a periodic timer, storing the handle to the created timer in
     * xPeriodicTimer1.
     *
     * The xTimerCreate parameters in order are:
     *  - The text name for the software timer - for debug only as it is not
     *    used by the kernel.
     *  - The software timer's period in ticks.
     *  - Setting uxAutoReload to pdTRUE to create a periodic software timer.
     *  - Unused as this example does not use the timer id.
     *  - The callback function to be used by the generated software timer. */
    xPeriodicTimer1 = xTimerCreate(
                      "PeriodicLEDs",
                      SWTimer1PERIODIC_TIMER_PERIOD,
                      pdTRUE,
                      0,
                      prvPeriodicTimer1Callback );

    /* Check that the software timer was created. */
    if( xPeriodicTimer1 != NULL )
    {
        /* Start the software timer, using a block time of 0 (no block time).
         * The scheduler has not been started yet so any block time specified
         * here would be ignored anyway. */
        g_xTimer1Started = xTimerStart( xPeriodicTimer1, 0 );
    }

    /* Output an initial message. */
    UARTprintf("Periodic FreeRTOS software timer example.\n"
               "Blink rate of the blue LED: 1 Hz.\n");
}
/*-----------------------------------------------------------*/

static void prvPeriodicTimer1Callback( TimerHandle_t xTimer )
{
uint32_t ui32LEDValue;

    /* Read current LED status. */
    LEDRead(&ui32LEDValue);

    /* Toggle the blue LED. */
    LEDWrite(BLUE_LED, (ui32LEDValue ^ BLUE_LED) );

    /* Output a string to show the time at which the callback was executed. */
    if (ui32LEDValue)
    {
        UARTprintf("LED Status: Off\n");
    }
    else
    {
        UARTprintf("LED Status: On\n");
    }
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


