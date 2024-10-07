#include<stdint.h>
#include<stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/adc.h"
#include "driverlib/timer.h"
#include "configADC.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

static QueueHandle_t cola_adc, cola_adc_live;
int Fs = 5;
uint32_t s = 0;
//Provoca el disparo de una conversion (hemos configurado el ADC con "disparo software" (Processor trigger)
void configADC_DisparaADC(void)
{
    ADCProcessorTrigger(ADC0_BASE, 1);
}

void configADC_IniciaADC(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_ADC0);

    //HABILITAMOS EL GPIOE
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_GPIOE);
    //HABILITAMOS EL GPIOD
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_GPIOD);

    // Enable pin PE3 for ADC AIN0|AIN1|AIN2|AIN3
    GPIOPinTypeADC(GPIO_PORTE_BASE,
    GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0);

    // Enable pin PE3 for ADC AIN5|AIN6
    GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_3 | GPIO_PIN_2);

    //CONFIGURAR SECUENCIADOR 1
    ADCSequenceDisable(ADC0_BASE, 1);

    //Configuramos la velocidad de conversion al maximo (1MS/s)
    ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_RATE_FULL, 1);

    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 1); //Disparo software (processor trigger)
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH0);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH1);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_CH2);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 3,
                             ADC_CTL_TS | ADC_CTL_IE | ADC_CTL_END); //La ultima muestra provoca la interrupcion
    ADCSequenceEnable(ADC0_BASE, 1); //ACTIVO LA SECUENCIA

    //Habilita las interrupciones
    ADCIntClear(ADC0_BASE, 1);
    ADCIntEnable(ADC0_BASE, 1);
    IntPrioritySet(INT_ADC0SS1, configMAX_SYSCALL_INTERRUPT_PRIORITY);
    IntEnable(INT_ADC0SS1);

    //Creamos una cola de mensajes para la comunicacion entre la ISR y la tara que llame a configADC_LeeADC(...)
    cola_adc = xQueueCreate(8, sizeof(MuestrasADC));
    if (cola_adc == NULL)
    {
        while (1)
            ;
    }
}

void configADC_LeeADC(MuestrasADC *datos)
{
    xQueueReceive(cola_adc, datos, portMAX_DELAY);
}
void configADC_Timer(void)
{
   SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_ADC0);

    //HABILITAMOS EL GPIOE
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_GPIOE);
    //HABILITAMOS EL GPIOD
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_GPIOD);

    // Enable pin PE3 for ADC AIN0|AIN1|AIN2|AIN3
    GPIOPinTypeADC(GPIO_PORTE_BASE,
    GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0);

    // Enable pin PD3 for ADC AIN5|AIN6
    GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_3 | GPIO_PIN_2);

    //CONFIGURAR SECUENCIADOR 0
    ADCSequenceDisable(ADC0_BASE, 0);

    //Configuramos la velocidad de conversion al maximo (1MS/s)
    ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_RATE_FULL, 1);

    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_TIMER, 1); //Disparo  (timer trigger)
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH0);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH1);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 2, ADC_CTL_CH2);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 3, ADC_CTL_TS); // |ADC_CTL_IE |ADC_CTL_END);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 4, ADC_CTL_CH4);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 5, ADC_CTL_CH5);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 6, ADC_CTL_CH6);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 7,
    ADC_CTL_TS | ADC_CTL_IE | ADC_CTL_END); //La ultima muestra provoca la interrupcion
    ADCSequenceEnable(ADC0_BASE, 0); //ACTIVO LA SECUENCIA
    IntPrioritySet(INT_ADC0SS0, configMAX_SYSCALL_INTERRUPT_PRIORITY);
    //Habilitar interrupción por parte del ADC
    ADCIntEnable(ADC0_BASE, 0);
    ADCIntRegister(ADC0_BASE, 0, Timer2IntHandler);
    IntEnable(INT_ADC0SS0);
    //Habilitar Temporizador 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_TIMER2);
    //Configurar temporizador como periódico
    TimerConfigure(TIMER2_BASE, TIMER_CFG_A_PERIODIC);
    //Cargar el valor que contará el temporizador
    uint32_t ui32Period = SysCtlClockGet() / Fs;
    TimerLoadSet(TIMER2_BASE, TIMER_A, ui32Period - 1);    //Habilitar disparador de ADC por temporizador
    TimerControlTrigger(TIMER2_BASE, TIMER_A, true);
    TimerEnable(TIMER2_BASE, TIMER_A);
    //Registrar ISR
    IntMasterEnable();

    cola_adc_live = xQueueCreate(8, sizeof(MuestrasADCLive));
    if (cola_adc_live == NULL)
    {
        while (1)
            ;
    }

}
void configADC_LiveADC(MuestrasADCLive *datos)
{
//    UARTprintf("configADC_LeeADC(MuestrasADC *datos) en configADC antes \r\n");

    xQueueReceive(cola_adc_live, datos, portMAX_DELAY);
//  UARTprintf("configADC_LeeADC(MuestrasADC *datos) en configADC despues \r\n");
}
void configADC_ISR(void)
{
    portBASE_TYPE higherPriorityTaskWoken = pdFALSE;

    MuestrasLeidasADC leidas;
    MuestrasADC finales;
    ADCIntClear(ADC0_BASE, 1); //LIMPIAMOS EL FLAG DE INTERRUPCIONES
    ADCSequenceDataGet(ADC0_BASE, 1, (uint32_t*) &leidas); //COGEMOS LOS DATOS GUARDADOS

    //Pasamos de 32 bits a 16 (el conversor es de 12 bits, así que sólo son significativos los bits del 0 al 11)
    finales.chan1 = leidas.chan1;
    finales.chan2 = leidas.chan2;
    finales.chan3 = leidas.chan3;
    finales.chan4 = leidas.chan4;
    finales.chan5 = leidas.chan5;
    finales.chan6 = leidas.chan6;
    finales.chan7 = leidas.chan7;
    finales.chan8 = leidas.chan8;

    //Guardamos en la cola
    xQueueSendFromISR(cola_adc, &finales, &higherPriorityTaskWoken);
    portEND_SWITCHING_ISR(higherPriorityTaskWoken);
}

void Timer2IntHandler(void)
{
    ADCIntClear(ADC0_BASE, 0);
    portBASE_TYPE higherPriorityTaskWoken = pdFALSE;

    MuestrasLeidasADCLive leidas;
    MuestrasADCLive finales;
    // Borra la interrupcion de Timer
    TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);

    ADCSequenceDataGet(ADC0_BASE, 0, (uint32_t*) &leidas); //COGEMOS LOS DATOS GUARDADOS

    //Pasamos de 32 bits a 16 (el conversor es de 12 bits, así que sólo son significativos los bits del 0 al 11)
    finales.chan1 = leidas.chan1;
    finales.chan2 = leidas.chan2;
    finales.chan3 = leidas.chan3;
    finales.chan4 = leidas.chan4;
    finales.chan5 = leidas.chan5;
    finales.chan6 = leidas.chan6;
    finales.chan7 = leidas.chan7;
    finales.chan8 = leidas.chan8;

    //Guardamos en la cola
    xQueueSendFromISR(cola_adc_live, &finales, &higherPriorityTaskWoken);
    portEND_SWITCHING_ISR(higherPriorityTaskWoken);

}

void CambiarFrecuencia(float freq)
{
//    uint32_t ui32Period = SysCtlClockGet() / Fs;
//    TimerLoadSet(TIMER2_BASE, TIMER_A, ui32Period - 1); //5 segundo
}
