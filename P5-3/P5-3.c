#include <msp430g2553.h>
//char recived[4];
//char transmited[4];
//volatile int control  = 0;
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;    // Detiene el watchdog timer
    DCOCTL = 0; // Establece la frecuencia a 1MHz
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    P1SEL |= (BIT1 | BIT2) ;       // P1.1 = RXD, P1.2=TXD
    P1SEL2 |= (BIT1 | BIT2) ;      // P1.1 = RXD, P1.2=TXD
    P2SEL &= ~( BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 |BIT7);
    P2SEL2 &= ~(BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 |BIT7);

    UCA0CTL1 |= UCSSEL_2;       // SMCLK como fuente de reloj
    UCA0BR0 = 0x68;              // Configura los baudios a 9600
    UCA0BR1 = 0x00;
    UCA0MCTL |= UCBRS_1;        // Modulación de frecuencia
    UCA0CTL1 &= ~UCSWRST;       // Activa la USCI
    UC0IE |= UCA0RXIE;          // Habilita la interrupción de recepción de UART
    __enable_interrupt();

}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    static char recived[4];
    static int count = 0;
    char receivedByte = UCA0RXBUF;
    recived[count] = receivedByte;
    count++;

    if (count == 4) // Si se han recibido 4 bytes
    {
        count = 0;

        while (!(IFG2 & UCA0TXIFG)); // Espera a que el buffer de transmisión esté vacío
        UCA0TXBUF = recived[3];
        while (!(IFG2 & UCA0TXIFG)); // Espera a que el buffer de transmisión esté vacío
        UCA0TXBUF = recived[2];
        while (!(IFG2 & UCA0TXIFG)); // Espera a que el buffer de transmisión esté vacío
        UCA0TXBUF = recived[1];
        while (!(IFG2 & UCA0TXIFG)); // Espera a que el buffer de transmisión esté vacío
        UCA0TXBUF = recived[0];
    }

    return;
}
