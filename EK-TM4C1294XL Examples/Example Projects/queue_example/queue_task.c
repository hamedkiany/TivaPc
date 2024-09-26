/*
 * queue_task
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
 * This example demonstrates passing a complex structure by value as well as
 * by reference using FreeRTOS's queue between multiple tasks.  Two queues are
 * set up with one for passing messages by value and another for passing by
 * reference.  A total of six tasks are created with four of them sending
 * messages through the two queues while the other two tasks receiving the
 * message from the two queues either by value or by reference.  The four tasks
 * transmit their messages at different times.  Their time stamp is sent as
 * the data as part of the message.  Once the data is received, the receiving
 * tasks print the task ID and its corresponding time stamp on the terminal
 * window.
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
#include "driverlib/sysctl.h"
#include "utils/uartstdio.h"
/*-----------------------------------------------------------*/

/*
 * Define the task IDs.
 */
#define TASK1_ID 0
#define TASK2_ID 1
#define TASK3_ID 2
#define TASK4_ID 3

/*
 * Priorities at which the tasks are created.
 */
#define mainQUEUE_RECEIVE_TASK_PRIORITY     ( tskIDLE_PRIORITY + 2 )
#define mainQUEUE_SEND_TASK_PRIORITY        ( tskIDLE_PRIORITY + 1 )

/*
 * The number of items the queue can hold.  This is 4 as the receive task
 * will remove items as they are added, meaning the send task should always find
 * the queue empty.
 */
#define mainQUEUE_LENGTH                    ( 4 )

/*
 * The queue used by both tasks.
 */
struct AMessage
{
    uint32_t ulMessageID;
    uint32_t ulTimeStamp;
} xMessage;

/*
 * Queue used to send and receive complete struct AMessage structures.
 */
QueueHandle_t xStructQueue = NULL;

/*
 * Queue used to send and receive pointers to struct AMessage structures.
 */
QueueHandle_t xPointerQueue = NULL;

/*
 * The tasks as described in the comments at the top of this file.
 */
static void prvQueueReceiveTask1( void *pvParameters );
static void prvQueueReceiveTask2( void *pvParameters );
static void prvQueueSendTask1( void *pvParameters );
static void prvQueueSendTask2( void *pvParameters );
static void prvQueueSendTask3( void *pvParameters );
static void prvQueueSendTask4( void *pvParameters );

/*
 * Called by main() to create the various queue tasks.
 */
void vQueueTask( void );
/*-----------------------------------------------------------*/

void vQueueTask( void )
{
    /* Create the queue used to send complete struct AMessage structures.  This can
    also be created after the schedule starts, but care must be task to ensure
    nothing uses the queue until after it has been created. */
    xStructQueue = xQueueCreate(
                          /* The number of items the queue can hold. */
                          mainQUEUE_LENGTH,
                          /* Size of each item is big enough to hold the
                          whole structure. */
                          sizeof( xMessage ) );

    /* Create the queue used to send pointers to struct AMessage structures. */
    xPointerQueue = xQueueCreate(
                          /* The number of items the queue can hold. */
                          mainQUEUE_LENGTH,
                          /* Size of each item is big enough to hold only a
                          pointer. */
                          sizeof( &xMessage ) );

    if( ( xStructQueue == NULL ) || ( xPointerQueue == NULL ) )
    {

    }
    /* Create the task as described in the comments at the top of this file.
     *
     * The xTaskCreate parameters in order are:
     *  - The function that implements the task.
     *  - The text name RX by value processing task - for debug only as it is
     *    not used by the kernel.
     *  - The size of the stack to allocate to the task.
     *  - No parameter passed to the task
     *  - The priority assigned to the task.
     *  - The task handle is NULL */
    xTaskCreate( prvQueueReceiveTask1,
                "RX by value",
                configMINIMAL_STACK_SIZE,
                NULL,
                mainQUEUE_RECEIVE_TASK_PRIORITY,
                NULL );

    /* Create the task as described in the comments at the top of this file.
     *
     * The xTaskCreate parameters in order are:
     *  - The function that implements the task.
     *  - The text name RX by reference processing task - for debug only as it
     *    is not used by the kernel.
     *  - The size of the stack to allocate to the task.
     *  - No parameter passed to the task
     *  - The priority assigned to the task.
     *  - The task handle is NULL */
    xTaskCreate( prvQueueReceiveTask2,
                "RX by reference",
                configMINIMAL_STACK_SIZE,
                NULL,
                mainQUEUE_RECEIVE_TASK_PRIORITY,
                NULL );

    /* Create the task as described in the comments at the top of this file.
     *
     * The xTaskCreate parameters in order are:
     *  - The function that implements the task.
     *  - The text name TX1 by value processing task - for debug only as it is
     *    not used by the kernel.
     *  - The size of the stack to allocate to the task.
     *  - No parameter passed to the task
     *  - The priority assigned to the task.
     *  - The task handle is NULL */
    xTaskCreate( prvQueueSendTask1,
                 "TX1",
                 configMINIMAL_STACK_SIZE,
                 NULL,
                 mainQUEUE_SEND_TASK_PRIORITY,
                 NULL );
    /* Create the task as described in the comments at the top of this file.
     *
     * The xTaskCreate parameters in order are:
     *  - The function that implements the task.
     *  - The text name TX2 by value processing task - for debug only as it is
     *    not used by the kernel.
     *  - The size of the stack to allocate to the task.
     *  - No parameter passed to the task
     *  - The priority assigned to the task.
     *  - The task handle is NULL */
    xTaskCreate( prvQueueSendTask2,
                 "TX2",
                 configMINIMAL_STACK_SIZE,
                 NULL,
                 mainQUEUE_SEND_TASK_PRIORITY,
                 NULL );
    /* Create the task as described in the comments at the top of this file.
     *
     * The xTaskCreate parameters in order are:
     *  - The function that implements the task.
     *  - The text name TX3 by value processing task - for debug only as it is
     *    not used by the kernel.
     *  - The size of the stack to allocate to the task.
     *  - No parameter passed to the task
     *  - The priority assigned to the task.
     *  - The task handle is NULL */
    xTaskCreate( prvQueueSendTask3,
                 "TX3",
                 configMINIMAL_STACK_SIZE,
                 NULL,
                 mainQUEUE_SEND_TASK_PRIORITY,
                 NULL );

    /* Create the task as described in the comments at the top of this file.
     *
     * The xTaskCreate parameters in order are:
     *  - The function that implements the task.
     *  - The text name TX4 by value processing task - for debug only as it is
     *    not used by the kernel.
     *  - The size of the stack to allocate to the task.
     *  - No parameter passed to the task
     *  - The priority assigned to the task.
     *  - The task handle is NULL */
    xTaskCreate( prvQueueSendTask4,
                 "TX4",
                 configMINIMAL_STACK_SIZE,
                 NULL,
                 mainQUEUE_SEND_TASK_PRIORITY,
                 NULL );
}
/*-----------------------------------------------------------*/

static void prvQueueSendTask1( void *pvParameters )
{
struct AMessage xMessage, *pxPointerToxMessage;
xMessage.ulMessageID = TASK1_ID;

    for (;;)
    {
       /* Pull the current time stamp. */
        xMessage.ulTimeStamp = xTaskGetTickCount();

       /* Send the entire structure by value to the queue. */
       xQueueSend( /* The handle of the queue. */
                   xStructQueue,
                   /* The address of the xMessage variable.
                    * sizeof( struct AMessage ) bytes are copied from here into
                    * the queue. */
                   ( void * ) &xMessage,
                   /* Block time of 0 says don't block if the queue is already
                    * full.  Check the value returned by xQueueSend() to know
                    * if the message was sent to the queue successfully. */
                   ( TickType_t ) 0 );

       /* Store the address of the xMessage variable in a pointer variable. */
       pxPointerToxMessage = &xMessage;

       /* Update the time stamp. */
       xMessage.ulTimeStamp = xTaskGetTickCount();

       /* Send the address (by reference) of xMessage to the queue. */
       xQueueSend( /* The handle of the queue. */
                   xPointerQueue,
                   /* The address of the variable that holds the address of
                    * xMessage.  sizeof( &xMessage ) bytes are copied from here
                    * into the queue.  As the variable holds the address of
                    * xMessage it is the address of xMessage that is copied
                    * into the queue. */
                   ( void * ) &pxPointerToxMessage,
                   ( TickType_t ) 0 );
    }
}
/*-----------------------------------------------------------*/

static void prvQueueSendTask2( void *pvParameters )
{
struct AMessage xMessage, *pxPointerToxMessage;
xMessage.ulMessageID = TASK2_ID;

    for (;;)
    {
       /* Pull the current time stamp. */
        xMessage.ulTimeStamp = xTaskGetTickCount();

       /* Send the entire structure by value to the queue. */
       xQueueSend( /* The handle of the queue. */
                   xStructQueue,
                   /* The address of the xMessage variable.
                    * sizeof( struct AMessage ) bytes are copied from here into
                    * the queue. */
                   ( void * ) &xMessage,
                   /* Block time of 0 says don't block if the queue is already
                    * full.  Check the value returned by xQueueSend() to know
                    * if the message was sent to the queue successfully. */
                   ( TickType_t ) 0 );

       /* Create a 5ms delay. */
       vTaskDelay( pdMS_TO_TICKS( 5 ));

       /* Update the time stamp. */
       xMessage.ulTimeStamp = xTaskGetTickCount();

       /* Store the address of the xMessage variable in a pointer variable. */
       pxPointerToxMessage = &xMessage;

       /* Send the address (by reference) of xMessage to the queue. */
       xQueueSend( /* The handle of the queue. */
                   xPointerQueue,
                   /* The address of the variable that holds the address of
                    * xMessage.  sizeof( &xMessage ) bytes are copied from here
                    * into the queue.  As the variable holds the address of
                    * xMessage it is the address of xMessage that is copied
                    * into the queue. */
                   ( void * ) &pxPointerToxMessage,
                   ( TickType_t ) 0 );

       /* Create a 5ms delay. */
       vTaskDelay( pdMS_TO_TICKS( 5 ));
    }
}
/*-----------------------------------------------------------*/

static void prvQueueSendTask3( void *pvParameters )
{
struct AMessage xMessage, *pxPointerToxMessage;
xMessage.ulMessageID = TASK3_ID;

    for (;;)
    {
       /* Pull the current time stamp. */
        xMessage.ulTimeStamp = xTaskGetTickCount();

       /* Send the entire structure by value to the queue. */
       xQueueSend( /* The handle of the queue. */
                   xStructQueue,
                   /* The address of the xMessage variable.
                    * sizeof( struct AMessage ) bytes are copied from here into
                    * the queue. */
                   ( void * ) &xMessage,
                   /* Block time of 0 says don't block if the queue is already
                    * full.  Check the value returned by xQueueSend() to know
                    * if the message was sent to the queue successfully. */
                    ( TickType_t ) 0 );

       /* Create a 10ms delay. */
       vTaskDelay( pdMS_TO_TICKS( 10 ));

       /* Update the time stamp. */
       xMessage.ulTimeStamp = xTaskGetTickCount();

       /* Store the address of the xMessage variable in a pointer variable. */
       pxPointerToxMessage = &xMessage;

       /* Send the address (by reference) of xMessage to the queue. */
       xQueueSend( /* The handle of the queue. */
                   xPointerQueue,
                   /* The address of the variable that holds the address of
                    * xMessage.  sizeof( &xMessage ) bytes are copied from here
                    * into the queue.  As the variable holds the address of
                    * xMessage it is the address of xMessage that is copied
                    * into the queue. */
                   ( void * ) &pxPointerToxMessage,
                   ( TickType_t ) 0 );

       /* Create a 10ms delay. */
       vTaskDelay( pdMS_TO_TICKS( 10 ));
    }
}
/*-----------------------------------------------------------*/

static void prvQueueSendTask4( void *pvParameters )
{
struct AMessage xMessage, *pxPointerToxMessage;
xMessage.ulMessageID = TASK4_ID;

    for (;;)
    {
       /* Pull the current time stamp. */
        xMessage.ulTimeStamp = xTaskGetTickCount();

       /* Send the entire structure by value to the queue. */
       xQueueSend( /* The handle of the queue. */
                   xStructQueue,
                   /* The address of the xMessage variable.
                    * sizeof( struct AMessage ) bytes are copied from here into
                    * the queue. */
                   ( void * ) &xMessage,
                   /* Block time of 0 says don't block if the queue is already
                    * full.  Check the value returned by xQueueSend() to know
                    * if the message was sent to the queue successfully. */
                   ( TickType_t ) 0 );

       /* Create a 15ms delay. */
       vTaskDelay( pdMS_TO_TICKS( 15 ));

       /* Update the time stamp. */
       xMessage.ulTimeStamp = xTaskGetTickCount();

       /* Store the address of the xMessage variable in a pointer variable. */
       pxPointerToxMessage = &xMessage;

       /* Send the address (by reference) of xMessage to the queue. */
       xQueueSend( /* The handle of the queue. */
                   xPointerQueue,
                   /* The address of the variable that holds the address of
                    * xMessage.  sizeof( &xMessage ) bytes are copied from here
                    * into the queue.  As the variable holds the address of
                    * xMessage it is the address of xMessage that is copied
                    * into the queue. */
                   ( void * ) &pxPointerToxMessage,
                   ( TickType_t ) 0 );

       /* Create a 15ms delay. */
       vTaskDelay( pdMS_TO_TICKS( 15 ));
    }
}
/*-----------------------------------------------------------*/

static void prvQueueReceiveTask1( void *pvParameters )
{
struct AMessage xRxedStructure;

    for (;;)
    {
        if( xStructQueue != NULL )
        {
            /* Receive a message from the created queue to hold complex struct
             * AMessage structure.  Block for 10 ticks if a message is not
             * immediately available.  The value is read into a struct AMessage
             * variable, so after calling xQueueReceive() xRxedStructure will
             * hold a copy of xMessage. */
            if( xQueueReceive( xStructQueue,
                               &( xRxedStructure ),
                               ( TickType_t ) 10 ) == pdPASS )
            {
                /* xRxedStructure now contains a copy of xMessage. */
                UARTprintf("Queue 1 receives from Task %d at time stamp = %d\n",
                           xRxedStructure.ulMessageID,
                           xRxedStructure.ulTimeStamp);
            }
        }
    }
}
/*-----------------------------------------------------------*/

static void prvQueueReceiveTask2( void *pvParameters )
{
struct AMessage *pxRxedPointer;

    for (;;)
    {
        if( xPointerQueue != NULL )
        {
            /* Receive a message from the created queue to hold pointers.
             * Block for 10 ticks if a message is not immediately available.
             * The value is read into a pointer variable, and as the value
             * received is the address of the xMessage variable, after this
             * call pxRxedPointer will point to xMessage. */
            if( xQueueReceive( xPointerQueue,
                               &( pxRxedPointer ),
                               ( TickType_t ) 10 ) == pdPASS )
            {
                /* *pxRxedPointer now points to xMessage. */
                UARTprintf("Queue 2 receives from Task %d at time stamp = %d\n",
                           pxRxedPointer->ulMessageID,
                           pxRxedPointer->ulTimeStamp);

            }
        }
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


