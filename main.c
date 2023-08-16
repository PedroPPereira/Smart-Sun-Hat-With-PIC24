#include <p24fxxxx.h>
#include <stdio.h>
#include <string.h>
#include <p24fj128ga010.h>
#include "UART2.h"
#include "ADC.h"
#include "LCD.h"

// Configuration Bits
#ifdef __PIC24FJ64GA004__ //Defined by MPLAB when using 24FJ64GA004 device
_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx1 & IOL1WAY_ON) 
_CONFIG2( FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRI & I2C1SEL_SEC)
#else
_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx2) 
_CONFIG2( FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRI)
#endif

#define LDR_DIFF 150 //allowed difference between the 2 LDR sensors
#define TEMP_30C 480 //corresponding value for 30ºC in mV
#define PASS_SIZE 7

void initPorts(void){
	//inputs buttons
	TRISDbits.TRISD13 = 1; 
	TRISDbits.TRISD7 = 1;

	//outputs leds
	TRISAbits.TRISA0 = 0;
	TRISAbits.TRISA1 = 0;
	TRISAbits.TRISA2 = 0;
	TRISAbits.TRISA3 = 0;
	TRISAbits.TRISA4 = 0;
	TRISAbits.TRISA5 = 0;
	TRISAbits.TRISA6 = 0;
	TRISAbits.TRISA7 = 0;	 
}

void println(char *s){
	writeUART2(s);
	putsLCD(s);
}








void move_motor(int lat7, int lat6){
	if (lat7==1 && lat6==0) println("Moving right\r\n");
	if (lat7==0 && lat6==1) println("Moving left\r\n");
	if (lat7==0 && lat6==0) println("Not Moving\r\n");
	LATAbits.LATA7 = lat7;
	LATAbits.LATA6 = lat6;
}

int main(void){
	int boolPass = 0; //boolean for password verification
	int boolSun = 0; //boolean to select hat mode
	int check = 0; //bool to filter UART notifications
	int ldr1, ldr2, diff, temp, v_fan = 0; 
	int temp_butt = 0;
	int a, j; //temp values for temperature
	char pass[] = "20asad"; //password
	char s[PASS_SIZE], str[30], codeUART;

	//inicializations
	initPorts();
	initUART2();
	LCDinit();
	initADC();
	println("Welcome to the Intelligent Sun Hat!\r\n");

	while (!boolPass){ //loop for password verification
		if (!PORTDbits.RD13){ 
			println("Insert password:\r\n");
			delay_time(DELAY);
			readUART2(s, PASS_SIZE); //read a full line of text
			if(!strcmp(s, pass)) { 
			   boolPass = 1; //correct password
			   println("\nPassword correct\r\n");
			}
			else println("Password incorrect, try again\r\n");
			delay_time(DELAY);
		}	
	}

	LATAbits.LATA0 = 1;
	while(1){
		if (!PORTDbits.RD7 && !temp_butt)  {
			boolSun = !boolSun;
			check = 0;
			if(boolSun) println("Operation: Get Sun\r\n");
			else println("Operation: Get Shadow\r\n");
		}
		temp_butt = !PORTDbits.RD7;
		//read LDR1 sensor
		ldr1 = readADC(LDR1);
		//read LDR2 sensor
		ldr2 = readADC(LDR2);
		//get the difference 
		diff = ldr1 - ldr2;

        if(boolSun){ //goes to the darkest direction
            if(diff > LDR_DIFF && !check){
		 	    move_motor(0, 1);
			    check = 1;
		    }else if(diff < -LDR_DIFF && !check){
			    move_motor(1, 0);
			    check = 1; 	
		    }else if(check && diff<=LDR_DIFF && diff>=-LDR_DIFF){
			    move_motor(0, 0);
			    check = 0;
		    }
        }else{ //goes to the lightest direction
            if(diff > LDR_DIFF && !check){
		 	    move_motor(1, 0);
			    check = 1;			  
		    }else if(diff < -LDR_DIFF && !check){
			    move_motor(0, 1);
			    check = 1; 	
		    } else if(check && diff<=LDR_DIFF && diff>=-LDR_DIFF){
			    move_motor(0, 0);
			    check = 0;
		    }
        }
		delay_time(DELAY);
		
		//read temp. sensor
		a = 0;
		for ( j= 16; j >0; j--)
			a += readADC( TSENS); // add up 16 successive temperature readings
		temp = a >> 4; // divide the result by 16 to obtain the average
		//if Temp.>30 then turn on fan and notify UART
		if(temp > TEMP_30C){ //see page 171 from book
			if(v_fan==0) println("Fan turned ON\r\n");
			v_fan = readADC(POT);
			LATAbits.LATA1 = 1;
		}
		if(temp <= TEMP_30C && v_fan){
			println("Fan turned OFF\r\n");
			v_fan = 0;
			LATAbits.LATA1 = 0;
		}
		delay_time(DELAY);

		//notify user
		if(U2STAbits.URXDA){
			//U2RXREG = ' ';
			codeUART = U2RXREG;
			//if 'T' then notify UART with temp. value
			if(codeUART =='T') {
				sprintf(str, "Temperature: %d\r\n", temp);
				println(str);
			}
			//if 'P' then notify UART with fan speed
			if(codeUART == 'P') {
				sprintf(str, "Fan Speed: %d\r\n", v_fan);
				println(str);
			}
			//if 'L' then notify UART with LDR values
			if(codeUART == 'L') {
				sprintf(str, "LDR Values: %d and %d\r\n", ldr1, ldr2);
				println(str);
			}
		}
	}
}



