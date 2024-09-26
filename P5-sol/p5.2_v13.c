// edu@uma.es - 12/01/23 (v13)

// p5.2: Cuando se presione el pulsador Sx se enviara el caracter 'x'
// [Sx: Interrupcion ANT 32ms WD ; UART: Interrupcion, SMCLK 1MHz, 9600-8N1]

#include "msp430g2553.h"

unsigned char pulsador;

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
	UCA0CTL1 |= (UCSWRST);
	UCA0CTL1 |= (UCSSEL_2);
	UCA0BR1 = 0x00;
	UCA0BR0 = 0x68;
	UCA0MCTL = (UCBRF_0|UCBRS_1);
	UCA0CTL1 &= ~(UCSWRST);
	P1SEL |= (BIT2);
	P1SEL2 |= (BIT2);

	__enable_interrupt();

	while (1) {
	} // while (1)

} // void main(void)

#pragma vector = PORT1_VECTOR
__interrupt void RTI_P1(void) {

	if ((P1IE & P1IFG & BIT4) != 0) {
		P1IE &= ~(BIT4);
		WDTCTL = WDT_MDLY_32;
		IFG1 &= ~(WDTIFG);
		IE1 |= WDTIE;

		if ((P1IES & BIT4) != 0) {
			pulsador = 3;
			IE2 |= (UCA0TXIE);
		} // if ((P1IES & BIT4) != 0)

		else {
		} // else: if ((P1IES & BIT4) != 0)

		P1IES ^= (BIT4);
		P1IFG &= ~(BIT4);
	} // if ((P1IE & P1IFG & BIT4) != 0)

} // __interrupt void RTI_P1(void)

#pragma vector = PORT2_VECTOR
__interrupt void RTI_P2(void) {

	if ((P2IE & P2IFG & BIT1) != 0) {
		P2IE &= ~(BIT1);
		WDTCTL = WDT_MDLY_32;
		IFG1 &= ~(WDTIFG);
		IE1 |= WDTIE;

		if ((P2IES & BIT1) != 0) {
			pulsador = 4;
			IE2 |= (UCA0TXIE);
		} // if ((P2IES & BIT1) != 0)

		else {
		} // else: if ((P2IES & BIT1) != 0)

		P2IES ^= (BIT1);
		P2IFG &= ~(BIT1);
	} // if ((P2IE & P2IFG & BIT1) != 0)

	if ((P2IE & P2IFG & BIT2) != 0) {
		P2IE &= ~(BIT2);
		WDTCTL = WDT_MDLY_32;
		IFG1 &= ~(WDTIFG);
		IE1 |= WDTIE;

		if ((P2IES & BIT2) != 0) {
			pulsador = 5;
			IE2 |= (UCA0TXIE);
		} // if ((P2IES & BIT2) != 0)

		else {
		} // else: if ((P2IES & BIT2) != 0)

		P2IES ^= (BIT2);
		P2IFG &= ~(BIT2);
	} // if ((P2IE & P2IFG & BIT2) != 0)

	if ((P2IE & P2IFG & BIT3) != 0) {
		P2IE &= ~(BIT3);
		WDTCTL = WDT_MDLY_32;
		IFG1 &= ~(WDTIFG);
		IE1 |= WDTIE;

		if ((P2IES & BIT3) != 0) {
			pulsador = 6;
			IE2 |= (UCA0TXIE);
		} // if ((P2IES & BIT3) != 0)

		else {
		} // else: if ((P2IES & BIT3) != 0)

		P2IES ^= (BIT3);
		P2IFG &= ~(BIT3);
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

#pragma vector=USCIAB0TX_VECTOR
__interrupt void RTI_USCIA0TX(void) {
	
	if (((IE2 & UCA0TXIE) && (IFG2 & UCA0TXIFG)) != 0) {
		UCA0TXBUF = '0'+pulsador;
		IE2 &= ~(UCA0TXIE);
	} // if (((IE2 & UCA0TXIE) && (IFG2 & UCA0TXIFG)) != 0)
		
} // __interrupt void RTI_USCIA0TX(void)
