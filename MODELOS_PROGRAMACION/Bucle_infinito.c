// gonzalez@uma.es - 10/04/22

// Cuando se presione S3 se encienden todos los LED cuando se presione
// S4, S5 o S6 se apagaran todos los LED
// [Sx: Sondeo]

//BUCLE INFINITO

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

	s3_ant = s4_ant = s5_ant = s6_ant = 1;

	while (1) {

		if ((P1IN & BIT4) == 0) {

			if (s3_ant == 1) {
				P2OUT |= (BIT4|BIT5|BIT6|BIT7);
			} // if (s3_ant == 1)

			s3_ant = 0;
		} // if ((P1IN & BIT4) == 0)

		else {
			s3_ant = 1;
		} // else: if ((P1IN & BIT4) == 0)

		if ((P2IN & BIT1) == 0) {

			if (s4_ant == 1) {
				P2OUT &= ~(BIT4|BIT5|BIT6|BIT7);
			} // if (s4_ant == 1)

			s4_ant = 0;
		} // if ((P2IN & BIT1) == 0)

		else {
			s4_ant = 1;
		} // else: if ((P2IN & BIT1) == 0)

		if ((P2IN & BIT2) == 0) {

			if (s5_ant == 1) {
			    P2OUT &= ~(BIT4|BIT5|BIT6|BIT7);
			} // if (s5_ant == 1)

			s5_ant = 0;
		} // if ((P2IN & BIT2) == 0)

		else {
			s5_ant = 1;
		} // else: if ((P2IN & BIT2) == 0)

		if ((P2IN & BIT3) == 0) {

			if (s6_ant == 1) {
			    P2OUT &= ~(BIT4|BIT5|BIT6|BIT7);
			} // if (s6_ant == 1)

			s6_ant = 0;
		} // if ((P2IN & BIT3) == 0)

		else {
			s6_ant = 1;
		} // else: if ((P2IN & BIT3) == 0)

	} // while (1)

} // void main(void)