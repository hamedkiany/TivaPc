// edu@uma.es - 12/01/23 (v11)

// p5.1: Cuando se reciba el caracter 'x' conmutara el LEDx
// [UART: Interrupcion, SMCLK 1MHz, 9600-8N1]

#include "msp430g2553.h"

void main(void) {
	WDTCTL = (WDTPW|WDTHOLD);	// Detiene el Watchdog
	DCOCTL = 0;						// Establece la frecuencia a 1MHz
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
	P2DIR |= (BIT7|BIT6|BIT5|BIT4);
	P2OUT &= ~(BIT7|BIT6|BIT5|BIT4);
	P2SEL &= ~(BIT7|BIT6);
	UCA0CTL1 |= (UCSWRST);
	UCA0CTL1 |= (UCSSEL_2);
	UCA0BR1 = 0x00;
	UCA0BR0 = 0x68;
	UCA0MCTL = (UCBRF_0|UCBRS_1);
	UCA0CTL1 &= ~(UCSWRST);
	IFG2 &= ~(UCA0RXIFG);
	IE2 |= (UCA0RXIE);
	P1SEL |= (BIT1);
	P1SEL2 |= (BIT1);

	__enable_interrupt();

	while (1) {
	} // while (1)

} // void main(void)

#pragma vector=USCIAB0RX_VECTOR
__interrupt void RTI_USCIA0RX(void) {

	if (((IE2 & UCA0RXIE) && (IFG2 & UCA0RXIFG)) != 0) {

		switch (UCA0RXBUF) {

			case ('3'): {
				P2OUT ^= (BIT4);
				break;
			} // case ('3')

			case ('4'): {
				P2OUT ^= (BIT5);
				break;
			} // case ('4')

			case ('5'): {
				P2OUT ^= (BIT6);
				break;
			} // case ('5')

			case ('6'): {
				P2OUT ^= (BIT7);
				break;
			} // case ('6')

		} // switch (UCA0RXBUF)
		
	} // if (((IE2 & UCA0RXIE) && (IFG2 & UCA0RXIFG)) != 0)

} // __interrupt void RTI_USCIA0RX(void)
