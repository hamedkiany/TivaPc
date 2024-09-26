// edu@uma.es - 22/03/22 (v6)

// p4.5: Cuando se presione Sx parpadeara el LEDx con distinta frecuencia segun
// se trate de LED3 (1Hz), LED4 (1.25Hz), LED5 (1.5Hz) o LED6 (1.75Hz)
// [Sx: Interrupcion ; TA0: Interrupcion, SMCLK 1MHz/8, TA0CCR0 CONT (LED3),
// TA0CCR1 CONT (LED4), TA1CCR1 CONT (LED5), TA1CCR2 CONT (LED6)]

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
	TA0CTL = TASSEL_2|ID_3|MC_2;
	TA1CTL = TASSEL_2|ID_3|MC_2;

	__enable_interrupt();

	while (1) {
	} // while (1)

} // void main(void)

#pragma vector = PORT1_VECTOR
__interrupt void RTI_P1(void) {

	if ((P1IE & P1IFG & BIT4) != 0) {
		P1IFG &= ~(BIT4);
		P2OUT |= (BIT4);
		TA0CCR0 = TA0R+62500;
		TA0CCTL0 &= ~(CCIFG);
		TA0CCTL0 |= (CCIE);
	} // if ((P1IE & P1IFG & BIT4) != 0)

} // __interrupt void RTI_P1(void)

#pragma vector = PORT2_VECTOR
__interrupt void RTI_P2(void) {

	if ((P2IE & P2IFG & BIT1) != 0) {
		P2IFG &= ~(BIT1);
		P2OUT |= (BIT5);
		TA0CCR1 = TA0R+50000;
		TA0CCTL1 &= ~(CCIFG);
		TA0CCTL1 |= (CCIE);
	} // if ((P2IE & P2IFG & BIT1) != 0)

	if ((P2IE & P2IFG & BIT2) != 0) {
		P2IFG &= ~(BIT2);
		P2OUT |= (BIT6);
		TA1CCR1 = TA1R+41667;
		TA1CCTL1 &= ~(CCIFG);
		TA1CCTL1 |= (CCIE);
	} // if ((P2IE & P2IFG & BIT2) != 0)

	if ((P2IE & P2IFG & BIT3) != 0) {
		P2IFG &= ~(BIT3);
		P2OUT |= (BIT7);
		TA1CCR2 = TA1R+35714;
		TA1CCTL2 &= ~(CCIFG);
		TA1CCTL2 |= (CCIE);
	} // if ((P2IE & P2IFG & BIT3) != 0)

} // __interrupt void RTI_P2(void)

#pragma vector = TIMER0_A0_VECTOR
__interrupt void RTI_TA0CCR0(void) {
	P2OUT ^= (BIT4);
	TA0CCR0 += 62500;
} // __interrupt void RTI_TA0CCR0(void)

#pragma vector = TIMER0_A1_VECTOR
__interrupt void RTI_TA0CCR12(void) {

	switch (TA0IV) {

		case (TA0IV_TACCR1): {
			P2OUT ^= (BIT5);
			TA0CCR1 += 50000;
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

#pragma vector = TIMER1_A1_VECTOR
__interrupt void RTI_TA1CCR12(void) {

	switch (TA1IV) {

		case (TA1IV_TACCR1): {
			P2OUT ^= (BIT6);
			TA1CCR1 += 41667;
			break;
		} // case (TA1IV_TACCR1)

		case (TA1IV_TACCR2): {
			P2OUT ^= (BIT7);
			TA1CCR2 += 35714;
			break;
		} // case (TA1IV_TACCR2)

		case (TA1IV_TAIFG): {
			break;
		} // case (TA1IV_TAIFG)

	} // switch (TA1IV)

} // __interrupt void RTI_TA1CCR12(void)
