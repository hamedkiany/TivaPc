#include <msp430g2553.h>
void config_prifericos(void);
void initTimer_A(void);
int led, count1, count2, count3, count4;
const unsigned int TARADD1 = 62500; //32770
const unsigned int TARADD2 = 50000; //25000
const unsigned int TARADD3 = 41667; //20845
const unsigned int TARADD4 = 34714; //17870
void main()
{
    WDTCTL = WDTPW + WDTHOLD;
    config_prifericos();
    initTimer_A();
    __enable_interrupt();
    for (;;)
    {

    }

}

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
    P1IES |=(BIT4);
    P1IFG &= ~(BIT4);
    P2DIR &= ~(BIT1 | BIT2 | BIT3);
    P2REN |= (BIT1 | BIT2 | BIT3);
    P2OUT |= (BIT1 | BIT2 | BIT3);
    P2IE |= (BIT1 | BIT2 | BIT3);
    P2IES |= (BIT1 | BIT2 | BIT3);
    P2IFG &= ~(BIT1 | BIT2 | BIT3);

}

void initTimer_A(void)
{
    DCOCTL = 0; // Establece la frecuencia a 1MHz
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    //Timer0_A3 Configuration
    TA0CTL = (TASSEL_2 | ID_3 | MC_2); //Select SMCLK, SMCLK/8, Up continue
    //TA0CCTL0 &= ~(CCIFG);
    TA0CCTL0 |= CCIE; //Enable interrupt for CCR0.
    TA0CCTL0 &= ~(CAP); // modo comparacion
    //TA0CCTL1 &= ~(CCIFG);
    TA0CCTL1 |= CCIE; //Enable interrupt for CCR1.
    TA0CCTL1 &= ~(CAP); // modo comparacion

    //Timer1_A3 Configuration
    TA1CTL = (TASSEL_2 | ID_3 | MC_2); //Select SMCLK, SMCLK/8, Up continue
    //TA1CCTL0 &= ~(CCIFG);
    TA1CCTL1 |= CCIE; //Enable interrupt for CCR1.
    TA1CCTL1 &= ~(CAP); // modo comparacion
    TA1CCTL2 |= CCIE; //Enable interrupt for CCR1.
    TA1CCTL2 &= ~(CAP); // modo comparacion

}

//Timer ISR

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A_CCR0_ISR(void)
{

        ++count1;
        TA0CCR0 = TAR + 32770;
        TA0CCTL0 &= ~(CCIFG);
        if (count1 == 2)
        {
            count1 = 0;
            P2OUT ^= (BIT4);
        }

}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A_CCR1_ISR(void)
{

    switch (TA0IV)
    {
    case (TA0IV_TACCR1): //TA0IV_TACCR1
    {
        ++count2;
        TA0CCR1 = TAR + TARADD2;
        if (count2 == 2)
        {
            count2 = 0;
            P2OUT ^= (BIT5);
        }
        break;

    }
    }

}

#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer_A1_CCR1_ISR(void)
{

    switch (TA1IV)
    {
    case (TA1IV_TACCR1):
    {
        ++count3;
        TA1CCR1 = TAR + TARADD3;
        if (count3 == 2)
        {
            count3 = 0;
            P2OUT ^= (BIT6);
        }
        break;

    }
    case (TA1IV_TACCR2):
    {
        ++count4;
        TA1CCR2 = TAR + TARADD4;
        if (count4 == 2)
        {
            count4 = 0;
            P2OUT ^= (BIT7);
        }
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
        TA0CCR0 = TAR + TARADD1;
        //TA0CTL |= (MC_2);
        count1 = 0;
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
        TA0CCR1 = TAR + TARADD2;
        //TA0CTL |= (MC_2);
        count2 = 0;
    }
    P2IFG &= ~BIT1;
    if (P2IFG & BIT2)
    {
        P2DIR |= (BIT6);
        P2OUT |= (BIT6);
        led = 5;
        TA1CCR1 = TAR + TARADD3;
        //TA1CTL |= (MC_2);
        count3 = 0;
    }
    P2IFG &= ~BIT2;
    if (P2IFG & BIT3)
    {
        P2DIR |= (BIT7);
        P2OUT |= (BIT7);
        led = 6;
        TA1CCR2 = TAR + TARADD4;
        //TA1CTL |= (MC_2);
        count4 = 0;

    }
    P2IFG &= ~BIT3;

}

