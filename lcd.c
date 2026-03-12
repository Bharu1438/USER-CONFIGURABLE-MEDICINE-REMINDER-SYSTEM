/*
include lcd header file
this file contains lcd pin definitions, command macros and lcd function declarations
used for controlling lcd operations like command, data, and display functions
*/
#include "lcd_header.h"

/*
include lpc214x header file
this file contains special function register definitions for lpc2148 microcontroller
used to access gpio registers like iodir0, ioset0, ioclr0 etc
*/
#include <lpc214x.h>

/*
include delay header file
this file contains delay function declarations used to generate timing delays
delay functions are required to satisfy lcd timing requirements
*/
#include "delay_header.h"


/*
function used to write data or command to lcd
this function sends 8-bit data to lcd data pins
enable signal is toggled to latch the data into lcd
*/
void writeLCD(int ch){

	/*set rw pin to 0 to select write mode of lcd*/
	IOCLR0 = 1<<RW;

	/*clear previous data from lcd data lines*/
	IOCLR0 = 0xff<<LCD;

	/*place new 8-bit data on lcd data pins*/
	IOSET0 = ch<<LCD;

	/*generate enable pulse to latch data into lcd*/
	IOSET0 = 1<<EN;

	/*small delay to satisfy lcd timing*/
	delay_US(1);

	/*bring enable low to complete data write*/
	IOCLR0 = 1<<EN;
	
	/*delay for lcd command execution*/
	delay_MS(2);
}


/*
function to display single character on lcd
rs pin is set high to select data register
character is sent to lcd using writeLCD function
*/
void charLCD(unsigned char ch){

	/*set rs = 1 to select lcd data register*/
	IOSET0 = 1<<RS;

	/*send character to lcd*/
	writeLCD(ch);
}


/*
function used to send command to lcd
rs pin is cleared to select command register
command byte is written to lcd
*/
void cmdLCD(unsigned char ch){

	/*set rs = 0 to select command register*/
	IOCLR0 = 1<<RS;

	/*send command to lcd*/
	writeLCD(ch);
}


/*
custom character pattern for upward arrow
each value represents one row of 5x8 lcd character matrix
these characters are stored in lcd cgram memory
*/
unsigned char up_arrow[8] = {0x04,0x0E,0x15,0x04,0x04,0x04,0x04,0x00};

/*
custom character pattern for downward arrow
used for menu navigation or indicators
*/
unsigned char down_arrow[8] = {0x04,0x04,0x04,0x04,0x15,0x0E,0x04,0x00};

/*
custom character pattern for left arrow
*/
unsigned char left_araow[8]={0x02,0x04,0x08,0x1f,0x1f,0x08,0x04,0x02};

/*
custom character pattern for right arrow
*/
unsigned char right_arrow[8]={0x08,0x04,0x02,0x1f,0x1f,0x02,0x04,0x08};


/*
function used to create custom character in lcd cgram
loc parameter specifies cgram location (0 to 7)
msg pointer contains 8-byte pattern of custom character
*/
void createCharLCD(unsigned char loc, unsigned char *msg)
{
    int i;

		/*set cgram address for custom character location*/
    cmdLCD(0x40 + (loc * 8));

		/*write 8 bytes pattern into cgram*/
    for(i=0;i<8;i++)
    {
        charLCD(msg[i]);
    }
}


/*
function used to initialize lcd module
configures lcd gpio pins and sends initialization commands
initialization sequence follows hd44780 lcd datasheet
*/
void initLCD(void){

	/*configure lcd data pins and control pins as output*/
	IODIR0 |= (255<<LCD) | (1<<RS) | (1<<RW) | (1<<EN);

	/*wait for lcd power stabilization*/
	delay_MS(15);

	/*send function set command multiple times as per datasheet*/
	cmdLCD(BIT8LINE1);
	delay_MS(5);

	cmdLCD(BIT8LINE1);
	delay_US(100);

	cmdLCD(BIT8LINE1);
	
	/*configure lcd for 8-bit mode and 2-line display*/
	cmdLCD(BIT8LINE2);

	/*turn on display and cursor settings*/
	cmdLCD(DISPLAY_ON);

	/*clear lcd display*/
	cmdLCD(CLEAR_DISPLAY);

	/*set cursor movement direction*/
	cmdLCD(CUR_SHIFT);

	/*create custom arrow characters in cgram*/
	createCharLCD(0, up_arrow);
	createCharLCD(1, down_arrow);
	createCharLCD(2, left_araow);
	createCharLCD(3, right_arrow);
	
}


/*
function used to display string on lcd
characters are printed sequentially until null character is found
*/
void stringLCD(char *p){

	/*loop runs until null terminator of string*/
	while(*p){

		/*send each character to lcd*/
		charLCD(*p++);
	}
}


/*
function used to display integer number on lcd
number is converted into digits and printed sequentially
*/
void numberLCD(int n){

	/*
	unsigned int range is about 4.3 billion
	which corresponds to maximum 10 digits
	array used to store individual digits
	*/
	int arr[10];

	int cnt = 0;

	/*special case when number is zero*/
	if(n==0){
		charLCD('0');
		return;
	}

	/*extract digits from number*/
	while(n>0){
		arr[cnt++]=n%10;
		n/=10;
	}

	/*display digits in correct order*/
	for(--cnt; cnt>=0; cnt--){
		charLCD(arr[cnt]+'0');
	}
}

