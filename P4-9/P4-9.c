#include <msp430g2553.h>
#define number 12
void config_prifericos(void);
void initTimer_A(void);
int s1,s2,s3,s4,s11,s22,s33,s44,count;
unsigned int press_botton;
void main()
{
    WDTCTL = WDTPW + WDTHOLD;
    config_prifericos();
    initTimer_A();
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
    //P1DIR |= (BIT4);
//    P2DIR |= (BIT7);//Led 6
//    P2OUT |= BIT7; // turn on led 6 on p2.7

}

void initTimer_A(void)
{
    DCOCTL = 0; // Establece la frecuencia a 1MHz
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    //Timer0_A0 Configuration
    TA0CTL = (TASSEL_2 | ID_3 | MC_0 | TACLR); //Select SMCLK, SMCLK/8, stop
    TA0CCTL0 |= CCIE;
    TA0CCTL0 &= ~(CAP); // modo comparacion
    TA0CCR0 = 31250;
    WDTCTL = WDT_MDLY_32; //opciOn 1
 //   IE1 |= WDTIE;
}

#pragma vector=WDT_VECTOR
__interrupt void RTI_WDT(void)
{
    P1IE |= (BIT4);
     P1IFG &= ~(BIT4);

    P2IE |= (BIT1 | BIT2 | BIT3);
    P2IFG &= ~(BIT1 | BIT2 | BIT3);

    IE1 &= ~(WDTIE);
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A_CCR0_ISR(void)
{
    ++count;
    TA0CCTL0 &= ~(CCIFG);
    if (s11 == 1)
    {
        if (count  <= number)
        {
            if(s1==1){
                TA0CTL |= (MC_0);
                    P2DIR |= (BIT4);
                    P2OUT ^= (BIT4);
                }
                count = 0;
                s1=0;
                s11 = 0;
        }
        else if (count  >= number)
        {
            TA0CTL |= (MC_0);
            P2DIR |= (BIT5 | BIT6 | BIT7);
            P2OUT ^= (BIT5 | BIT6 | BIT7);
            count = 0;
            s1=0;
            s11 = 0;

        }

    }

    if(s2==1){
        TA0CTL |= (MC_0);
        if (count >= number)
        {
            P2DIR |= ( BIT4 | BIT6 | BIT7);
            P2OUT ^= ( BIT4 | BIT6 | BIT7);
        }
        else
        {
            P2DIR |= (BIT5);
            P2OUT ^= (BIT5);
        }
        count = 0;
        s2=0;
    }

    if(s3==1){
        TA0CTL |= (MC_0);
        if (count >= number)
        {
            P2DIR |= ( BIT4 | BIT5 | BIT7);
            P2OUT ^= ( BIT4 | BIT5 | BIT7);
        }
        else
        {
            P2DIR |= (BIT6);
            P2OUT ^= (BIT6);
        }
        count = 0;
        s3=0;
    }
    if(s4==1){
        TA0CTL |= (MC_0);
        if (count >= number)
        {
            P2DIR |= ( BIT4 | BIT6 | BIT5);
            P2OUT ^= ( BIT4 | BIT6 | BIT5);
        }
        else
        {
            P2DIR |= (BIT7);
            P2OUT ^= (BIT7);
        }
        count = 0;
        s4=0;
    }

}



#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if (P1IFG & BIT4)
    {
        if(P1IES & BIT4){
            P1IFG &= ~BIT4;
            P1IE &= ~BIT4;
            P1IES ^= BIT4; // paraleer doos flanco
            TA0CTL |= (TASSEL_2 | ID_3 | MC_1);
            TA0CCR0 = 31250;
            IE1 |= WDTIE;
            s11 = 1;
        }
        else
        {
            P1IFG &= ~BIT4;
            P1IE &= ~BIT4;
            P1IES ^= BIT4; // paraleer doos flanco
            IE1 |= WDTIE;
            s1=1;
        }
    }

}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{

    if (P2IFG & BIT1)
    {
        if(P2IES & BIT1){
            P2IFG &= ~BIT1;
            P2IE &= ~BIT1;
            P2IES ^= BIT1; // paraleer doos flanco
            TA0CTL |= (TASSEL_2 | ID_3 | MC_1);
            TA0CCR0 = 31250;
            IE1 |= WDTIE;
            s22 = 1;
        }
        else
        {
            P2IFG &= ~BIT1;
            P2IE &= ~BIT1;
            P2IES ^= BIT1; // paraleer doos flanco
            IE1 |= WDTIE;
            s2=1;
        }
    }
    if (P2IFG & BIT2)
    {
        if(P2IES & BIT2){
            P2IFG &= ~BIT2;
            P2IE &= ~BIT2;
            P2IES ^= BIT2; // paraleer doos flanco
            TA0CTL |= (TASSEL_2 | ID_3 | MC_1);
            TA0CCR0 = 31250;
            IE1 |= WDTIE;
            s33 = 1;
        }
        else
        {
            P2IFG &= ~BIT2;
            P2IE &= ~BIT2;
            P2IES ^= BIT2; // paraleer doos flanco
            IE1 |= WDTIE;
            s3=1;
        }
    }
    if (P2IFG & BIT3)
    {
        if(P2IES & BIT3){
            P2IFG &= ~BIT3;
            P2IE &= ~BIT3;
            P2IES ^= BIT3; // paraleer doos flanco
            TA0CTL |= (TASSEL_2 | ID_3 | MC_1);
            TA0CCR0 = 31250;
            IE1 |= WDTIE;
            s44 = 1;
        }
        else
        {
            P2IFG &= ~BIT3;
            P2IE &= ~BIT3;
            P2IES ^= BIT3; // paraleer doos flanco
            IE1 |= WDTIE;
            s4=1;
        }
    }
}

