// Las salidas PWM conectadas a los LED son las siguientes:
//(TIMER0B)T0CCP1  --> PF1 (LED rojo)
//(TIMER1A)T1CCP0  --> PF2 (LED azul)
//(TIMER1B)T1CCP1  --> PF3 (LED verde)
// Esto se puede ver en el Data-Sheet del micro (Tiva TM4C123HG6PM Microcontroller Data Sheet, pag.706-707)

// Hemos hecho una variante que cambio el ciclo de trabajo de un solo LED a lo largo del tiempo, lo cual supone un aumento
// del brillo hasta su valor maximo y una disminucion posterior.

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h" // Include para poder configurar el pin como salida PWM

#define PWMCYCLE    60000         // Periodo de la onda PWM que hemos definido (el valor maximo posible es 65535)
#define DUTYCYCLE99 PWMCYCLE*0.99 // Diferentes ciclos de trabajo de ejemplo; cuanto mas
#define DUTYCYCLE75 PWMCYCLE*0.75 // cercano a PWMCYCLE, MENOS tiempo se ilumina el LED
#define DUTYCYCLE25 PWMCYCLE*0.25

int main(void) {
    uint32_t ui32Period, ui32DutyCycle;

    // Reloj del sistema a 40MHz (PLL-200MHz/5=40MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_64|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    SysCtlPWMClockSet(SYSCTL_PWMDIV_64);

    // Definicion de valore para la onda PWM
    ui32Period = PWMCYCLE ; // Periodo de la señal PWM; 1000/40MHz= 25uS
    ui32DutyCycle = DUTYCYCLE75;// Ciclo de trabajo inicial de la señal PWM

    // Configuracion de puertos (LEDs)
    // Habilita puerto GPIOF (LEDs)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    // Configura pines conectados a LEDs como salidas GPIO
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    // Apaga los LEDs
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);

    // Configura pin PF1 en su funcion especial como salida PWM del Timer0B (T0CCP1)
    // Las definiciones de funciones especiales de los pines, como GPIO_PF1_T0CCP1,  para cada modelo de micro las puedes encontrar en "/driverlib/pin_map.h"
    GPIOPinConfigure(GPIO_PF2_T1CCP0);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2); // Observa que aqui se usa otro "formato" para indicar el pin afectado. Consulta la documentaci�n de la funci�n
    // Configuracion de TIMERs

    // Habilita el Timer0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    // "Separa" el Timer0 en los Timer0A y 0B; esto se hace porque el modo PWM solo funciona con los Timers por separado;
    // ademas, configura el TIMERB, como PWM
    TimerConfigure(TIMER1_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_PWM);
    // Carga el periodo de cuenta del Timer (el valor afecta a la tasa de intermitencia del LED)
    TimerLoadSet(TIMER1_BASE, TIMER_A, ui32Period);
    // Carga el punto de trabajo, que determina la longitud del ciclo de trabajo de la onda PWM
    TimerMatchSet(TIMER1_BASE, TIMER_A, ui32DutyCycle);
    // Habilita el TIMER (indispensable para que funcione!)
    TimerEnable(TIMER1_BASE, TIMER_A);
    // Codigo principal, en bucle infinito
    while(1) {
    }
}
