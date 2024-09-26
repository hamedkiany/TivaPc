/* compute crc's */
/* crc-ccitt is based on the polynomial x^16+x^12+x^5+1 */
/*  The bits are inserted from most to least significant */
/* The prescription for determining the mask to use for a given polynomial
	is as follows:
		1.  Represent the polynomial by a 17-bit number
		2.  Assume that the most and least significant bits are 1
		3.  Place the right 16 bits into an integer
		4.  Bit reverse if serial LSB's are sent first
*/


unsigned short int create_checksum(unsigned char *paquete,unsigned char longitud);
