// gonzalez@uma.es - 01/03/23 (v1)

// P3.extra: Al soltar Sx se conmutará el LEDx, excepto si mientras se está pulsado S3
// se pulsan y sueltan por este orden S4, S5 y S6 que se encenderán todo los LED
// [Sx: Interrupción]

#include "msp430g2553.h"

#define PULSADO 1
#define SUELTO 0
#define ELEMENTOS_CLAVE 3

const char clave[3]={4,5,6}; // array que contiene el orden de pulsación y
                             // soltado de los botones
char flanco_bajada[3];
char flanco_subida[3];
char i_bajada=0;
char i_subida=0;

char s3=SUELTO ;

char comprueba_clave(void);
void reset_flancos(void);

void main(void) {
	WDTCTL = WDTPW|WDTHOLD;
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

	__enable_interrupt();

	while (1) {
	} // while (1)

} // void main(void)

#pragma vector = PORT1_VECTOR
__interrupt void RTI_P1(void) {

    //s3
	if ((P1IE & P1IFG & BIT4) != 0) {
		P1IFG &= ~(BIT4);

		if ((P1IES & BIT4) != 0) { // flanco de bajada
			s3=PULSADO;
		} // if ((P1IES & BIT4) != 0)

		else {                      //flanco subida
			if((comprueba_clave()) &&
			        (i_bajada==ELEMENTOS_CLAVE) &&
			        (i_subida==ELEMENTOS_CLAVE)) P2OUT|= (BIT4|BIT5|BIT6|BIT7);
			else P2OUT ^= (BIT4);
			i_bajada=0;
			i_subida=0;
			reset_flancos();
			s3=SUELTO;

		} // else: if ((P1IES & BIT4) != 0)

		P1IES ^= (BIT4);
	} // if ((P1IE & P1IFG & BIT4) != 0)

} // __interrupt void RTI_P1(void)

#pragma vector = PORT2_VECTOR
__interrupt void RTI_P2(void) {

    //s4

	if ((P2IE & P2IFG & BIT1) != 0) {
		P2IFG &= ~(BIT1);

		if ((P2IES & BIT1) != 0) { //flanco bajada
            if(s3==PULSADO){
                if (i_bajada< ELEMENTOS_CLAVE) flanco_bajada[i_bajada]=4;
                i_bajada++;
            }//if(s3==PULSADO)

		} // if ((P2IES & BIT1) != 0)

		else {                 //flanco subida
			P2OUT ^= (BIT5);
			if(s3==PULSADO){
			    if (i_subida< ELEMENTOS_CLAVE) flanco_subida[i_subida]=4;
			    i_subida++;
			}//if(s3==PULSADO)
		} // else: if ((P2IES & BIT1) != 0)

		P2IES ^= (BIT1);
	} // if ((P2IE & P2IFG & BIT1) != 0)

	//s5

	if ((P2IE & P2IFG & BIT2) != 0) {
		P2IFG &= ~(BIT2);

		if ((P2IES & BIT2) != 0) {   //flanco bajada
		    if(s3==PULSADO){
		        if (i_bajada< ELEMENTOS_CLAVE) flanco_bajada[i_bajada]=5;
                i_bajada++;
		    }//if(s3==PULSADO)
		} // if ((P2IES & BIT2) != 0)

		else {                      //flanco subida
			P2OUT  ^=(BIT6);
			if(s3==PULSADO){
			    if (i_subida< ELEMENTOS_CLAVE) flanco_subida[i_subida]=5;
			    i_subida++;
			}//if(s3==PULSADO)
		} // else: if ((P2IES & BIT2) != 0)

		P2IES ^= (BIT2);
	} // if ((P2IE & P2IFG & BIT2) != 0)

	//s6
	if ((P2IE & P2IFG & BIT3) != 0) {
		P2IFG &= ~(BIT3);

		if ((P2IES & BIT3) != 0) {    //flanco bajada
		    if(s3==PULSADO){
		        if (i_bajada< ELEMENTOS_CLAVE) flanco_bajada[i_bajada]=6;
                i_bajada++;
		    }//if(s3==PULSADO)
		} // if ((P2IES & BIT3) != 0)

		else {                      //flanco subida
			P2OUT  ^= (BIT7);
			if(s3==PULSADO){
			    if (i_subida< ELEMENTOS_CLAVE) flanco_subida[i_subida]=6;
                i_subida++;
			}//if(s3==PULSADO)
		} // else: if ((P2IES & BIT3) != 0)

		P2IES ^= (BIT3);
	} // if ((P2IE & P2IFG & BIT3) != 0)

} // __interrupt void RTI_P2(void)

char comprueba_clave (void)
{
  char i;
  for(i=0; i<ELEMENTOS_CLAVE;i++)
  {

      if(clave[i]!= flanco_bajada[i]) return 0;
      if(clave[i]!= flanco_subida[i]) return 0;
  }
 return 1;
}//char comprueba_clave (void)

void reset_flancos(void){
    char i;
    for(i=0; i<ELEMENTOS_CLAVE;i++)
    {
        flanco_bajada[i]=0;
        flanco_subida[i]=0;
    }
}//void reset_flancos(void)

