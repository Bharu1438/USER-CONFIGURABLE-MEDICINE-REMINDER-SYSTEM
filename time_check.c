//include time check header file containing function prototype
#include "time_check.h"

//include lcd driver header file for displaying alert messages
#include "lcd_header.h"


/*
   function used to compare rtc time with stored medicine reminder times
   if current rtc time matches any medicine slot
   lcd message is displayed and buzzer alert is activated
*/

void compare_medtime(int hour, int min, int sec,int medhour[], int medmin[],int medcnt, int *ack)
{
    int i;   //loop counter for iterating medicine slots

    //loop through all stored medicine slots
    for(i = 0; i < medcnt; i++)
    {

        //check if current rtc hour and minute match medicine slot
        //also ensure second is zero so alert triggers only once per minute
        if(medhour[i] == hour && medmin[i] == min && sec == 0)
        {

            //clear lcd before displaying alert message
            cmdLCD(CLEAR_DISPLAY);

            //move cursor to first line
            cmdLCD(GOTO_LINE_ONE);

            //display medicine reminder message
            stringLCD("take medicine");

            //move cursor to second line
            cmdLCD(GOTO_LINE_TWO);

            //display alert text
            stringLCD("now....!");


            //activate buzzer to notify user
            buzzerAlert();


            //clear lcd after alert
            cmdLCD(CLEAR_DISPLAY);


            //reset acknowledge flag after interrupt handling
            *ack = 0;
        }
    }
}



