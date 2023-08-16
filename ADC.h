

#define POT 5 //10k potentiometer connected to AN5 input
#define TSENS 0 // TC1047 Temperature sensor with voltage output
#define LDR2 2 //LDR2 sensor connected to AN2 input
#define LDR1 3 //LDR1 sensor connected to AN1 input
#define AINPUTS 0xFFD2 //analog inputs  


void initADC(void);


int readADC( int ch);