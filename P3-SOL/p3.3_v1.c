// edu@uma.es - 09/02/22 (v1)

// P3.3: Durante el tiempo que permanezca presionado Sx se encender� el LEDx
// [Sx: Interrupci�n]

#include "msp430g2553.h"

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

		if ((P1IES & BIT4) != 0) {
			P2OUT |= (BIT4);
		} // if ((P1IES & BIT4) != 0)

		else {
			P2OUT &= ~(BIT4);
		} // else: if ((P1IES & BIT4) != 0)

		P1IES ^= (BIT4);
	} // if ((P1IE & P1IFG & BIT4) != 0)

} // __interrupt void RTI_P1(void)

#pragma vector = PORT2_VECTOR
__interrupt void RTI_P2(void) {

	if ((P2IE & P2IFG & BIT1) != 0) {
		P2IFG &= ~(BIT1);

		if ((P2IES & BIT1) != 0) {
			P2OUT |= (BIT5);
		} // if ((P2IES & BIT1) != 0)

		else {
			P2OUT &= ~(BIT5);
		} // else: if ((P2IES & BIT1) != 0)

		P2IES ^= (BIT1);
	} // if ((P2IE & P2IFG & BIT1) != 0)

	if ((P2IE & P2IFG & BIT2) != 0) {
		P2IFG &= ~(BIT2);

		if ((P2IES & BIT2) != 0) {
			P2OUT |= (BIT6);
		} // if ((P2IES & BIT2) != 0)

		else {
			P2OUT &= ~(BIT6);
		} // else: if ((P2IES & BIT2) != 0)

		P2IES ^= (BIT2);
	} // if ((P2IE & P2IFG & BIT2) != 0)

	if ((P2IE & P2IFG & BIT3) != 0) {
		P2IFG &= ~(BIT3);

		if ((P2IES & BIT3) != 0) {
			P2OUT |= (BIT7);
		} // if ((P2IES & BIT3) != 0)

		else {
			P2OUT &= ~(BIT7);
		} // else: if ((P2IES & BIT3) != 0)

		P2IES ^= (BIT3);
	} // if ((P2IE & P2IFG & BIT3) != 0)

} // __interrupt void RTI_P2(void)