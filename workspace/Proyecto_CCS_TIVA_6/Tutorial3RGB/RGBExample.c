#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "drivers/rgb.h"

int main(void)
{
	uint32_t ui32Color[3];
	// Establece como reloj del sistema el PLL (200Mhz, alimentado por el XTAL de 16MHz), dividido por 4--> 50MHz
	// Indica las 3 componentes del color a sacar: prueba a cambiarlas para diferentes colores
	ui32Color[RED] = 0x7FFF;   // Minimo: 0; Maxima 0xFFFF
	ui32Color[BLUE] = 0x7FFF; // Minimo: 0; Maxima 0xFFFF
	ui32Color[GREEN] = 0x0;	   // Minimo: 0; Maxima 0xFFFF
	SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    RGBInit(0);             // Inicializa el sistema de control de LEDs
    RGBColorSet(ui32Color); // Saca el color indicado en el array por los LEDs
    RGBIntensitySet(0.5);   // Reduce la intensidad al 50% del maximo
    // RGBSet(ui32Color,0.5); // Esta instruccion equivale a las dos anteriores
    RGBEnable();              // Habilita la generacion PWM para el encendido de los LEDs
    //RGBBlinkRateSet(1.0f);  // Parpadeo del LED a 1Hz. Hay que definir la posicion del WTIMER5B de la tabla de
                              // vectores con la RTI de API "RGBBlinkIntHandler"
    while(1){
    }
}
