/*
   setup module header file
   this file contains declarations related to setup menu of the project
   setup menu allows user to configure rtc time date and medicine reminder slots
   this header also includes all required peripheral driver headers
*/

//include lpc214x register definitions
#include <lpc214x.h>

//include lcd driver header file for lcd display functions
#include "lcd_header.h"

//include keypad driver header file for keypad scanning functions
#include "keypad_header.h"

//include delay driver header file for delay functions
#include "delay_header.h"

//include display module for time and date display/edit functions
#include "display_time.h"

//include rtc driver header file for rtc initialization and access
#include "rtc.h"


//macro defining maximum number of medicine reminder slots
#define MAXMED 5


/*
   function prototype for setup menu
   this function displays menu options on lcd
   user can configure time date day and medicine slots
   parameters are pointers so changes directly update main variables
*/

//setup menu function declaration
void SetupMenu(int *hour, int *min, int *sec, int *date, int *month, int *year,
               int medhour[], int medmin[], int *medcnt, int *exitflag, int *);


