/********************************
 * Hamed kiany Nejad
 *
 *
 *
 */


#include "msp430g2553.h"

#define Estado_A 0 // A
#define Estado_B 1 // B
#define Estado_C 2 // C

#define EV_TIMER BIT0
#define EV_S5_DOWN BIT1
#define EV_S4_UP BIT2
#define EV_S4_DOWN BIT3

#define SET(ev) (eventos |= (ev))
#define RESET(ev) (eventos &= ~(ev))
#define TEST(ev) (eventos & (ev))
void config_prifericos(void);
void setupled(void);
void initTimer_A(void);
unsigned char eventos;

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;
    unsigned char estado;
    config_prifericos();
    setupled();
    initTimer_A();
    eventos = 0;
    estado = Estado_A;

    while (1) {
        __low_power_mode_0();

        while (eventos) {

            switch (estado) {

                case (Estado_A): {

                    if (TEST(EV_S5_DOWN)) {
                        P2OUT |= (BIT4 | BIT5 | BIT6 | BIT7);
                        TA1CTL |= MC_2;
                        estado = Estado_B;
                        RESET(EV_S5_DOWN);
                    } // if(TEST( EV_S5_DOWN))

                    if (TEST(EV_S4_UP)) {
                        RESET(EV_S4_UP);
                    } // if(TEST( EV_S4_UP))

                    if (TEST(EV_TIMER)) {
                        RESET(EV_TIMER);
                    } // if(TEST( EV_TIMER))

                    break;
                } // case (ST_A):

                case (Estado_B):{

                    if (TEST(EV_S5_DOWN)) {
                        RESET(EV_S5_DOWN);
                    } // if(TEST( EV_S5_DOWN))

                    if (TEST(EV_S4_UP)) {
                        P2OUT &= ~(BIT4 | BIT5 | BIT6 | BIT7);
                        TA1CTL &= ~(MC_0 | MC_2);
                        estado = Estado_A;
                        RESET(EV_S4_UP);
                    } // if(TEST( EV_S4_UP))

                    if (TEST(EV_TIMER)) {
                        TA1CTL &= ~(MC_0 | MC_2);
                        estado = Estado_C;
                        RESET(EV_TIMER);
                    } // if(TEST( EV_TIMER))

                    break;
                } // case (ST_B):

                case (Estado_C): {

                    if (TEST(EV_S5_DOWN)) {
                        RESET(EV_S5_DOWN);
                    } // if(TEST( EV_S5_DOWN))

                    if (TEST(EV_S4_UP)) {
                        P2OUT &= ~(BIT4 | BIT5 | BIT6 | BIT7);
                        estado = Estado_A;
                        RESET(EV_S4_UP);
                    } // if(TEST( EV_S4_UP))

                    if (TEST(EV_TIMER)) {
                        RESET(EV_TIMER);
                    } // if(TEST( EV_TIMER))

                    break;
                } // case (ST_C):

            } // switch(estado)

        } // while (eventos)

    } // while(1)

} // void main(void)

void config_prifericos(void)
{
    P1SEL &= ~(BIT4);
    P1SEL2 &= ~(BIT4);
    P2SEL &= ~(BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT7);
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

void initTimer_A(void)
{
    DCOCTL = 0; // Establece la frecuencia a 1MHz
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    //Timer0_A3 Configuration
    TA1CTL = (TASSEL_2 | ID_2 | MC_0); //Select SMCLK, SMCLK/1, continue
    //TA1CCTL0 &= ~(CCIFG);
    TA1CCTL0 |= (OUTMOD_4); //Enable interrupt for CCR0,PWM set/reset
    TA1CCTL0 &= ~(CAP); // modo comparacion
    //TA1CCTL1 |= CCIE; //Enable interrupt for CCR1.
    TA1CCTL1 &= ~(CAP); // modo comparacion
    WDTCTL = WDT_MDLY_32; // opciOn 1

}

#pragma vector = WDT_VECTOR
__interrupt void RTI_WDT(void)
{
    IFG1 &= ~(WDTIFG);
    IE1 &= ~(WDTIE);
    P1IFG &= ~(BIT4);
    P1IE |= (BIT4);
    P2IFG &= ~(BIT1 | BIT2 | BIT3);
    P2IE |= (BIT1 | BIT2 | BIT3);
}



#pragma vector = PORT2_VECTOR
__interrupt void RTI_P2(void)
{
    if ((P2IE & P2IFG & BIT1) != 0)
    {
        P2IFG &= ~(BIT1);
        P2IE &= ~(BIT1);
        P2IES ^= (BIT1);
        IE1 |= (WDTIE);
        if (P2IES & BIT1)
        {
            SET(EV_S4_UP);

        }
        else
        {
        }
    } // if ((P2IE & P2IFG & BIT1) != 0)
    if ((P2IE & P2IFG & BIT2) != 0)
    {
        P2IFG &= ~(BIT2);
        P2IE &= ~(BIT2);
        P2IES ^= (BIT2);
        IE1 |= (WDTIE);
        if (P2IES & BIT2)
        {
        }
        else
        {
            SET(EV_S5_DOWN);

        }
    } // if ((P2IE & P2IFG & BIT2) != 0)
    if (eventos)
    {
        __low_power_mode_off_on_exit();
    } // if (eventos)
} // __interrupt void RTI_P2(void)
