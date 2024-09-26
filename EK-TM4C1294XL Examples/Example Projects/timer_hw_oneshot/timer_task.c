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
 * The vHWTimerTask task first configures the Timer 0 peripheral of the
 * TM4C1294NCPDT MCU to run in one-shot mode for half a second.  It also
 * configures a timeout interrupt for Timer 0 and creates a task that will
 * wait for the timeout interrupt to notify it to execute code.
 * 
 * The ISR for Timer 0 is xTimerHandler and it is linked to the interrupt
 * vector table for the TM4C1294NCPDT MCU in the startup_ccs.c file.  Instead of
 * manually linking the ISR, the IntRegister API can be used instead.  The ISR
 * clears the hardware interrupt flag and then uses the FreeRTOS Task Notify
 * feature to defer further processing to a task.  This makes for a very lean
 * ISR which is recommended, especially for hardware interrupts.

 * The prvTimerIntTask task simply waits for the notification from the ISR and
 * then once received it outputs a UART message with the current RTOS tick
 * count.  The task notify feature is valuable for this application use case
 * as full UART messages output slowly and entire strings should not be sent
 * within an ISR (even when running bare-metal code).
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
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
/*-----------------------------------------------------------*/

/*
 * Declare a variable that is used to hold the handle of the timer
 * interrupt task.
 */
TaskHandle_t xTimerIntTask = NULL;

/*
 * The tasks as described in the comments at the top of this file.
 */
static void prvTimerIntTask( void *pvParameters );

/*
 * Called by main() to create the hardware timer task.
 */
void vHWTimerTask( void );

/* 
 * Configure the hardware timer 0A on the TM4C1294NCPDT to run in one-shot
 * mode with a timeout interrupt.
*/
static void prvConfigureHWTimer( void );
/*-----------------------------------------------------------*/

void vHWTimerTask( void )
{
    /* Configure the hardware timer to run in one-shot mode. */
    prvConfigureHWTimer();

    /* Create the task as described in the comments at the top of this file.
     *
     * The xTaskCreate parameters in order are:
     *  - The function that implements the task.
     *  - The text name for Timer 0A - for debug only as it is not used by
     *    the kernel.
     *  - The size of the stack to allocate to the task.
     *  - The parameter passed to the task - just to check the functionality.
     *  - The priority assigned to the task.
     *  - The task handle used for the task notify. */
    xTaskCreate( prvTimerIntTask,
                 "Timer0A",
                 configMINIMAL_STACK_SIZE,
                 NULL,
                 tskIDLE_PRIORITY + 1,
                 &xTimerIntTask );

    /* Tasks have been created, now enable Timer 0A.
     * Depending on the application, this can be placed inside of the Configure
     * Hardware function too.  For this example, the task to receive the ISR
     * notification needs to be created first, so the Timer is only enabled
     * after the task has been created. */
    TimerEnable(TIMER0_BASE, TIMER_A);
}
/*-----------------------------------------------------------*/

static void prvTimerIntTask( void *pvParameters )
{
unsigned long ulEventsToProcess;
TickType_t xTimeNow;

    for( ;; )
    {
        /* Wait to receive a notification sent directly to this task from the
        interrupt service routine. */
        ulEventsToProcess = ulTaskNotifyTake( pdTRUE, portMAX_DELAY );

        if (ulEventsToProcess != 0)
        {
            /* Obtain the current tick count. */
            xTimeNow = xTaskGetTickCount();

            /* Display a message indicating that the one shot interrupt was
             * received. */
            UARTprintf("\nOne shot timer interrupt received at %d.",xTimeNow);
        }
        else
        {
            /* Timed out. */
        }
    }
}
/*-----------------------------------------------------------*/

static void prvConfigureHWTimer( void )
{
    /* The Timer 0 peripheral must be enabled for use. */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    /* Configure Timer 0 in full-width one-shot mode. */
    TimerConfigure(TIMER0_BASE, TIMER_CFG_ONE_SHOT);

    /* Set the Timer 0A load value to half a second. */
    TimerLoadSet(TIMER0_BASE, TIMER_A, configCPU_CLOCK_HZ / 2);

    /* Configure the Timer 0A interrupt for timeout. */
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    /* Enable the Timer 0A interrupt in the NVIC. */
    IntEnable(INT_TIMER0A);

    /* Enable global interrupts in the NVIC. */
    IntMasterEnable();
}
/*-----------------------------------------------------------*/

void xTimerHandler( void )
{
BaseType_t xHigherPriorityTaskWoken;

    /* Clear the hardware interrupt flag for Timer 0A. */
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    /* The xHigherPriorityTaskWoken parameter must be initialized to pdFALSE as
    it will get set to pdTRUE inside the interrupt safe API function if a
    context switch is required. */
    xHigherPriorityTaskWoken = pdFALSE;

    /* Defer the interrupt processing to a Task to minimize time spent within
     * the hardware interrupt service routine.  Send a notification directly to
     * the task to which interrupt processing is being deferred. */
    vTaskNotifyGiveFromISR( xTimerIntTask, &xHigherPriorityTaskWoken );

    /* This FreeRTOS API call will handle the context switch if it is required
     * or have no effect if that is not needed. */
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
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


