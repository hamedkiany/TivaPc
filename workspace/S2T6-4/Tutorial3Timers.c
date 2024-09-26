// Parpadeo del LED azul controlado por una interrupcion de Timer.

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
  uint32_t ui32Period;
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
  // Configura el Timer0 para cuenta periodica de 32 bits (no lo separa en TIMER0A y TIMER0B)
  TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
  // Periodo de cuenta de 0.05s. SysCtlClockGet() te proporciona la frecuencia del reloj del sistema, por lo que una cuenta
  // del Timer a SysCtlClockGet() tardara 1 segundo, a 0.5*SysCtlClockGet(), 0.5seg, etc...
  ui32Period = (SysCtlClockGet() / 10) / 2;
  // Carga la cuenta en el Timer0A
  TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);
  // Habilita interrupcion del modulo TIMER
  IntEnable(INT_TIMER0A);
  // Y habilita, dentro del modulo TIMER0, la interrupcion de particular de "fin de cuenta"
  TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
  // Habilita permiso general de interrupciones el sistema.
  IntMasterEnable();
  // Activa el Timer0A (empezara a funcionar)
  TimerEnable(TIMER0_BASE, TIMER_A);

  while(1)
  {

  }
}

void Timer0IntHandler(void)
{
	// Borra la interrupcion de Timer
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

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

