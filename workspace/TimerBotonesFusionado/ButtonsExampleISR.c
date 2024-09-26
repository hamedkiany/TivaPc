// Se necesita incluir el simbolo predefinido "TARGET_IS_TM4C123_RB1" en las propiedades del sistema
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "drivers/buttons.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"

int main(void)
{
	uint8_t ui8Buttons, ui8Changed;
	uint8_t ui8LEDS=0;
	uint32_t ui32Period;

	// Establece como reloj del sistema el PLL (200Mhz, alimentado por el XTAL de 16MHz), dividido por 4--> 50MHz
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	// Habilita periferico GPIO; los nombres de los perifericos se encuentran en "/driverlib/sysctl.h"
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	// Establece los pines 1,2,y 3 del PuertoF como salidas; las definiciones de los registros (GPIO_PORTF_BASE)
	// se encuentran en "/inc/hw_memmap.h", y la de los pines en "/driverlib/gpio.h"
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	// Funcion API de la placa para inicializacion de botones (definida en /drivers/buttons).
	// Documentacion de estas funciones en EK-TM4C123GXL Firmware Development Packages Users Guide, pag 11 y 13
	ButtonsInit();

	GPIOIntClear(GPIO_PORTF_BASE,GPIO_PIN_4|GPIO_PIN_0);
	GPIOIntEnable(GPIO_PORTF_BASE,GPIO_PIN_4|GPIO_PIN_0);
    IntEnable(INT_GPIOF);

	// Configuracion TIMER0
	  // Habilita periferico Timer0
	  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	  // Configura el Timer0 para cuenta periodica de 32 bits (no lo separa en TIMER0A y TIMER0B)
	  TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	  // Periodo de cuenta de 0.05s. SysCtlClockGet() te proporciona la frecuencia del reloj del sistema, por lo que una cuenta
	  // del Timer a SysCtlClockGet() tardara 1 segundo, a 0.5*SysCtlClockGet(), 0.5seg, etc...
	  ui32Period = SysCtlClockGet()/10; //0.1s
	  // Carga la cuenta en el Timer0A
	  TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);
	  	  // Y habilita, dentro del modulo TIMER0, la interrupcion de particular de "fin de cuenta"
	  TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	  // Habilita interrupcion del modulo TIMER
	  IntEnable(INT_TIMER0A);

	  // Activa el Timer0A (empezara a funcionar)
	  TimerEnable(TIMER0_BASE, TIMER_A);

    //Habilita las Interrupciones en el micro
	IntMasterEnable();

	while(1)
	{
       SysCtlDelay(6600000); // Poll cada 10ms (50MHz*10ms/3=166000)
	}
}

void RutinaISR(void)
{
	uint32_t status;
    static uint32_t valor_periodo=1;
	uint32_t ui32Period_local;

	status=GPIOIntStatus(GPIO_PORTF_BASE,true);

	//La eficiencia de este codigo es mejorable...
	if (status&GPIO_PIN_4)
	   {
	        valor_periodo++;
	        if(valor_periodo > 20) valor_periodo =20;
	        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_2)^GPIO_PIN_2);
	   }
	//
	if (status&GPIO_PIN_0)
	    {
	        valor_periodo--;
	        if(valor_periodo < 1) valor_periodo =1;
	        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_1)^GPIO_PIN_1);
	    }
	    //

    ui32Period_local = (SysCtlClockGet()/10)*valor_periodo;
    // Carga la cuenta en el Timer0A
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period_local -1);
	GPIOIntClear(GPIO_PORTF_BASE,GPIO_PIN_4|GPIO_PIN_0);

}


void Timer0IntHandler(void)
{

    // Borra la interrupcion de Timer
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // Lee el estado actual del LED y escribe el estado opuesto
    if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3))
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
    }
    else
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
    }
}
