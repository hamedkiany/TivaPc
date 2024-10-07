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
#include "inc/PWMLib.h"

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

#include "drivers/rgb.h"
#include "drivers/configADC.h"
#include "commands.h"

#include <remotelink.h>
#include <serialprotocol.h>


//parametros de funcionamiento de la tareas
#define REMOTELINK_TASK_STACK (512)
#define REMOTELINK_TASK_PRIORITY (tskIDLE_PRIORITY+2)
#define COMMAND_TASK_STACK (512)
#define COMMAND_TASK_PRIORITY (tskIDLE_PRIORITY+1)
#define ADC_TASK_STACK (256)
#define ADC_TASK_PRIORITY (tskIDLE_PRIORITY+1)
#define SW1TASKPRIO (tskIDLE_PRIORITY+1)            // Prioridad para la tarea SW1TASK
#define SW1TASKSTACKSIZE (256)     // Tamaño de pila para la tarea SW1TASK
#define SW2TASKPRIO (tskIDLE_PRIORITY+1)            // Prioridad para la tarea SW1TASK
#define SW2TASKSTACKSIZE (256)     // Tamaño de pila para la tarea SW1TASK
//Globales
uint32_t g_ui32CPUUsage;
uint32_t g_ulSystemClock;
int VelocidadF2 = 75 , VelocidadF3 = 75;
SemaphoreHandle_t miSemaforo,miSemaforo2;
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


    //
    // Bucle infinito, las tareas en FreeRTOS no pueden "acabar", deben "matarse" con la funcion xTaskDelete().
    //
    while(1)
    {

        configADC_LeeADC(&muestras);    //Espera y lee muestras del ADC (BLOQUEANTE)

        //Copia los datos en el parametro (es un poco redundante)
        parameter.chan1=muestras.chan1;
        parameter.chan2=muestras.chan2;
        parameter.chan3=muestras.chan3;
        parameter.chan4=muestras.chan4;

        //Encia el mensaje hacia QT
        remotelink_sendMessage(MESSAGE_ADC_SAMPLE,(void *)&parameter,sizeof(parameter));
    }
}

//static void Switch1Task(void *pvParameters)
static portTASK_FUNCTION(Switch1Task,pvParameters)
{

    xSemaphoreTake(miSemaforo,portMAX_DELAY);
    //
    // Loop forever.
    //
    while(1)
    {
//        MESSAGE_SW_PARAMETER parametro;
//        parametro.sw.number = 1;
//        //parametro.sw.state = 1;
        if (VelocidadF2 > 74 && VelocidadF2 < 101){
            if(!(VelocidadF2 == 75))
                    VelocidadF2 = VelocidadF2 - 5;
            activatePWM(VelocidadF2,VelocidadF2);
        }
       xSemaphoreTake(miSemaforo,portMAX_DELAY);
//        remotelink_sendMessage(MESSAGE_SW,&parametro,sizeof(parametro));
//       UARTprintf("He puesto botton ye mandado mensaje\n");
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
//        MESSAGE_SW_PARAMETER parametro;
//        parametro.sw.number = 2;
//        //parametro.sw.state = 1;
        if (VelocidadF2 > 74 && VelocidadF2 < 101){
            if(!(VelocidadF2 == 100))
                    VelocidadF2 = VelocidadF2 + 5;
                    activatePWM(VelocidadF2,VelocidadF2);
                }
        xSemaphoreTake(miSemaforo2,portMAX_DELAY);
//        remotelink_sendMessage(MESSAGE_SW,&parametro,sizeof(parametro));
        //UARTprintf("He puesto botton drecha ye mandado mensaje\n");
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
//        case MESSAGE_LED_PWM_BRIGHTNESS:
//        {
//            MESSAGE_LED_PWM_BRIGHTNESS_PARAMETER parametro;
//
//            if (check_and_extract_command_param(parameters, parameterSize, &parametro, sizeof(parametro))>0)
//            {
//                RGBIntensitySet(parametro.rIntensity);
//            }
//            else
//            {
//                status=PROT_ERROR_INCORRECT_PARAM_SIZE; //Devuelve un error
//            }
//        }
//        break;
        case MESSAGE_ADC_SAMPLE:
        {
            configADC_DisparaADC(); //Dispara la conversion (por software)
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
	// Set the clocking to run at 50 MHz from the PLL.
	//
//	MAP_SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
//			SYSCTL_OSC_MAIN);	//Ponermos el reloj principal a 50 MHz (200 Mhz del Pll dividido por 5)
   MAP_SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL |   SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);


	// Get the system clock speed.
	g_ulSystemClock = SysCtlClockGet();


	//Habilita el clock gating de los perifericos durante el bajo consumo --> perifericos que se desee activos en modo Sleep
	//                                                                        deben habilitarse con SysCtlPeripheralSleepEnable
	MAP_SysCtlPeripheralClockGating(true);

	// Inicializa el subsistema de medida del uso de CPU (mide el tiempo que la CPU no esta dormida)
	// Para eso utiliza un timer, que aqui hemos puesto que sea el TIMER3 (ultimo parametro que se pasa a la funcion)
	// (y por tanto este no se deberia utilizar para otra cosa).
	CPUUsageInit(g_ulSystemClock, configTICK_RATE_HZ/10, 3);

	//Inicializa los LEDs usando libreria RGB --> usa Timers 0 y 1
	RGBInit(1);
	MAP_SysCtlPeripheralSleepEnable(GREEN_TIMER_PERIPH);
	MAP_SysCtlPeripheralSleepEnable(BLUE_TIMER_PERIPH);
	MAP_SysCtlPeripheralSleepEnable(RED_TIMER_PERIPH);	//Redundante porque BLUE_TIMER_PERIPH y GREEN_TIMER_PERIPH son el mismo
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	//Volvemos a configurar los LEDs en modo GPIO POR Defecto
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	ButtonsInit();
    MAP_IntPrioritySet(INT_GPIOF,configMAX_SYSCALL_INTERRUPT_PRIORITY);//para añadir prioridad by HAMED
    MAP_GPIOIntEnable(GPIO_PORTF_BASE,ALL_BUTTONS);
    MAP_IntEnable(INT_GPIOF);


    miSemaforo = xSemaphoreCreateBinary();
    miSemaforo2 = xSemaphoreCreateBinary();

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

	   miSemaforo = xSemaphoreCreateBinary();
	    miSemaforo2 = xSemaphoreCreateBinary();
	//Para especificacion 2: Inicializa el ADC y crea una tarea...
	configADC_IniciaADC();
    if((xTaskCreate(ADCTask, (portCHAR *)"ADC", ADC_TASK_STACK,NULL,ADC_TASK_PRIORITY, NULL) != pdTRUE))
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

	//
	// Arranca el  scheduler.  Pasamos a ejecutar las tareas que se hayan activado.
	//
	vTaskStartScheduler();	//el RTOS habilita las interrupciones al entrar aqui, asi que no hace falta habilitarlas
	//De la funcion vTaskStartScheduler no se sale nunca... a partir de aqui pasan a ejecutarse las tareas.

	while(1)
	{
		//Si llego aqui es que algo raro ha pasado
	}
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


