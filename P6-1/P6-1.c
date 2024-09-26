#include "msp430g2553.h"

#define ST_A 0 // Todos los LED apagados
#define ST_B 1 // LED 5 y 3 encendidos
#define ST_C 2 // LED 6 y 4 encendidos


#define EV_S3 BIT0
#define EV_S4 BIT1
#define EV_S5 BIT2
#define EV_S6 BIT3

#define SET(ev) (eventos |= (ev))
#define RESET(ev)   (eventos &= ~(ev))
#define TEST(ev)    (eventos & (ev))
void config_prifericos(void);
void setupled(void);
unsigned char eventos;

void main(void) {
    unsigned char estado;
    WDTCTL = WDTPW|WDTHOLD;
    DCOCTL = 0; // Establece la frecuencia a 1MHz
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    WDTCTL = WDT_MDLY_32; //opciOn 1
    config_prifericos();
    setupled();
    eventos=0;
    estado=ST_A;

    while(1)
    {
        __low_power_mode_0();

        while (eventos)
        {
            switch(estado)
            {
            case (ST_A):
            {
                if(TEST( EV_S3))
                {
                    P2OUT |= (BIT4|BIT6);
                    estado=ST_B;
                    RESET( EV_S3);
                }//if(TEST( EV_S3))

                if(TEST( EV_S4))
                {
                    RESET( EV_S4);
                }//if(TEST( EV_S4))

                if(TEST( EV_S5))
                {
                     RESET( EV_S5);
                }//if(TEST( EV_S5))

                if(TEST( EV_S6))
                {
                    RESET( EV_S6);
                }//if(TEST( EV_S6))
                break;
            }// case (ST_A):
            case (ST_B):
             {
                if(TEST( EV_S3))
                {
                    RESET( EV_S3);
                }//if(TEST( EV_S3))

                if(TEST( EV_S4))
                {
                    P2OUT |= (BIT5|BIT7);
                    estado=ST_C;
                    RESET( EV_S4);
                }//if(TEST( EV_S4))

                if(TEST( EV_S5))
                {
                    RESET( EV_S5);
                }//if(TEST( EV_S5))

                if(TEST(EV_S6))
                {
                    P2OUT &= ~(BIT4 | BIT5 | BIT6 | BIT7);
                    estado=ST_A;
                    RESET( EV_S6);
                }//if(TEST( EV_S6))
                break;
             }// case (ST_B):
            case (ST_C):
             {
                if(TEST( EV_S3))
                {
                    RESET( EV_S3);
                }//if(TEST( EV_S3))

                if(TEST( EV_S4))
                {
                    RESET( EV_S4);
                }//if(TEST( EV_S4))

                if(TEST( EV_S5))
                {
                    P2OUT |= (BIT4|BIT6);
                    estado=ST_B;

                    RESET( EV_S5);
                }//if(TEST( EV_S5))

                if(TEST(EV_S6))
                {
                    RESET( EV_S6);
                }//if(TEST( EV_S6))
                break;
             }// case (ST_C):

            }// switch(estado)

        }//while (eventos)
    }//while(1)
} // void main(void)


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
    P2IES &= ~(BIT1 | BIT3); // 4 y 6 flango subido
    P2IES |= (BIT2);
    P2IFG &= ~(BIT1 | BIT2 | BIT3);

}

void setupled(void)
{
    P2DIR |= (BIT4 | BIT5 | BIT6 | BIT7);
    P2OUT &= ~(BIT4 | BIT5 | BIT6 | BIT7);

}


#pragma vector=WDT_VECTOR
__interrupt void RTI_WDT(void)
{
    IFG1 &= ~(WDTIFG);
    IE1 &= ~(WDTIE);
    P1IFG &= ~(BIT4);
    P1IE |= (BIT4);
    P2IFG &= ~(BIT1 | BIT2 | BIT3);
    P2IE |= (BIT1 | BIT2 | BIT3);
}

#pragma vector = PORT1_VECTOR
__interrupt void RTI_P1(void) {

    if ((P1IE & P1IFG & BIT4) != 0) {
        if(P1IES & BIT4){
            P1IFG &= ~(BIT4);
            P1IE &= ~(BIT4);
            P1IES ^= (BIT4);
            SET( EV_S3);
            IE1 |= (WDTIE);
        }
        else
        {
            P1IFG &= ~(BIT4);
            P1IE &= ~(BIT4);
            P1IES ^= (BIT4);
            IE1 |= (WDTIE);
        }
    } // if ((P1IE & P1IFG & BIT4) != 0)
    if (eventos) {
        __low_power_mode_off_on_exit();
    } // if (eventos)

} // __interrupt void RTI_P1(void)

#pragma vector = PORT2_VECTOR
__interrupt void RTI_P2(void) {

    if ((P2IE & P2IFG & BIT1) != 0) {
        if(P2IES & BIT1){

            P2IFG &= ~(BIT1);
            P2IE &= ~(BIT1);
            P2IES ^= (BIT1);
            IE1 |= (WDTIE);

        }
        else
        {
            P2IFG &= ~(BIT1);
            P2IE &= ~(BIT1);
            P2IES ^= (BIT1);
            SET( EV_S4);
            IE1 |= (WDTIE);
        }

    } // if ((P2IE & P2IFG & BIT1) != 0)

    if ((P2IE & P2IFG & BIT2) != 0) {
        if(P2IES & BIT2){
            P2IFG &= ~(BIT2);
            P2IE &= ~(BIT2);
            P2IES ^= (BIT2);
            SET( EV_S5);
            IE1 |= (WDTIE);
        }
        else
        {
            P2IFG &= ~(BIT2);
            P2IE &= ~(BIT2);
            P2IES ^= (BIT2);
            IE1 |= (WDTIE);
        }

    } // if ((P2IE & P2IFG & BIT2) != 0)

    if ((P2IE & P2IFG & BIT3) != 0) {
        if(P2IES & BIT3){

            P2IFG &= ~(BIT3);
            P2IE &= ~(BIT3);
            P2IES ^= (BIT3);
            IE1 |= (WDTIE);
        }
        else
        {
            P2IFG &= ~(BIT3);
            P2IE &= ~(BIT3);
            P2IES ^= (BIT3);
            SET( EV_S6);
            IE1 |= (WDTIE);
        }

    } // if ((P2IE & P2IFG & BIT3) != 0)

    if (eventos) {
        __low_power_mode_off_on_exit();
    } // if (eventos)

} // __interrupt void RTI_P2(void)
