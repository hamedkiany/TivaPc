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
 * The Software Timer task creates three software timers which have periods of
 * 1, 2, and 4 seconds respectively.  Each timer is associated with a specific
 * RGB color: Timer 1 for Red, Timer 2 for Blue, and Timer 3 for Green.  When
 * a timeout callback is triggered, the associated LED is toggled.
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
#include "drivers/rtos_hw_drivers.h"
/*-----------------------------------------------------------*/

/*
 * The periods assigned to the periodic timers to toggle the LED colors.
 */
#define SWTimer1PERIODIC_TIMER_PERIOD pdMS_TO_TICKS( 500 )
#define SWTimer2PERIODIC_TIMER_PERIOD pdMS_TO_TICKS( 1000 )
#define SWTimer3PERIODIC_TIMER_PERIOD pdMS_TO_TICKS( 2000 )

/*
 * To pass if the SW periodic timers have started successfully to main.
 */
extern BaseType_t g_xTimer1Started;
extern BaseType_t g_xTimer2Started;
extern BaseType_t g_xTimer3Started;

/*
 * The tasks as described in the comments at the top of this file.
 */
static void prvPeriodicTimer1Callback( TimerHandle_t xTimer );
static void prvPeriodicTimer2Callback( TimerHandle_t xTimer );
static void prvPeriodicTimer3Callback( TimerHandle_t xTimer );

/*
 * Called by main() to create the software timer task.
 */
void vSWTimerTask( void );
/*-----------------------------------------------------------*/

void vSWTimerTask( void )
{
TimerHandle_t xPeriodicTimer1;
TimerHandle_t xPeriodicTimer2;
TimerHandle_t xPeriodicTimer3;

    /* Create a periodic timer, storing the handle to the created timer in
     * xPeriodicTimer1.
     *
     * The xTimerCreate parameters in order are:
     *  - The text name for the software timer - for debug only as it is not
     *    used by the kernel.
     *  - The software timer's period in ticks.
     *  - Setting uxAutoRealod to pdTRUE to create a periodic software timer.
     *  - Unused as this example does not use the timer id.
     *  - The callback function to be used by the generated software timer. */
    xPeriodicTimer1 = xTimerCreate(
                      "PeriodicRed",
                      SWTimer1PERIODIC_TIMER_PERIOD,
                      pdTRUE,
                      tskIDLE_PRIORITY,
                      prvPeriodicTimer1Callback );

    /* Create a periodic timer, storing the handle to the created timer in
     * xPeriodicTimer2.
     *
     * The xTimerCreate parameters in order are:
     *  - The text name for the software timer - for debug only as it is not
     *    used by the kernel.
     *  - The software timer's period in ticks.
     *  - Setting uxAutoRealod to pdTRUE to create a periodic software timer.
     *  - Unused as this example does not use the timer id.
     *  - The callback function to be used by the generated software timer. */
    xPeriodicTimer2 = xTimerCreate(
                      "PeriodicBlue",
                      SWTimer2PERIODIC_TIMER_PERIOD,
                      pdTRUE,
                      tskIDLE_PRIORITY,
                      prvPeriodicTimer2Callback );

    /* Create a periodic timer, storing the handle to the created timer in
     * xPeriodicTimer3.
     *
     * The xTimerCreate parameters in order are:
     *  - The text name for the software timer - for debug only as it is not
     *    used by the kernel.
     *  - The software timer's period in ticks.
     *  - Setting uxAutoRealod to pdTRUE to create a periodic software timer.
     *  - Unused as this example does not use the timer id.
     *  - The callback function to be used by the generated software timer. */
    xPeriodicTimer3 = xTimerCreate(
                      "PeriodicGreen",
                      SWTimer3PERIODIC_TIMER_PERIOD,
                      pdTRUE,
                      tskIDLE_PRIORITY,
                      prvPeriodicTimer3Callback );

    /* Check that the software timers were created. */
    if(( xPeriodicTimer1 != NULL ) && ( xPeriodicTimer2 != NULL ) &&
            ( xPeriodicTimer3 != NULL ))
    {
        /* Start the software timers, using a block time of 0 (no block time).
         * The scheduler has not been started yet so any block time specified
         * here would be ignored anyway. */
        g_xTimer1Started = xTimerStart( xPeriodicTimer1, 0 );
        g_xTimer2Started = xTimerStart( xPeriodicTimer2, 0 );
        g_xTimer3Started = xTimerStart( xPeriodicTimer3, 0 );
    }
}
/*-----------------------------------------------------------*/

static void prvPeriodicTimer1Callback( TimerHandle_t xTimer )
{
uint32_t ui32LEDValue;

    /* Read current LED status. */
    LEDRead(&ui32LEDValue);

    /* Toggle the D1 LED. */
    LEDWrite(RED_LED, (ui32LEDValue ^ RED_LED) );
}
/*-----------------------------------------------------------*/

static void prvPeriodicTimer2Callback( TimerHandle_t xTimer )
{
uint32_t ui32LEDValue;

    /* Read current LED status. */
    LEDRead(&ui32LEDValue);

    /* Toggle the D2 LED. */
    LEDWrite(BLUE_LED, (ui32LEDValue ^ BLUE_LED) );
}
/*-----------------------------------------------------------*/

static void prvPeriodicTimer3Callback( TimerHandle_t xTimer )
{
uint32_t ui32LEDValue;

    /* Read current LED status. */
    LEDRead(&ui32LEDValue);

    /* Toggle the D3 LED. */
    LEDWrite(GREEN_LED, (ui32LEDValue ^ GREEN_LED) );
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

