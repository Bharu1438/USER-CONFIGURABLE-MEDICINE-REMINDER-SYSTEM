/*
   time comparison module header file
   this file contains declaration of function used to compare
   current rtc time with stored medicine reminder slots
   if match occurs buzzer alert and lcd message will be triggered
*/


//include lcd header for displaying alert message
#include "lcd_header.h"

//include buzzer header for generating alarm sound
#include "buzzer_header.h"

/*
   function prototype for medicine time comparison
   hour      -> current rtc hour
   min       -> current rtc minute
   sec       -> current rtc second
   medhour[] -> array storing medicine slot hours
   medmin[]  -> array storing medicine slot minutes
   medcnt    -> total number of medicine slots currently stored
   ack       -> pointer used to acknowledge alarm event
*/

//function declaration for comparing rtc time with medicine slots
void compare_medtime(int hour, int min, int sec,int medhour[], int medmin[],int medcnt, int *ack);


