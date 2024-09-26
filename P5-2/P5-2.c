#include <msp430g2553.h>
void config_prifericos(void);
void configure_uart(void);
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;    // Detiene el watchdog timer
    DCOCTL = 0; // Establece la frecuencia a 1MHz
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    config_prifericos();
    configure_uart();
    __enable_interrupt();

}

void config_prifericos(void)
{
    P1SEL &= ~(BIT4);
    P1SEL2 &= ~(BIT4);
    P2SEL &= ~( BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P2SEL2 &= ~(BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P2SEL |=(BIT0);
    P2SEL2 &= ~(BIT0);
    P1DIR &= ~(BIT4);
    P1REN |= (BIT4);
    P1OUT |= (BIT4);
    P1IE |= (BIT4);
    P1IES |= (BIT4);
    P1IFG &= ~(BIT4);
    P2DIR &= ~(BIT1 | BIT2 | BIT3);
    P2REN |= (BIT1 | BIT2 | BIT3);
    P2OUT |= (BIT1 | BIT2 | BIT3);
    P2IE |= (BIT1 | BIT2 | BIT3);
    P2IES |= (BIT1 | BIT2 | BIT3);
    P2IFG &= ~(BIT1 | BIT2 | BIT3);

}

void configure_uart(void)
{
    P1SEL |= (BIT1 | BIT2) ;       // P1.1 = RXD, P1.2=TXD
    P1SEL2 |= (BIT1 | BIT2) ;      // P1.1 = RXD, P1.2=TXD
    UCA0CTL1 |= UCSSEL_2;       // SMCLK como fuente de reloj
    UCA0BR0 = 0x68;              // Configura los baudios a 9600
    UCA0BR1 = 0x00;
    UCA0MCTL |= UCBRS_1;        // Modulación de frecuencia
    UCA0CTL1 &= ~UCSWRST;       // Activa la USCI
    UC0IE |= UCA0RXIE;          // Habilita la interrupción de recepción de UART

}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if ((P1IE & P1IFG & BIT4)!= 0)
    {
        UCA0TXBUF = '1';
    }
    P1IFG &= ~BIT4;
}



#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    if (( P2IE & P2IFG & BIT1)!= 0)
    {
        UCA0TXBUF = '2';
    }
    P2IFG &= ~BIT1;
    if (( P2IE & P2IFG & BIT2)!= 0)
    {
        UCA0TXBUF = '3';
    }
    P2IFG &= ~BIT2;
    if (( P2IE &P2IFG & BIT3)!= 0)
    {
        UCA0TXBUF = '4';
    }
    P2IFG &= ~BIT3;

}


#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    char receivedByte;

    receivedByte = UCA0RXBUF;    // Lee el byte recibido en el buffer
    UCA0TXBUF = 'R'; // envía el carácter 'H'
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
