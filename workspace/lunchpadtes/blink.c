#include <msp430.h>
#include <stdio.h>


/**
 * blink.c
 */
int main(void) {

    WDTCTL = WDTPW | WDTHOLD;// Stop watchdog timer

    P1DIR = 0X01;    //Declare PIN0 OF PORT 1 AS OUTPUT

    P1OUT = 0X01; //MAKE PIN0 HIGH INITIALLY

    unsigned int i;   //Delay variable

    while(1)

    {

        P1OUT ^=0X01;   ////toggle the bits

        for(i=0;i<20000;i++){   // delay till you make LED LOW HIGH

  }

 }

    return 0;

}
