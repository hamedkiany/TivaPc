

#include "msp430g2553.h"
int eventos = 1;
void main2(){

    WDTCTL = WDTPW + WDTHOLD;
    TA0CTL |= TASSEL0 + TACLR;
    P2SEL &= (~BIT1);
    P2SEL2 &= (~BIT1);
    P2DIR &= ~(BIT1);
    P2REN |= (BIT1);
    P2OUT |= (BIT1);
    P2IE |= (BIT1);
    P2IES |= (BIT1);
    P2IFG &= ~(BIT1);

    __enable_interrupt();
 //   unsigned int i,j;
    for(;;){
        while(eventos == 1){

        P2DIR |= (BIT4);
        P2OUT |= (BIT4);

        /*
        i = 64000000;
        do{
            i--;
        }while(i<1);
        P2OUT &= ~(BIT5);
        P2OUT &= ~(BIT4);
        j = 64000000;
        do{
            j--;
        }while(j<1);
        P2DIR |= (BIT4);
        P2OUT |= (BIT4)
*/
        }
        P2OUT &= ~(BIT4);
        //P2OUT &= ~(BIT5);

    }



}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    if(P2IFG & BIT1)
       eventos = ~eventos;
    TA0CCTL0 &= ~(CCIE);
    P2IFG &= ~BIT1;
}

