//*****************************************************************************
//
// Codigo de partida Practica 1.
// Autores: Eva Gonzalez, Ignacio Herrero, Jose Manuel Cano
//
//*****************************************************************************

#include<stdbool.h>
#include<stdint.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "driverlib/adc.h"
#include "driverlib/timer.h"
#include "utils/uartstdio.h"
#include "drivers/buttons.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "utils/cpu_usage.h"
#include "timers.h"              // FreeRTOS: definiciones relacionadas con timers

#include "drivers/rgb.h"
#include "drivers/configADC.h"
#include "commands.h"

#include <remotelink.h>
#include <serialprotocol.h>
#include "drivers/ACME_3416.h"

//parametros de funcionamiento de la tareas
#define REMOTELINK_TASK_STACK (512)
#define REMOTELINK_TASK_PRIORITY (tskIDLE_PRIORITY+2)
#define COMMAND_TASK_STACK (512)
#define COMMAND_TASK_PRIORITY (tskIDLE_PRIORITY+1)
#define ADC_TASK_STACK (256)
#define ADC_TASK_PRIORITY (tskIDLE_PRIORITY+1)
#define ADC_LIVE_STACK (256)
#define ADC_LIVE_PRIORITY (tskIDLE_PRIORITY+1)
#define SW1TASKPRIO (tskIDLE_PRIORITY+1)            // Prioridad para la tarea SW1TASK
#define SW1TASKSTACKSIZE (256)     // Tamaño de pila para la tarea SW1TASK
#define SW2TASKPRIO (tskIDLE_PRIORITY+1)            // Prioridad para la tarea SW1TASK
#define SW2TASKSTACKSIZE (256)     // Tamaño de pila para la tarea SW1TASK
#define ACMINTTASKPRIO (tskIDLE_PRIORITY+1)            // Prioridad para la tarea ACMINT
#define ACMINTTASKSTACKSIZE (256)     // Tamaño de pila para la tarea ACMINT

//Globales
uint32_t g_ui32CPUUsage;
uint32_t g_ulSystemClock;
SemaphoreHandle_t miSemaforo,miSemaforo2,miSemaforoACM;
TimerHandle_t xTimer;
float freqKNOB =5.0;

/* An array to hold a count of the number of times each timer expires. */
uint32_t ui32ExpireCounters =  0 ;
int32_t i32Estado_led=0;

//static uint32_t pui32Parametros1[2];
//static uint32_t pui32Parametros2[2];

//Variables globales tarea Switch1Task + interrupcion boton
//volatile int32_t i32Semaforo_malo1=0;

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
// Esta funcion se llama si la biblioteca driverlib o FreeRTOS comprueban la existencia de un error (mediante
// las macros ASSERT(...) y configASSERT(...)
// Los parametros nombrefich y linea contienen informacion de en que punto se encuentra el error...
//
//*****************************************************************************
#ifdef DEBUG
void __error__(char *nombrefich, uint32_t linea)
{
    while(1) //Si la ejecucion esta aqui dentro, es que el RTOS o alguna de las bibliotecas de perifericos han comprobado que hay un error
    { //Mira el arbol de llamadas en el depurador y los valores de nombrefich y linea para encontrar posibles pistas.
       // UARTprintf("dentro de while __error__\r\n");
        //break;
    }
}
#endif

//*****************************************************************************
//
// Aqui incluimos los "ganchos" a los diferentes eventos del FreeRTOS
//
//*****************************************************************************

//Esto es lo que se ejecuta cuando el sistema detecta un desbordamiento de pila
//

void vApplicationStackOverflowHook(TaskHandle_t pxTask,  char *pcTaskName)
{
	//
	// This function can not return, so loop forever.  Interrupts are disabled
	// on entry to this function, so no processor interrupts will interrupt
	// this loop.
	//
	while(1)
	{
	}
}

//Esto se ejecuta cada Tick del sistema. LLeva la estadistica de uso de la CPU (tiempo que la CPU ha estado funcionando)
void vApplicationTickHook( void )
{
	static uint8_t count = 0;

	if (++count == 10)
	{
		g_ui32CPUUsage = CPUUsageTick();
		count = 0;
	}
	//return;
}

//Esto se ejecuta cada vez que entra a funcionar la tarea Idle
void vApplicationIdleHook (void)
{
	SysCtlSleep();
}


//Esto se ejecuta cada vez que entra a funcionar la tarea Idle
void vApplicationMallocFailedHook (void)
{
	while(1);
}


void vTimerCallback( TimerHandle_t pxTimer )
{
const uint32_t xMaxExpiryCountBeforeStopping = 100;

    /* Optionally do something if the pxTimer parameter is NULL. */
    configASSERT( pxTimer );
    MESSAGE_B160_VELO_PARAMETER paramvelo;
    MESSAGE_B160_ACC_PARAMETER paramacc;
    BMI160_getAcceleration(&paramvelo.x,&paramvelo.y,&paramvelo.z);
    BMI160_getRotation(&paramacc.x,&paramacc.y,&paramacc.z);
    remotelink_sendMessage(MESSAGE_B160_VELO,(void *)&paramvelo,sizeof(paramvelo));
    remotelink_sendMessage(MESSAGE_B160_ACC,(void *)&paramacc,sizeof(paramacc));

    /* Increment the number of times that pxTimer has expired. */
    ui32ExpireCounters += 1;

    /* If the timer has expired 10 times then stop it from running. */
    if( ui32ExpireCounters == 100000 * xMaxExpiryCountBeforeStopping )
    {
        /* Do not use a block time if calling a timer API function from a
        timer callback function, as doing so could cause a deadlock! */
        xTimerStop( pxTimer, 0 );
    }
    else
       {
          /* Store the incremented count back into the timer's ID field
          so it can be read back again the next time this software timer
          expires. */
          vTimerSetTimerID( xTimer, ( void * ) ui32ExpireCounters );
       }
}
//*****************************************************************************
//
// A continuacion van las tareas...
//
//*****************************************************************************


//Para especificacion 2. Esta tarea no tendria por que ir en main.c
static portTASK_FUNCTION(ADCTask,pvParameters)
{

    MuestrasADC muestras;
    MESSAGE_ADC_SAMPLE_PARAMETER parameter;

//    UARTprintf("portTASK_FUNCTION(ADCTask,pvParameters) \r\n");
    //
    // Bucle infinito, las tareas en FreeRTOS no pueden "acabar", deben "matarse" con la funcion xTaskDelete().
    //
    while(1)
    {

//        UARTprintf("portTASK_FUNCTION(ADCTask,pvParameters) antes leer \r\n");

        configADC_LeeADC(&muestras);    //Espera y lee muestras del ADC (BLOQUEANTE)

        //Copia los datos en el parametro (es un poco redundante)
        parameter.chan1=muestras.chan1;
        parameter.chan2=muestras.chan2;
        parameter.chan3=muestras.chan3;
        parameter.chan4=muestras.chan4;
        parameter.chan5=muestras.chan5;
        parameter.chan6=muestras.chan6;
        parameter.chan7=muestras.chan7;
        parameter.chan8=muestras.chan8;

        //UARTprintf("portTASK_FUNCTION(ADCTask,pvParameters) antes mandar \r\n");

        //Encia el mensaje hacia QT
        remotelink_sendMessage(MESSAGE_ADC_SAMPLE,(void *)&parameter,sizeof(parameter));
    }
}


//Para especificacion 2. Esta tarea no tendria por que ir en main.c
static portTASK_FUNCTION(ADCTaskLive,pvParameters)
{

    MuestrasADCLive muestras;
    MESSAGE_ADC_LIVE_PARAMETER parameter;

//    UARTprintf("portTASK_FUNCTION(ADCTask,pvParameters) \r\n");
    //
    // Bucle infinito, las tareas en FreeRTOS no pueden "acabar", deben "matarse" con la funcion xTaskDelete().
    //
    while(1)
    {

//        UARTprintf("portTASK_FUNCTION(ADCTask,pvParameters) antes leer \r\n");

        configADC_LiveADC(&muestras);    //Espera y lee muestras del ADC (BLOQUEANTE)

        //Copia los datos en el parametro (es un poco redundante)
        parameter.chan1=muestras.chan1;
        parameter.chan2=muestras.chan2;
        parameter.chan3=muestras.chan3;
        parameter.chan4=muestras.chan4;
        parameter.chan5=muestras.chan5;
        parameter.chan6=muestras.chan6;
        parameter.chan7=muestras.chan7;
        parameter.chan8=muestras.chan8;

//        UARTprintf("portTASK_FUNCTION(ADCLIVE,pvParameters) antes mandar \r\n");

        //Encia el mensaje hacia QT
        remotelink_sendMessage(MESSAGE_ADC_LIVE,(void *)&parameter,sizeof(parameter));
    }
}


//*****************************************************************************
//
// This task waits for button pushed
//
//*****************************************************************************

//static void Switch1Task(void *pvParameters)
static portTASK_FUNCTION(Switch1Task,pvParameters)
{

    xSemaphoreTake(miSemaforo,portMAX_DELAY);
    //
    // Loop forever.
    //
    while(1)
    {
        MESSAGE_SW_PARAMETER parametro;
        parametro.sw.number = 1;
        //parametro.sw.state = 1;

        xSemaphoreTake(miSemaforo,portMAX_DELAY);
        remotelink_sendMessage(MESSAGE_SW,&parametro,sizeof(parametro));
       // UARTprintf("He puesto botton ye mandado mensaje\n");
    }
}

static portTASK_FUNCTION(Switch2Task,pvParameters)
{
    xSemaphoreTake(miSemaforo2,portMAX_DELAY);
    //
    // Loop forever.
    //
    while(1)
    {
        MESSAGE_SW_PARAMETER parametro;
        parametro.sw.number = 2;
        //parametro.sw.state = 1;

        xSemaphoreTake(miSemaforo2,portMAX_DELAY);
        remotelink_sendMessage(MESSAGE_SW,&parametro,sizeof(parametro));
        //UARTprintf("He puesto botton drecha ye mandado mensaje\n");
    }
}

static portTASK_FUNCTION(ACMINTTask,pvParameters)
{

    xSemaphoreTake(miSemaforoACM,portMAX_DELAY);
    //
    // Loop forever.
    //
    while(1)
    {
       uint8_t pinn ;
//       if (ACME_readPin(&pinn)<0)
//        {
//            UARTprintf("Error en el proceso...\r\n");
//        }
//        else
//        {
//            UARTprintf("OK.");
//            UARTprintf(" %x \r\n",(uint32_t)pinn);
//        }
        MESSAGE_ACM_INT_PARAMETER parametro;
        uint8_t mask0 = 1 << 0;
        uint8_t mask1 = 1 << 1;
        uint8_t mask2 = 1 << 2;
        int mask3 = 1 << 3;
        parametro.GPacm.GP0 = (pinn & mask0) >> 0;
        parametro.GPacm.GP1 = (pinn & mask1) >> 1;
        parametro.GPacm.GP2 = (pinn & mask2) >> 2;
        parametro.GPacm.GP3 = (pinn & mask3) >> 3;
        UARTprintf(" %x \r\n",(uint8_t)pinn);
        xSemaphoreTake(miSemaforoACM,portMAX_DELAY);
        remotelink_sendMessage(MESSAGE_ACM_INT,&parametro,sizeof(parametro));
       // UARTprintf("He puesto botton ye mandado mensaje\n");
    }
}


//Funcion callback que procesa los mensajes recibidos desde el PC (ejecuta las acciones correspondientes a las ordenes recibidas)
static int32_t messageReceived(uint8_t message_type, void *parameters, int32_t parameterSize)
{
    int32_t status=0;   //Estado de la ejecucion (positivo, sin errores, negativo si error)

    //Comprueba el tipo de mensaje
    switch (message_type)
    {
        case MESSAGE_PING:
        {
            status=remotelink_sendMessage(MESSAGE_PING,NULL,0);
        }
        break;
        case MESSAGE_LED_GPIO:
        {
                MESSAGE_LED_GPIO_PARAMETER parametro;

                if (check_and_extract_command_param(parameters, parameterSize, &parametro, sizeof(parametro))>0)
                {
                    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,parametro.value);
                }
                else
                {
                    status=PROT_ERROR_INCORRECT_PARAM_SIZE; //Devuelve un error
                }
        }
        break;
        case MESSAGE_ACM_GPIO:
        {
                MESSAGE_ACM_GPIO_PARAMETER parametro;

                if (check_and_extract_command_param(parameters, parameterSize, &parametro, sizeof(parametro))>0)
                {
                    uint8_t val;
                    val = (((uint8_t)parametro.GPacm.GP7) << 7) | ((uint8_t)parametro.GPacm.GP6 << 6) | ((uint8_t)parametro.GPacm.GP5 << 5) | ((uint8_t)parametro.GPacm.GP4 << 4) | (0 << 3) | (0 << 2) |(0 << 1) | (0)  ;
                    if (ACME_setPinDir(val)<0)
                    {
                        UARTprintf("Error en el proceso...\r\n");
                    }
                    else
                    {
                        UARTprintf("OK\r\n");
                    }
                    //                    //UARTprintf("  MESSAGE_ACM_GPIO %d\r\n",val);
                    UARTprintf("  MESSAGE_ACM_GPIO\r\n");
                }
                else
                {
                    status=PROT_ERROR_INCORRECT_PARAM_SIZE; //Devuelve un error
                }
        }
        break;

        case MESSAGE_Mode_Cambio:
        {
            MESSAGE_Mode_PARAMETER parametro;

            if (check_and_extract_command_param(parameters, parameterSize, &parametro, sizeof(parametro)) > 0)
            {
                if(parametro.Mode == 1)
                {
                    RGBEnable();
                    //UARTprintf("cambio a modo PWM \r\n");
                }
                else
                {
                    RGBDisable();
                    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
                    //UARTprintf("cambio a modo GPIO\r\n");

                }

            }
            else
            {
                UARTprintf(" no cambio nada GPIO\r\n");            }
        }
        break;

        case MESSAGE_LED_Color:
        {
            MESSAGE_LED_Color_PARAMETER parametro;

            if (check_and_extract_command_param(parameters, parameterSize, &parametro, sizeof(parametro))>0)
            {
                uint32_t arrayRGB[3];
                arrayRGB[0]=parametro.leds.red * 100;
                arrayRGB[1]=parametro.leds.green * 100;
                arrayRGB[2]=parametro.leds.blue * 100 ;
                //UARTprintf(" valor de color %d %d %d\r\n",arrayRGB[0],arrayRGB[1],arrayRGB[2]);
                if ((arrayRGB[0]>=65535)||(arrayRGB[1]>=65535)||(arrayRGB[2]>=65535))
                {

                    UARTprintf(" \r\n");
                }
                else{
                    RGBColorSet(arrayRGB);
                }

            }
            else
            {
                status=PROT_ERROR_INCORRECT_PARAM_SIZE; //Devuelve un error
            }
        }
        break;

        case MESSAGE_LED_PWM_BRIGHTNESS:
        {
            MESSAGE_LED_PWM_BRIGHTNESS_PARAMETER parametro;
            //UARTprintf(" MESSAGE_LED_PWM_BRIGHTNESS\r\n");

            if (check_and_extract_command_param(parameters, parameterSize, &parametro, sizeof(parametro))>0)
            {
                //UARTprintf(" MESSAGE_LED_PWM_BRIGHTNESS parameter\r\n" );
                //UARTprintf("%d ",parametro.rIntensity);
                if ((parametro.rIntensity>=0.0)&&(parametro.rIntensity<=1.0))
                 {
                     RGBIntensitySet(parametro.rIntensity);

                 }
                 else
                 {
                     UARTprintf("El valor de intensidad deberia valer entre 0 y 1\r\n");
                 }
            }
            else
            {
                status=PROT_ERROR_INCORRECT_PARAM_SIZE; //Devuelve un error
            }
        }
        break;
        case MESSAGE_ADC_SAMPLE:
        {
//            UARTprintf("MESSAGE_ADC_SAMPLE \r\n");
            configADC_DisparaADC(); //Dispara la conversion (por software)
//            UARTprintf("MESSAGE_ADC_SAMPLE Read\r\n");
        }
        break;
        case MESSAGE_SW:
        {
            MESSAGE_SW_PARAMETER parametro;
            //UARTprintf("cambio a sw mode \r\n");

            if (check_and_extract_command_param(parameters, parameterSize, &parametro, sizeof(parametro)) > 0)
            {
                //UARTprintf("  cambio en SW\r\n");
                //UARTprintf("%d\r\n", parametro.sw.state);
                if(parametro.sw.state == 1)
                {
                    MAP_IntPrioritySet(INT_GPIOF,configMAX_SYSCALL_INTERRUPT_PRIORITY);//para aÃ±adir prioridad by HAMED
                    MAP_GPIOIntEnable(GPIO_PORTF_BASE,ALL_BUTTONS);
                    MAP_IntEnable(INT_GPIOF);
                }
                else
                {
                    MAP_GPIOIntDisable(GPIO_PORTF_BASE,ALL_BUTTONS);
                    MAP_IntDisable(INT_GPIOF);

                }

            }
            else
            {
                UARTprintf(" no cambio nada GPIO\r\n");            }
        }

        break;

        case MESSAGE_B160_FRQ:
        {
            MESSAGE_B160_FRQ_PARAMETER parametro;
            if (check_and_extract_command_param(parameters, parameterSize, &parametro, sizeof(parametro))>0)
            {
                if ((parametro.frq>=5.0)&&(parametro.frq<=20.0))
                 {
                    freqKNOB = parametro.frq;

                 }
                 else
                 {
                     UARTprintf("El valor de freq deberia valer entre 5 y 20\r\n");
                 }
            }
            else
            {
                status=PROT_ERROR_INCORRECT_PARAM_SIZE; //Devuelve un error
            }
        }
        break;

       default:
           //mensaje desconocido/no implementado
           status=PROT_ERROR_UNIMPLEMENTED_COMMAND; //Devuelve error.
    }


    return status;   //Devuelve status
}


//*****************************************************************************
//
// Funcion main(), Inicializa los perifericos, crea las tareas, etc... y arranca el bucle del sistema
//
//*****************************************************************************
int main(void)
{

	//
	// Set the clocking to run at 40 MHz from the PLL.
	//
	MAP_SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
			SYSCTL_OSC_MAIN);	//Ponermos el reloj principal a 40 MHz (200 Mhz del Pll dividido por 5)


	// Get the system clock speed.
	g_ulSystemClock = SysCtlClockGet();


	//Habilita el clock gating de los perifericos durante el bajo consumo --> perifericos que se desee activos en modo Sleep
	//                                                                        deben habilitarse con SysCtlPeripheralSleepEnable
	MAP_SysCtlPeripheralClockGating(true);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	//MAP_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_GPIOF);
	// Inicializa el subsistema de medida del uso de CPU (mide el tiempo que la CPU no esta dormida)
	// Para eso utiliza un timer, que aqui hemos puesto que sea el TIMER3 (ultimo parametro que se pasa a la funcion)
	// (y por tanto este no se deberia utilizar para otra cosa).
	CPUUsageInit(g_ulSystemClock, configTICK_RATE_HZ/10, 3);

	//Inicializa los LEDs usando libreria RGB --> usa Timers 0 y 1
	RGBInit(1);
	MAP_SysCtlPeripheralSleepEnable(GREEN_TIMER_PERIPH);
	MAP_SysCtlPeripheralSleepEnable(BLUE_TIMER_PERIPH);
	MAP_SysCtlPeripheralSleepEnable(RED_TIMER_PERIPH);	//Redundante porque BLUE_TIMER_PERIPH y GREEN_TIMER_PERIPH son el mismo

	//Volvemos a configurar los LEDs en modo GPIO POR Defecto
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    //Inicializa los botones (tambien en el puerto F) y habilita sus interrupciones---Hamed
	 ButtonsInit();
	    //MAP_GPIOIntTypeSet(GPIO_PORTF_BASE, ALL_BUTTONS,GPIO_FALLING_EDGE);
     MAP_IntPrioritySet(INT_GPIOF,configMAX_SYSCALL_INTERRUPT_PRIORITY);//para aÃ±adir prioridad by HAMED
     MAP_GPIOIntEnable(GPIO_PORTF_BASE,ALL_BUTTONS);
     MAP_IntEnable(INT_GPIOF);

     MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
     MAP_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_GPIOA);
     GPIODirModeSet(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_DIR_MODE_IN);
     GPIOPadConfigSet(GPIO_PORTA_BASE,GPIO_PIN_3,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
     GPIOPinRead(GPIO_PORTA_BASE,GPIO_PIN_3);
     MAP_IntPrioritySet(INT_GPIOA,configMAX_SYSCALL_INTERRUPT_PRIORITY);//para aÃ±adir prioridad by HAMED
     MAP_GPIOIntTypeSet(GPIO_PORTA_BASE,GPIO_PIN_3,GPIO_FALLING_EDGE);
     MAP_GPIOIntEnable(GPIO_PORTA_BASE,GPIO_PIN_3);
     MAP_IntEnable(INT_GPIOA);



    miSemaforo = xSemaphoreCreateBinary();
    miSemaforo2 = xSemaphoreCreateBinary();
    miSemaforoACM = xSemaphoreCreateBinary();



	/********************************      Creacion de tareas *********************/

	//Tarea del interprete de comandos (commands.c)
    if (initCommandLine(COMMAND_TASK_STACK,COMMAND_TASK_PRIORITY) != pdTRUE)
    {
        while(1);
    }

	//Esta funcion crea internamente una tarea para las comunicaciones USB.
	//Ademas, inicializa el USB y configura el perfil USB-CDC
	if (remotelink_init(REMOTELINK_TASK_STACK,REMOTELINK_TASK_PRIORITY,messageReceived)!=pdTRUE)
	{
	    while(1); //Inicializo la aplicacion de comunicacion con el PC (Remote). Ver fichero remotelink.c
	}


	//Para especificacion 2: Inicializa el ADC y crea una tarea...
	configADC_IniciaADC();
    if((xTaskCreate(ADCTask, (portCHAR *)"ADC", ADC_TASK_STACK,NULL,ADC_TASK_PRIORITY, NULL) != pdTRUE))
    {
        while(1);
    }
    configADC_Timer();
    if((xTaskCreate(ADCTaskLive, (portCHAR *)"ADCLIVE", ADC_LIVE_STACK,NULL,ADC_LIVE_PRIORITY, NULL) != pdTRUE))
    {
        while(1);
    }

    if((xTaskCreate(Switch1Task,(portCHAR *) "Sw1",SW1TASKSTACKSIZE, NULL,SW1TASKPRIO, NULL) != pdTRUE))
    {
        while(1);
    }
    if((xTaskCreate(Switch2Task,(portCHAR *) "Sw2",SW2TASKSTACKSIZE, NULL,SW2TASKPRIO, NULL) != pdTRUE))
    {
        while(1);
    }

    if((xTaskCreate(ACMINTTask,(portCHAR *) "ACMINT",ACMINTTASKSTACKSIZE, NULL,ACMINTTASKPRIO, NULL) != pdTRUE))
    {
        while(1);
    }
	// Arranca el  scheduler.  Pasamos a ejecutar las tareas que se hayan activado.
	//
    xTimer = xTimerCreate("TimerSW",(1/freqKNOB) * configTICK_RATE_HZ, pdTRUE,NULL,vTimerCallback);
     if( xTimer == NULL )
              {
                  /* The timer was not created. */
                 while(1);
              }
     else{
         /* Start the timer.  No block time is specified, and even if one was
         it would be ignored because the RTOS scheduler has not yet been
         started. */
         if( xTimerStart( xTimer, 0 ) != pdPASS )
         {
             /* The timer could not be set into the Active state. */
             while(1);
         }
     }
	vTaskStartScheduler();	//el RTOS habilita las interrupciones al entrar aqui, asi que no hace falta habilitarlas
	//De la funcion vTaskStartScheduler no se sale nunca... a partir de aqui pasan a ejecutarse las tareas.

	while(1)
	{
		//Si llego aqui es que algo raro ha pasado
	}
}
void ACMGPIOHandler(void){

    int32_t i32PinStatus=MAP_GPIOPinRead(GPIO_PORTA_BASE,GPIO_PIN_3);
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (!(i32PinStatus & LEFT_BUTTON))
    {
//        uint8_t intrup=0x00;
//         ACME_clearInt(intrup);
         xSemaphoreGiveFromISR(miSemaforoACM,&xHigherPriorityTaskWoken);

    }
    else
    {
        uint8_t intrup=0x00;
         ACME_clearInt(intrup);
         xSemaphoreGiveFromISR(miSemaforoACM,&xHigherPriorityTaskWoken);

    }

    MAP_GPIOIntClear(GPIO_PORTA_BASE,GPIO_PIN_3);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

}
// Rutinas de interrupcion
void GPIOFIntHandler(void){
    //Lee el estado del puerto (activos a nivel bajo)

    int32_t i32PinStatus=MAP_GPIOPinRead(GPIO_PORTF_BASE,ALL_BUTTONS);
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (!(i32PinStatus & LEFT_BUTTON))
    {
        xSemaphoreGiveFromISR(miSemaforo,&xHigherPriorityTaskWoken);

    }

    if (!(i32PinStatus & RIGHT_BUTTON))
    {
        xSemaphoreGiveFromISR(miSemaforo2,&xHigherPriorityTaskWoken);

    }


    MAP_GPIOIntClear(GPIO_PORTF_BASE,ALL_BUTTONS);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

