// gonzalez@uma.es - 09/02/22 (v1)

// P3.4: Cuando se suelte el pulsador conmutará el LEDx. Cuando se pulse de forma simultánea
//       S3 y S4 se encenderán todos los LED
// [Sx: Interrupción]

#include "msp430g2553.h"

int     pulsa_doble=0;
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


	__enable_interrupt();

	while (1) {
	} // while (1)

} // void main(void)

#pragma vector = PORT1_VECTOR
__interrupt void RTI_P1(void) {

	if ((P1IE & P1IFG & BIT4) != 0) {
		P1IFG &= ~(BIT4);

		if ((P1IES & BIT4) == 0) { //flanco subida S3
		    if((P2IN & BIT1)==0){
		        pulsa_doble=1;
		        P2OUT |= (BIT7|BIT6|BIT5|BIT4);
		    }//if((P2IN & BIT1)==0)
		    else{
		        if (pulsa_doble==1) pulsa_doble=0;
		        else P2OUT  ^= (BIT4);
		    }//else:if((P2IN & BIT1)==0)
		} // if ((P1IES & BIT4) == 0)

		P1IES ^= (BIT4);
	} // if ((P1IE & P1IFG & BIT4) != 0)

} // __interrupt void RTI_P1(void)

#pragma vector = PORT2_VECTOR
__interrupt void RTI_P2(void) {

	if ((P2IE & P2IFG & BIT1) != 0) {
		P2IFG &= ~(BIT1);

		if ((P2IES & BIT1) == 0) { //flanco subida S4
            if((P1IN & BIT4)==0){
                pulsa_doble=1;
                P2OUT |= (BIT7|BIT6|BIT5|BIT4);
            }//if((P1IN & BIT4)==0)
            else{
                if (pulsa_doble==1) pulsa_doble=0;
                else P2OUT  ^= (BIT5);
            }//else:if((P1IN & BIT4)==0)
		} // if ((P2IES & BIT1) != 0)

		P2IES ^= (BIT1);
	} // if ((P2IE & P2IFG & BIT1) != 0)

	if ((P2IE & P2IFG & BIT2) != 0) {
		P2IFG &= ~(BIT2);

		if ((P2IES & BIT2) == 0) {
			P2OUT  ^= (BIT6);
		} // if ((P2IES & BIT2) != 0)

		P2IES ^= (BIT2);
	} // if ((P2IE & P2IFG & BIT2) != 0)

	if ((P2IE & P2IFG & BIT3) != 0) {
		P2IFG &= ~(BIT3);

		if ((P2IES & BIT3) == 0) {
			P2OUT  ^= (BIT7);
		} // if ((P2IES & BIT3) != 0)


		P2IES ^= (BIT3);
	} // if ((P2IE & P2IFG & BIT3) != 0)

} // __interrupt void RTI_P2(void)
