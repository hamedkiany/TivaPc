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

void RutinaISR(void);
void RutinaISR_2(void);


int main(void)
{

	// Establece como reloj del sistema el PLL (200Mhz, alimentado por el XTAL de 16MHz), dividido por 5--> 40MHz
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	// Habilita periferico GPIO; los nombres de los perifericos se encuentran en "/driverlib/sysctl.h"
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	// Establece los pines 1,2,y 3 del PuertoF como salidas; las definiciones de los registros (GPIO_PORTF_BASE)
	// se encuentran en "/inc/hw_memmap.h", y la de los pines en "/driverlib/gpio.h"
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	// Funcion API de la placa para inicializacion de botones (definida en /drivers/buttons).
	// Documentacion de estas funciones en EK-TM4C123GXL Firmware Development Packages Users Guide, pag 11 y 13
	ButtonsInit();

	GPIOIntClear(GPIO_PORTF_BASE,GPIO_PIN_0);
	//Se programa el flanco de bajada, no haría falta porque es el flanco por defecto
	GPIOIntTypeSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_FALLING_EDGE);

	GPIOIntEnable(GPIO_PORTF_BASE,GPIO_PIN_0);
	//IntRegister(INT_GPIOF, RutinaISR); En su lugar incluir el manejador en el fichero tm4c123gh6pm_startup_ccs.c

	IntEnable(INT_GPIOF);
	IntMasterEnable();

	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1,0);

	while(1)
	{
	   SysCtlSleep();

	}
}
void RutinaISR(void)
{
	uint32_t status;

	volatile static uint8_t cuentas=0;

	status=GPIOIntStatus(GPIO_PORTF_BASE,true);
    //antirrebotes muy malo: no se puede meter una espera en una RTI
    SysCtlDelay(266666); //  20ms (40MHz*20ms/3=266666)

	//La eficiencia de este codigo es mejorable...
	//Cuando se pulsa el botón derecho se enciende el azul
	if (status&GPIO_PIN_0)
	{
	    if (GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0) //Flanco de bajada y no un rebote
	    {
	        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_2)^GPIO_PIN_2);
	        cuentas++;
	        if(cuentas == 4)
	        {
	            IntRegister(INT_GPIOF, RutinaISR_2);
	            cuentas=0;
	        }
	    }//if (GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0)

	}//if (status&GPIO_PIN_0)

	GPIOIntClear(GPIO_PORTF_BASE,GPIO_PIN_0);

}//void RutinaISR(void)

void RutinaISR_2(void)
{
    uint32_t status;

    volatile static uint8_t cuentas_2=0;

    status=GPIOIntStatus(GPIO_PORTF_BASE,true);
    //antirrebotes muy malo: no se puede meter una espera en una RTI
    SysCtlDelay(266666); //  20ms (40MHz*20ms/3=266666)
    //La eficiencia de este codigo es mejorable...
    //Cuando se pulsa el botón derecho se enciende el rojo
    if (status&GPIO_PIN_0)
    {
        if (GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0) //Flanco de bajada y no un rebote
        {
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_1)^GPIO_PIN_1);
            cuentas_2++;
            if(cuentas_2 == 4)
            {
                IntRegister(INT_GPIOF, RutinaISR);
                cuentas_2=0;
            }
        }// if (GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0)

    }//if (status&GPIO_PIN_0)

    GPIOIntClear(GPIO_PORTF_BASE,GPIO_PIN_0);
}//void RutinaISR_2(void)


