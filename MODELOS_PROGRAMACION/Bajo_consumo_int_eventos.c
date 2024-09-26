// gonzalez@uma.es - 10/04/22

// Cuando se presione S3 se encienden todos los LED cuando se presione
// S4, S5 o S6 se apagaran todos los LED
// [Sx: Sondeo]

//BAJO CONSUMO CON INTERRUPCIONES Y EVENTOS
#include "msp430g2553.h"

#define EV_S3 BIT0
#define EV_S4 BIT1
#define EV_S5 BIT2
#define EV_S6 BIT3
unsigned char eventos;

void main(void) {
	WDTCTL = WDTPW|WDTHOLD;
	P1DIR &= ~(BIT4);
	P1REN |= (BIT4);
	P1OUT |= (BIT4);
	P1IES |= (BIT4);
	P1IFG &= ~(BIT4);
	P1IE |= (BIT4);
	P2DIR &= ~(BIT3|BIT2|BIT1);
	P2REN |= (BIT3|BIT2|BIT1);
	P2OUT |= (BIT3|BIT2|BIT1);
	P2IES |= (BIT3|BIT2|BIT1);
	P2IFG &= ~(BIT3|BIT2|BIT1);
	P2IE |= (BIT3|BIT2|BIT1);
	P2DIR |= (BIT7|BIT6|BIT5|BIT4);
	P2OUT &= ~(BIT7|BIT6|BIT5|BIT4);
	P2SEL &= ~(BIT7|BIT6);

	while(1)
	{
	    __low_power_mode_0();

	    while (eventos)
	    {
	        if(eventos & EV_S3)
	        {
	            P2OUT |= (BIT4|BIT5|BIT6|BIT7);
	            eventos &= ~EV_S3;
	        }//if(eventos & EV_S3)

	        if(eventos & EV_S4)
	        {
	            P2OUT &= ~(BIT4|BIT5|BIT6|BIT7);
	            eventos &= ~EV_S4;
	        }//if(eventos & EV_S4)

	        if(eventos & EV_S5)
	        {
	            P2OUT &= ~(BIT4|BIT5|BIT6|BIT7);
	            eventos &= ~EV_S5;
	        }//if(eventos & EV_S5)

	        if(eventos & EV_S6)
	        {
	            P2OUT &= ~(BIT4|BIT5|BIT6|BIT7);
	            eventos &= ~EV_S6;
	        }//if(eventos & EV_S6)

	    }//while (eventos)
	}//while(1)
} // void main(void)

#pragma vector = PORT1_VECTOR
__interrupt void RTI_P1(void) {

	if ((P1IE & P1IFG & BIT4) != 0) {
		P1IFG &= ~(BIT4);
		eventos |= EV_S3;
	} // if ((P1IE & P1IFG & BIT4) != 0)
    if (eventos) {
        __low_power_mode_off_on_exit();
    } // if (eventos)

} // __interrupt void RTI_P1(void)

#pragma vector = PORT2_VECTOR
__interrupt void RTI_P2(void) {

	if ((P2IE & P2IFG & BIT1) != 0) {
		P2IFG &= ~(BIT1);
		eventos |= EV_S4;
	} // if ((P2IE & P2IFG & BIT1) != 0)

	if ((P2IE & P2IFG & BIT2) != 0) {
		P2IFG &= ~(BIT2);
		eventos |= EV_S5;
	} // if ((P2IE & P2IFG & BIT2) != 0)

	if ((P2IE & P2IFG & BIT3) != 0) {
		P2IFG &= ~(BIT3);
		eventos |= EV_S6;
	} // if ((P2IE & P2IFG & BIT3) != 0)

    if (eventos) {
        __low_power_mode_off_on_exit();
    } // if (eventos)

} // __interrupt void RTI_P2(void)
