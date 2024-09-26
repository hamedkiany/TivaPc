#include <msp430g2553.h>
void config_prifericos(void);
void initTimer_A(void);
int led, count;
void main() //main function
{
    WDTCTL = WDTPW + WDTHOLD;
    config_prifericos(); //config ports
    initTimer_A(); //config timer
    __enable_interrupt();
    for (;;) //loop
    {

    }

}
//port config function
void config_prifericos(void)
{
    P1SEL &= ~(BIT4);
    P1SEL2 &= ~(BIT4);
    P2SEL &= ~(BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P2SEL2 &= ~(BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
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

}
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
