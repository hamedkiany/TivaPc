#include <msp430g2553.h>
volatile unsigned int keyp;
void config_prifericos(void);
void initTimer_AW(void);
void setupled(void);
void main()
{
    WDTCTL = WDTPW + WDTHOLD;
    config_prifericos();
    setupled();
    initTimer_AW();
    __enable_interrupt();
    __low_power_mode_0();
/*
     for (;;)
     {

     }
*/
}

void config_prifericos(void)
{
    P1SEL &= ~(BIT4);
    P1SEL2 &= ~(BIT4);
    P2SEL &= ~( BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT7);
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
    P2IES |= (BIT1 | BIT2 | BIT3);
    P2IFG &= ~(BIT1 | BIT2 | BIT3);

}
void setupled(void)
{
    P2DIR |= (BIT4 | BIT5 | BIT6 | BIT7);
    P2OUT &= ~(BIT4 | BIT5 | BIT6 | BIT7);

}

void initTimer_AW(void)
{
    DCOCTL = 0; // Establece la frecuencia a 1MHz
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    //Timer0_A3 Configuration
    //TA0CTL = (TASSEL_2 | ID_0 | MC_1); //Select SMCLK, SMCLK/1, accendente
    WDTCTL = WDT_MDLY_32; //opcion 1
    //IE1 |= WDTIE;
}

#pragma vector=WDT_VECTOR
__interrupt void RTI_WDT(void)
{
    switch (keyp)
    {
    case (3):
        P1IFG &= ~BIT4;
        P1IE |= BIT4;

        break;
    case (4):
        P2IFG &= ~BIT1;
        P2IE |= BIT1;

        break;
    case (5):
        P2IFG &= ~BIT2;
        P2IE |= BIT2;

        break;
    case (6):
        P2IFG &= ~BIT3;
        P2IE |= BIT3;

        break;

    }

    IE1 &= ~(WDTIE);
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if ((P1IE & P1IFG & BIT4) != 0)
    {
        if (P1IES & BIT4)
        {
            P1IFG &= ~BIT4;
            P1IE &= ~BIT4;
            P1IES ^= BIT4;
            P2DIR |= (BIT4);
            P2OUT ^= (BIT4);
            IE1 |= WDTIE;
            keyp = 3;
        }
        else
        {
            P1IFG &= ~BIT4;
            P1IE &= ~BIT4;
            P1IES ^= BIT4;
            IE1 |= WDTIE;
            keyp = 3;
        }

    }
    //P1IFG &= ~BIT4;

}

//ademas IES

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    if (P2IFG & BIT1)
    {
        if (P2IES & BIT1)
        {
            P2IFG &= ~BIT1;
            P2IE &= ~BIT1;
            P2IES ^= BIT1;
            P2DIR |= (BIT5);
            P2OUT ^= (BIT5);
            IE1 |= WDTIE;
            keyp = 4;
        }
        else
        {
            P2IFG &= ~BIT1;
            P2IE &= ~BIT1;
            P2IES ^= BIT1;
            IE1 |= WDTIE;
            keyp = 4;

        }
    }

    if (P2IFG & BIT2)
    {
        if (P2IES & BIT2)
        {
            P2IFG &= ~BIT2;
            P2IE &= ~BIT2;
            P2IES ^= BIT2;
            P2DIR |= (BIT6);
            P2OUT ^= (BIT6);
            IE1 |= WDTIE;
            keyp = 5;
        }
        else
        {
            P2IFG &= ~BIT2;
            P2IE &= ~BIT2;
            P2IES ^= BIT2;
            IE1 |= WDTIE;
            keyp = 5;

        }
    }

    if (P2IFG & BIT3)
    {
        if (P2IES & BIT3)
        {
            P2IFG &= ~BIT3;
            P2IE &= ~BIT3;
            P2IES ^= BIT3;
            P2DIR |= (BIT7);
            P2OUT ^= (BIT7);
            IE1 |= WDTIE;
            keyp = 6;
        }
        else
        {
            P2IFG &= ~BIT3;
            P2IE &= ~BIT3;
            P2IES ^= BIT3;
            IE1 |= WDTIE;
            keyp = 6;
        }
    }

}

