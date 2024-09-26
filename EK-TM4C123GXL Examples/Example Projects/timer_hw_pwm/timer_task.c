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
 * The vHWTimerTask task configures the Timer 2 peripheral of the TM4C123GH6PM
 * MCU to run in split pair mode with both the A and B timers configured for
 * PWM outputs.  For this example, the PWM frequencies for Timers 2A and 2B
 * are set at 50kHz and 25kHz respectively.  These frequencies can be quickly
 * changed by updating the #define values in this file.
 *
 * The duty cycle will be controlled with a task that cycles between various
 * duty cycles at a 200ms interval.  The initial duty cycle for Timer 2A is set
 * is set to 33%.  The initial duty cycle for Timer 2B is set to 50%.  The duty
 * cycle management task will also start both timers together at the beginning
 * of the task so the 200ms schedule is kept.
 * 
 * The UART terminal is used to print out statistics about the PWM outputs.
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
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
/*-----------------------------------------------------------*/

/*
 * Define the PWM Rates and rate for updating the duty cycle.
 */
#define TIMER_2A_PWM_RATE configCPU_CLOCK_HZ / 50000 /* 50kHz PWM */
#define TIMER_2B_PWM_RATE configCPU_CLOCK_HZ / 25000 /* 25kHz PWM */
#define DUTY_CYCLE_UPDATE_RATE 200

/*
 * The tasks as described in the comments at the top of this file.
 */
static void prvDutyCycleMgmtTask( void *pvParameters );

/*
 * Called by main() to create the hardware timer task.
 */
void vHWTimerTask( void );

/* 
 * Configure the hardware timer 2 on the TM4C123GH6PM to run in split pair
 * PWM mode at the rates defined in this file.
 */
static void prvConfigureHWTimer( void );
/*-----------------------------------------------------------*/

void vHWTimerTask( void )
{
    /* Configure the hardware timer to run in PWM mode. */
    prvConfigureHWTimer();

    /* The xTaskCreate parameters in order are:
     *  - The function that implements the task.
     *  - The text name for the task - for debug only as it is not
     *    used by the kernel.
     *  - The size of the stack to allocate to the task.
     *  - The parameter passed to the task - just to check the functionality.
     *  - The priority assigned to the task.
     *  - The task handle is not required, so NULL is passed. */
    xTaskCreate( prvDutyCycleMgmtTask,
                 "PWM",
                 configMINIMAL_STACK_SIZE,
                 NULL,
                 tskIDLE_PRIORITY + 1,
                 NULL );

    /* Display initial message about the timer PWM example. */
    UARTprintf("Timer in PWM mode example.\n");
    UARTprintf("Timer 0A output pin: PL4\n");
    UARTprintf("Timer 0A PWM Rate: 50kHz\n");
    UARTprintf("Timer 0A Duty Cycle: 33%%\n");
    UARTprintf("Timer 0B output pin: PL5\n");
    UARTprintf("Timer 0B PWM Rate: 25kHz\n");
    UARTprintf("Timer 0B Duty Cycle: 50%%\n");
}
/*-----------------------------------------------------------*/

static void prvDutyCycleMgmtTask( void *pvParameters )
{
    /* Enable both timers to begin PWM output. */
    TimerEnable(TIMER2_BASE, TIMER_BOTH);

    /* Enter a blocked state until 200 ms passes to delay initial update. */
    vTaskDelay( DUTY_CYCLE_UPDATE_RATE );

    /* Adjust the PWM Duty Cycles every 200 ms. */
    for( ;; )
    {
        /* Set the Timer 2A PWM to 66% duty cycle. */
        /* Set the Timer 2B PWM to 75% duty cycle. */
        TimerMatchSet(TIMER2_BASE, TIMER_A, TIMER_2A_PWM_RATE / 3);
        TimerMatchSet(TIMER2_BASE, TIMER_B, TIMER_2B_PWM_RATE / 4);
        UARTprintf("Timer 0A Duty Cycle: 66%%\n" \
                   "Timer 0B Duty Cycle: 75%%\n");

        /* Enter a blocked state until 200 ms passes. */
        vTaskDelay( DUTY_CYCLE_UPDATE_RATE );

        /* Set the Timer 2A PWM to 33% duty cycle. */
        /* Set the Timer 2B PWM to 50% duty cycle. */
        TimerMatchSet(TIMER2_BASE, TIMER_A, TIMER_2A_PWM_RATE / 3 * 2);
        TimerMatchSet(TIMER2_BASE, TIMER_B, TIMER_2B_PWM_RATE / 2);
        UARTprintf("Timer 0A Duty Cycle: 33%%\n" \
                   "Timer 0B Duty Cycle: 50%%\n");

        /* Enter a blocked state until 200 ms passes. */
        vTaskDelay( DUTY_CYCLE_UPDATE_RATE );

        /* Set the Timer 2A PWM to 16.7% duty cycle. */
        /* Set the Timer 2B PWM to 25% duty cycle. */
        TimerMatchSet(TIMER2_BASE, TIMER_A, TIMER_2A_PWM_RATE / 6 * 5);
        TimerMatchSet(TIMER2_BASE, TIMER_B, TIMER_2B_PWM_RATE / 4 * 3);
        UARTprintf("Timer 0A Duty Cycle: 16.7%%\n" \
                   "Timer 0B Duty Cycle: 25%%\n");

        /* Enter a blocked state until 200 ms passes. */
        vTaskDelay( DUTY_CYCLE_UPDATE_RATE );

        /* Set the Timer 2A PWM to 33% duty cycle. */
        /* Set the Timer 2B PWM to 50% duty cycle. */
        TimerMatchSet(TIMER2_BASE, TIMER_A, TIMER_2A_PWM_RATE / 3 * 2);
        TimerMatchSet(TIMER2_BASE, TIMER_B, TIMER_2B_PWM_RATE / 2);
        UARTprintf("Timer 0A Duty Cycle: 33%%\n" \
                   "Timer 0B Duty Cycle: 50%%\n");

        /* Enter a blocked state until 200 ms passes. */
        vTaskDelay( DUTY_CYCLE_UPDATE_RATE );
    }
}
/*-----------------------------------------------------------*/

static void prvConfigureHWTimer( void )
{
    /* Configure the GPIO pin muxing for the Timer/CCP function. */
    GPIOPinConfigure(GPIO_PB0_T2CCP0);
    GPIOPinConfigure(GPIO_PB1_T2CCP1);

    /* Configure the GPIO pin type for Timer/CCP function. */
    GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_0);
    GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_1);

    /* The Timer 2 peripheral must be enabled for use. */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);

    /* Configure Timer 2 in split pair PWM mode. */
    TimerConfigure(TIMER2_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM |
                                TIMER_CFG_B_PWM);

    /* Set the Timer 2A load value. */
    TimerLoadSet(TIMER2_BASE, TIMER_A, TIMER_2A_PWM_RATE);

    /* Set the Timer 2B load value. */
    TimerLoadSet(TIMER2_BASE, TIMER_B, TIMER_2B_PWM_RATE);

    /* Set the Timer 2A PWM to 33% duty cycle. */
    TimerMatchSet(TIMER2_BASE, TIMER_A, TIMER_2A_PWM_RATE / 3 * 2);

    /* Set the Timer 2B PWM to 50% duty cycle. */
    TimerMatchSet(TIMER2_BASE, TIMER_B, TIMER_2B_PWM_RATE / 2);
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


