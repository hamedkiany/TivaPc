/*
 * uart_task
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
 * The vUARTTask first configures the UART 0 port to run from the 16 MHZ PIOSC
 * and output data with a configuration of 115,200, 8-N-1.  Next, a mutex is
 * created and only if successful, the two data output tasks are created after.
 *
 * The prvDataOutputTask1 is created with a string parameter passed into it,
 * and it takes that string and sends it to the simple prvUARTWrite task. This
 * task is given a higher priority so it outputs it's message first.
 *
 * The prvDataOutputTask2 is created with no parameter passed into it, and
 * instead a string is defined within the task along with a timestamp which are
 * then passed into the prvUARTPrintf that serves as alternate wrapper to the
 * bare metal UART standard IO API, UARTprintf.
 *
 * The prvUARTWrite function takes the mutex and uses a basic loop that scans
 * for the end of string null character while sending out data one bye at a
 * time with UARTCharPut.  The function includes a variable delay to simulate
 * an application where the UART output is event driven and not an endless
 * stream.  After the UART message has been sent, it gives back the mutex.
 *
 * The prvUARTPrintf function takes the mutex and processes any variable
 * arguments that are input to support the advanced functionality of the
 * UARTvprintf to display specific character types.  Then UARTvprintf is called
 * with the string parameter and the variable arguments being passed to it.
 * The function includes a variable delay to simulate an application where the
 * UART output is event driven and not an endless stream.  After the UART
 * message has been sent, it gives back the mutex.  For a full list of valid
 * variable arguments, refer to the UARTvprintf function in uartstdio.c.
 *
 */

/* Standard includes. */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
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
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "drivers/rtos_hw_drivers.h"
#include "utils/uartstdio.h"
/*-----------------------------------------------------------*/

/*
 * The mutex used to guard the UART resource between tasks.
 */
static SemaphoreHandle_t xMutex = NULL;

/*
 * The tasks and functions as described in the comments at the top of this
 * file.
 */
static void prvDataOutputTask1( void *pvParameters );
static void prvDataOutputTask2( void *pvParameters );
static void prvUARTWrite( const char *pcString );
static void prvUARTPrintf( const char *pcString, ... );

/*
 * Called by main() to create the UART output tasks.
 */
void vUARTTask( void );

/*
 * Configure UART0 to send messages to a terminal.
 */
static void prvConfigureUART( void );
/*-----------------------------------------------------------*/

void vUARTTask( void )
{
    /* Configure UART0 to send messages to terminal. */
    prvConfigureUART();

    /* Before a semaphore is used it must be explicitly created. In this example a
     * mutex type semaphore is created. */
    xMutex = xSemaphoreCreateMutex();

    if( xMutex != NULL )
    {
        /* Create the tasks as described in the comments at the top of this
         * file. */

        /* The xTaskCreate parameters in order are:
         *  - The function that implements the task.
         *  - The text name for the output task - for debug only as it is not
         *    used by the kernel.
         *  - The size of the stack to allocate to the task.
         *  - The parameter passed to the task - just to check the functionality.
         *  - The priority assigned to the task.
         *  - The task handle is not required, so NULL is passed. */
        xTaskCreate( prvDataOutputTask1,
                     "Output1",
                     configMINIMAL_STACK_SIZE,
                     "Task 1 Output - TM4C123GH6PM UART thread safe "
                     "demonstration\r\n",
                     tskIDLE_PRIORITY + 2,
                     NULL );

        /* The xTaskCreate parameters in order are:
         *  - The function that implements the task.
         *  - The text name for the output task - for debug only as it is not
         *    used by the kernel.
         *  - The size of the stack to allocate to the task.
         *  - The parameter passed to the task - just to check the functionality.
         *  - The priority assigned to the task.
         *  - The task handle is not required, so NULL is passed. */
        xTaskCreate( prvDataOutputTask2,
                     "Output2",
                     configMINIMAL_STACK_SIZE,
                     NULL,
                     tskIDLE_PRIORITY + 1,
                     NULL );
    }
}
/*-----------------------------------------------------------*/

static void prvDataOutputTask1( void *pvParameters )
{
char *pcStringToPrint;

    /* The string printed by the task is passed into the task using the task’s
     * parameter.  The parameter is cast to the required type. */
    pcStringToPrint = ( char * ) pvParameters;

    for( ;; )
    {
        /* Print out the string using the newly defined function. */
        prvUARTWrite( pcStringToPrint );

        vTaskDelay( ( rand() % 0xFF ) );
    }
}
/*-----------------------------------------------------------*/

static void prvDataOutputTask2( void *pvParameters )
{
TickType_t xTimeNow;

    for( ;; )
    {
        /* Obtain the current tick count. */
        xTimeNow = xTaskGetTickCount();

        /* Print out the string using the newly defined function. */
        prvUARTPrintf( "Task 2 Output - This example uses a mutex to ensure "
                       "thread safety, time stamp = %d\n", xTimeNow);

        vTaskDelay( ( rand() % 0xFF ) );
    }
}
/*-----------------------------------------------------------*/

static void prvUARTWrite( const char *pcString )
{
uint32_t ui32Idx;

    /* The mutex is created before the scheduler is started, so already exists
     * by the time this task executes.  Attempt to take the mutex, blocking
     * indefinitely to wait for the mutex if it is not available right away. */
    if ( xSemaphoreTake( xMutex, portMAX_DELAY ) == pdTRUE )
    {
        /* Loop through until reaching the end of the string. */
        for(ui32Idx = 0; pcString[ui32Idx] != '\0'; ui32Idx++)
        {
            /* Output the full string. */
            UARTCharPut(UART0_BASE, pcString[ui32Idx]);
        }
    }

    /* The mutex MUST be given back. */
    xSemaphoreGive( xMutex );
}
/*-----------------------------------------------------------*/

static void prvUARTPrintf( const char *pcString, ... )
{
/* vaArgP is a variable argument list pointer whose content will depend
 * upon the format string passed in pcString. */
va_list vaArgP;

    /* The mutex is created before the scheduler is started, so already exists
     * by the time this task executes.  Attempt to take the mutex, blocking
     * indefinitely to wait for the mutex if it is not available right away. */
    if ( xSemaphoreTake( xMutex, portMAX_DELAY ) == pdTRUE )
    {
        /* Process any passed variable arguments. */
        va_start(vaArgP, pcString);

        /* Call the UART standard IO API to output the UART message by
         * providing both the string and any received variable arguments. */
        UARTvprintf(pcString, vaArgP);

        /* After the UART message is output, the variable arguments are no
         * longer needed. */
        va_end(vaArgP);
    }

    /* The mutex MUST be given back. */
    xSemaphoreGive( xMutex );
}
/*-----------------------------------------------------------*/

static void prvConfigureUART( void )
{
    /* Enable GPIO port A which is used for UART0 pins. */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    /* Configure the pin muxing for UART0 functions on port A0 and A1.
     * This step is not necessary if your part does not support pin muxing. */
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    /* Enable UART0 so that we can configure the clock. */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    /* Use the internal 16MHz oscillator as the UART clock source. */
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    /* Select the alternate (UART) function for these pins. */
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    /* Initialize the UART for console I/O. */
    UARTStdioConfig(0, 115200, 16000000);
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


