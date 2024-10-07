//*****************************************************************************
//
// commands.c - FreeRTOS porting example on CCS4
//
// Este fichero contiene errores que seran explicados en clase
//
//*****************************************************************************


#include <stdbool.h>
#include <stdint.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>


/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* Standard TIVA includes */
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"

#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"

#include "driverlib/interrupt.h"
#include "driverlib/i2c.h"

/* Other TIVA includes */
#include "utils/cpu_usage.h"
#include "utils/cmdline.h"
#include "utils/uartstdio.h"

#include "drivers/rgb.h"

#include "drivers/i2cm_drv.h"
#include "drivers/ACME_3416.h"
#include "drivers/BMI160.h"


#include "emul/ACMESim_i2c_slave.h"


#define ACME_I2C_ADDRESS 0x3C
#define BMI160_I2C_ADDR 0x69

static tI2CMInstance g_sI2CInst;

// ==============================================================================
// The CPU usage in percent, in 16.16 fixed point format.
// ==============================================================================
extern uint32_t g_ui32CPUUsage;

// ==============================================================================
// Implementa el comando cpu que muestra el uso de CPU
// ==============================================================================
static int  Cmd_cpu(int argc, char *argv[])
{
    //
    // Print some header text.
    //
    UARTprintf("ARM Cortex-M4F %u MHz - ",SysCtlClockGet() / 1000000);
    UARTprintf("%2u%% de uso\r\n", (g_ui32CPUUsage+32768) >> 16);

    // Return success.
    return(0);
}

// ==============================================================================
// Implementa el comando free, que muestra cuanta memoria "heap" le queda al FreeRTOS
// ==============================================================================
static int Cmd_free(int argc, char *argv[])
{
    //
    // Print some header text.
    //
    UARTprintf("%d bytes libres\r\n", xPortGetFreeHeapSize());

    // Return success.
    return(0);
}

// ==============================================================================
// Implementa el comando task. Sï¿½lo es posible si la opciï¿½n configUSE_TRACE_FACILITY de FreeRTOS estï¿½ habilitada
// ==============================================================================
#if ( configUSE_TRACE_FACILITY == 1 )

extern char *__stack;
static  int Cmd_tasks(int argc, char *argv[])
{
	char*	pcBuffer;
	uint8_t*	pi8Stack;
	portBASE_TYPE	x;
	
	pcBuffer = pvPortMalloc(1024);
	vTaskList(pcBuffer);
	UARTprintf("\t\t\t\tUnused\r\nTaskName\tStatus\tPri\tStack\tTask ID\r\n");
	UARTprintf("=======================================================\r\n");
	UARTprintf("%s", pcBuffer);
	
	// Calculate kernel stack usage
	x = 0;
	pi8Stack = (uint8_t *) &__stack;
	while (*pi8Stack++ == 0xA5)
	{
		x++;	//Esto sï¿½lo funciona si hemos rellenado la pila del sistema con 0xA5 en el arranque
	}
	sprintf((char *) pcBuffer, "%%%us", configMAX_TASK_NAME_LEN);
	sprintf((char *) &pcBuffer[10], (const char *) pcBuffer, "kernel");
	UARTprintf("%s\t-\t*%u\t%u\t-\r\n", &pcBuffer[10], configKERNEL_INTERRUPT_PRIORITY, x/sizeof(portBASE_TYPE));
	vPortFree(pcBuffer);
	return 0;
}

#endif /* configUSE_TRACE_FACILITY */

#if configGENERATE_RUN_TIME_STATS
// ==============================================================================
// Implementa el comando "Stats"
// ==============================================================================
static Cmd_stats(int argc, char *argv[])
{
	char*	pBuffer;

	pBuffer = pvPortMalloc(1024);
	if (pBuffer)
	{
		vTaskGetRunTimeStats(pBuffer); //Es un poco inseguro, pero por ahora nos vale...
		UARTprintf("TaskName\tCycles\t\tPercent\r\n");
		UARTprintf("===============================================\r\r\n");
		UARTprintf("%s", pBuffer);
		vPortFree(pBuffer);
	}
	return 0;
}
#endif

// ==============================================================================
// Implementa el comando help
// ==============================================================================
static int Cmd_help(int argc, char *argv[])
{
    tCmdLineEntry *pEntry;

    //
    // Print some header text.
    //
    UARTprintf("Comandos disponibles\r\n");
    UARTprintf("------------------\r\n");

    //
    // Point at the beginning of the command table.
    //
    pEntry = &g_psCmdTable[0];

    //
    // Enter a loop to read each entry from the command table.  The end of the
    // table has been reached when the command name is NULL.
    //
    while(pEntry->pcCmd)
    {
        //
        // Print the command name and the brief description.
        //
        UARTprintf("%s%s\r\n", pEntry->pcCmd, pEntry->pcHelp);

        //
        // Advance to the next entry in the table.
        //
        pEntry++;
    }

    //
    // Return success.
    //
    return(0);
}

// ==============================================================================
// Implementa el comando "LED"
// ==============================================================================
static int Cmd_led(int argc, char *argv[])
{
    uint32_t cualLED;

	if (argc != 3)
	{
		//Si los parametros no son suficientes o son demasiados, muestro la ayuda
		UARTprintf(" led [red|green|blue] [on|off]\r\n");
	}
	else
	{
	    if (0==strncmp( argv[1], "red",3))
	    {

	        cualLED=GPIO_PIN_1;
	    }
	    else if (0==strncmp( argv[1], "green",5))
	    {
	        cualLED=GPIO_PIN_3;
	    }
	    else if (0==strncmp(argv[1], "blue",4))
	    {
	        cualLED=GPIO_PIN_2;
	    }
	    else
	    {
	        UARTprintf(" led [red|green|blue] [on|off]\r\n");
	        return 0;
	    }

		if (0==strncmp( argv[2], "on",2))
		{
			UARTprintf("Enciendo el LED\r\n");
			GPIOPinWrite(GPIO_PORTF_BASE, cualLED,cualLED);
		}
		else if (0==strncmp( argv[2], "off",3))
		{
			UARTprintf("Apago el LED\r\n");
			GPIOPinWrite(GPIO_PORTF_BASE,cualLED,0);
		}
		else
		{
			//Si el parametro no es correcto, muestro la ayuda
			UARTprintf(" led [red|green|blue] [on|off]\r\n");
		}

	}


    return 0;
}


// ==============================================================================
// Implementa el comando "MODE"
// ==============================================================================
static int Cmd_mode(int argc, char *argv[])
{
	if (argc != 2)
	{
		//Si los parametros no son suficientes o son demasiados, muestro la ayuda
		UARTprintf(" mode [gpio|pwm]\r\n");
	}
	else
	{
		if (0==strncmp( argv[1], "gpio",4))
		{
			UARTprintf("cambio a modo GPIO\r\n");
			RGBDisable();
			GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
		}
		else if (0==strncmp( argv[1], "pwm",3))
		{
			UARTprintf("Cambio a modo PWM (rgb)\r\n");
			RGBEnable();
		}
		else
		{
			//Si el parametro no es correcto, muestro la ayuda
			UARTprintf(" mode [gpio|pwm]\r\n");
		}
	}


    return 0;
}


static int Cmd_intensity(int argc, char *argv[])
{
    float intensity;

    if (argc != 2)
    {
        //Si los parametros no son suficientes o son demasiados, muestro la ayuda
        UARTprintf(" intensity [floatnum]\r\n");
    }
    else
    {
        intensity=strtof(argv[1],NULL);
        if ((intensity>=0.0)&&(intensity<=1.0))
        {
            RGBIntensitySet(intensity);

        }
        else
        {
            UARTprintf("El valor de intensidad deberia valer entre 0 y 1\r\n");
        }

    }


    return 0;
}


// ==============================================================================
// Implementa el comando "RGB"
// ==============================================================================
static int Cmd_rgb(int argc, char *argv[])
{
	uint32_t arrayRGB[3];

	if (argc != 4)
	{
		//Si los parï¿½metros no son suficientes, muestro la ayuda
		UARTprintf(" rgb [red] [green] [blue]\r\n");
	}
	else
	{
		arrayRGB[0]=strtoul(argv[1], NULL, 10)<<8;
		arrayRGB[1]=strtoul(argv[2], NULL, 10)<<8;
		arrayRGB[2]=strtoul(argv[3], NULL, 10)<<8;

		if ((arrayRGB[0]>=65535)||(arrayRGB[1]>=65535)||(arrayRGB[2]>=65535))
		{

			UARTprintf(" \r\n");
		}
		else{
			RGBColorSet(arrayRGB);
		}

	}

    return 0;
}

// ==============================================================================
// Implementa el comando "readACC"
// ==============================================================================
static int Cmd_readAcc(int argc, char *argv[])
{


    if (argc != 1)
    {
        //Si los parï¿½metros no son suficientes, muestro la ayuda
        UARTprintf(" readacc\r\n");
    }
    else
    {
        int16_t x,y,z;

        BMI160_getAcceleration(&x,&y,&z);

        UARTprintf("Acc: %d %d %d \r\n",(uint32_t)x,(uint32_t)y,(uint32_t)z);
    }

    return 0;
}

static int Cmd_ACME(int argc, char *argv[])
{


    if (argc < 2)
    {
        //Si los parametros no son suficientes, muestro la ayuda
        UARTprintf(" acme <op> [value]\r\n");
        UARTprintf(" <op> can be dir, input, output, clear, itype, istat, connect, disconnect,read \r\n");
    }
    else
    {

        if (0==strncmp( argv[1], "dir",3))
        {
            if (argc<3)
            {
                UARTprintf("acme dir <val>\r\n");
            }
            else
            {
                uint8_t val=strtoul(argv[2],NULL,16);

                if (ACME_setPinDir(val)<0)
                {
                    UARTprintf("Error en el proceso...\r\n");
                }
                else
                {
                    UARTprintf("OK\r\n");
                }
            }

        }
        else if (0==strncmp( argv[1], "input",5))
        {
            uint8_t val;
            if (ACME_readPin(&val))
            {
                UARTprintf("Error en el proceso...\r\n");
            }
            else
            {
                UARTprintf("OK.");
                UARTprintf(" %x \r\n",(uint32_t)val);
            }
        }
        else if (0==strncmp( argv[1], "output",6))
        {
            if (argc<3)
            {
                UARTprintf("acme input <val>\r\n");
            }
            else
            {
                uint8_t val=strtoul(argv[2],NULL,16);
                if (ACME_writePin(val)<0)
                {
                    UARTprintf("Error en el proceso...\r\n");
                }
                else
                {
                    UARTprintf("OK\r\n");
                }
            }

        }
        else if (0==strncmp( argv[1], "clear",5))
        {
            if (argc<3)
            {
                UARTprintf("acme clear <val>\r\n");
            }
            else
            {
                uint8_t val=strtoul(argv[2],NULL,16);
                if (ACME_clearInt (val)<0)
                {
                    UARTprintf("Error en el proceso...\r\n");
                }
                else
                {
                    UARTprintf("OK\r\n");
                }
            }

        }
        else if (0==strncmp( argv[1], "itype",5))
        {
            if (argc<3)
            {
                UARTprintf("acme itype <val>\r\n");
            }
            else
            {
                uint16_t val=strtoul(argv[2],NULL,16);
                if (ACME_setIntType ((val&0xFF),(val>>8)&0xFF)<0)
                {
                    UARTprintf("Error en el proceso...\r\n");
                }
                else
                {
                    UARTprintf("OK\r\n");
                }
            }
        }
        else if (0==strncmp( argv[1], "istat",5))
        {
            uint8_t val;
            if (ACME_readInt(&val))
            {
                UARTprintf("Error en el proceso...\r\n");
            }
            else
            {
                UARTprintf("OK.");
                UARTprintf(" %x \r\n",(uint32_t)val);
            }
        }
        else if (0==strncmp( argv[1], "read",4))
        {
             uint8_t buf[8];
             if (ACME_read(buf,sizeof(buf)))
             {
                 UARTprintf("Error en el proceso...\r\n");
             }
             else
             {
                int i;

                for (i=0;i<8;i++)
                {
                    UARTprintf(" %x ",(uint32_t)buf[i]);
                }
                UARTprintf("OK.\r\n");
             }
        }
        else if (0==strncmp( argv[1], "disconnect",10))
        {
            ACMEsim_SlaveDisable();
        }
        else if (0==strncmp( argv[1], "connect",7))
        {
            ACMEsim_SlaveEnable();
        }
        else
        {
            UARTprintf(" <op> can be dir, input, output, clear, itype, istat, connect, disconnect,read \r\n");
        }
    }
    return 0;
}


// ==============================================================================
// Tabla con los comandos y su descripcion. Si quiero anadir alguno, debo hacerlo aqui
// ==============================================================================
//Este array tiene que ser global porque es utilizado por la biblioteca cmdline.c para implementar el interprete de comandos
//No es muy elegante, pero es lo que ha implementado Texas Instruments.
tCmdLineEntry g_psCmdTable[] =
{
    { "help",     Cmd_help,      "     : Lista de comandos" },
    { "?",        Cmd_help,      "        : lo mismo que help" },
    { "cpu",      Cmd_cpu,       "      : Muestra el uso de  CPU " },
    { "led",  	  Cmd_led,       "      : Apaga y enciende el led verde" },
    { "mode",  	  Cmd_mode,       "      : Cambia los pines PF1, PF2 y PF3 entre modo GPIO y modo PWM (rgb)" },
    { "rgb",  	  Cmd_rgb,       "      : Establece el color RGB" },
    { "readacc",      Cmd_readAcc,       "      : Lee valores de aceleracion" },
    { "acme",      Cmd_ACME,       "      : Lee valores de aceleracion" },
    { "intensity",      Cmd_intensity,       "      : Cambia el nivel de intensidad" },
    { "free",     Cmd_free,      "     : Muestra la memoria libre" },
#if ( configUSE_TRACE_FACILITY == 1 )
	{ "tasks",    Cmd_tasks,     "    : Muestra informacion de las tareas" },
#endif
#if (configGENERATE_RUN_TIME_STATS)
	{ "stats",    Cmd_stats,      "     : Muestra estadisticas de las tareas" },
#endif
    { 0, 0, 0 }
};


// ==============================================================================
// Tarea UARTTask.  Espera la llegada de comandos por el puerto serie y los ejecuta al recibirlos...
// ==============================================================================
static void vCommandTask( void *pvParameters )
{
    char    pcCmdBuf[64];
    int32_t i32Status;
	
    //
    // Mensaje de bienvenida inicial.
    //

    vTaskDelay(500);


    if (!ACME_initDevice(&g_sI2CInst,ACME_I2C_ADDRESS))
        {

        UARTprintf("\r\n Sensor ACME Inicializado \r\n");
        }
    else UARTprintf("\r\n Error en la inicizalizacion del sensor ACME \r\n");

    //  ANULADO HASTA COMPLETAR BIBLIOTECA I2C_DRIVER y conectar el sensor
//        BMI160_initialize(&g_sI2CInst,BMI160_I2C_ADDR);
//        if (BMI160_testConnection())
//        {
//                UARTprintf("\r\n Sensor BCI160 Inicializado \r\n");
//                }
//            else UARTprintf("\r\n Error en la inicizalizacion del sensor BCI160 \r\n");


    UARTprintf("\r\n\r\nWelcome to the TIVA FreeRTOS Demo!\r\n");
	UARTprintf("\r\n\r\n FreeRTOS %s \r\n",
		tskKERNEL_VERSION_NUMBER);
	UARTprintf("\r\n Teclee ? para ver la ayuda \r\n");
	UARTprintf("> ");    

	/* Loop forever */
	while (1)
	{

		/* Read data from the UART and process the command line */
		UARTgets(pcCmdBuf, sizeof(pcCmdBuf));
		if (strlen(pcCmdBuf) == 0)
		{
			UARTprintf("> ");
			continue;
		}

		//
		// Pass the line from the user to the command processor.  It will be
		// parsed and valid commands executed.
		//
		i32Status = CmdLineProcess(pcCmdBuf);

		//
		// Handle the case of bad command.
		//
		if(i32Status == CMDLINE_BAD_CMD)
		{
			UARTprintf("Comando erroneo!\r\n");	//No pongo acentos adrede
		}

		//
		// Handle the case of too many arguments.
		//
		else if(i32Status == CMDLINE_TOO_MANY_ARGS)
		{
			UARTprintf("El interprete de comandos no admite tantos parametros\r\n");	//El maximo, CMDLINE_MAX_ARGS, esta definido en cmdline.c
		}

		//
		// Otherwise the command was executed.  Print the error code if one was
		// returned.
		//
		else if(i32Status != 0)
		{
			UARTprintf("El comando devolvio el error %d\r\n",i32Status);
		}

		UARTprintf("> ");

	}
}

#define ACME_EMULATION_MODE 1

#if ACME_EMULATION_MODE
//Habilitar para emulacion dispositivo ACME...
#include "emul/ACMESim_i2c_slave.h"
#endif



int32_t I2CDriver_Open()
{

    // Inicializacion del interfaz con los sensores
    //
    // The I2C1 peripheral must be enabled before use.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_6|GPIO_PIN_7,
                            GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD_WPU);
    //
    // Configure the pin muxing for I2C1 functions on port D0 and D1.
    // This step is not necessary if your part does not support pin muxing.
    //
    GPIOPinConfigure(GPIO_PA6_I2C1SCL);
    GPIOPinConfigure(GPIO_PA7_I2C1SDA);

    //
    // Select the I2C function for these pins.  This function will also
    // configure the GPIO pins pins for I2C operation, setting them to
    // open-drain operation with weak pull-ups.  Consult the data sheet
    // to see which functions are allocated per pin.
    //
    GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);

    //necesario por tener activado  el clock gating
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_I2C1);

    //Llama a la función de inicialización de la biblioteca I2CM_drv.c
    I2CMInit(&g_sI2CInst, I2C1_BASE, INT_I2C1, 0xff, 0xff,
                 MAP_SysCtlClockGet());

    //Deshabilitar cuando se conecte el sensor BOSCH u otro sensor externo.
    I2CLoopbackEnable(I2C1_BASE);


    IntPrioritySet(INT_I2C1,configMAX_SYSCALL_INTERRUPT_PRIORITY); //jose: La prioridad debe ser mayor o igual que configMAX_SYSCALL_INTERRUPT_PRIORITY
    IntEnable(INT_I2C1);



#if ACME_EMULATION_MODE
    ACMEsim_InitSlave(); //habilita la configuración del dispositivo I2C ACME emulado
#endif

    return 0;
}

//Interrupcion del I2C. Llama a la funcion I2CMIntHandler de
void I2CDriver_ISR(void)
{
    //BaseType_t xHigherPriorityTaskWoken=pdFALSE;

#if ACME_EMULATION_MODE
    //Esto es necesario para la emulacion del dispositivo ACME.... NO QUITAR!!!
    if (I2CSlaveIntStatus(I2C1_BASE, true) )
    {
        ACMEsim_I2C1SlaveIntHandler();
    }
#endif

    if ((I2CMasterIntStatus(I2C1_BASE, true))||(g_sI2CInst.ui8State==0) )
    {
        I2CMIntHandler(&g_sI2CInst);
    }
}


//
// Crea la tarea que gestiona los comandos (definida en el fichero commands.c)
//
BaseType_t initCommandLine(uint16_t stack_size,uint8_t prioriry )
{

    // Inicializa la UARTy la configura a 115.200 bps, 8-N-1 .
    //se usa para mandar y recibir mensajes y comandos por el puerto serie
    // Mediante un programa terminal como gtkterm, putty, cutecom, etc...
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //Esta funcion habilita la interrupcion de la UART y le da la prioridad adecuada si esta activado el soporte para FreeRTOS
    UARTStdioConfig(0,115200,SysCtlClockGet());
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_UART0);   //La UART tiene que seguir funcionando aunque el micro esta dormido

    I2CDriver_Open();


    return xTaskCreate(vCommandTask, (signed portCHAR *)"UartComm", stack_size,NULL,prioriry, NULL);
}
