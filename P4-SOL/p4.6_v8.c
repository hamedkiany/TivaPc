// edu@uma.es - 06/04/22 (v8)

// p4.6: Cuando se presione el pulsador Sx conmutara el Zumbador, generando una
// nota de frecuencia 440Hz y ciclo de trabajo 50%
// [Sx: Interrupcion ; TA1: SMCLK 1MHz]

#include "msp430g2553.h"

void main(void) {
	WDTCTL = (WDTPW|WDTHOLD);	// Detiene el Watchdog
	DCOCTL = 0;						// Establece la frecuencia a 1MHz
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
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
	P2DIR |= (BIT0);
	P2SEL |= (BIT0);
	TA1CTL = (TASSEL_2);
	TA1CCR0 = 1136-1;
	TA1CCTL0 = OUTMOD_4;

	__enable_interrupt();

	while (1) {
	} // while (1)

} // void main(void)

#pragma vector = PORT1_VECTOR
__interrupt void RTI_P1(void) {

	if ((P1IE & P1IFG & BIT4) != 0) {
		P1IFG &= ~(BIT4);

		if ((TA1CTL & MC_1) == 0) {
			TA1CTL |= (MC_1|TACLR);
		} // if ((TA1CTL & MC_1) == 0)

		else {
			TA1CTL &= ~(MC_1);
		} // else: if ((TA1CTL & MC_1) == 0)

	} // if ((P1IE & P1IFG & BIT4) != 0)

} // __interrupt void RTI_P1(void)

#pragma vector = PORT2_VECTOR
__interrupt void RTI_P2(void) {

	if ((P2IE & P2IFG & BIT1) != 0) {
		P2IFG &= ~(BIT1);

		if ((TA1CTL & MC_1) == 0) {
			TA1CTL |= (MC_1|TACLR);
		} // if ((TA1CTL & MC_1) == 0)

		else {
			TA1CTL &= ~(MC_1);
		} // else: if ((TA1CTL & MC_1) == 0)

	} // if ((P2IE & P2IFG & BIT1) != 0)

	if ((P2IE & P2IFG & BIT2) != 0) {
		P2IFG &= ~(BIT2);

		if ((TA1CTL & MC_1) == 0) {
			TA1CTL |= (MC_1|TACLR);
		} // if ((TA1CTL & MC_1) == 0)

		else {
			TA1CTL &= ~(MC_1);
		} // else: if ((TA1CTL & MC_1) == 0)

	} // if ((P2IE & P2IFG & BIT2) != 0)

	if ((P2IE & P2IFG & BIT3) != 0) {
		P2IFG &= ~(BIT3);

		if ((TA1CTL & MC_1) == 0) {
			TA1CTL |= (MC_1|TACLR);
		} // if ((TA1CTL & MC_1) == 0)

		else {
			TA1CTL &= ~(MC_1);
		} // else: if ((TA1CTL & MC_1) == 0)

	} // if ((P2IE & P2IFG & BIT3) != 0)

} // __interrupt void RTI_P2(void)
