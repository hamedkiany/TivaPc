#include <msp430g2553.h>
unsigned int s3_ant, s4_ant;
void main()
{
    WDTCTL = WDTPW + WDTHOLD;
    P1SEL &= ~(BIT4);
    P1SEL2 &= ~(BIT4);
    P2SEL &= ~(BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P2SEL2 &= ~(BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
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
    __enable_interrupt();
    for (;;)
    {

    }

}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if (P1IFG & BIT4)
    {

        if (s4_ant == 1)
        {
            if ((P2IN & BIT1) == 0)
            {
                P2DIR |= (BIT7 | BIT6 | BIT5 | BIT4);
                P2OUT |= (BIT7 | BIT6 | BIT5 | BIT4);

            }
            else
                    {

                        P2DIR |= (BIT4);
                        P2OUT ^= (BIT4);
                        s4_ant = 0;
                        s3_ant = 1;
                    }

        }
        else
        {

            P2DIR |= (BIT4);
            P2OUT ^= (BIT4);
            s4_ant = 0;
            s3_ant = 1;
        }    }
    P1IFG &= ~BIT4;

}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    if (P2IFG & BIT1)
    {
        if (s3_ant == 1)
         {
             if ((P1IN & BIT4) == 0)
             {
                 P2DIR |= (BIT7 | BIT6 | BIT5 | BIT4);
                 P2OUT |= (BIT7 | BIT6 | BIT5 | BIT4);

             }
             else
                     {

                         P2DIR |= (BIT5);
                         P2OUT ^= (BIT5);
                         s3_ant = 0;
                         s4_ant = 1;
                     }

         }
         else
         {

             P2DIR |= (BIT5);
             P2OUT ^= (BIT5);
             s3_ant = 0;
             s4_ant = 1;
         }
      }
    P2IFG &= ~BIT1;
    if (P2IFG & BIT2)
    {

        P2DIR |= (BIT6);
        P2OUT ^= (BIT6);

    }

    P2IFG &= ~BIT2;
    if (P2IFG & BIT3)
    {

        P2DIR |= (BIT7);
        P2OUT ^= (BIT7);
    }
    P2IFG &= ~BIT3;

}
