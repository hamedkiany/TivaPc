/*******************************
 * ANTI RUO 4.8
 *
 *
 */

// edu@uma.es - 30/03/22 (v6) -eva

// p4.8: Cuando se presione el pulsador Sx conmutara el LEDx
// [Sx: Interrupcion ANT 32ms WD]

#include "msp430g2553.h"

void main(void) {
    WDTCTL = (WDTPW|WDTHOLD);   // Detiene el Watchdog
    DCOCTL = 0;                     // Establece la frecuencia a 1MHz
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
            P2OUT ^= (BIT4);
        } // if ((P1IES & BIT4) != 0)

        else {
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
            P2OUT ^= (BIT5);
        } // if ((P2IES & BIT1) != 0)

        else {
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
            P2OUT ^= (BIT6);
        } // if ((P2IES & BIT2) != 0)

        else {
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
            P2OUT ^= (BIT7);
        } // if ((P2IES & BIT3) != 0)

        else {
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





/**********************************/
// edu@uma.es - 30/03/22 (v6)-eva

// p4.9: Cuando se presione y se suelte Sx durante menos de 1s conmutara el LEDx
// al soltar Sx, y cuando se presione y se suelte Sx durante 1s o mas conmutaran
// todos los LEDs excepto el LEDx al alcanzar 1s
// [Sx: Interrupcion ANT 32ms WD ; TA0: Interrupcion, SMCLK 1.1MHz/8, TA0CCR0 UP]

#include "msp430g2553.h"

unsigned char pulsador, multifuncion, cont;

void main(void) {
    WDTCTL = (WDTPW|WDTHOLD);   // Detiene el Watchdog
    DCOCTL = 0;                     // Establece la frecuencia a 1MHz
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



/* zumbador *********************************
 *
 *
 *
 */

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
    P1IES &= ~(BIT4);
    P1IFG &= ~(BIT4);
    P2DIR &= ~(BIT1 | BIT2 | BIT3);
    P2REN |= (BIT1 | BIT2 | BIT3);
    P2OUT |= (BIT1 | BIT2 | BIT3);
    P2IE |= (BIT1 | BIT2 | BIT3);
    P2IES &= ~(BIT1 | BIT2 | BIT3);
    P2IFG &= ~(BIT1 | BIT2 | BIT3);
    P2DIR |= BIT0;     //config zumbador port

}


void initTimer_A(void)
{
    DCOCTL = 0; // Establece la frecuencia a 1MHz
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    //Timer0_A3 Configuration
    TA1CTL = (TASSEL_2 | ID_0 | MC_0); //Select SMCLK, SMCLK/1, accendente
    //TA1CCTL0 &= ~(CCIFG);
    TA1CCTL0 |= (OUTMOD_3); //Enable interrupt for CCR0,PWM set/reset
    TA1CCTL0 &= ~(CAP); // modo comparacion
    //TA1CCTL1 |= CCIE; //Enable interrupt for CCR1.
    TA1CCTL1 &= ~(CAP); // modo comparacion


}


#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if (P1IFG & BIT4)
    {
        if (buzzer_on) {
            TA1CCR0 = 0; // Desactiva el zumbador
            buzzer_on = 0; // Actualiza el estado
        } else {
            TA1CCR0 = 2272; // Configura la frecuencia del zumbador a 440Hz
            TA1CCR1 = 1136; // Ciclo de trabajo del 50%
            buzzer_on = 1; // Actualiza el estado
        }
        TA1CTL |= (MC_1);
    }
    P1IFG &= ~BIT4;
}



#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    if (P2IFG & BIT1)
    {
        if (buzzer_on) {
               TA1CCR0 = 0; // Desactiva el zumbador
               buzzer_on = 0; // Actualiza el estado
           } else {
               TA1CCR0 = 2272; // Configura la frecuencia del zumbador a 440Hz
               TA1CCR1 = 1136; // Ciclo de trabajo del 50%
               buzzer_on = 1; // Actualiza el estado
           }
           TA1CTL |= (MC_1);
   }
    P2IFG &= ~BIT1;
    if (P2IFG & BIT2)
    {
        if (buzzer_on) {
               TA1CCR0 = 0; // Desactiva el zumbador
               buzzer_on = 0; // Actualiza el estado
           } else {
               TA1CCR0 = 2272; // Configura la frecuencia del zumbador a 440Hz
               TA1CCR1 = 1136; // Ciclo de trabajo del 50%
               buzzer_on = 1; // Actualiza el estado
           }
           TA1CTL |= (MC_1);
    }
    P2IFG &= ~BIT2;
    if (P2IFG & BIT3)
    {
        if (buzzer_on) {
               TA1CCR0 = 0; // Desactiva el zumbador
               buzzer_on = 0; // Actualiza el estado
           } else {
               TA1CCR0 = 2272; // Configura la frecuencia del zumbador a 440Hz
               TA1CCR1 = 1136; // Ciclo de trabajo del 50%
               buzzer_on = 1; // Actualiza el estado
           }
           TA1CTL |= (MC_1);
    }
    P2IFG &= ~BIT3;

}


 */


 // edu@uma.es - 06/04/22 (v8) --- EVA

 // p4.6: Cuando se presione el pulsador Sx conmutara el Zumbador, generando una
 // nota de frecuencia 440Hz y ciclo de trabajo 50%
 // [Sx: Interrupcion ; TA1: SMCLK 1MHz]

 #include "msp430g2553.h"

 void main(void) {
     WDTCTL = (WDTPW|WDTHOLD);   // Detiene el Watchdog
     DCOCTL = 0;                     // Establece la frecuencia a 1MHz
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
     P2DIR |= (BIT0);
     P2SEL |= (BIT0);
     TA1CTL = (TASSEL_2);
     TA1CCR0 = 1136-1;
     TA1CCTL0 = OUTMOD_4;

     __enable_interrupt();

     while (1) {
     } // while (1)

 } // void main(void)

 #pragma vector = PORT1_VECTOR
 __interrupt void RTI_P1(void) {

     if ((P1IE & P1IFG & BIT4) != 0) {
         P1IFG &= ~(BIT4);

         if ((TA1CTL & MC_1) == 0) {
             TA1CTL |= (MC_1|TACLR);
         } // if ((TA1CTL & MC_1) == 0)

         else {
             TA1CTL &= ~(MC_1);
         } // else: if ((TA1CTL & MC_1) == 0)

     } // if ((P1IE & P1IFG & BIT4) != 0)

 } // __interrupt void RTI_P1(void)

 #pragma vector = PORT2_VECTOR
 __interrupt void RTI_P2(void) {

     if ((P2IE & P2IFG & BIT1) != 0) {
         P2IFG &= ~(BIT1);

         if ((TA1CTL & MC_1) == 0) {
             TA1CTL |= (MC_1|TACLR);
         } // if ((TA1CTL & MC_1) == 0)

         else {
             TA1CTL &= ~(MC_1);
         } // else: if ((TA1CTL & MC_1) == 0)

     } // if ((P2IE & P2IFG & BIT1) != 0)

     if ((P2IE & P2IFG & BIT2) != 0) {
         P2IFG &= ~(BIT2);

         if ((TA1CTL & MC_1) == 0) {
             TA1CTL |= (MC_1|TACLR);
         } // if ((TA1CTL & MC_1) == 0)

         else {
             TA1CTL &= ~(MC_1);
         } // else: if ((TA1CTL & MC_1) == 0)

     } // if ((P2IE & P2IFG & BIT2) != 0)

     if ((P2IE & P2IFG & BIT3) != 0) {
         P2IFG &= ~(BIT3);

         if ((TA1CTL & MC_1) == 0) {
             TA1CTL |= (MC_1|TACLR);
         } // if ((TA1CTL & MC_1) == 0)

         else {
             TA1CTL &= ~(MC_1);
         } // else: if ((TA1CTL & MC_1) == 0)

     } // if ((P2IE & P2IFG & BIT3) != 0)

 } // __interrupt void RTI_P2(void)



/**********************************************
 * UART
 *
 */////////////////////////////////////////////////

 // edu@uma.es - 12/01/23 (v11) -eva

 // p5.3: Se implementara un eco invertido de 4 caracteres, de forma que una vez
 // recibidos 4 caracteres cualquiera se enviaran en orden inverso a como se
 // recibieron
 // [UART: Interrupcion, SMCLK 1MHz, 9600-8N1]

 #include "msp430g2553.h"

 #define RX_BUFFER_SIZE  4
 #define TX_BUFFER_SIZE  4

 unsigned char rx_buffer[RX_BUFFER_SIZE];
 unsigned char tx_buffer[TX_BUFFER_SIZE];

 void main(void) {
     WDTCTL = (WDTPW|WDTHOLD);   // Detiene el Watchdog
     DCOCTL = 0;                     // Establece la frecuencia a 1MHz
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


 */
/*****************************************************
 * 4-2 timer A0 TA0CCR0 acedente 1.25s
 *
 *
 *
 *
 *//////


 //timer function
 void initTimer_A(void)
 {
     DCOCTL = 0; // Establece la frecuencia a 1MHz
     BCSCTL1 = CALBC1_1MHZ;
     DCOCTL = CALDCO_1MHZ;

     //Timer0 Configuration
     TACCR0 = 0; //Initially, Stop the Timer y modo de operación ascendente

     TA0CTL = (TASSEL_2 | ID_3 | MC_1); //Select SMCLK, SMCLK/8, Up Mode to CCR0
     TA0CCTL0 |= CCIE; //Enable interrupt for CCR0.

 }

 //Timer ISR
 #pragma vector = TIMER0_A0_VECTOR
 __interrupt void Timer_A_CCR0_ISR(void)
 {
     ++count;
     if (count == 5)
     {
         count = 0;
         TA0CTL = (TASSEL_2 | ID_3 | MC_0);

         switch (led)
         {
         case 3:
             P2OUT &= ~(BIT4);
             TA0CCTL0 &= ~(CCIFG);
             break;
         case 4:
             P2OUT &= ~(BIT5);
             TA0CCTL0 &= ~(CCIFG);
             break;
         case 5:
             P2OUT &= ~(BIT6);
             TA0CCTL0 &= ~(CCIFG);
             break;
         case 6:
             P2OUT &= ~(BIT7);
             TA0CCTL0 &= ~(CCIFG);
             break;

         }

     }

 }

 #pragma vector=PORT1_VECTOR
 __interrupt void Port_1(void)
 {
     if (P1IFG & BIT4)
     {
         P2DIR |= (BIT4);
         P2OUT |= (BIT4);
         led = 3;
         TA0CCR0 = 31250;
         count = 0;
         TA0CTL = (TASSEL_2 | ID_3 | MC_1);
     }
     P1IFG &= ~BIT4;

 }

 #pragma vector=PORT2_VECTOR
 __interrupt void Port_2(void)
 {
     if (P2IFG & BIT1)
     {
         P2DIR |= (BIT5);
         P2OUT |= (BIT5);
         led = 4;
         TA0CCR0 = 31250;
         count = 0;
         TA0CTL = (TASSEL_2 | ID_3 | MC_1);
     }
     P2IFG &= ~BIT1;
     if (P2IFG & BIT2)
     {
         P2DIR |= (BIT6);
         P2OUT |= (BIT6);
         led = 5;
         TA0CCR0 = 31250;
         count = 0;
         TA0CTL = (TASSEL_2 | ID_3 | MC_1);
     }
     P2IFG &= ~BIT2;
     if (P2IFG & BIT3)
     {
         P2DIR |= (BIT7);
         P2OUT |= (BIT7);
         led = 6;
         TA0CCR0 = 31250;
         count = 0;
         TA0CTL = (TASSEL_2 | ID_3 | MC_1);
     }
     P2IFG &= ~BIT3;

 }

 */



 /*****************************************************
  * 4-4 timer A0 TA0CCR1 continuo 1.25s
  *
  *
  *
  *
  *//////


 void initTimer_A(void)
 {
     DCOCTL = 0; // Establece la frecuencia a 1MHz
     BCSCTL1 = CALBC1_1MHZ;
     DCOCTL = CALDCO_1MHZ;

     //Timer0_A3 Configuration
 //    TA0CCR1 = 31250 - 1; //Initially, Stop the Timer
     TA0CTL = (TASSEL_2 | ID_3 | MC_0); //Select SMCLK, SMCLK/8, Up continue
     TA0CCTL1 |= CCIE; //Enable interrupt for CCR1.
     TA1CCTL1 &= ~(CAP); // modo comparacion

 }

 //Timer ISR
 #pragma vector = TIMER0_A1_VECTOR
 __interrupt void Timer_A_CCR1_ISR(void)
 {
     ++count;
     TA0CCR1 += 31250;
     TA0CCTL1 &= ~(CCIFG);
     if (count == 5)
     {
         count = 0;

         switch (led)
         {
         case 3:
             P2OUT &= ~(BIT4);
             TA0CTL &= ~(MC0 | MC1);
             TA0CCTL1 &= ~(CCIFG);
             break;
         case 4:
             P2OUT &= ~(BIT5);
             TA0CTL &= ~(MC0 | MC1);
             TA0CCTL1 &= ~(CCIFG);
             break;
         case 5:
             P2OUT &= ~(BIT6);
             TA0CTL &= ~(MC0 | MC1);
             TA0CCTL1 &= ~(CCIFG);
             break;
         case 6:
             P2OUT &= ~(BIT7);
             TA0CTL &= ~(MC0 | MC1);
             TA0CCTL1 &= ~(CCIFG);
             break;

         }
     }

 }

 #pragma vector=PORT1_VECTOR
 __interrupt void Port_1(void)
 {
     if (P1IFG & BIT4)
     {
         P2DIR |= (BIT4);
         P2OUT |= (BIT4);
         led = 3;
         TA0CCR1 = TAR + 31250;
         TA0CTL |= (MC_2);
         count = 0;
     }
     P1IFG &= ~BIT4;

 }

 #pragma vector=PORT2_VECTOR
 __interrupt void Port_2(void)
 {
     if (P2IFG & BIT1)
     {
         P2DIR |= (BIT5);
         P2OUT |= (BIT5);
         led = 4;
         TA0CCR1 = TAR + 31250;
         TA0CTL |= (MC_2);
         count = 0;
     }
     P2IFG &= ~BIT1;
     if (P2IFG & BIT2)
     {
         P2DIR |= (BIT6);
         P2OUT |= (BIT6);
         led = 5;
         TA0CCR1 = TAR + 31250;
         TA0CTL |= (MC_2);
         count = 0;
     }
     P2IFG &= ~BIT2;
     if (P2IFG & BIT3)
     {
         P2DIR |= (BIT7);
         P2OUT |= (BIT7);
         led = 6;
         TA0CCR1 = TAR + 31250;
         TA0CTL |= (MC_2);
         count = 0;

     }
     P2IFG &= ~BIT3;

 }


/********************************************
 *
 * P6-2  eventos
 *
 */
#include "msp430g2553.h"

#define ST_A 0 // Todos los LED apagados
#define ST_B 1 // LED 5 y 3 encendidos
#define ST_C 2 // LED 6 y 4 encendidos

#define EV_S3 BIT0
#define EV_S4 BIT1
#define EV_S5 BIT2
#define EV_S6 BIT3

#define SET(ev) (eventos |= (ev))
#define RESET(ev) (eventos &= ~(ev))
#define TEST(ev) (eventos & (ev))
void config_prifericos(void);
void setupled(void);
unsigned char eventos;

void main(void) {
    unsigned char estado;

    WDTCTL = WDTPW | WDTHOLD;
    DCOCTL = 0; // Establece la frecuencia a 1MHz
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    WDTCTL = WDT_MDLY_32; // opciOn 1
    config_prifericos();
    setupled();
    eventos = 0;
    estado = ST_A;

    while (1) {
        __low_power_mode_0();

        while (eventos) {

            switch (estado) {

                case (ST_A): {

                    if (TEST(EV_S3)) {
                        P2OUT |= (BIT4 | BIT6);
                        estado = ST_B;
                        RESET(EV_S3);
                    } // if(TEST( EV_S3))

                    if (TEST(EV_S4)) {
                        RESET(EV_S4);
                    } // if(TEST( EV_S4))

                    if (TEST(EV_S5)) {
                        RESET(EV_S5);
                    } // if(TEST( EV_S5))

                    if (TEST(EV_S6)) {
                        RESET(EV_S6);
                    } // if(TEST( EV_S6))
                    break;
                } // case (ST_A):

                case (ST_B):{

                    if (TEST(EV_S3)) {
                        RESET(EV_S3);
                    } // if(TEST( EV_S3))

                    if (TEST(EV_S4)) {
                        P2OUT |= (BIT5 | BIT7);
                        estado = ST_C;
                        RESET(EV_S4);
                    } // if(TEST( EV_S4))

                    if (TEST(EV_S5)) {
                        RESET(EV_S5);
                    } // if(TEST( EV_S5))

                    if (TEST(EV_S6)) {
                        P2OUT &= ~(BIT4 | BIT5 | BIT6 | BIT7);
                        estado = ST_A;
                        RESET(EV_S6);
                    } // if(TEST( EV_S6))
                    break;
                } // case (ST_B):

                case (ST_C): {

                    if (TEST(EV_S3)) {
                        RESET(EV_S3);
                    } // if(TEST( EV_S3))

                    if (TEST(EV_S4)) {
                        RESET(EV_S4);
                    } // if(TEST( EV_S4))

                    if (TEST(EV_S5)) {
                        P2OUT |= (BIT4 | BIT6);
                        estado = ST_B;
                        RESET(EV_S5);
                    } // if(TEST( EV_S5))

                    if (TEST(EV_S6)) {
                        RESET(EV_S6);
                    } // if(TEST( EV_S6))

                    break;
                } // case (ST_C):

            } // switch(estado)

        } // while (eventos)

    } // while(1)

} // void main(void)

void config_prifericos(void)
{
    P1SEL &= ~(BIT4);
    P1SEL2 &= ~(BIT4);
    P2SEL &= ~(BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT7);
    P2SEL2 &= ~(BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT7);
    P2SEL &= ~(BIT6);
    P2SEL2 &= ~(BIT6);
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
    P2IES &= ~(BIT1 | BIT3); // 4 y 6 flango subido
    P2IES |= (BIT2);
    P2IFG &= ~(BIT1 | BIT2 | BIT3);
}

void setupled(void)
{
    P2DIR |= (BIT4 | BIT5 | BIT6 | BIT7);
    P2OUT &= ~(BIT4 | BIT5 | BIT6 | BIT7);
}

#pragma vector = WDT_VECTOR
__interrupt void RTI_WDT(void)
{
    IFG1 &= ~(WDTIFG);
    IE1 &= ~(WDTIE);
    P1IFG &= ~(BIT4);
    P1IE |= (BIT4);
    P2IFG &= ~(BIT1 | BIT2 | BIT3);
    P2IE |= (BIT1 | BIT2 | BIT3);
}

#pragma vector = PORT1_VECTOR
__interrupt void RTI_P1(void)
{
    if ((P1IE & P1IFG & BIT4) != 0)
    {
        P1IFG &= ~(BIT4);
        P1IE &= ~(BIT4);
        P1IES ^= (BIT4);
        IE1 |= (WDTIE);
        if (P1IES & BIT4)
        {
            SET(EV_S3);
        }
        else
        {
        }
    } // if ((P1IE & P1IFG & BIT4) != 0)
    if (eventos)
    {
        __low_power_mode_off_on_exit();
    } // if (eventos)
} // __interrupt void RTI_P1(void)

#pragma vector = PORT2_VECTOR
__interrupt void RTI_P2(void)
{
    if ((P2IE & P2IFG & BIT1) != 0)
    {
        P2IFG &= ~(BIT1);
        P2IE &= ~(BIT1);
        P2IES ^= (BIT1);
        IE1 |= (WDTIE);
        if (P2IES & BIT1)
        {
        }
        else
        {
            SET(EV_S4);
        }
    } // if ((P2IE & P2IFG & BIT1) != 0)
    if ((P2IE & P2IFG & BIT2) != 0)
    {
        P2IFG &= ~(BIT2);
        P2IE &= ~(BIT2);
        P2IES ^= (BIT2);
        IE1 |= (WDTIE);
        if (P2IES & BIT2)
        {
            SET(EV_S5);
        }
        else
        {
        }
    } // if ((P2IE & P2IFG & BIT2) != 0)
    if ((P2IE & P2IFG & BIT3) != 0)
    {
        P2IFG &= ~(BIT3);
        P2IE &= ~(BIT3);
        P2IES ^= (BIT3);
        IE1 |= (WDTIE);
        if (P2IES & BIT3)
        {
        }
        else
        {
            SET(EV_S6);
        }
    } // if ((P2IE & P2IFG & BIT3) != 0)
    if (eventos)
    {
        __low_power_mode_off_on_exit();
    } // if (eventos)
} // __interrupt void RTI_P2(void)

 */
