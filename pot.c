#include <p24fj128ga010.h>
#include "pot.h"
#include <stdio.h>
#include <string.h>
#include <p24fxxxx.h>


void initADC(void)
{
	AD1PCFG = AINPUTS; 
	AD1CON1 = 0x0000; // SAMP bit = 0 ends sampling
	AD1CSSL = 0;
	AD1CON3 = 0x0002; // Manual Sample, Tad = 3 Tcy
	AD1CON2 = 0;
	AD1CON1bits.ADON = 1; // turn ADC ON
} 

void delay(){
	int i; 
	for(i=0; i<1; i++){};
}
int readADC( int ch)
{
	AD1CHS = ch; // 1. select analog input channel
	AD1CON1bits.SAMP = 1; // 2. start sampling
	delay();
	AD1CON1bits.SAMP = 0;
	while (!AD1CON1bits.DONE); // 3. wait for the conversion to complete
	return ADC1BUF0; // 4. read the conversion result
} 

