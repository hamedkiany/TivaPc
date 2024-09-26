/*
 * adc_task
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
 * The vADCTask task first configures the ADC 0 peripheral of the TM4C1294NCPDT
 * MCU to sample on channel 0 (PE3).  It then creates the queue that will be
 * used to pass the ADC data to the data processes task.  Lastly, it creates
 * the tasks for both receiving ADC data after an interrupt and then processing
 * the data once a buffer is filled.
 *
 * The ISR for ADC 0 Sequence 0 is xADCSeq0Handler and it is linked to the
 * interrupt vector table for the TM4C1294NCPDT MCU in the startup_ccs.c file.
 * Instead of manually linking the ISR, the IntRegister API can be used
 * instead.  The ISR clears the hardware interrupt flag and then uses the
 * FreeRTOS Task Notify feature to defer further processing to a task.  This
 * makes for a very lean ISR which is recommended, especially for hardware
 * interrupts.
 *
 * The prvADCIntTask waits for the for the notification from the ISR and then
 * once received it checks which buffer is being filled, stores the ADC data
 * in the correct buffer, and then checks if the buffer is full.  If a buffer
 * has been filled, then the queue is used to pass the pointer of the buffer
 * to the data processing task so that the full contents can be accessed from
 * the current memory location.  By using a queue, there is no need to change
 * the memory location for the 256 bytes of data which improves processing
 * time and lowers overhead.
 *
 * The prvADCTask waits for a queue to be sent and then begins to process and
 * clear data from the buffer.  The data is summed together to calculate the
 * average and output the result over UART.  Since this task is altering the
 * data in the buffer that was passed by a queue, the timing was tested to
 * validate that the buffer alternations are completed well before the original
 * task would access / alter the buffer contents.
 *
 * This example uses AIN0 which is mapped to PE3 as the ADC input channel.
 * Supply the test voltage to this input.
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
#include "semphr.h"

/* Hardware includes. */
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
/*-----------------------------------------------------------*/

/*
 * Define the size of the ADC buffers.
 */
#define ADC_SAMPLE_BUF_SIZE 256

/*
 * Declare a variable of type QueueHandle_t to hold the handle of
 * the queue being created.
 */
QueueHandle_t xPointerQueue = NULL;

/*
 * Declare a variable that is used to hold the handle of the ADC
 * interrupt task.
 */
TaskHandle_t xADCIntTask = NULL;

/*
 * The tasks as described in the comments at the top of this file.
 */
static void prvADCTask( void *pvParameters );
static void prvADCIntTask( void *pvParameters );

/*
 * Called by main() to create the ADC task.
 */
void vADCTask( void );

/* 
 * Hardware configuration for the ADC to configure channel, sequencer,
 * and interrupts.
 */
static void prvConfigureADC( void );
/*-----------------------------------------------------------*/

void vADCTask( void )
{
    /* Configure the ADC to use Channel 0 on Sequencer 0, be timer triggered,
     * and to fire an interrupt at the end of each sample sequence. */
    prvConfigureADC();

    /* Create a queue that can hold a maximum of 2 pointers. */
    xPointerQueue = xQueueCreate( 2, sizeof( uint32_t * ) );

    /* Create the task as described in the comments at the top of this file.
     *
     * The xTaskCreate parameters in order are:
     *  - The function that implements the task.
     *  - The text name for ADC data processing task - for debug only as it is
     *    not used by the kernel.
     *  - The size of the stack to allocate to the task.
     *  - The parameter passed to the task - just to check the functionality.
     *  - The priority assigned to the task.
     *  - The task handle is not required, so NULL is passed. */
    xTaskCreate( prvADCTask,
                 "ADC Data",
                 configMINIMAL_STACK_SIZE,
                 NULL,
                 tskIDLE_PRIORITY + 1,
                 NULL );

    /* Create the task as described in the comments at the top of this file.
     *
     * The xTaskCreate parameters in order are:
     *  - The function that implements the task.
     *  - The text name for ADC interrupt task - for debug only as it is not
     *    used by the kernel.
     *  - The size of the stack to allocate to the task.  Since this task is
     *    creating multiple buffers that are used to store data, the stack size
     *    must be increased to account for the size of these buffers or else
     *    the task would run out of memory.
     *  - The parameter passed to the task - just to check the functionality.
     *  - The priority assigned to the task.  Because it is interrupt driven,
     *    this task should preempt the data processing task.
     *  - The task handle used for the task notify. */
    xTaskCreate( prvADCIntTask,
                 "ADC SS0 Int",
                 configMINIMAL_STACK_SIZE + ADC_SAMPLE_BUF_SIZE*2,
                 NULL,
                 tskIDLE_PRIORITY + 2,
                 &xADCIntTask );
}
/*-----------------------------------------------------------*/

static void prvADCIntTask( void *pvParameters )
{
unsigned long ulEventsToProcess;
uint32_t pui32ADCBuffer1[ADC_SAMPLE_BUF_SIZE] = {0};
uint32_t pui32ADCBuffer2[ADC_SAMPLE_BUF_SIZE] = {0};
uint32_t *pui32PointerToData;
uint32_t ui32BufferIndex = 0;
bool bBufferSelect = 1;

    for( ;; )
    {
        /* Wait to receive a notification sent directly to this task from the
         * interrupt service routine. */
        ulEventsToProcess = ulTaskNotifyTake( pdTRUE, portMAX_DELAY );

        if (ulEventsToProcess != 0)
        {
            /* Read latest ADC Data. */
            if (bBufferSelect)
            {
                /* Store ADC reading into the next slot in the first buffer. */
                ADCSequenceDataGet(ADC0_BASE, 0,
                                   &pui32ADCBuffer1[ui32BufferIndex]);

                /* Increase buffer and check count. */
                ui32BufferIndex++;

                if (ui32BufferIndex == ADC_SAMPLE_BUF_SIZE)
                {
                    /* Buffer is full, clear index and swap buffers. */
                    ui32BufferIndex = 0;
                    bBufferSelect = 0;

                    pui32PointerToData = &pui32ADCBuffer1[0];

                    /* Use a queue to send the pointer of the array to the ADC
                    task.
                    * The xQueueSend parameters in order are:
                    *  - The handle of the queue.
                    *  - The address of the pointer that points to the buffer.
                    *  - Max time to block until there is space on the queue.
                    */
                    xQueueSend( xPointerQueue,
                                &pui32PointerToData,
                                portMAX_DELAY );
                }
            }
            else
            {
                /* Store ADC reading into the next slot in the first buffer. */
                ADCSequenceDataGet(ADC0_BASE, 0,
                                   &pui32ADCBuffer2[ui32BufferIndex]);

                /* Increase buffer and check count. */
                ui32BufferIndex++;

                if (ui32BufferIndex == ADC_SAMPLE_BUF_SIZE)
                {
                    /* Buffer is full, clear index and swap buffers. */
                    ui32BufferIndex = 0;
                    bBufferSelect = 1;

                    pui32PointerToData = &pui32ADCBuffer2[0];

                    /* Use a queue to send the pointer of the array to the ADC
                    task.
                    * The xQueueSend parameters in order are:
                    *  - The handle of the queue.
                    *  - The address of the pointer that points to the buffer.
                    *  - Max time to block until there is space on the queue.
                    */
                    xQueueSend( xPointerQueue,
                                &pui32PointerToData,
                                portMAX_DELAY );
                }
            }
        }
        else
        {
            /* Timed out. */
        }
    }
}
/*-----------------------------------------------------------*/

static void prvADCTask( void *pvParameters )
{
BaseType_t xStatus;
uint32_t *pui32ADCBuffer;
uint32_t ui32Count;
uint32_t ui32AverageResult;

    for( ;; )
    {
        /* Receive the address of a buffer. */
        xStatus = xQueueReceive( xPointerQueue, /* The handle of the queue. */
        &pui32ADCBuffer, /* Store the buffer’s address in pui32ADCBuffer. */
        portMAX_DELAY );

        if ( xStatus == pdPASS )
        {
            /* Clear the average result for the calculation. */
            ui32AverageResult = 0;

            /* Process the data in pui16ADCBuffer and clear buffer entries. */
            for(ui32Count = 0; ui32Count < ADC_SAMPLE_BUF_SIZE; ui32Count++)
            {
                ui32AverageResult += pui32ADCBuffer[ui32Count];
                pui32ADCBuffer[ui32Count] = 0;
            }

            /* Calculate the average result. */
            ui32AverageResult = ((ui32AverageResult +
                    (ADC_SAMPLE_BUF_SIZE / 2)) /
                    ADC_SAMPLE_BUF_SIZE);

            /* Display a message with the received ADC data. */
            UARTprintf("ADC Result: %4d\rSamples Taken:%4d\r",
                       ui32AverageResult, ADC_SAMPLE_BUF_SIZE);
        }
    }
}
/*-----------------------------------------------------------*/

static void prvConfigureADC( void )
{
    /* Enable the GPIO pin for ADC0 Channel 0 (PE3) and configure it for
     * analog functionality. */
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);

    /* Enable the peripherals used by this application. */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0))
    {
    }

    /* Disable interrupts for ADC0 sample sequence 0 to configure it. */
    ADCIntDisable(ADC0_BASE, 0);

    /* Disable ADC0 sample sequence 0.  With the sequence disabled, it is now
     * safe to load the new configuration parameters. */
    ADCSequenceDisable(ADC0_BASE, 0);

    /* Enable sample sequence 0 with a timer trigger.  Sequence 0 will start
     * the next conversion each time the timer times out. */
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_TIMER, 0);

    /* Configure step 0 on sequence 0.  Sample channel 0 (ADC_CTL_CH0) in
     * single-ended mode (default) and configure the interrupt flag
     * (ADC_CTL_IE) to be set when the sample is done.  Tell the ADC logic
     * that this is the last conversion on sequence 0 (ADC_CTL_END).  Sequence
     * 0 has 8 programmable steps.  Since we are only doing a single conversion
     * using sequence 0 we will only configure step 0.  For more information
     * on the ADC sequences and steps, reference the datasheet. */
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH0 | ADC_CTL_IE |
                             ADC_CTL_END);

    /* Since sample sequence 0 is now configured, it must be enabled. */
    ADCSequenceEnable(ADC0_BASE, 0);

    /* Clear the interrupt status flag.  This is done to make sure the
     * interrupt flag is cleared before we sample. */
    ADCIntClear(ADC0_BASE, 0);

    /* Enable the ADC 0 sample sequence 0 interrupt. */
    ADCIntEnable(ADC0_BASE, 0);

    /* Enable the interrupt for ADC0 sequence 0 on the processor (NVIC). */
    IntEnable(INT_ADC0SS0);
}
/*-----------------------------------------------------------*/

void xADCSeq0Handler( void )
{
BaseType_t xADCTaskWoken;

    /* Clear the hardware interrupt flag for ADC0. */
    ADCIntClear(ADC0_BASE, 0);

    /* The xADCTaskWoken parameter must be initialized to pdFALSE as
     * it will get set to pdTRUE inside the interrupt safe API function if a
     * context switch is required. */
    xADCTaskWoken = pdFALSE;

    /* Defer the interrupt processing to a Task to minimize time spent within
     * the hardware interrupt service routine.  Send a notification directly to
     * the task to which interrupt processing is being deferred. */
    vTaskNotifyGiveFromISR( xADCIntTask, &xADCTaskWoken );

    /* This FreeRTOS API call will handle the context switch if it is required
     * or have no effect if that is not needed. */
    portYIELD_FROM_ISR( xADCTaskWoken );
}


