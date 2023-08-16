/*
** UART2.h
** console I/O library for Explorer16 board
*/
// I/O definitions for the Explorer16
#define CTS _RF12 // Clear To Send, input, HW handshake
#define RTS _RF13 // Request To Send, output, HW handshake
#define BACKSPACE 0x8 // ASCII backspace character code
#define TRTS TRISFbits.TRISF13 // Tris control for RTS pin
#define BRATE 25 // 9600 Bd
#define DELAY 5000

// init the serial port UART2
void initUART2( void);
// send a character to the serial port
void putUART2( char c);
// wait for a new character to arrive to the serial port
char getUART2( void);
// send a null terminated string to the serial port
void writeUART2( char *s);
// receive a null terminated string in a buffer of len char
char * readUART2( char *s, int n);
// delay time
void delay_time(int num);