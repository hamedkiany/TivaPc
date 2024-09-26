#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

int main(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_3);
	while(1)
	{
		// Turn on the LED
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
		// Delay for a bit
		SysCtlDelay(600000);
		// Turn off the LED
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
		// Delay for a bit
		SysCtlDelay(600000);
	}
}
