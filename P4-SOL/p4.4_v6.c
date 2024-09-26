// edu@uma.es - 19/04/22 (v6)
// gonzalez@uma.es


// p4.4: Cuando se presione el pulsador Sx se encendera el LEDx durante 1.25s
// [Sx: Interrupcion ; TA0: Interrupcion, SMCLK 1MHz/8, TA0CCR1 CONT]

#include "msp430g2553.h"

void main(void) {
	WDTCTL = WDTPW|WDTHOLD;		// Detiene el Watchdog
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
	TA0CTL = TASSEL_2|ID_3;

	__enable_interrupt();

	while (1) {
	} // while (1)

} // void main(void)

#pragma vector = PORT1_VECTOR
__interrupt void RTI_P1(void) {

	if ((P1IE & P1IFG & BIT4) != 0) {
		P1IFG &= ~(BIT4);
		P2OUT |= (BIT4);
		TA0CTL |= (MC_2);
		TA0CCR1 = TA0R+31250;
		TA0CCTL1 &= ~(CCIFG);
		TA0CCTL1 |= (CCIE);
	} // if ((P1IE & P1IFG & BIT4) != 0)

} // __interrupt void RTI_P1(void)

#pragma vector = PORT2_VECTOR
__interrupt void RTI_P2(void) {

	if ((P2IE & P2IFG & BIT1) != 0) {
		P2IFG &= ~(BIT1);
		P2OUT |= (BIT5);
		TA0CTL |= (MC_2);
		TA0CCR1 = TA0R+31250;
		TA0CCTL1 &= ~(CCIFG);
		TA0CCTL1 |= (CCIE);
	} // if ((P2IE & P2IFG & BIT1) != 0)

	if ((P2IE & P2IFG & BIT2) != 0) {
		P2IFG &= ~(BIT2);
		P2OUT |= (BIT6);
		TA0CTL |= (MC_2);
		TA0CCR1 = TA0R+31250;
		TA0CCTL1 &= ~(CCIFG);
		TA0CCTL1 |= (CCIE);
	} // if ((P2IE & P2IFG & BIT2) != 0)

	if ((P2IE & P2IFG & BIT3) != 0) {
		P2IFG &= ~(BIT3);
		P2OUT |= (BIT7);
		TA0CTL |= (MC_2);
		TA0CCR1 = TA0R+31250;
		TA0CCTL1 &= ~(CCIFG);
		TA0CCTL1 |= (CCIE);
	} // if ((P2IE & P2IFG & BIT3) != 0)

} // __interrupt void RTI_P2(void)

#pragma vector = TIMER0_A1_VECTOR
__interrupt void RTI_TA0CCR12(void) {
	static unsigned char cont = 0;

	switch (TA0IV) {

		case (TA0IV_TACCR1): {
			cont++;

			if (cont == 5) {
				P2OUT &= ~(BIT7|BIT6|BIT5|BIT4);
				TA0CTL &= ~(MC0 | MC1);
				TA0CCTL1 &= ~(CCIE);
				cont = 0;
			} // if (cont == 4)

			else {
				TA0CCR1 += 31250;
			} // else: if (cont == 5)

			break;
		} // case (TA0IV_TACCR1)

		case (TA0IV_TACCR2): {
			break;
		} // case (TA0IV_TACCR2)

		case (TA0IV_TAIFG): {
			break;
		} // case (TA0IV_TAIFG)

	} // switch (TA0IV)

} // __interrupt void RTI_TA0CCR12(void)
