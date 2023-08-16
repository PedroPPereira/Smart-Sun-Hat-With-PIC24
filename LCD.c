#include <p24fj128ga010.h>
#include "LCD.h"
#include <p24fxxxx.h>


void LCDinit( void)
{
// PMP initialization
PMCON = 0x83BF; // Enable the PMP, long waits
PMMODE = 0x3FF; // Master Mode 1
PMAEN = 0x0001; //or PMPEN

// init TMR1
T1CON = 0x8030; // Fosc/2, prescaled 1:256, 16us/tick
// wait for >30ms
TMR1 = 0; while( TMR1<2000); // 2000 x 16us = 32ms
PMDIN1 = 0b00111000; // function set: 8-bit interface, 2 lines, 5x7
TMR1 = 0; while( TMR1<3); // 3 x 16us = 48us
PMDIN1 = 0b00001100; // display ON, cursor off, blink off
TMR1 = 0; while( TMR1<3); // 3 x 16us = 48us
PMDIN1 = 0b00000001; // clear display
TMR1 = 0; while( TMR1<100); // 100 x 16us = 1.6ms
PMDIN1 = 0b00000110; // increment cursor, no shift
TMR1 = 0; while( TMR1<100); // 100 x 16us = 1.6ms
LCDclr();
LCDhome();
}


void LCDdelay(unsigned int n){
	//while(n) n--;
	T1CON = 0x8030;
	TMR1 = 0; while( TMR1<n); // 100 x 16us = 1.6ms
}
void LCDclr() {
	while( PMMODEbits.BUSY);
	PMADDR = 0x0000;
 	PMDIN1 = 0b00000001;	
	LCDdelay(100);
}
void LCDhome(){
	while( PMMODEbits.BUSY);
	PMADDR = 0x0000; 
	PMDIN1 = 0b00000010; 
	LCDdelay(100);
}
void putLCD(char d){
	while( PMMODEbits.BUSY);
	PMADDR = 0x0001; 
	PMDIN1 = d; 
	LCDdelay(500);
}

void putsLCD( char *s)
{
LCDclr();
LCDhome();
while( *s)
	putLCD( *s++);
} 

