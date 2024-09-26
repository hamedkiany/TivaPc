/***********************************
 *          Microcontroladores
 *      Práctica 2: Puertos E/S
 *      p2.2
 *      Hamed Kiany Nejad
 *      21/02/2023
 ***********************************/

#include <msp430g2553.h>
#include <stdbool.h>

void main()
{
    WDTCTL = (WDTPW | WDTHOLD); //disable watch dog
    P1SEL &= ~(BIT4); //select p1.4
    P1SEL2 &= ~(BIT4); //2select p1.4
    P1DIR &= ~(BIT4); //set p1.4 as input
    P2SEL &= ~(BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7); //select p2.1-2-3-4-5-6-7
    P2SEL2 &= ~(BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7); //2select p2.1-2-3-4-5-6-7
//    P2DIR |= (BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7); // set p2 as output
    P1REN |= (BIT4); //enable pullup on p1.4
    P1OUT |= (BIT4); //activate pullup on p1.4
    P2REN |= (BIT1 | BIT2 | BIT3); //enable pullup on p2.1-2-3
    P2OUT |= (BIT1 | BIT2 | BIT3); //activate pullup on p2.1-2-3

    bool firsttime1 = true;
    bool firsttime2 = true;
    bool firsttime3 = true;
    bool firsttime4 = true;

    for (;;)
    { // infinite loop
        if ((P1IN & BIT4) == 0) //control  S3
        {
            if ((firsttime1 == true)) // if is for first time
            {
                P2DIR |= BIT4; // set p2.4 as output
                P2OUT ^= BIT4; // turn on led 4 on p2.4
            }
            firsttime1 = false; //change value of bottom´s control
        }
        else
        {
            firsttime1 = true; //reset to default
        }
        if ((P2IN & BIT1) == 0) //control  S4
        {
            if ((firsttime2 == true)) // if is for first time
            {
                P2DIR |= BIT5; // set p2.5 as output
                P2OUT ^= BIT5; // turn on led 5 on p2.5
            }
            firsttime2 = false;  //change value of bottom´s control
        }
        else
        {
            firsttime2 = true; //reset to default
        }

        if ((P2IN & BIT2) == 0) //control   S5
        {
            if ((firsttime3 == true))
            {
                P2DIR |= BIT6; // set p2.6 as output
                P2OUT ^= BIT6; // turn on led 6 on p2.6
            }
            firsttime3 = false;  //change value of bottom´s control
        }
        else
        {
            firsttime3 = true; //reset to default
        }

        if ((P2IN & BIT3) == 0) //control   S6
        {
            if ((firsttime4 == true))
            {
                P2DIR |= BIT7; // set p2.7 as output
                P2OUT ^= BIT7; // turn on led7 on p2.7
            }
            firsttime4 = false; //change value of bottom´s control
        }
        else
        {
            firsttime4 = true; //reset to default

        }
    } // end for

} // end of main
