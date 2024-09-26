// edu@uma.es - 30/03/22 (v6)

// p4.9: Cuando se presione y se suelte Sx durante menos de 1s conmutara el LEDx
// al soltar Sx, y cuando se presione y se suelte Sx durante 1s o mas conmutaran
// todos los LEDs excepto el LEDx al alcanzar 1s
// [Sx: Interrupcion ANT 32ms WD ; TA0: Interrupcion, SMCLK 1.1MHz/8, TA0CCR0 UP]

#include "msp430g2553.h"

unsigned char pulsador, multifuncion, cont;

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
	TA0CTL = (TASSEL_2|ID_3);
	TA0CCR0 = 62500-1;

	__enable_interrupt();

	while (1) {
	} // while (1)

} // void main(void)

#pragma vector = PORT1_VECTOR
__interrupt void RTI_P1(void) {

	if ((P1IE & P1IFG & BIT4) != 0) {
		P1IFG &= ~(BIT4);
		P1IE &= ~(BIT4);
		WDTCTL = WDT_MDLY_32;
		IFG1 &= ~(WDTIFG);
		IE1 |= WDTIE;

		if ((P1IES & BIT4) != 0) {
			pulsador = 3;
			multifuncion = 0;
			cont = 0;
			TA0CTL |= (MC_1|TACLR);
			TA0CCTL0 &= ~(CCIFG);
			TA0CCTL0 |= (CCIE);
		} // if ((P1IES & BIT4) != 0)

		else {

			if (multifuncion == 0) {
				P2OUT ^= (BIT4);
				TA0CTL &= ~(MC_1);
				TA0CCTL0 &= ~(CCIE);
			} // if (multifuncion == 0)

		} // else: if ((P1IES & BIT4) != 0)

		P1IES ^= (BIT4);
	} // if ((P1IE & P1IFG & BIT4) != 0)

} // __interrupt void RTI_P1(void)

#pragma vector = PORT2_VECTOR
__interrupt void RTI_P2(void) {

	if ((P2IE & P2IFG & BIT1) != 0) {
		P2IFG &= ~(BIT1);
		P2IE &= ~(BIT1);
		WDTCTL = WDT_MDLY_32;
		IFG1 &= ~(WDTIFG);
		IE1 |= WDTIE;

		if ((P2IES & BIT1) != 0) {
			pulsador = 4;
			multifuncion = 0;
			cont = 0;
			TA0CTL |= (MC_1|TACLR);
			TA0CCTL0 &= ~(CCIFG);
			TA0CCTL0 |= (CCIE);
		} // if ((P2IES & BIT1) != 0)

		else {

			if (multifuncion == 0) {
				P2OUT ^= (BIT5);
				TA0CTL &= ~(MC_1);
				TA0CCTL0 &= ~(CCIE);
			} // if (multifuncion == 0)

		} // else: if ((P2IES & BIT1) != 0)

		P2IES ^= (BIT1);
	} // if ((P2IE & P2IFG & BIT1) != 0)

	if ((P2IE & P2IFG & BIT2) != 0) {
		P2IFG &= ~(BIT2);
		P2IE &= ~(BIT2);
		WDTCTL = WDT_MDLY_32;
		IFG1 &= ~(WDTIFG);
		IE1 |= WDTIE;

		if ((P2IES & BIT2) != 0) {
			pulsador = 5;
			multifuncion = 0;
			cont = 0;
			TA0CTL |= (MC_1|TACLR);
			TA0CCTL0 &= ~(CCIFG);
			TA0CCTL0 |= (CCIE);
		} // if ((P2IES & BIT2) != 0)

		else {

			if (multifuncion == 0) {
				P2OUT ^= (BIT6);
				TA0CTL &= ~(MC_1);
				TA0CCTL0 &= ~(CCIE);
			} // if (multifuncion == 0)

		} // else: if ((P2IES & BIT2) != 0)

		P2IES ^= (BIT2);
	} // if ((P2IE & P2IFG & BIT2) != 0)

	if ((P2IE & P2IFG & BIT3) != 0) {
		P2IFG &= ~(BIT3);
		P2IE &= ~(BIT3);
		WDTCTL = WDT_MDLY_32;
		IFG1 &= ~(WDTIFG);
		IE1 |= WDTIE;

		if ((P2IES & BIT3) != 0) {
			pulsador = 6;
			multifuncion = 0;
			cont = 0;
			TA0CTL |= (MC_1|TACLR);
			TA0CCTL0 &= ~(CCIFG);
			TA0CCTL0 |= (CCIE);
		} // if ((P2IES & BIT3) != 0)

		else {

			if (multifuncion == 0) {
				P2OUT ^= (BIT7);
				TA0CTL &= ~(MC_1);
				TA0CCTL0 &= ~(CCIE);
			} // if (multifuncion == 0)

		} // else: if ((P2IES & BIT3) != 0)

		P2IES ^= (BIT3);
	} // if ((P2IE & P2IFG & BIT3) != 0)

} // __interrupt void RTI_P2(void)

#pragma vector = WDT_VECTOR
__interrupt void RTI_WD(void) {
	WDTCTL = (WDTPW|WDTHOLD);
	IE1 &= ~(WDTIE);
	P1IFG &= ~(BIT4);
	P2IFG &= ~(BIT3|BIT2|BIT1);
	P1IE |= (BIT4);
	P2IE |= (BIT3|BIT2|BIT1);
} // __interrupt void RTI_WD(void)

#pragma vector = TIMER0_A0_VECTOR
__interrupt void RTI_TA0CCR0(void) {

	cont++;

	if (cont == 2) {

		switch (pulsador) {

			case (3): {
				P2OUT ^= (BIT7|BIT6|BIT5);
				break;
			} // case (3)

			case (4): {
				P2OUT ^= (BIT7|BIT6|BIT4);
				break;
			} // case (4)

			case (5): {
				P2OUT ^= (BIT7|BIT5|BIT4);
				break;
			} // case (5)

			case (6): {
				P2OUT ^= (BIT6|BIT5|BIT4);
				break;
			} // case (6)

		} // switch (pulsador)

		multifuncion = 1;
		TA0CTL &= ~(MC_1);
		TA0CCTL0 &= ~(CCIE);
	} // if (cont == 2)

} // __interrupt void RTI_TA0CCR0(void)
