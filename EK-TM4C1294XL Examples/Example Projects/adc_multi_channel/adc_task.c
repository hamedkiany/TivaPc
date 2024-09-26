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
 * This example samples four ADC channels and display the conversion values on
 * the UART terminal window.  The example sets up two tasks with one task
 * periodically taking sample of four ADC channels. When the conversion values
 * are gathered they are sent to the receiving task by means of queue.  The
 * receiving task display the conversion value on the terminal whenever there
 * is data in the queue.
 *
 * This example uses the following I/O signals as the ADC input channels.
 * Supply test voltages to these inputs.  The user must review these and change
 * as needed for any custom boards:
 * - AIN0 - PE3
 * - AIN1 - PE2
 * - AIN2 - PE1
 * - AIN3 - PE0
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
#include "utils/uartstdio.h"
#include "utils/ustdlib.h"
/*-----------------------------------------------------------*/

/*
 * Number of ADC channels to sample.
 */
#define NUM_ADC_CHS                         4

/*
 * Values passed to the two tasks just to check the task parameter
 * functionality.
 */
#define mainQUEUE_SEND_PARAMETER            ( 0x1111UL )
#define mainQUEUE_RECEIVE_PARAMETER         ( 0x22UL )

/*
 * Declare a variable of type QueueHandle_t to hold the handle of
 * the queue being created.
 */
static QueueHandle_t xQueue = NULL;

/*
 * The tasks as described in the comments at the top of this file.
 */
static void prvQueueReceiveTask( void *pvParameters );
static void prvQueueSendTask( void *pvParameters );

/* 
 * Hardware configuration for the ADC to configure channel, sequencer,
 * and interrupts.
 */
static void prvConfigureADC( void );
/*-----------------------------------------------------------*/

void vAdcTask( void )
{
    /* Configure the ADC. */
    prvConfigureADC();

    /* Create the queue with the number of items being equal to the number of
     * of ADC channels being used for the conversions. */
    xQueue = xQueueCreate( NUM_ADC_CHS, sizeof( uint32_t) * NUM_ADC_CHS);

    if( xQueue != NULL )
    {
        /* Create the task as described in the comments at the top of this file.
         *
         * The xTaskCreate parameters in order are:
         *  - The function that implements the task.
         *  - The text name for RX processing task - for debug only as it is
         *    not used by the kernel.
         *  - The size of the stack to allocate to the task.
         *  - The parameter passed to the task - just to check the functionality.
         *  - The priority assigned to the task.
         *  - The task handle is not required, so NULL is passed. */
        xTaskCreate( prvQueueReceiveTask,
                    "RX",
                    configMINIMAL_STACK_SIZE,
                    ( void * ) mainQUEUE_RECEIVE_PARAMETER,
                    tskIDLE_PRIORITY + 1,
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
         *  - The task handle is not required, so NULL is passed. */
        xTaskCreate( prvQueueSendTask,
                     "TX",
                     configMINIMAL_STACK_SIZE,
                     ( void * ) mainQUEUE_SEND_PARAMETER,
                     tskIDLE_PRIORITY + 1,
                     NULL );
    }
}
/*-----------------------------------------------------------*/

static void prvQueueSendTask( void *pvParameters )
{
uint32_t pui32ADC0Value[NUM_ADC_CHS];
BaseType_t xStatus;

    /* Check the task parameter is as expected. */
    configASSERT( ( ( unsigned long ) pvParameters ) == mainQUEUE_SEND_PARAMETER );

    /* Trigger the initial ADC conversion. */
    ADCProcessorTrigger(ADC0_BASE, 0);

    for( ;; )
    {
        /* Wait for the SS0 sequencer to finish conversions on the specified
         * channels. */
        if (ADCIntStatus(ADC0_BASE, 0, false))
        {
            ADCIntClear(ADC0_BASE, 0);
        
            /* Receive the data from the ADC FIFO. */
            ADCSequenceDataGet(ADC0_BASE, 0, &pui32ADC0Value[0]);
            
            /* Send the ADC data to the queue. */
            xStatus = xQueueSendToBack( xQueue, &pui32ADC0Value[0], 0U );
            if (xStatus != pdPASS)
            {
                UARTprintf("Queue was full\n" );
            }
        }
        
        /* Delay for 10ms before triggering the next sample */
        vTaskDelay( pdMS_TO_TICKS( 10 ));
        ADCProcessorTrigger(ADC0_BASE, 0);
    }
}
/*-----------------------------------------------------------*/

static void prvQueueReceiveTask( void *pvParameters )
{
unsigned long ulReceivedValue[NUM_ADC_CHS];
uint32_t ui32LoopCount;
float fVoltage;
char pcBuf[16];

    /* Check the task parameter is as expected. */
    configASSERT( ( ( unsigned long ) pvParameters ) == mainQUEUE_RECEIVE_PARAMETER );

    for( ;; )
    {
        /* Wait until something arrives in the queue - this task will block
         * indefinitely provided INCLUDE_vTaskSuspend is set to 1 in
         * FreeRTOSConfig.h. */
        xQueueReceive( xQueue, &ulReceivedValue[0], portMAX_DELAY );

        /*  To get here something must have been received from the queue */
        for (ui32LoopCount = 0; ui32LoopCount < NUM_ADC_CHS; ui32LoopCount++)
        {
            /* Convert the ADC value to a voltage and display as a string
             * on the terminal. */
            fVoltage = ulReceivedValue[ui32LoopCount] * (3.3 / 4096);
            usprintf(pcBuf, "%d.%d",(int)fVoltage,
                     (int)(fVoltage * 100 - ((int)fVoltage * 100)));
            UARTprintf("AIN%d = %sV\n", ui32LoopCount, pcBuf );
            ulReceivedValue[ui32LoopCount] = 0U;
        }
    }
}
/*-----------------------------------------------------------*/

static void prvConfigureADC( void )
{
    /* Enable the peripherals used by this application. */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0))
    {
    }

    /* Select the analog ADC function for these pins. */
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1 |
                   GPIO_PIN_0);

    /* Enable sample sequence 0 with a processor trigger. */
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);

    /* Configure step 0, 1, 2, 3 on sequence 0.  Sample channel 0, 1, 2 ,3 in
     * single-ended mode (default) and configure the interrupt flag
     * (ADC_CTL_IE) to be set when the channel 3 is done.  Tell the ADC logic
     * that this is the last conversion on sequence 3 (ADC_CTL_END). */
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH0 );
    ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH1 );
    ADCSequenceStepConfigure(ADC0_BASE, 0, 2, ADC_CTL_CH2 );
    ADCSequenceStepConfigure(ADC0_BASE, 0, 3, ADC_CTL_CH3 | ADC_CTL_IE |
                             ADC_CTL_END );

    /* Since sample sequence 0 is now configured, it must be enabled. */
    ADCSequenceEnable(ADC0_BASE, 0);

    /* Clear the interrupt status flag.  This is done to make sure the
     * interrupt flag is cleared before we sample. */
    ADCIntClear(ADC0_BASE, 0);
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
    /* This function will be called by each tick interrupt if
        configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
        added here, but the tick hook is called from an interrupt context, so
        code must not attempt to block, and only the interrupt safe FreeRTOS API
        functions can be used (those that end in FromISR()). */

}


