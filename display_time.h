/* header file for time and date display module
   this file contains only function declarations (prototypes)
   these functions are implemented in display_time.c
   other files like main.c include this file to use these functions 
*/

 /* displays complete time, date and day of week on lcd
    time format  : hh:mm:ss
    date format  : dd/mm/yyyy
    parameters are pointers so actual variables get updated or read 
*/
 void displayDateTime(int *, int *, int *, int *, int *, int *, int *);

 /* collects time input from keypad and updates rtc time variables
    user enters hh:mm:ss using keypad
    also checks medication time list to avoid duplicate reminder times 
*/
 void inputTimeFromKeypad(int *hour, int *min, int *sec, int medhour[], int medmin[], int *);

 /* allows user to change current date using keypad
    user can adjust day, month and year values
    updated values are stored using pointer variables 
*/
 void adjustDate(int *, int *, int *);

 /* used to select day of week
    user scrolls through sun mon tue wed thu fri sat using keypad
    selected value is stored in dow variable 
		
*/
 void selectDayOfWeek(int *);

 /* used to adjust time values
    user can increase or decrease hours minutes seconds
    changes are reflected directly through pointer variables 
*/
 void adjustTime(int *, int *, int *);

 /* displays date on lcd
    format : dd/mm/yyyy
    digits are extracted and printed one by one 
*/
 void displayDate(int *, int *, int *);

 /* displays time on lcd
    format : hh:mm:ss
    digits are extracted and printed individually 
*/
 void displayTime(int *hour, int *min, int *sec);




