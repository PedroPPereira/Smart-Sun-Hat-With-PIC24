#include <p24fxxxx.h>
#include "UART2.h"
#include <stdio.h>
#include <string.h>
#include <p24fj128ga010.h>

void initUART2( void)
{
	U2MODEbits.UARTEN  = 0; //disable it in order to configure it.
	U2BRG = BRATE; //initialize the baud rate generator
	U2MODE = 0x8000; //enable Uart for 8-bit data
	U2STA = 0; //enable the Transmitter
	TRTS = 0; //make RTS an output pin
	RTS = 1; //set RTS default status (not ready)
	U2MODEbits.UARTEN  = 1; //UART System enabled.
	U2STAbits.UTXEN = 1; //enable transmition
} 


void putUART2( char c)
{
	while (CTS); //wait for !CTS, clear to send
	while (U2STAbits.UTXBF); //wait while Tx buffer full
	U2TXREG = c;
} 


char getUART2( void)
{
	RTS = 0; // assert Request To Send !RTS
	while (!U2STAbits.URXDA); // wait for a new character to arrive
	return U2RXREG; // read the character from the receive buffer
}


void writeUART2( char *s)
{
	while( *s) //loop until *s == ‘\0’, end of string
 		putUART2( *s++); //send the character and point to the next one
} 


char *readUART2( char *s, int len)
{
	char *p = s; // copy the buffer pointer
	do{
		*s = getUART2(); // wait for a new character
		putUART2( *s); // echo character
		if (( *s==BACKSPACE)&&( s>p))
		{
			putUART2( ' '); // overwrite the last character
			putUART2( BACKSPACE);
			len++;
			s--; // back the pointer
			continue;
		}
		if ( *s=='\n') // line feed, ignore it
			continue;
		if ( *s=='\r') // end of line, end loop
			break;
		s++; // increment buffer pointer
		len--;
	} while ( len>1 ); // until buffer full
	*s = '\0'; // null terminate the string
	return p; // return buffer pointer
} // getsnU2

void delay_time(int num){
	int i;
	for( i = 0 ; i < num ; i++){};
}