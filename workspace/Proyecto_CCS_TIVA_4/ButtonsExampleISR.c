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

int main(void)
{
	uint8_t ui8Buttons, ui8Changed;
	uint8_t ui8LEDS=0;
	// Establece como reloj del sistema el PLL (200Mhz, alimentado por el XTAL de 16MHz), dividido por 4--> 50MHz
	SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
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
	IntMasterEnable();

	while(1)
	{
	   // Testea el estado de los botones mediante la funcion de API de la placa ButtonsPoll; esta funcion se debe llamar
	   // de forma periodica para que funcione correctamente (ver EK-TM4C123GX Firmware Development Packages Users Guide)
	   // ucButtons contiene el estado de los botones; ucChanged, los que han cambiado desde la ultima llamada
	   // a esta funcion
	   GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,0);

	   // Bucle de espera; el parametro es el numero de "loops" de espera; cada "loop" tarda 3 ciclos de reloj
       SysCtlDelay(6600000); // Poll cada 10ms (50MHz*10ms/3=166000)

       GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,GPIO_PIN_3);
       SysCtlDelay(6600000); // Poll cada 10ms (50MHz*10ms/3=166000)
	}
}

void RutinaISR(void)
{
	uint32_t status;

	status=GPIOIntStatus(GPIO_PORTF_BASE,true);

	//La eficiencia de este codigo es mejorable...
	if (status&GPIO_PIN_4) GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_2)^GPIO_PIN_2);
	if (status&GPIO_PIN_0) GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_1)^GPIO_PIN_1);

	GPIOIntClear(GPIO_PORTF_BASE,GPIO_PIN_4|GPIO_PIN_0);

}

