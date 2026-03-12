#include<lpc214x.h>

//include rtc header file containing macros and prototypes
#include "rtc.h"



 /* 
function used to initialize rtc module
it configures prescaler registers and starts rtc counter
*/
void initRTC(void){

	//reset rtc counters before configuration
	CCR = RTC_RESET;

	//load integer prescaler value into preint register
	PREINT = PRESCALAR_INTEGER;

	//load fractional prescaler value into prefrac register
	PREFRAC = PRESCALAR_FRACTION;

	//enable rtc counter so it starts running
	CCR = RTC_ENABLE;
}


 /* 
function used to update rtc time and date registers
values are provided by user through keypad interface 
*/
void editRTCTIME_DATE(int h, int m, int s, int d, int mon, int y, int dow){

	//store hour value into rtc hour register
	HOUR = h;

	//store minute value into rtc minute register
	MIN = m;

	//store second value into rtc second register
	SEC = s;

	//store day of month into dom register
	DOM = d;

	//store month value into month register
	MONTH = mon;

	//store year value into year register
	YEAR = y;

	//store day of week value into dow register
	DOW = dow;
}


 /*  
function used to update rtc time and date registers
values are provided by user through keypad interface 
*/
void readRTCTIME_DATE(int *h, int *m, int *s, int *d, int *mon, int *y, int *dow){

	//read hour register value
	*h = HOUR;

	//read minute register value
	*m = MIN;

	//read second register value
	*s = SEC;

	//read date register value
	*d = DOM;

	//read month register value
	*mon = MONTH;

	//read year register value
	*y = YEAR;

	//read day of week register value
	*dow = DOW;
}



