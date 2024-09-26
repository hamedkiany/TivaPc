// Parpadeo del LED azul controlado por una interrupcion de Timeren modo half-width .

// Nota: solo se comentan las novedades respecto a programas anteriores, o aspectos
// mas significativos del codigo

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h" // Definicion de nombres de interrupciones (ej. INT_TIMER0A))
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h" // Definicion de funciones de interrupcion (Int...())
#include "driverlib/gpio.h"
#include "driverlib/timer.h" // Definicion de funciones de Timer (Timer..())

// Programa principal
int main(void)
{
  uint16_t ui16Period;
  // Configura reloj del sistema a 40MHz (PLL a 200MHz/5=40MHz)
  SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

  // Configuracion de puerto GPIOF (LEDs)
  // Habilita Puerto F
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  // Configura pines PF1, PF2, y PF3 como salidas (control de LEDs)
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
  // Y apaga los LEDs
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);

  // Configuracion TIMER0
  // Habilita periferico Timer0
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
  // Configura el Timer0 para cuenta periodica de 16 bits con la mitad B del TIMER0
  TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PERIODIC);
  // Periodo de cuenta de m�ximo posible, para medio TIMER (16bits)
  ui16Period = 0xFFFF;
  // Carga la cuenta en el Timer0B
  TimerLoadSet(TIMER0_BASE, TIMER_B, ui16Period);
  // Habilita interrupcion del modulo TIMER
  IntEnable(INT_TIMER0B);
  // Y habilita, dentro del modulo TIMER0, la interrupcion de particular de "fin de cuenta"
  TimerIntEnable(TIMER0_BASE, TIMER_TIMB_TIMEOUT);
  // Habilita permiso general de interrupciones el sistema.
  IntMasterEnable();
  // Activa el Timer0B (empezara a funcionar)
  TimerEnable(TIMER0_BASE, TIMER_B);

  while(1)
  {
      SysCtlSleep();
  }
}

void Timer0IntHandler(void)
{
	static uint16_t counter=0;
	counter++;
	// Borra la interrupcion de Timer
	TimerIntClear(TIMER0_BASE, TIMER_TIMB_TIMEOUT);
	if(counter == 100){
		counter=0;
		// Lee el estado actual del LED y escribe el estado opuesto
		if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
		}
		else
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
		}
	}
}

