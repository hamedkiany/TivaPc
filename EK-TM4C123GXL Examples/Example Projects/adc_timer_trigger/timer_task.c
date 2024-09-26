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
 * The vHWTimerTask task configures the Timer 0B peripheral of the TM4C123GH6PM
 * MCU to run in periodic mode with a period of 16 kHz based on the system
 * clock and sets that the timer is used to trigger ADC samples. Lastly, it
 * enables global interrupts before starting the timer as this example is setup
 * where the timer configuration is the final step.
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
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
/*-----------------------------------------------------------*/

/*
 * Called by main() to create the hardware timer task.
 */
void vHWTimerTask( void );

/* 
 * Configure the hardware timer 0B on the TM4C123GH6PM to run in periodic
 * mode at a rate of 16 kHz.
 */
static void prvConfigureHWTimer( void );
/*-----------------------------------------------------------*/

void vHWTimerTask( void )
{
    /* Configure the hardware timer to run in periodic mode. */
	prvConfigureHWTimer();
}
/*-----------------------------------------------------------*/

static void prvConfigureHWTimer( void )
{
    /* The Timer 0 peripheral must be enabled for use. */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    /* The Timer 0 peripheral must be enabled for use. */
    TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PERIODIC);

    /* Set ADC sampling frequency to be 16KHz i.e. every 62.5uS. */
    TimerLoadSet(TIMER0_BASE, TIMER_B, (configCPU_CLOCK_HZ / 16000) - 1);

    /* Enable the ADC trigger output for Timer B. */
    TimerControlTrigger(TIMER0_BASE, TIMER_B, true);

    /* Enable global interrupts in the NVIC. */
    IntMasterEnable();

    /* All configuration are completed, enable Timer 0B which will trigger
    the ADC sampling process. */
    TimerEnable(TIMER0_BASE, TIMER_B);
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


