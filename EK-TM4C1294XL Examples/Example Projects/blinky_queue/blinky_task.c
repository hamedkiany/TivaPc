/*
 * blinky_task
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
 * This project demonstrates how to configure the TM4C1294NCPDT to blink a LED
 * using FreeRTOS queue.  Two tasks and one queue are created for this example.
 * The sending task sends a parameter at an interval of 1000ms to the queue.
 * The receiving task is in blocked state until  something is received in the
 * queue.  Upon receiving a correct parameter from the queue it sets the LED
 * for 500ms and then clears the LED for another 500ms.
 *
 * When either user switch SW1 or SW2 on the EK-TM4C1294XL is pressed, an
 * interrupt is generated to change the blink rate.  The amount of increase
 * or decrease is controlled by the #define BLINK_RATE.  SW1 is pressed to
 * speed up the blink rate while SW2 is pressed to slow down the blink rate.
 *
 * vBlinkyTask() creates one queue, and two tasks.  It then starts the
 * scheduler.
 *
 * The Queue Send Task:
 * The queue send task is implemented by the prvQueueSendTask() function in
 * this file.  prvQueueSendTask() sits in a loop that causes it to repeatedly
 * block for 1000 milliseconds, before sending the value 100 to the queue that
 * was created within vBlinkyTask().  Once the value is sent, the task loops
 * back around to block for another 1000 milliseconds.
 *
 * The Queue Receive Task:
 * The queue receive task is implemented by the prvQueueReceiveTask() function
 * in this file.  prvQueueReceiveTask() sits in a loop where it repeatedly
 * blocks on attempts to read data from the queue that was created within
 * vBlinkyTask().  When data is received, the task checks the value of the
 * data, and if the value equals the expected 100, sets the LED.  The 'block
 * time' parameter passed to the queue receive function specifies that the
 * task should be held in the Blocked state indefinitely to wait for data to
 * be available on the queue.  The queue receive task will only leave the
 * Blocked state when the queue send task writes to the queue.  As the queue
 * send task writes to the queue every 1000 milliseconds, the queue receive
 * task leaves the Blocked state every 1000 milliseconds.  Once unblocked,
 * the LED is first set for 500ms and then clear for 500ms.
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
 * Priorities at which the tasks are created.
 */
#define mainQUEUE_RECEIVE_TASK_PRIORITY     ( tskIDLE_PRIORITY + 2 )
#define mainQUEUE_SEND_TASK_PRIORITY        ( tskIDLE_PRIORITY + 1 )

/*
 * Values passed to the two tasks just to check the task parameter
 * functionality.
 */
#define mainQUEUE_RECEIVE_PARAMETER         ( 0x22UL )
#define mainQUEUE_SEND_PARAMETER            ( 0x1111UL )

/*
 * The rate at which data is sent to the queue.  The 1000ms value is converted
 * to ticks using the portTICK_PERIOD_MS constant.
 */
#define mainQUEUE_SEND_FREQUENCY_MS         ( pdMS_TO_TICKS( 1000UL ) )

/*
 * The blank rate variable which is expressed as a decimal as to increase
 * or decrease the blink rate.
 */
#define BLINK_RATE 0.1

/*
 * Initial blink speed.
 */
volatile float g_fSpeed = 1;

/*
 * Time stamp global variable.
 */
volatile uint32_t g_ui32TimeStamp = 0;

/*
 * The queue used by both tasks.
 */
static QueueHandle_t xQueue = NULL;

/*
 * The tasks as described in the comments at the top of this file.
 */
static void prvQueueReceiveTask( void *pvParameters );
static void prvQueueSendTask( void *pvParameters );

/*
 * Called by main() to create the simple blinky style application.
 */
void vBlinkyTask( void );

/*
 * Hardware configuration for the buttons SW1 and SW2 to generate interrupts.
 */
static void prvConfigureButton( void );
/*-----------------------------------------------------------*/

void vBlinkyTask( void )
{
    /* Configure a button to generate interrupts (for test purposes). */
    prvConfigureButton();

    /* Create the queue with one item that can be held. This is set to one as
     * the receive task will remove items as they are added, meaning the send
     * task should always find the queue empty. */
    xQueue = xQueueCreate( 1, sizeof( uint32_t ) );

    if( xQueue != NULL )
    {
        /* Create the task as described in the comments at the top of this file.
         *
         * The xTaskCreate parameters in order are:
         *  - The function that implements the task.
         *  - The text name RX processing task - for debug only as it is
         *    not used by the kernel.
         *  - The size of the stack to allocate to the task.
         *  - The parameter passed to the task - just to check the functionality.
         *  - The priority assigned to the task.
         *  - The task handle is NULL */
        xTaskCreate( prvQueueReceiveTask,
                     "Rx",
                     configMINIMAL_STACK_SIZE,
                     ( void * ) mainQUEUE_RECEIVE_PARAMETER,
                     mainQUEUE_RECEIVE_TASK_PRIORITY,
                     NULL );

        /* Create the task as described in the comments at the top of this file.
         *
         * The xTaskCreate parameters in order are:
         *  - The function that implements the task.
         *  - The text name for TX processing task - for debug only as it is
         *    not used by the kernel.
         *  - The size of the stack to allocate to the task.
         *  - The parameter passed to the task - just to check the functionality.
         *  - The priority assigned to the task.
         *  - The task handle is NULL */
        xTaskCreate( prvQueueSendTask,
                     "TX",
                     configMINIMAL_STACK_SIZE,
                     ( void * ) mainQUEUE_SEND_PARAMETER,
                     mainQUEUE_SEND_TASK_PRIORITY,
                     NULL );
    }
}
/*-----------------------------------------------------------*/

static void prvQueueSendTask( void *pvParameters )
{
TickType_t xNextWakeTime;
const unsigned long ulValueToSend = 100UL;

    /* Check the task parameter is as expected. */
    configASSERT( ( ( unsigned long ) pvParameters ) == mainQUEUE_SEND_PARAMETER );

    /* Initialize xNextWakeTime - this only needs to be done once. */
    xNextWakeTime = xTaskGetTickCount();

    for( ;; )
    {
        /* Place this task in the blocked state until it is time to run again.
        The block time is specified in ticks, the constant used converts ticks
        to ms.  While in the Blocked state this task will not consume any CPU
        time. */
        vTaskDelayUntil( &xNextWakeTime, mainQUEUE_SEND_FREQUENCY_MS * g_fSpeed);

        /* Send to the queue - causing the queue receive task to unblock and
        toggle the LED.  0 is used as the block time so the sending operation
        will not block - it shouldn't need to block as the queue should always
        be empty at this point in the code. */
        xQueueSend( xQueue, &ulValueToSend, 0U );
    }
}
/*-----------------------------------------------------------*/

static void prvQueueReceiveTask( void *pvParameters )
{
unsigned long ulReceivedValue;
static TickType_t xShortBlock = pdMS_TO_TICKS( 500 )  ;

    /* Check the task parameter is as expected. */
    configASSERT( ( ( unsigned long ) pvParameters ) == mainQUEUE_RECEIVE_PARAMETER );

    for( ;; )
    {
        /* Wait until something arrives in the queue - this task will block
        indefinitely provided INCLUDE_vTaskSuspend is set to 1 in
        FreeRTOSConfig.h. */
        xQueueReceive( xQueue, &ulReceivedValue, portMAX_DELAY );

        /*  To get here something must have been received from the queue, but
        is it the expected value?  If it is, toggle the LED. */
        if( ulReceivedValue == 100UL )
        {
            /* Toggle between LED1 and LED2, using vTaskDelay to delay the
             * toggle rate based on g_fSpeed. */
            GPIOPinWrite(GPIO_PORTN_BASE, (LED_D1|LED_D2), LED_D1);
            vTaskDelay( xShortBlock * g_fSpeed);
            GPIOPinWrite(GPIO_PORTN_BASE, (LED_D1|LED_D2), LED_D2);
            ulReceivedValue = 0U;
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

    /* Enable the interrupt for Port J in the GPIO peripheral. */
    GPIOIntEnable(BUTTONS_GPIO_BASE, ALL_BUTTONS);

    /* Enable the Port J interrupt in the NVIC. */
    IntEnable(INT_GPIOJ);

    /* Enable global interrupts in the NVIC. */
    IntMasterEnable();
}
/*-----------------------------------------------------------*/

void xButtonsHandler( void )
{
uint32_t ui32Status;

    /* Read the buttons interrupt status to find the cause of the interrupt. */
    ui32Status = GPIOIntStatus(BUTTONS_GPIO_BASE, true);

    /* Clear the interrupt. */
    GPIOIntClear(BUTTONS_GPIO_BASE, ui32Status);

    /* Debounce the input with 200ms filter */
    if ((xTaskGetTickCount() - g_ui32TimeStamp ) > 200)
    {
        /* Adjust the blink rate based on which switch was pressed. */
        if ((ui32Status & GPIO_PIN_0) == GPIO_PIN_0)
        {
            g_fSpeed *= (1 - BLINK_RATE);
        }
        else if ((ui32Status & GPIO_PIN_1) == GPIO_PIN_1)
        {
            g_fSpeed *= (1 + BLINK_RATE);
        }
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


