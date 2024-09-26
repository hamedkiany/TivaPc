// gonzalez@uma.es - 06/04/23 (v11)

// p4.7: Se encendera el LED6, y cuando se presione el pulsador Sx el LED5
// parpadeara con un PWM de frecuencia 50Hz y con distinto ciclo de trabajo
// segun se presione S4 (10%), S5 (50%) o S6 (90%), y cuando se presione S3 el
// LED5 se apagara
// [Sx: Interrupcion ; TA0: SMCLK 1MHz, OUTMOD_7,  ALTERNATIVA OUTMOD_4]

#include "msp430g2553.h"

void main(void) {
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
	P2DIR |= (BIT6);
	P2SEL |= (BIT6);
	TA0CTL = (TASSEL_2);
	TA0CCR0 = 20000-1;

	P2OUT |= (BIT7);

	__enable_interrupt();

	while (1) {
	} // while (1)

} // void main(void)

#pragma vector = PORT1_VECTOR
__interrupt void RTI_P1(void) {

	if ((P1IE & P1IFG & BIT4) != 0) {
		P1IFG &= ~(BIT4);
		TA0CTL &= ~(MC0|MC1);
		TA0CCTL1 = 0;
	} // if ((P1IE & P1IFG & BIT4) != 0)

} // __interrupt void RTI_P1(void)

#pragma vector = PORT2_VECTOR
__interrupt void RTI_P2(void) {

	if ((P2IE & P2IFG & BIT1) != 0) {
		P2IFG &= ~(BIT1);
	    TA0CCR0 = 20000-1; // ALTERNATIVA OUTMOD_4
		TA0CCR1 = 2000-1;
		TA0CCTL1 = OUTMOD_7;
		TA0CTL |= (MC_1|TACLR);
	} // if ((P2IE & P2IFG & BIT1) != 0)

	if ((P2IE & P2IFG & BIT2) != 0) {
		P2IFG &= ~(BIT2);
//		TA0CCR1 = 10000-1;
//		TA0CCTL1 = OUTMOD_7;
//		TA0CTL |= (MC_1|TACLR);
        TA0CCR0 = 10000-1; // ALTERNATIVA OUTMOD_4
        TA0CCR1 = 0; // ESTE VALOR NO TIENE RELEVANCIA EN EL CT
        TA0CCTL1 = OUTMOD_4;
        TA0CTL |= (MC_1|TACLR);


	} // if ((P2IE & P2IFG & BIT2) != 0)

	if ((P2IE & P2IFG & BIT3) != 0) {
		P2IFG &= ~(BIT3);
        TA0CCR0 = 20000-1; // ALTERNATIVA OUTMOD_4
		TA0CCR1 = 18000-1;
		TA0CCTL1 = OUTMOD_7;
		TA0CTL |= (MC_1|TACLR);
	} // if ((P2IE & P2IFG & BIT3) != 0)

} // __interrupt void RTI_P2(void)
