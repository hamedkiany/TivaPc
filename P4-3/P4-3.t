#include <msp430g2553.h>
void config_prifericos(void);
void initTimer_A(void);
int led;
void main1(){
    WDTCTL = WDTPW + WDTHOLD;
    config_prifericos();
    initTimer_A();
    __enable_interrupt();
for(;;){

}

}

void config_prifericos(void){
    P1SEL &= ~(BIT4);
    P1SEL2 &= ~(BIT4);
    P2SEL &= ~(BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7);
    P2SEL2 &= ~(BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7);
    P1DIR &= ~(BIT4);
    P1REN |= (BIT4);
    P1OUT |= (BIT4);
    P1IE |= (BIT4);
    P1IES &= ~(BIT4);
    P1IFG &= ~(BIT4);
    P2DIR &= ~(BIT1|BIT2|BIT3);
    P2REN |= (BIT1|BIT2|BIT3);
    P2OUT |= (BIT1|BIT2|BIT3);
    P2IE |= (BIT1|BIT2|BIT3);
    P2IES &= ~(BIT1|BIT2|BIT3);
    P2IFG &= ~(BIT1|BIT2|BIT3);

}

void initTimer_A(void)
{
    DCOCTL = 0; // Establece la frecuencia a 1MHz
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    //Timer0_A3 Configuration
    TA0CCR1 = 375 - 1; //Initially, Stop the Timer
    TA0CCTL1 |= CCIE; //Enable interrupt for CCR1.
    TA1CCTL1 &= ~(CAP); // modo comparacion
    TA0CTL = (TASSEL_2 | ID_3 | MC_0); //Select SMCLK, SMCLK/8, Up continue
}


//Timer ISR
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A_CCR1_ISR(void)
{
    switch (led)
    {
    case 3:
        P2OUT &= ~(BIT4);
        TA0CTL = (TASSEL_2 | ID_3 | MC_0);
        TA0CCTL1 &= ~(CCIFG);
        break;
    case 4:
            P2OUT &= ~(BIT5);
            TA0CTL = (TASSEL_2 | ID_3 | MC_0);
            TA0CCTL1 &= ~(CCIFG);
            break;
    case 5:
            P2OUT &= ~(BIT6);
            TA0CTL = (TASSEL_2 | ID_3 | MC_0);
            TA0CCTL1 &= ~(CCIFG);
            break;
    case 6:
            P2OUT &= ~(BIT7);
            TA0CTL = (TASSEL_2 | ID_3 | MC_0);
            TA0CCTL1 &= ~(CCIFG);
            break;

    }

}


#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void){
    if(P1IFG & BIT4){
            P2DIR |= (BIT4);
            P2OUT |= (BIT4);
            led = 3;
            TA0R = 0X0000   ;
            TA0CTL = (TASSEL_2 | ID_3 | MC_2);
    }
          P1IFG &= ~BIT4;

}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void){
    if(P2IFG & BIT1){
        P2DIR |= (BIT5);
        P2OUT |= (BIT5);
        led = 4;
        TA0R = 0X0000;
        TA0CTL = (TASSEL_2 | ID_3 | MC_2);
    }
      P2IFG &= ~BIT1;
      if(P2IFG & BIT2){
          P2DIR |= (BIT6);
          P2OUT |= (BIT6);
          led = 5;
          TA0R = 0X0000;
          TA0CTL = (TASSEL_2 | ID_3 | MC_2);
      }
        P2IFG &= ~BIT2;
        if(P2IFG & BIT3){
            P2DIR |= (BIT7);
            P2OUT |= (BIT7);
            led = 6;
            TA0R = 0X0000;
            TA0CTL = (TASSEL_2 | ID_3 | MC_2);

        }
          P2IFG &= ~BIT3;

}
