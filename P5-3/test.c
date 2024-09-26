#include <msp430g2553.h>

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
    char receivedByte;

    receivedByte = UCA0RXBUF;    // Lee el byte recibido en el buffer

    // Insertar aquí el código para manejar el byte recibido
    P2DIR |= (BIT4 | BIT5 | BIT6 | BIT7);
    if (receivedByte == '1'){
        P2OUT ^= BIT4;      // Conmuta el estado de los LEDs
    }
    else if (receivedByte == '2')
    {
        P2OUT ^= BIT5;      // Conmuta el estado de los LEDs
    }
    else if (receivedByte == '3')
    {
        P2OUT ^= BIT6;      // Conmuta el estado de los LEDs
    }
    else if (receivedByte == '4')
    {
        P2OUT ^= BIT7;      // Conmuta el estado de los LEDs
    }
    return;
}
