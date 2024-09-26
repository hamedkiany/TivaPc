// edu@uma.es - 12/02/22 (v2)

// p2.1: Durante el tiempo que permanezca presionado Sx se encenderá el LEDx
// [Sx: Sondeo]

#include "msp430g2553.h"

void main(void) {
	WDTCTL = WDTPW|WDTHOLD;
	P1DIR &= ~(BIT4);
	P1REN |= (BIT4);
	P1OUT |= (BIT4);
	P2DIR &= ~(BIT3|BIT2|BIT1);
	P2REN |= (BIT3|BIT2|BIT1);
	P2OUT |= (BIT3|BIT2|BIT1);
	P2DIR |= (BIT7|BIT6|BIT5|BIT4);
	P2OUT &= ~(BIT7|BIT6|BIT5|BIT4);
	P2SEL &= ~(BIT7|BIT6);

	while (1) {

		if ((P1IN & BIT4) == 0) {
			P2OUT |= (BIT4);
		} // if ((P1IN & BIT4) == 0)

		else {
			P2OUT &= ~(BIT4);
		} // else: if ((P1IN & BIT4) == 0)

		if ((P2IN & BIT1) == 0) {
			P2OUT |= (BIT5);
		} // if ((P2IN & BIT1) == 0)

		else {
			P2OUT &= ~(BIT5);
		} // else: if ((P2IN & BIT1) == 0)

		if ((P2IN & BIT2) == 0) {
			P2OUT |= (BIT6);
		} // if ((P2IN & BIT2) == 0)

		else {
			P2OUT &= ~(BIT6);
		} // else: if ((P2IN & BIT2) == 0)

		if ((P2IN & BIT3) == 0) {
			P2OUT |= (BIT7);
		} // if ((P2IN & BIT3) == 0)

		else {
			P2OUT &= ~(BIT7);
		} // else: if ((P2IN & BIT3) == 0)

	} // while (1)

} // void main(void)
