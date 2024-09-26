// edu@uma.es - 12/01/23 (v11)

// p5.3: Se implementara un eco invertido de 4 caracteres, de forma que una vez
// recibidos 4 caracteres cualquiera se enviaran en orden inverso a como se
// recibieron
// [UART: Interrupcion, SMCLK 1MHz, 9600-8N1]

#include "msp430g2553.h"

#define	RX_BUFFER_SIZE	4
#define	TX_BUFFER_SIZE	4

unsigned char rx_buffer[RX_BUFFER_SIZE];
unsigned char tx_buffer[TX_BUFFER_SIZE];

void main(void) {
	WDTCTL = (WDTPW|WDTHOLD);	// Detiene el Watchdog
	DCOCTL = 0;						// Establece la frecuencia a 1MHz
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
	UCA0CTL1 |= (UCSWRST);
	UCA0CTL1 |= (UCSSEL_2);
	UCA0BR1 = 0x00;
	UCA0BR0 = 0x68;
	UCA0MCTL = (UCBRF_0|UCBRS_1);
	UCA0CTL1 &= ~(UCSWRST);
	IFG2 &= ~(UCA0RXIFG);
	IE2 |= (UCA0RXIE);
	P1SEL |= (BIT2|BIT1);
	P1SEL2 |= (BIT2|BIT1);

	__enable_interrupt();

	while (1) {
	} // while (1)

} // void main(void)

#pragma vector=USCIAB0RX_VECTOR
__interrupt void RTI_USCIA0RX(void) {
	static unsigned char cont_rx = 0;
	unsigned char cont;

	if (((IE2 & UCA0RXIE) && (IFG2 & UCA0RXIFG)) != 0) {
		rx_buffer[cont_rx++] = UCA0RXBUF;

		if (cont_rx == RX_BUFFER_SIZE) {
			cont_rx = 0;

			for (cont = 0; cont < TX_BUFFER_SIZE; cont++) {
				tx_buffer[cont] = rx_buffer[TX_BUFFER_SIZE-1-cont];
			} // for (cont = 0; cont < RX_BUFFER_SIZE; cont++)

			IE2 |= (UCA0TXIE);
		} // if (cont_rx == RX_BUFFER_SIZE)
		
	} // if (((IE2 & UCA0RXIE) && (IFG2 & UCA0RXIFG)) != 0)

} // __interrupt void RTI_USCIA0RX(void)

#pragma vector=USCIAB0TX_VECTOR
__interrupt void RTI_USCIA0TX(void) {
	static unsigned int cont_tx = 0;

	if (((IE2 & UCA0TXIE) && (IFG2 & UCA0TXIFG)) != 0) {
		UCA0TXBUF = tx_buffer[cont_tx++];

		if (cont_tx == TX_BUFFER_SIZE) {
			cont_tx = 0;
			IE2 &= ~(UCA0TXIE);
		} // if (cont_tx == TX_BUFFER_SIZE)
			
	} // if (((IE2 & UCA0TXIE) && (IFG2 & UCA0TXIFG)) != 0)
		
} // __interrupt void RTI_USCIA0TX(void)
