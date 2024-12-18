// gonzalez@uma.es - 20/02/23 (v1)

// p2.extra1: Cuando se suelte Sx conmutar� el LEDx
// [Sx: Sondeo]

#include "msp430g2553.h"

void main(void) {
	unsigned char s3_ant, s4_ant, s5_ant, s6_ant;

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

	s3_ant = s4_ant = s5_ant = s6_ant = 0;

	while (1) {

		if ((P1IN & BIT4) == BIT4) {

			if (s3_ant == 1) {
				P2OUT ^= (BIT4);
				s3_ant = 0;
			} // if (s3_ant == 1)

		} //if ((P1IN & BIT4) == BIT4)

		else {
			s3_ant = 1;
		} // else: if ((P1IN & BIT4) == 0)

		if ((P2IN & BIT1) == BIT1) {

			if (s4_ant == 1) {
				P2OUT ^= (BIT5);
				s4_ant = 0;
			} // if (s4_ant == 1)

		} // if ((P2IN & BIT1) == BIT1)

		else {
			s4_ant = 1;
		} // else: if ((P2IN & BIT1) == BIT1)

		if ((P2IN & BIT2) == BIT2) {

			if (s5_ant == 1) {
				P2OUT ^= (BIT6);
				s5_ant = 0;
			} // if (s5_ant == 1)

		} // if ((P2IN & BIT2) == BIT2)

		else {
			s5_ant = 1;
		} // else: if ((P2IN & BIT2) == BIT2)

		if ((P2IN & BIT3) == BIT3) {

			if (s6_ant == 1) {
				P2OUT ^= (BIT7);
				s6_ant = 0;
			} // if (s6_ant == 1)

		} // if ((P2IN & BIT3) == BIT3)

		else {
			s6_ant = 1;
		} // else: if ((P2IN & BIT3) == BIT3)

	} // while (1)

} // void main(void)
