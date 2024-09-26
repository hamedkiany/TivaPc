// edu@uma.es - 12/01/23 (v11)

// p6.1: Se implementara el siguiente diagrama de estados:
//	ESTADO_A (*) -> S3_DOWN: ESTADO_B (LED5+LED3)
//	ESTADO_B     -> S4_UP  : ESTADO_C (LED6+LED4)
//	             -> S6_UP  : ESTADO_A (INICIO)
//	ESTADO_C     -> S5_DOWN: ESTADO_B (LED5+LED3)
//	[Sx: Interrupcion ANT 32ms WD ; LPM0]

#include "msp430g2553.h"

#define ESTADO_A	0
#define ESTADO_B	1
#define ESTADO_C	2

#define S3_DOWN	BIT0
#define S4_UP		BIT1
#define S5_DOWN	BIT2
#define S6_UP		BIT3

#define SET(ev)	(eventos |= (ev))
#define RESET(ev)	(eventos &= ~(ev))
#define TEST(ev)	(eventos & (ev))

unsigned char eventos;

void main(void) {
	unsigned char estado;
	
	WDTCTL = (WDTPW|WDTHOLD);	// Detiene el Watchdog
	DCOCTL = 0;						// Establece la frecuencia a 1MHz
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
	P1DIR &= ~(BIT4);
	P1REN |= (BIT4);
	P1OUT |= (BIT4);
	P1IES |= (BIT4);
	P1IFG &= ~(BIT4);
	P1IE |= (BIT4);
	P2DIR &= ~(BIT3|BIT2|BIT1);
	P2REN |= (BIT3|BIT2|BIT1);
	P2OUT |= (BIT3|BIT2|BIT1);
	P2IES |= (BIT3|BIT2|BIT1);
	P2IFG &= ~(BIT3|BIT2|BIT1);
	P2IE |= (BIT3|BIT2|BIT1);
	P2DIR |= (BIT7|BIT6|BIT5|BIT4);
	P2OUT &= ~(BIT7|BIT6|BIT5|BIT4);
	P2SEL &= ~(BIT7|BIT6);

	estado = ESTADO_A;
	P2OUT &= ~(BIT7|BIT6|BIT5|BIT4);

	eventos = 0;

	while (1) {
		__low_power_mode_0();

		while (eventos) {

			switch (estado) {

				case (ESTADO_A): {

					if (TEST(S3_DOWN)) {
						RESET(S3_DOWN);
						estado = ESTADO_B;
						P2OUT |= (BIT6|BIT4);
						P2OUT &= ~(BIT7|BIT5);
					} // if (TEST(S3_DOWN))

					if (TEST(S4_UP)) {
						RESET(S4_UP);
					} // if (TEST(S4_UP))

					if (TEST(S5_DOWN)) {
						RESET(S5_DOWN);
					} // if (TEST(S5_DOWN))

					if (TEST(S6_UP)) {
						RESET(S6_UP);
					} // if (TEST(S6_UP))

					break;
				} // case (ESTADO_A)

				case (ESTADO_B): {

					if (TEST(S3_DOWN)) {
						RESET(S3_DOWN);
					} // if (TEST(S3_DOWN))

					if (TEST(S4_UP)) {
						RESET(S4_UP);
						estado = ESTADO_C;
						P2OUT &= ~(BIT6|BIT4);
						P2OUT |= (BIT7|BIT5);
					} // if (TEST(S4_UP))

					if (TEST(S5_DOWN)) {
						RESET(S5_DOWN);
					} // if (TEST(S5_DOWN))

					if (TEST(S6_UP)) {
						RESET(S6_UP);
						estado = ESTADO_A;
						P2OUT &= ~(BIT7|BIT6|BIT5|BIT4);
					} // if (TEST(S6_UP))

					break;
				} // case (ESTADO_B)

				case (ESTADO_C): {

					if (TEST(S3_DOWN)) {
						RESET(S3_DOWN);
					} // if (TEST(S3_DOWN))

					if (TEST(S4_UP)) {
						RESET(S4_UP);
					} // if (TEST(S4_UP))

					if (TEST(S5_DOWN)) {
						RESET(S5_DOWN);
						estado = ESTADO_B;
						P2OUT |= (BIT6|BIT4);
						P2OUT &= ~(BIT7|BIT5);
					} // if (TEST(S5_DOWN))

					if (TEST(S6_UP)) {
						RESET(S6_UP);
					} // if (TEST(S6_UP))

					break;
				} // case (ESTADO_C)

			} // switch (estado)

		} // while (eventos)

	} // while (1)

} // void main(void)

#pragma vector = PORT1_VECTOR
__interrupt void RTI_P1(void) {

	if ((P1IE & P1IFG & BIT4) != 0) {
		P1IE &= ~(BIT4);
		WDTCTL = WDT_MDLY_32;
		IFG1 &= ~(WDTIFG);
		IE1 |= WDTIE;

		if ((P1IES & BIT4) != 0) {
			SET(S3_DOWN);
		} // if ((P1IES & BIT4) != 0)

		else {
		} // else: if ((P1IES & BIT4) != 0)

		P1IES ^= (BIT4);
		P1IFG &= ~(BIT4);
	} // if ((P1IE & P1IFG & BIT4) != 0)

	if (eventos) {
		__low_power_mode_off_on_exit();
	} // if (eventos)

} // __interrupt void RTI_P1(void)

#pragma vector = PORT2_VECTOR
__interrupt void RTI_P2(void) {

	if ((P2IE & P2IFG & BIT1) != 0) {
		P2IE &= ~(BIT1);
		WDTCTL = WDT_MDLY_32;
		IFG1 &= ~(WDTIFG);
		IE1 |= WDTIE;

		if ((P2IES & BIT1) != 0) {
		} // if ((P2IES & BIT1) != 0)

		else {
			SET(S4_UP);
		} // else: if ((P2IES & BIT1) != 0)

		P2IES ^= (BIT1);
		P2IFG &= ~(BIT1);
	} // if ((P2IE & P2IFG & BIT1) != 0)

	if ((P2IE & P2IFG & BIT2) != 0) {
		P2IE &= ~(BIT2);
		WDTCTL = WDT_MDLY_32;
		IFG1 &= ~(WDTIFG);
		IE1 |= WDTIE;

		if ((P2IES & BIT2) != 0) {
			SET(S5_DOWN);
		} // if ((P2IES & BIT2) != 0)

		else {
		} // else: if ((P2IES & BIT2) != 0)

		P2IES ^= (BIT2);
		P2IFG &= ~(BIT2);
	} // if ((P2IE & P2IFG & BIT2) != 0)

	if ((P2IE & P2IFG & BIT3) != 0) {
		P2IE &= ~(BIT3);
		WDTCTL = WDT_MDLY_32;
		IFG1 &= ~(WDTIFG);
		IE1 |= WDTIE;

		if ((P2IES & BIT3) != 0) {
		} // if ((P2IES & BIT3) != 0)

		else {
			SET(S6_UP);
		} // else: if ((P2IES & BIT3) != 0)

		P2IES ^= (BIT3);
		P2IFG &= ~(BIT3);
	} // if ((P2IE & P2IFG & BIT3) != 0)

	if (eventos) {
		__low_power_mode_off_on_exit();
	} // if (eventos)

} // __interrupt void RTI_P2(void)

#pragma vector = WDT_VECTOR
__interrupt void RTI_WD(void) {
	WDTCTL = (WDTPW|WDTHOLD);
	IE1 &= ~(WDTIE);
	P1IFG &= ~(BIT4);
	P2IFG &= ~(BIT3|BIT2|BIT1);
	P1IE |= (BIT4);
	P2IE |= (BIT3|BIT2|BIT1);
} // __interrupt void RTI_WD(void)
