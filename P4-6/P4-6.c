#include <msp430g2553.h>
void config_prifericos(void);
void initTimer_A(void);
int led, count1, count2, count3, count4;
int buzzer_on = 0;
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

void initTimer_A(void)
{
    DCOCTL = 0; // Establece la frecuencia a 1MHz
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    //Timer0_A3 Configuration
    TA1CTL = (TASSEL_2 | ID_0 | MC_0); //Select SMCLK, SMCLK/1, accendente
    //TA1CCTL0 &= ~(CCIFG);
    TA1CCTL0 |= (OUTMOD_4); //Enable interrupt for CCR0,PWM set/reset
    TA1CCTL0 &= ~(CAP); // modo comparacion
    //TA1CCTL1 |= CCIE; //Enable interrupt for CCR1.
    TA1CCTL1 &= ~(CAP); // modo comparacion
    P2DIR |= BIT0;

}


#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if ((P1IE & P1IFG & BIT4)!= 0)
    {
        if (buzzer_on) {
            TA1CCR0 = 0; // Desactiva el zumbador
            buzzer_on = 0; // Actualiza el estado
        } else {
            TA1CCR0 = 2272; //2272 Configura la frecuencia del zumbador a 440Hz
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
    if (( P2IE & P2IFG & BIT1)!= 0)
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
    if (( P2IE & P2IFG & BIT2)!= 0)
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
    if (( P2IE &P2IFG & BIT3)!= 0)
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

