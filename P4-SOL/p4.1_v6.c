// edu@uma.es - 19/04/22 (v6)
// gonzalez@uma.es

// p4.1: Cuando se presione el pulsador Sx se encendera el LEDx durante 300ms
// [Sx: Interrupcion ; TA0: Interrupcion, SMCLK 1MHz/8, TA0CCR0 UP]

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
	TA0CCR0 = 37500-1;

	__enable_interrupt();

	while (1) {
	} // while (1)

} // void main(void)

#pragma vector = PORT1_VECTOR
__interrupt void RTI_P1(void) {

	if ((P1IE & P1IFG & BIT4) != 0) {
		P1IFG &= ~(BIT4);
		P2OUT |= (BIT4);
		TA0CTL |= (MC_1|TACLR);
		TA0CCTL0 &= ~(CCIFG); // Antes de hablitar la interrupción es recomedable borrar el flag
		TA0CCTL0 |= (CCIE);
	} // if ((P1IE & P1IFG & BIT4) != 0)

} // __interrupt void RTI_P1(void)

#pragma vector = PORT2_VECTOR
__interrupt void RTI_P2(void) {

	if ((P2IE & P2IFG & BIT1) != 0) {
		P2IFG &= ~(BIT1);
		P2OUT |= (BIT5);
		TA0CTL |= (MC_1|TACLR);
		TA0CCTL0 &= ~(CCIFG);
		TA0CCTL0 |= (CCIE);
	} // if ((P2IE & P2IFG & BIT1) != 0)

	if ((P2IE & P2IFG & BIT2) != 0) {
		P2IFG &= ~(BIT2);
		P2OUT |= (BIT6);
		TA0CTL |= (MC_1|TACLR);
		TA0CCTL0 &= ~(CCIFG);
		TA0CCTL0 |= (CCIE);
	} // if ((P2IE & P2IFG & BIT2) != 0)

	if ((P2IE & P2IFG & BIT3) != 0) {
		P2IFG &= ~(BIT3);
		P2OUT |= (BIT7);
		TA0CTL |= (MC_1|TACLR);
		TA0CCTL0 &= ~(CCIFG);
		TA0CCTL0 |= (CCIE);
	} // if ((P2IE & P2IFG & BIT3) != 0)

} // __interrupt void RTI_P2(void)

#pragma vector = TIMER0_A0_VECTOR
__interrupt void RTI_TA0CCR0(void) {
	P2OUT &= ~(BIT7|BIT6|BIT5|BIT4);
	TA0CTL &= ~(MC0 | MC1); // La opción programada es más recomendable que TA0CTL &= ~(MC_1);
	TA0CCTL0 &= ~(CCIE);
} // __interrupt void RTI_TA0CCR0(void)
