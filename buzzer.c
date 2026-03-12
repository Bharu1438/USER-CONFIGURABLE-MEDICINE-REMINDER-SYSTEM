/*
include header file for lpc21xx register definitions
this file contains sfr(register) definitions of lpc21xx/lpc2148 microcontrollers
used to access hardware registers like iodir0, ioset0, ioclr0 etc
*/
#include<lpc21xx.h>

/*
include buzzer module header file
this file contains buzzer pin definition and function declarations
functions available: initBUZZER(), buzz_on(), buzz_off(), buzzerAlert()
*/
#include "buzzer_header.h"

/*
include lcd module header file
this file contains lcd pin macros, lcd command macros and lcd function declarations
functions like initLCD(), cmdLCD(), charLCD(), stringLCD() are declared here
*/
#include "lcd_header.h"

/*
include delay module header file
this file contains software delay function declarations
functions delay_US(), delay_MS() and delay_S() are used to generate timing delays
*/
#include "delay_header.h"

/*
this variable is defined in main.c
when interrupt occurs the interrupt service routine
changes the value of this variable
ack = 1 -> interrupt occurred and alarm should stop
ack = 0 -> no interrupt and alarm continues
*/
extern int ack;


/*
function to initialize buzzer gpio pin
configure buzzer pin as output using iodir0 register
buzzer pin number is defined by macro BUZZ
*/
void initBUZZER(void){
	IODIR0 |= 1<<BUZZ;
}


/*
function to turn on buzzer
sets buzzer pin high using ioset0 register
when pin becomes high buzzer starts producing sound
*/
void buzz_on(void){
	IOSET0 = 1 << BUZZ;
}


/*
function to turn off buzzer
clears buzzer pin using ioclr0 register
when pin becomes low buzzer stops producing sound
*/
void buzz_off(void){
	IOCLR0 = 1 << BUZZ;
}


/*
this function generates alarm sound using buzzer
buzzer will beep for 60 seconds if interrupt does not occur
each cycle:
buzzer on  -> 500 ms
buzzer off -> 500 ms

so each cycle takes 1 second
total cycles = 60 -> total alarm duration = 60 seconds
if interrupt occurs (ack becomes 1)
buzzer stops immediately
lcd display is cleared
function returns to the calling function
*/
void buzzerAlert(void)
{
    int i;
	//loop runs for 60seconds
    for(i=0;i<60;i++)
    {
        if(ack){
            buzz_off();
            cmdLCD(CLEAR_DISPLAY);
            return;
        }

        buzz_on();
        delay_MS(500);

        buzz_off();
        delay_MS(500);
    }
}
