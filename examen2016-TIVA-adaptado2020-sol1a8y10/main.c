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
#include "drivers/rgb.h"
#include "drivers/configADC.h"
#include "utils/cpu_usage.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "commands.h"
#include <remotelink.h>
#include <serialprotocol.h>
#include "config_juego.h"


//parametros de funcionamiento de la tareas
#define REMOTELINK_TASK_STACK (512)
#define REMOTELINK_TASK_PRIORITY (tskIDLE_PRIORITY+2)
#define COMMAND_TASK_STACK (512)
#define COMMAND_TASK_PRIORITY (tskIDLE_PRIORITY+1)
#define ADC_TASK_STACK (256)
#define ADC_TASK_PRIORITY (tskIDLE_PRIORITY+1)

//Tipos y variables...

typedef enum { MOUSE_ROJO,MOUSE_AZUL, MAX_MICE } MiceColor;

/* Tabla con los colores*/
static const uint32_t TablaColores[4][3]={  {0xFFF0,0,0}, /* Rojo */
                                        {0,0,0xFFF0}, /* Azul */
                                        {0x8000,0x8000,0}, /* Amarillo */
                                        {0,0xFFF0,0} /* Verde */
                            };
//Globales
uint32_t g_ui32CPUUsage;
uint32_t g_ulSystemClock;
static int remoto=0;
uint32_t g_ulColors[3] = { 0x0000, 0x0000, 0x0000 };

typedef struct {
    uint8_t HuntedMouse;
} CatData;


//Manejadores de objetos de FreeRTOS.
QueueHandle_t ColaMice;
SemaphoreHandle_t xSemaphore=NULL;

TaskHandle_t ManejaTareaBF=NULL;
TaskHandle_t ManejaTareaMiceCats=NULL;

static EventGroupHandle_t FlagsEventos;
#define LEFT_BUTTON_EV 0x0001
#define RIGH_BUTTON_EV 0x0002
#define REMOTE_LEFT_EV 0x0004
#define REMOTE_RIGHT_EV 0x0008

//Declaracion de funciones para manejar el juego
void gamesInit(void);
void gamesStartBF(void);
void gamesStopBF(void);
void gameStartMC(void);
void gameStopMC(void);
void gameEnviarJugadaMC(uint8_t jugada);
void gameSetRemoteMC (int remote);



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
void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName)
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



//Funcion callback que procesa los mensajes recibidos desde el PC (ejecuta las acciones correspondientes a las ordenes recibidas)
// Se ejecuta desde dentro de una tarea
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
        case MESSAGE_EMPEZAR_BF:
                {
                    gamesStartBF(); // XXX P10. ver la implementacion más abajo (al final del fichero)
                }
                break;
                case MESSAGE_PARAR_BF:
                {
                    gamesStopBF();// XXX P10. ver la implementacion  más abajo (al final del fichero)
                }
                break;
                case MESSAGE_EMPEZAR_MICECATS:
                {
                    gameStartMC(); // XXX P5. Ver la implementación  más abajo (al final del fichero)
                }
                break;
                case MESSAGE_PARAR_MICECATS:
                {
                    gameStopMC(); // XXX P5. Ver la implementación  más abajo (al final del fichero)
                }
                break;
                case MESSAGE_BOTON_MICECATS:
                {
                    MESSAGE_BOTON_MICECATS_PARAMETER parametro;
                    if (check_and_extract_command_param(parameters, parameterSize, &parametro , sizeof(parametro))>0)
                    {
                        // XXX P6...
                        gameEnviarJugadaMC(parametro.HuntedMouse); //Esta funcion esta implementada al final del fichero y envia a la cola de mensaje
                    }

                }
                break;
                case MESSAGE_REMOTE:
                {
                    //XXX P8
                     MESSAGE_REMOTE_PARAMETER parametro;
                     if (check_and_extract_command_param(parameters, parameterSize, &parametro , sizeof(parametro))>0)
                     {
                         gameSetRemoteMC(parametro.remote); // XXX P8. ver implementacion más abajo (al final del fichero)
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
// A continuacion van las tareas...
//
//*****************************************************************************

//Examen:

//Esta funcion genera
//un valor aleatorio tipo float entre 0 y 1
//utilizando la funciï¿½n rand(...) de C
float scaledRand(void)
{

    return (float)rand()/(float)RAND_MAX;
}

//Examen:
static portTASK_FUNCTION(ButtonFight,pvParameters)
{
    uint32_t pulsaciones=0;
    int numdatos;
    MESSAGE_PULSACIONES_BF_PARAMETER parametro;
    //unsigned char frame[MAX_FRAME_SIZE];

    TickType_t LastMessageSendTime = xTaskGetTickCount();

    while(1)
    {
        do
        {
            xSemaphoreTake(xSemaphore,portMAX_DELAY); //Espera a que se pulse el boton
            pulsaciones++; //Incrementa las pulsaciones
        } while (((xTaskGetTickCount()-LastMessageSendTime)<BF_MAX_WAIT_TIME)&&(pulsaciones%BF_MAX_WAIT_PUSHES));
        /* Solo manda un mensaje cuando se completa un numero de pulsaciones o ha pasado un tiempo desde el anterior mensaje */

        LastMessageSendTime = xTaskGetTickCount();

        //Envia las pulsaciones hacia el PC
        parametro.pulsaciones=pulsaciones;

        numdatos=remotelink_sendMessage(MESSAGE_PULSACIONES_BF,&parametro,sizeof(parametro));

    }
}

static portTASK_FUNCTION(MiceCatsTask,pvParameters)
{
    MESSAGE_MOUSE_PARAMETER parametroMouse; //Parametros para el comando Mouse
    MESSAGE_PUNTOS_MICECATS_PARAMETER parametroPuntosMC; //Parametros para el comando Puntos


    MiceColor raton; //Color generado
    CatData pulsacion_gato;  //Pulsaciones

    int puntuacion=0;
    int numdatos;

    //XXX P5. Al arrancar la tarea mando la puntuacion hacia el PC
    parametroPuntosMC.puntuacion=puntuacion;
    numdatos=remotelink_sendMessage(MESSAGE_PUNTOS_MICECATS,&parametroPuntosMC,sizeof(parametroPuntosMC));

    //Cambia la semilla del generador pseudoaleatorio...
    srand(xTaskGetTickCount()+1);

    vTaskDelay(3*configTICK_RATE_HZ); //Espera inical...

    while(puntuacion>MC_MIN_POINTS)
    {
        raton=rand()%MAX_MICE; //genera aleatoriamente un raton (rojo o azul)
        RGBSet(TablaColores[raton],1.0);

        //Envia el color del "raton" al Qt
        parametroMouse.Mouse=raton;
        numdatos=remotelink_sendMessage(MESSAGE_MOUSE,&parametroMouse,sizeof(parametroMouse));


        //decide si la secuencia es corta o larga (aleatorio)
        if ((MC_PROB_LARGA)>scaledRand())
        {
            // SECUENCIA LARGA
            if (xQueueReceive(ColaMice,&pulsacion_gato,MC_DURACION_LARGA)==pdFALSE)
            {
                puntuacion -= 20;
            }
            else
            {   //El Usuario ha pulsado, analizamos lo que ha hecho
                if (raton==pulsacion_gato.HuntedMouse)
                {
                    puntuacion += 10;
                }
                else
                {
                    puntuacion -= 40;
                }
            }
        }
        else
        {
            //CORTA
            if (xQueueReceive(ColaMice,&pulsacion_gato,MC_DURACION_CORTA)==pdFALSE)
            {
            }
            else
            {
                //El Usuario ha pulsado, analizamos lo que ha hecho
                if (raton==pulsacion_gato.HuntedMouse)
                {
                    puntuacion += 20;
                }
                else
                {
                    puntuacion -= 80;
                }
            }
        }

        RGBIntensitySet(0.0);


        parametroMouse.Mouse=MAX_MICE;
        numdatos=remotelink_sendMessage(MESSAGE_MOUSE,&parametroMouse,sizeof(parametroMouse));


        parametroPuntosMC.puntuacion=puntuacion;
        numdatos=remotelink_sendMessage(MESSAGE_PUNTOS_MICECATS,&parametroPuntosMC,sizeof(parametroPuntosMC));

        //Por hora esperamos un tiempo aleatorio
        vTaskDelay(MC_PAUSA); //Pausa aleatoria
        //TODO P4: Modifica el comportamiento para que si se pulsa el bot�n tarde (despues de que "el raton se haya ido") se
        // descuenten 30 puntos.
        //XXX Implementacion P4: se descuentan 30 puntos si al final hay algo en la cola... (antes de resetearla)
        if (uxQueueMessagesWaiting(ColaMice)>0)
        {
            puntuacion -= 30;
            parametroPuntosMC.puntuacion=puntuacion;
            numdatos=remotelink_sendMessage(MESSAGE_PUNTOS_MICECATS,&parametroPuntosMC,sizeof(parametroPuntosMC));
        }

        xQueueReset(ColaMice); //TODO P3 �Por que es necesaria esta linea, que ocurriria si no estuviera?

    }

    // XXX: P1
    ManejaTareaMiceCats=NULL; //Pongo el manejador a NULL para indicar que la tarea no esta activa (para que cuadre con las preguntas de mas adelante)
    vTaskDelete(NULL); // La tarea se suicida...
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


    //Inicializa los LEDs usando libreria RGB --> usa Timers 0 y 1 (eliminar si no se usa finalmente)
    RGBInit(1);
    MAP_SysCtlPeripheralSleepEnable(GREEN_TIMER_PERIPH);
    MAP_SysCtlPeripheralSleepEnable(BLUE_TIMER_PERIPH);
    MAP_SysCtlPeripheralSleepEnable(RED_TIMER_PERIPH);  //Redundante porque BLUE_TIMER_PERIPH y GREEN_TIMER_PERIPH son el mismo

    RGBSet(g_ulColors,0.0);


    ButtonsInit();  //Inicializa los botones de la placa.
    GPIOIntTypeSet(GPIO_PORTF_BASE, ALL_BUTTONS,GPIO_FALLING_EDGE);
    IntPrioritySet(INT_GPIOF,configMAX_SYSCALL_INTERRUPT_PRIORITY);
    GPIOIntEnable(GPIO_PORTF_BASE,ALL_BUTTONS);
    //IntEnable(INT_GPIOF); //XXX P10, la interrupcion ya no se habilita aquí


    // XXX P5 y otras Esto se ha movido a gamesInit();
    //    ColaMice=xQueueCreate(1,sizeof(CatData));
    //    xSemaphore=xSemaphoreCreateBinary();

    gamesInit();    //Inicializa los IPC y variables necesarios para los juegos. Ver implementación más abajo

    //XXX P5 las tareas no se crean aqui sino cuando lleguen los comandos correspondientes
//#if (JUEGO_BUTTONFIGHT)
//    if((xTaskCreate(ButtonFight, (signed portCHAR *)"Prod", 512,NULL,tskIDLE_PRIORITY + 1, &ManejaTareaBF) != pdTRUE))
//        {
//            while(1)
//            {
//            }
//        }
//
//#elif JUEGO_MICECATS
//
//    if((xTaskCreate(MiceCatsTask, (signed portCHAR *)"Prod", 512,NULL,tskIDLE_PRIORITY + 1, &ManejaTareaMiceCats) != pdTRUE))
//            {
//                while(1)
//                {
//
//                }
//       }
//#endif




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


// ****************************** Manejadores de interrupcion *******************/
//Manejador de interrupcion alternativo para el ButtonFight
void GPIOFIntHandlerBF(void)
{
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    static uint32_t Right_last_time=0;
    static uint32_t Left_last_time=0;
    uint32_t tiempo;
    static uint8_t last_button=0;

    uint32_t lIntStatus=GPIOIntStatus(GPIO_PORTF_BASE,ALL_BUTTONS);
    uint32_t lPinStatus=GPIOPinRead(GPIO_PORTF_BASE,ALL_BUTTONS);
    GPIOIntClear(GPIO_PORTF_BASE,ALL_BUTTONS);

    if (lIntStatus&RIGHT_BUTTON)
    {
        tiempo=xTaskGetTickCountFromISR()-Right_last_time;
        Right_last_time=xTaskGetTickCountFromISR();

        if (tiempo>DEBOUNCE_TIME)
        {
            if ((last_button!=RIGHT_BUTTON)&&(lPinStatus&LEFT_BUTTON))
            {   //El otro boton debe estar como no pulsado y ser el ultimo pulsado
                //Da paso a la tarea
                xSemaphoreGiveFromISR(xSemaphore,&xHigherPriorityTaskWoken);
            }
            last_button=RIGHT_BUTTON;
        }
    }

    if (lIntStatus&LEFT_BUTTON)
    {
        tiempo=xTaskGetTickCountFromISR()-Left_last_time;
        Left_last_time=xTaskGetTickCountFromISR();

        if (tiempo>DEBOUNCE_TIME){
            if ((last_button!=LEFT_BUTTON)&&(lPinStatus&RIGHT_BUTTON))
            {   //El otro boton debe estar como no pulsado y ser el �ltimo pulsado
                //Da paso a la tarea
                xSemaphoreGiveFromISR(xSemaphore,&xHigherPriorityTaskWoken);
            }
            last_button=LEFT_BUTTON;
        }
    }

    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}



void GPIOFIntHandlerMC(void)
//XXX: Cambio de nombre P10.

{
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
    static uint32_t Right_last_time=0;
    static uint32_t Left_last_time=0;
    uint32_t tiempo;
    CatData pulsacion_gato;


    uint32_t lIntStatus=GPIOIntStatus(GPIO_PORTF_BASE,true);
    uint32_t lPinStatus=GPIOPinRead(GPIO_PORTF_BASE,ALL_BUTTONS);
    GPIOIntClear(GPIO_PORTF_BASE,ALL_BUTTONS);

    if (lIntStatus&RIGHT_BUTTON)
    {
        if (lPinStatus&RIGHT_BUTTON)
        {
            //Boton derecho Soltado
            GPIOIntTypeSet(GPIO_PORTF_BASE, RIGHT_BUTTON,GPIO_FALLING_EDGE);
            tiempo=xTaskGetTickCountFromISR()-Right_last_time;
            if (tiempo>DEBOUNCE_TIME)
            {

                pulsacion_gato.HuntedMouse=MOUSE_AZUL;
                xQueueSendFromISR(ColaMice,&pulsacion_gato,&xHigherPriorityTaskWoken);
            }
        }
        else
        {   //Boton derecho apretado
            GPIOIntTypeSet(GPIO_PORTF_BASE, RIGHT_BUTTON,GPIO_RISING_EDGE);
            Right_last_time=xTaskGetTickCountFromISR();
        }
    }

    if (lIntStatus&LEFT_BUTTON)
    {
        if (lPinStatus&LEFT_BUTTON)
        {
            //Boton izquierdo Soltado
            GPIOIntTypeSet(GPIO_PORTF_BASE, LEFT_BUTTON,GPIO_FALLING_EDGE);
            tiempo=xTaskGetTickCountFromISR()-Left_last_time;
            if (tiempo>DEBOUNCE_TIME)
            {
                pulsacion_gato.HuntedMouse=MOUSE_ROJO;
                xQueueSendFromISR(ColaMice,&pulsacion_gato,&xHigherPriorityTaskWoken);
            }
        }
        else
        {   //Boton izquierdo apretado.
            GPIOIntTypeSet(GPIO_PORTF_BASE, LEFT_BUTTON,GPIO_RISING_EDGE);
            Left_last_time=xTaskGetTickCountFromISR();
        }
    }

    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

// ******************************************************************
// Funciones que gestionan el juego
// ******************************************************************

//Configuracion inicial (main())
void gamesInit(void)
{

    remoto=0; // XXX P10

    RGBSet(g_ulColors,0.0);
    ColaMice=xQueueCreate(1,sizeof(CatData));
    xSemaphore=xSemaphoreCreateBinary();
}


//Arranca Button Fight
void gamesStartBF(void)
{
    if (ManejaTareaBF==NULL)
    {
        IntRegister(INT_GPIOF,GPIOFIntHandlerBF); //XXX Importante P10...
        GPIOIntClear(GPIO_PORTF_BASE,LEFT_BUTTON|RIGHT_BUTTON);
        IntEnable(INT_GPIOF);
        if((xTaskCreate(ButtonFight, (signed portCHAR *)"Prod", 512,NULL,tskIDLE_PRIORITY + 1, &ManejaTareaBF) != pdTRUE))
        {
            UARTprintf("No he podido crear la tarea\n");
        }
    }
}

//Para Button Fight
void gamesStopBF(void)
{
    if (ManejaTareaBF!=NULL)
    {   //Deshabilito las interrupciones y destruyo la tarea
        IntDisable(INT_GPIOF);
        vTaskDelete(ManejaTareaBF);
        ManejaTareaBF=NULL;
    }
}

// XXX P5
//Arranca Mice&Cats
void gameStartMC(void)
{
    if (ManejaTareaMiceCats==NULL)
    {
        IntRegister(INT_GPIOF,GPIOFIntHandlerMC); //XXX Importante P10...
        //Habilito las interrupciones y creo la tarea. Reseteo la cola por si acaso...
        xQueueReset(ColaMice);
        GPIOIntClear(GPIO_PORTF_BASE,LEFT_BUTTON|RIGHT_BUTTON);
        if (!remoto) /* P8 */
            IntEnable(INT_GPIOF);

        if((xTaskCreate(MiceCatsTask, (signed portCHAR *)"Prod", 512,NULL,tskIDLE_PRIORITY + 1, &ManejaTareaMiceCats) != pdTRUE))
        {
            UARTprintf("No he podido crear la tarea\n");
        }

    }
}

//XXX P5 //Para Mice&Cats
void gameStopMC(void)
{
    if (ManejaTareaMiceCats!=NULL)
    {   //Deshabilito las interrupciones y destruyo la tarea
        IntDisable(INT_GPIOF);
        vTaskDelete(ManejaTareaMiceCats);
        ManejaTareaMiceCats=NULL;
    }
}

// XXX P8. Cambia de modo remoto a local o viceversa
void gameSetRemoteMC (int remote)
{
    if (remote)
    {
        IntDisable(INT_GPIOF);
        remoto=true;
    }
    else
    {
        GPIOIntClear(GPIO_PORTF_BASE,LEFT_BUTTON|RIGHT_BUTTON);
        IntEnable(INT_GPIOF);
        remoto=false;
    }
}

// XXX P6
void gameEnviarJugadaMC(uint8_t jugada)
{
    if (remoto) //xxx P8
    {
        //Envia la jugada a la cola (P6)
        xQueueSend(ColaMice,&jugada,portMAX_DELAY); //Alternativamente podria poner algo distinto a portMAX_DELAY...
    }
}

