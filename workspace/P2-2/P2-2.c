/***********************************
 *          Microcontroladores
 *      Práctica 2: Puertos E/S
 *      p2.2
 *      Hamed Kiany Nejad
 *      18/02/2023
 ***********************************/


#include <msp430g2553.h>

void main1(){
    WDTCTL = (WDTPW|WDTHOLD); //disable watch dog
    P1SEL &= ~(BIT4); //select p1.4
    P1SEL2 &= ~(BIT4);//2select p1.4
    P1DIR &= ~(BIT4);//set p1.4 as input
    P2SEL &= ~(BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7); //select p2.1-2-3-4-5-6-7
    P2SEL2 &= ~(BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7);//2select p2.1-2-3-4-5-6-7
    P1REN |= (BIT4); //enable pullup on p1.4
    P1OUT |= (BIT4); //activate pullup on p1.4
    P2REN |= (BIT1|BIT2|BIT3); //enable pullup on p2.1-2-3
    P2OUT |= (BIT1|BIT2|BIT3); //activate pullup on p2.1-2-3
    P1IFG &= ~(BIT4); // enable flag on p1.4
    P2IFG &= ~(BIT1|BIT2|BIT3); // enable flag on p2.1-2-3
    for(;;){ // infinite loop
        if((P1IFG & BIT4)){ //control flag from  S3
            P2DIR |= BIT4; // set p2.4 as output
            P2OUT ^= BIT4; // turn on led 4 on p2.4
            P1IFG &= ~BIT4; // reset flag
        }
        if((P2IFG & BIT1)){ //control flag from  S3
            P2DIR |= BIT5; // set p2.5 as output
            P2OUT ^= BIT5; // turn on led 5 on p2.5
            P2IFG &= ~BIT1;// reset flag
        }
        if((P2IFG & BIT2)){ //control flag from  S3
            P2DIR |= BIT6; // set p2.6 as output
            P2OUT ^= BIT6; // turn on led 6 on p2.6
            P2IFG &= ~BIT2;// reset flag
        }
        if((P2IFG & BIT3)){ //control flag from  S3
            P2DIR |= BIT7; // set p2.7 as output
            P2OUT ^= BIT7; // turn on led7 on p2.7
            P2IFG &= ~BIT3;// reset flag
        }
    } // end for

} // end of main
