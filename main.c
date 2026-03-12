/*
main application file of medicine reminder system
this program reads rtc time displays it on lcd and checks medicine reminder slots
external interrupts are used to open setup menu and acknowledge alarm
*/


/*
lpc214x.h header file
this header contains all special function register definitions of lpc2148 microcontroller
it allows direct access to hardware peripherals like gpio rtc interrupts timers and uart
*/
#include<lpc214x.h>

/*
delay_header.h header file
this module provides delay functions such as microsecond millisecond and second delay
these delays are used for lcd timing keypad debounce and general timing control
*/
#include "delay_header.h"

/*
lcd_header.h header file
this file contains lcd command macros and function declarations
these functions allow displaying characters numbers and strings on lcd
*/
#include "lcd_header.h"

/*
keypad_header.h header file
this header provides keypad scanning and key detection functions
it allows user input through keypad for menu navigation and configuration
*/
#include "keypad_header.h"

/*
buzzer_header.h header file
this module contains functions to control buzzer hardware
buzzer is used to generate alarm sound when medicine reminder time occurs
*/
#include "buzzer_header.h"

/*
display_time.h header file
this module contains functions to display rtc date and time on lcd
it formats hour minute second date month and year for user display
*/
#include "display_time.h"

/*
rtc.h header file
this header contains rtc initialization read and edit functions
it allows configuration and access of real time clock registers of lpc2148
*/
#include "rtc.h"

/*
setup_meditionAdd.h header file
this module implements setup menu functionality
user can configure time date and add medicine reminder slots using keypad
*/
#include "setup_meditionAdd.h"

/*
time_check.h header file
this header contains function to compare current rtc time with medicine slots
if match occurs buzzer alert and lcd message will be triggered
*/
#include "time_check.h"


/*
macros related to external interrupt configuration
these macros define pin configuration values and interrupt channel numbers
external interrupts are used for setup menu and alarm acknowledgement
*/

//macro used to configure pin function for EINT0
#define EINT0_PIN 0x0000000C

//macro used to configure pin function for EINT1
#define EINT1_PIN 0x000000C0

//interrupt number for external interrupt 0
#define EINT0 14

//interrupt number for external interrupt 1
#define EINT1 15


/*
forward declaration of interrupt service routines
these functions execute automatically when interrupt occurs
__irq keyword tells compiler that function is an interrupt handler
*/

//interrupt function for setup menu
void eint0_irq(void) __irq;

//interrupt function for alarm acknowledgement
void eint1_irq(void) __irq;


/*
arrays used to store medicine reminder times
each index represents one medicine slot
hour and minute values are stored separately
*/

//array storing medicine hours
int medhour[MAXMED]={-1,-1,-1,-1,-1};

//array storing medicine minutes
int medmin[MAXMED]={-1,-1,-1,-1,-1};


/*
flags used for interrupt communication
interrupt service routines update these flags
main program checks these flags to execute tasks
*/

//flag set when setup interrupt occurs
int setup=0;

//flag set when alarm acknowledge interrupt occurs
int ack = 0;


/*
variables used for managing medicine slots
medcnt stores number of active medicine reminders
exitflag used to exit setup menu
*/

//count of medicine reminder slots
int medcnt=0;

//flag used to exit setup menu
int exitflag=0;


//optional debug macro
//#define DEBUG



/*
main function
program execution starts here
initializes peripherals and continuously monitors rtc time
*/
int main(){

	/*
	variables used to store current rtc time
	hour minute and second values are updated continuously
	*/
	int hour = 0, min = 0, sec = 0;

	/*
	variables used to store current date
	initial values can be modified through setup menu
	*/
	int date=28, month=02, year=2026;

	//variable storing day of week
	int day=0;

	/*
	initialization of all hardware modules
	lcd keypad rtc and buzzer drivers are initialized
	*/
	initLCD();
	initKPM();
	initRTC();
	initBUZZER();

	/*
	configure external interrupt pins
	clear previous pin function configuration bits
	*/
	PINSEL0 &= ~((3<<2) | (3<<6));

	/*
	configure p0.1 and p0.3 as external interrupt pins
	p0.1 -> EINT0
	p0.3 -> EINT1
	*/
	PINSEL0 |= (EINT0_PIN | EINT1_PIN);

	/*
	enable interrupts in vectored interrupt controller
	both EINT0 and EINT1 are enabled
	*/
	VICIntEnable = (1<<EINT0)|(1<<EINT1);

	/*
	configure vector slot 0 for EINT0
	assign interrupt handler address
	*/
	VICVectCntl0 = (1<<5)|EINT0;
	VICVectAddr0 = (unsigned int) eint0_irq;

	/*
	configure vector slot 1 for EINT1
	assign interrupt handler address
	*/
	VICVectCntl1 = (1<<5)|EINT1;
	VICVectAddr1 = (unsigned int) eint1_irq;


	/*
	main infinite loop
	system continuously reads rtc time and checks medicine reminders
	*/
	while(1){

		/*
		read current rtc date and time
		values are stored in hour minute second date month year day variables
		*/
		readRTCTIME_DATE(&hour,&min,&sec,&date,&month,&year,&day);

		/*
		display current date and time on lcd
		display updates continuously
		*/
		displayDateTime(&hour,&min,&sec,&date,&month,&year,&day);


		#ifdef DEBUG

		/*
		debug block used to display stored medicine slots
		useful during testing phase
		*/
		for(i = 0; i < medcnt; i++){
			cmdLCD(CLEAR_DISPLAY);
			cmdLCD(GOTO_LINE_ONE);
			numberLCD(i+1);
			charLCD('.');
			numberLCD(medhour[i]);
			charLCD(':');
			numberLCD(medmin[i]);
			delay_S(2);
		}

		#endif


		/*
		check if current time matches any medicine slot
		if match occurs buzzer alert will be triggered
		*/
		compare_medtime(hour, min, sec, medhour, medmin, medcnt, &ack);


		/*
		check if setup interrupt occurred
		if yes open setup menu
		*/
		if(setup)
        {
			/*
			open setup menu
			user can configure time date and medicine reminder slots
			*/
            SetupMenu(&hour, &min, &sec,&date, &month, &year,medhour, medmin,&medcnt, &exitflag, &day);

			//reset setup flag after menu execution
            setup = 0;
        }
	}
}



/*
external interrupt 0 service routine
this interrupt is triggered when setup button is pressed
*/
void eint0_irq(void) __irq{

	//set setup flag so main program opens setup menu
	setup = 1;

	//clear interrupt flag
	EXTINT = 1<<0;

	//signal end of interrupt to vic
	VICVectAddr=0;
}



/*
external interrupt 1 service routine
this interrupt is triggered when user acknowledges alarm
*/
void eint1_irq(void) __irq{

	//set acknowledge flag
	ack = 1;

	//clear interrupt flag
	EXTINT = 1<<1;

	//signal end of interrupt to vic
	VICVectAddr=0;
}

