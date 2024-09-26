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
#define Estado_D 3 // D
#define Estado_E 4 // E
#define Estado_F 5 // F
#define Estado_G 6 // G

#define EV_TIMER BIT0
#define EV_S3_UP BIT1
#define EV_S3_DOWN BIT2
#define EV_S4_UP BIT3
#define EV_S4_DOWN BIT4
#define EV_S5_UP BIT5
#define EV_S5_DOWN BIT6
#define EV_S6_UP BIT7
#define EV_S6_DOWN BIT8
#define EV_TIMER2 BIT9

#define SET(ev) (eventos |= (ev))
#define RESET(ev) (eventos &= ~(ev))
#define TEST(ev) (eventos & (ev))
void config_prifericos(void);
void setupled(void);
void initTimer_A(void);
unsigned char eventos;
volatile unsigned int tcounter,counter6 ;
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
                    if (TEST(EV_S3_UP)) {
                        RESET(EV_S3_UP);
                    } // if(TEST( EV_S3_UP))

                    if (TEST(EV_S3_DOWN)) {
                        RESET(EV_S3_DOWN);
                    } // if(TEST( EV_S3_DOWN))

                    if (TEST(EV_S4_UP)) {
                        RESET(EV_S4_UP);
                    } // if(TEST( EV_S4_UP))

                    if (TEST(EV_S4_DOWN)) {
                        RESET(EV_S4_DOWN);
                    } // if(TEST( EV_S4_DOWN))

                    if (TEST(EV_S5_UP)) {
                        RESET(EV_S5_UP);
                    } // if(TEST( EV_S5_UP))

                    if (TEST(EV_S5_DOWN)) {
                        RESET(EV_S5_DOWN);
                    } // if(TEST( EV_S5_DOWN))

                    if (TEST(EV_S6_UP)) {
                        RESET(EV_S6_UP);
                    } // if(TEST( EV_S6_UP))

                    if (TEST(EV_S6_DOWN)) {
                        RESET(EV_S6_DOWN);
                    } // if(TEST( EV_S6_DOWN))

                    if (TEST(EV_TIMER)) {
                        RESET(EV_TIMER);
                    } // if(TEST( EV_TIMER))

                    if (TEST(EV_TIMER2)) {
                        RESET(EV_TIMER2);
                    } // if(TEST( EV_TIMER2))

                    break;
                } // case (ST_A):

                case (Estado_B):{

                    if (TEST(EV_S3_UP)) {
                        RESET(EV_S3_UP);
                    } // if(TEST( EV_S3_UP))

                    if (TEST(EV_S3_DOWN)) {
                        RESET(EV_S3_DOWN);
                    } // if(TEST( EV_S3_DOWN))

                    if (TEST(EV_S4_UP)) {
                        RESET(EV_S4_UP);
                    } // if(TEST( EV_S4_UP))

                    if (TEST(EV_S4_DOWN)) {
                        RESET(EV_S4_DOWN);
                    } // if(TEST( EV_S4_DOWN))

                    if (TEST(EV_S5_UP)) {
                        RESET(EV_S5_UP);
                    } // if(TEST( EV_S5_UP))

                    if (TEST(EV_S5_DOWN)) {
                        RESET(EV_S5_DOWN);
                    } // if(TEST( EV_S5_DOWN))

                    if (TEST(EV_S6_UP)) {
                        RESET(EV_S6_UP);
                    } // if(TEST( EV_S6_UP))

                    if (TEST(EV_S6_DOWN)) {
                        RESET(EV_S6_DOWN);
                    } // if(TEST( EV_S6_DOWN))

                    if (TEST(EV_TIMER)) {
                        RESET(EV_TIMER);
                    } // if(TEST( EV_TIMER))


                    if (TEST(EV_TIMER2)) {
                        RESET(EV_TIMER2);
                    } // if(TEST( EV_TIMER2))

                    break;
                } // case (ST_C):

                case (Estado_D):{
                    if (TEST(EV_S3_UP)) {
                        RESET(EV_S3_UP);
                    } // if(TEST( EV_S3_UP))

                    if (TEST(EV_S3_DOWN)) {
                        RESET(EV_S3_DOWN);
                    } // if(TEST( EV_S3_DOWN))

                    if (TEST(EV_S4_UP)) {
                        RESET(EV_S4_UP);
                    } // if(TEST( EV_S4_UP))

                    if (TEST(EV_S4_DOWN)) {
                        RESET(EV_S4_DOWN);
                    } // if(TEST( EV_S4_DOWN))

                    if (TEST(EV_S5_UP)) {
                        RESET(EV_S5_UP);
                    } // if(TEST( EV_S5_UP))

                    if (TEST(EV_S5_DOWN)) {
                        RESET(EV_S5_DOWN);
                    } // if(TEST( EV_S5_DOWN))

                    if (TEST(EV_S6_UP)) {
                        RESET(EV_S6_UP);
                    } // if(TEST( EV_S6_UP))

                    if (TEST(EV_S6_DOWN)) {
                        RESET(EV_S6_DOWN);
                    } // if(TEST( EV_S6_DOWN))

                    if (TEST(EV_TIMER)) {
                        RESET(EV_TIMER);
                    } // if(TEST( EV_TIMER))


                    if (TEST(EV_TIMER2)) {
                        RESET(EV_TIMER2);
                    } // if(TEST( EV_TIMER2))

                    break;
                } // case (ST_D):


                case (Estado_E): {
                    if (TEST(EV_S3_UP)) {
                        RESET(EV_S3_UP);
                    } // if(TEST( EV_S3_UP))

                    if (TEST(EV_S3_DOWN)) {
                        RESET(EV_S3_DOWN);
                    } // if(TEST( EV_S3_DOWN))

                    if (TEST(EV_S4_UP)) {
                        RESET(EV_S4_UP);
                    } // if(TEST( EV_S4_UP))

                    if (TEST(EV_S4_DOWN)) {
                        RESET(EV_S4_DOWN);
                    } // if(TEST( EV_S4_DOWN))

                    if (TEST(EV_S5_UP)) {
                        RESET(EV_S5_UP);
                    } // if(TEST( EV_S5_UP))

                    if (TEST(EV_S5_DOWN)) {
                        RESET(EV_S5_DOWN);
                    } // if(TEST( EV_S5_DOWN))

                    if (TEST(EV_S6_UP)) {
                        RESET(EV_S6_UP);
                    } // if(TEST( EV_S6_UP))

                    if (TEST(EV_S6_DOWN)) {
                        RESET(EV_S6_DOWN);
                    } // if(TEST( EV_S6_DOWN))

                    if (TEST(EV_TIMER)) {
                        RESET(EV_TIMER);
                    } // if(TEST( EV_TIMER))


                    if (TEST(EV_TIMER2)) {
                        RESET(EV_TIMER2);
                    } // if(TEST( EV_TIMER2))

                    break;
                } // case (ST_E):

                case (Estado_F):{

                    if (TEST(EV_S3_UP)) {
                        RESET(EV_S3_UP);
                    } // if(TEST( EV_S3_UP))

                    if (TEST(EV_S3_DOWN)) {
                        RESET(EV_S3_DOWN);
                    } // if(TEST( EV_S3_DOWN))

                    if (TEST(EV_S4_UP)) {
                        RESET(EV_S4_UP);
                    } // if(TEST( EV_S4_UP))

                    if (TEST(EV_S4_DOWN)) {
                        RESET(EV_S4_DOWN);
                    } // if(TEST( EV_S4_DOWN))

                    if (TEST(EV_S5_UP)) {
                        RESET(EV_S5_UP);
                    } // if(TEST( EV_S5_UP))

                    if (TEST(EV_S5_DOWN)) {
                        RESET(EV_S5_DOWN);
                    } // if(TEST( EV_S5_DOWN))

                    if (TEST(EV_S6_UP)) {
                        RESET(EV_S6_UP);
                    } // if(TEST( EV_S6_UP))

                    if (TEST(EV_S6_DOWN)) {
                        RESET(EV_S6_DOWN);
                    } // if(TEST( EV_S6_DOWN))

                    if (TEST(EV_TIMER)) {
                        RESET(EV_TIMER);
                    } // if(TEST( EV_TIMER))


                    if (TEST(EV_TIMER2)) {
                        RESET(EV_TIMER2);
                    } // if(TEST( EV_TIMER2))

                    break;
                } // case (ST_F):
                case (Estado_G): {
                    if (TEST(EV_S3_UP)) {
                        RESET(EV_S3_UP);
                    } // if(TEST( EV_S3_UP))

                    if (TEST(EV_S3_DOWN)) {
                        RESET(EV_S3_DOWN);
                    } // if(TEST( EV_S3_DOWN))

                    if (TEST(EV_S4_UP)) {
                        RESET(EV_S4_UP);
                    } // if(TEST( EV_S4_UP))

                    if (TEST(EV_S4_DOWN)) {
                        RESET(EV_S4_DOWN);
                    } // if(TEST( EV_S4_DOWN))

                    if (TEST(EV_S5_UP)) {
                        RESET(EV_S5_UP);
                    } // if(TEST( EV_S5_UP))

                    if (TEST(EV_S5_DOWN)) {
                        RESET(EV_S5_DOWN);
                    } // if(TEST( EV_S5_DOWN))

                    if (TEST(EV_S6_UP)) {
                        RESET(EV_S6_UP);
                    } // if(TEST( EV_S6_UP))

                    if (TEST(EV_S6_DOWN)) {
                        RESET(EV_S6_DOWN);
                    } // if(TEST( EV_S6_DOWN))

                    if (TEST(EV_TIMER)) {
                        RESET(EV_TIMER);
                    } // if(TEST( EV_TIMER))

                    if (TEST(EV_TIMER2)) {
                        RESET(EV_TIMER2);
                    } // if(TEST( EV_TIMER2))

                    break;
                } // case (ST_G):


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
    P2IES |= (BIT1 | BIT2 | BIT3);
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

    BCSCTL2 |= (DIVS_3); // Select SMLCK por 8 y  MCLK source

    //Timer0_A3 Configuration
    TA1CTL = (TASSEL_2 | ID_2 | MC_2 | TACLR); //Select SMCLK, SMCLK/4, MODE continue , clear  TA1 Timer 31250 hz
    TA0CTL = (TASSEL_2 | ID_2 | MC_0 ); //Select SMCLK, SMCLK/4 , clear  TA1 Timer /TA0 Timer Timer LED5 (p2.6( TA0.1

    TA1CCTL0 &= ~(CCIFG);
    //TA1CCTL0 |= (OUTMOD_4); //Enable interrupt for CCR0,PWM set/reset
    //TA1CCTL0 &= ~(CAP); // modo comparacion
    //TA1CCTL0 |= CCIE; //Enable interrupt for CCR1.
    //TA1CCTL1 &= ~(CAP); // modo comparacion

    WDTCTL = WDT_MDLY_8; // Wathdog timer

}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void RTI_TA1CCR0(void) {
        ++tcounter;
        TA1CCR0 += 7812;
        TA1CCTL0 &= ~(CCIFG);
        if (tcounter > 9)
        {
            TA1CCTL0 &= ~(CCIE);
            tcounter = 0;
            SET(EV_TIMER);
        }
        if (eventos)
            {
                __low_power_mode_off_on_exit();
            } // if (eventos)

 } // __interrupt void RTI_TA1CCR0(void)

#pragma vector = TIMER1_A1_VECTOR
__interrupt void RTI_TA1CCR12(void) {

    switch (TA1IV) {

        case (TA1IV_TACCR1): {
            break;
        } // case (TA1IV_TACCR1)

        case (TA1IV_TACCR2): {
            ++counter6;
            TA1CCR2 += 7814;
            TA1CCTL2 &= ~(CCIFG);
            if(counter6 >2 ){
                P2OUT ^= (BIT7);
                TA1CCTL2 &= ~(CCIFG);
                TA1CCTL2 &= ~(CCIE);
                counter6 = 0;
            }
            break;
        } // case (TA1IV_TACCR2)

        case (TA1IV_TAIFG): {
            break;
        } // case (TA1IV_TAIFG)

    } // switch (TA1IV)
    if (eventos)
        {
            __low_power_mode_off_on_exit();
        } // if (eventos)


} // __interrupt void RTI_TA1CCR12(void)


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

#pragma vector = PORT1_VECTOR
__interrupt void RTI_P1(void) {

    if ((P1IE & P1IFG & BIT4) != 0) {
        P1IFG &= ~(BIT4);
        P1IE &= ~(BIT4);
        IFG1 &= ~(WDTIFG);
        IE1 |= WDTIE;

        if (P1IES & BIT4)
        {
            SET(EV_S3_UP);

        }
        else
        {
            SET(EV_S3_DOWN);

        }// else: if ((P1IES & BIT4) != 0)




    } // if ((P1IE & P1IFG & BIT4) != 0)

    if (eventos)
    {
        __low_power_mode_off_on_exit();
    } // if (eventos)

} // __interrupt void RTI_P1(void)


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
            SET(EV_S4_DOWN);

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
            SET(EV_S5_UP);
        }
        else
        {
            SET(EV_S5_DOWN);

        }
    } // if ((P2IE & P2IFG & BIT2) != 0)
    if ((P2IE & P2IFG & BIT3) != 0)
    {
        P2IFG &= ~(BIT3);
        P2IE &= ~(BIT3);
        P2IES ^= (BIT3);
        IE1 |= (WDTIE);
        if (P2IES & BIT3)
        {
            SET(EV_S6_UP);
        }
        else
        {
            SET(EV_S6_DOWN);

        }
    } // if ((P2IE & P2IFG & BIT3) != 0)

    if (eventos)
    {
        __low_power_mode_off_on_exit();
    } // if (eventos)
} // __interrupt void RTI_P2(void)


