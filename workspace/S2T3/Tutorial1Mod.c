	// Enciende de los LEDs rojo (PF1), azul (PF2) y verde (PF3) de forma consecutiva y continua,
// en intervalos de 1 segundo
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"     // Necesario para funciones API en driverlib
#include "inc/hw_memmap.h"    // Etiquetas de registros del micro, perifericos, ...
#include "driverlib/sysctl.h" // Definicion de funciones y etiquetas de control del sistema (SysCtl)
#include "driverlib/gpio.h"   // Definicion de funciones y etiquetas de puerto  (GPIOPin...)

int main(void)
{
	uint8_t ui8LED = 2; // En binario es 00000..010, es decir, BIT1 a 1
	// Configura reloj del sistema a 50MHz (PLL a 200MHz/4=50MHz)
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	//SysCtlClockSet(SYSCTL_SYSDIV_2|SYSCTL_USE_OSC|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	// Habilita Puerto F (LEDs)
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // Comentar para producir error FaulISR
	// Configura pines PF1, PF2, y PF3 como salidas (control de LEDs)
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	while(1)
	{
		// Enciende el LED indicado en la variable; 1er parametro: puerto afectado
		// 2� parametro: bits afectados (mascara); 3er parametro: valor a volcar al puerto
		// solo se modificaran los bits indicados por la mascara)
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8LED);
		// Retardo para ver encendido el LED; parametro= "loops" (3 ciclos por "loop")
		SysCtlDelay(16666666); //  1 seg (aprox)
		// Enciende el siguiente LED (el del bit que esta a continuacion)
		if (ui8LED == 8) {ui8LED = 2;} else {ui8LED = ui8LED*2;}
	}
}
