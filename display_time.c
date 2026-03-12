#include<lpc21xx.h>
#include "delay_header.h"
#include "lcd_header.h"
#include "keypad_header.h"

/* this file implements all time and date display functions
   it interacts with lcd driver keypad driver and rtc variables
   functions here allow user to view and modify time date and weekday */


/* display complete time date and weekday on lcd */
void displayDateTime(int *hour,int *min,int *sec,int *date,int *month,int *year,int *dow){

	/* array storing weekday names */
	char weekname[7][4]={"SUN","MON","TUE","WED","THU","FRI","SAT"};

	/* move cursor to first lcd line */
	cmdLCD(GOTO_LINE_ONE);

	/* display hour digits */
	numberLCD(*hour/10);
	numberLCD(*hour%10);

	/* display separator */
	charLCD(':');

	/* display minute digits */
	numberLCD(*min/10);
	numberLCD(*min%10);

	/* display separator */
	charLCD(':');

	/* display second digits */
	numberLCD(*sec/10);
	numberLCD(*sec%10);

	/* move cursor to second line */
	cmdLCD(GOTO_LINE_TWO);

	/* display date digits */
	numberLCD(*date/10);
	numberLCD(*date%10);
	charLCD('/');

	/* display month digits */
	numberLCD(*month/10);
	numberLCD(*month%10);
	charLCD('/');

	/* display year digits */
	numberLCD((*year/1000)%10);
	numberLCD((*year/100)%10);
	numberLCD((*year/10)%10);
	numberLCD((*year)%10);

	/* space before weekday */
	charLCD(' ');

	/* display weekday string */
	stringLCD(weekname[*dow]);
}


/* display only date on lcd */
void displayDate(int *date,int *month,int *year){

	/* move cursor to second line */
	cmdLCD(GOTO_LINE_TWO+3);

	/* extract and display date */
	charLCD((*date/10)+'0');
	charLCD((*date%10)+'0');

	charLCD('/');

	/* extract and display month */
	charLCD((*month/10)+'0');
	charLCD((*month%10)+'0');

	charLCD('/');

	/* extract and display year */
	charLCD((*year/1000)%10+'0');
	charLCD((*year/100)%10+'0');
	charLCD((*year/10)%10+'0');
	charLCD((*year)%10+'0');
}


/* display time in hh:mm:ss format */
void displayTime(int *hour,int *minute,int *sec){

	/* move cursor to time position */
	cmdLCD(GOTO_LINE_TWO+4);

	/* display hour digits */
	charLCD((*hour/10)+'0');
	charLCD((*hour%10)+'0');

	charLCD(':');

	/* display minute digits */
	charLCD((*minute/10)+'0');
	charLCD((*minute%10)+'0');

	charLCD(':');

	/* display second digits */
	charLCD((*sec/10)+'0');
	charLCD((*sec%10)+'0');
}


/* calculate lcd cursor position during time editing */
int getTimeCursorPos(int index){

	/* first two digits correspond to hour */
	if(index<=1)
	return index;

	/* skip colon after hour */
	else if(index<=3)
	return index+1;

	/* skip second colon */
	else
	return index+2;
}


/*
Function used to adjust Hour, Minute and Second using keypad input.
User can navigate between HH, MM and SS fields and increase/decrease values.
Pointers are used so that the updated values modify the original variables.
*/
void adjustTime(int *hour, int *min, int *sec)
{
	//rowcnt stores which field is currently selected (1=HH,2=MM,3=SS)
	int rowcnt=1,num;

	//clear LCD display
	cmdLCD(CLEAR_DISPLAY);

	//move cursor to position where field indicator will be shown
	cmdLCD(GOTO_LINE_ONE+4);

	//display HH as the default selected field
	stringLCD("HH");

	//move cursor to beginning of first line
	cmdLCD(GOTO_LINE_ONE);

	//display left arrow symbol
	charLCD(2);

	//move cursor to last position of first line
	cmdLCD(GOTO_LINE_ONE+15);

	//display right arrow symbol
	charLCD(3);

	//move cursor to beginning of second line
	cmdLCD(GOTO_LINE_TWO);

	//display down arrow symbol
	charLCD(0);

	//move cursor to last position of second line
	cmdLCD(GOTO_LINE_TWO+15);

	//display up arrow symbol
	charLCD(1);

	//move cursor to position where time will be displayed
	cmdLCD(GOTO_LINE_TWO+4);

	//display current hour, minute and second
	displayTime(hour,min,sec);

	//loop continuously until user exits
	while(1){

	//read key pressed on keypad
	num = keySCAN();

	//if ENTER key pressed exit from time adjustment
	if(num == 11)
	break;

	/*
	If RIGHT key pressed
	move selection forward HH ? MM ? SS
	*/
	else if(num == 12){
	rowcnt++;

	//wrap around if selection exceeds SS
	if(rowcnt>3) rowcnt=1;

	//clear previous indicator text
	cmdLCD(GOTO_LINE_ONE+1);
	stringLCD("              ");

	//display HH indicator
	if(rowcnt==1){
	cmdLCD(GOTO_LINE_ONE+(3+rowcnt));
	stringLCD("HH");
	}

	//display MM indicator
	else if(rowcnt==2){
	cmdLCD(GOTO_LINE_ONE+(5+rowcnt));
	stringLCD("MM");
	}

	//display SS indicator
	else if(rowcnt==3){
	cmdLCD(GOTO_LINE_ONE+(7+rowcnt));
	stringLCD("SS");
	}
	}

	/*
	If LEFT key pressed
	move selection backward SS ? MM ? HH
	*/
	else if(num == 13){
	rowcnt--;

	//wrap around if selection goes below HH
	if(rowcnt<1) rowcnt=3;

	//clear previous indicator text
	cmdLCD(GOTO_LINE_ONE+1);
	stringLCD("              ");

	//display HH indicator
	if(rowcnt==1){
	cmdLCD(GOTO_LINE_ONE+(3+rowcnt));
	stringLCD("HH");
	}

	//display MM indicator
	else if(rowcnt==2){
	cmdLCD(GOTO_LINE_ONE+(5+rowcnt));
	stringLCD("MM");
	}

	//display SS indicator
	else if(rowcnt==3){
	cmdLCD(GOTO_LINE_ONE+(7+rowcnt));
	stringLCD("SS");
	}
	}

	/*
	If UP key pressed
	increment selected field value
	*/
	else if(num == 15){

	//increase hour value
	if(rowcnt==1){
	(*hour)++;
	if(*hour>23) *hour=0;
	displayTime(hour,min,sec);
	}

	//increase minute value
	else if(rowcnt==2){
	(*min)++;
	if(*min>59) *min=0;
	displayTime(hour,min,sec);
	}

	//increase second value
	else if(rowcnt==3){
	(*sec)++;
	if(*sec>59) *sec=0;
	displayTime(hour,min,sec);
	}
	}

	/*
	If DOWN key pressed
	decrement selected field value
	*/
	else if(num == 14){

	//decrease hour value
	if(rowcnt==1){
	(*hour)--;
	if(*hour<0) *hour=23;
	displayTime(hour,min,sec);
	}

	//decrease minute value
	else if(rowcnt==2){
	(*min)--;
	if(*min<0) *min=59;
	displayTime(hour,min,sec);
	}

	//decrease second value
	else if(rowcnt==3){
	(*sec)--;
	if(*sec<0) *sec=59;
	displayTime(hour,min,sec);
	}
	}
	}
	cmdLCD(CLEAR_DISPLAY);
	cmdLCD(GOTO_LINE_ONE);
	stringLCD("Time Updated");
	delay_MS(1000);
}

/* take time input from keypad */
void inputTimeFromKeypad(int *hour,int *min,int *sec,int medhour[],int medmin[],int *medcnt)
{
    /* array to store entered digits */
    int arr[6];

    int i=0;
    int num,pos;

    /* initialize LCD with default time format */
    cmdLCD(GOTO_LINE_TWO);
    stringLCD("00:00:00");

    while(i<6)
    {
        /* scan keypad */
        num=keySCAN();

        /* BACKSPACE key */
        if(num==10)
        {
            if(i>0)
            {
                i--;

                /* get correct LCD cursor position */
                pos=getTimeCursorPos(i);

                cmdLCD(GOTO_LINE_TWO+pos);
                charLCD('0');

                cmdLCD(GOTO_LINE_TWO+pos);
            }

            continue;
        }

        /* CANCEL key */
        if(num==11)
        {
            return;
        }

        /* digit validation and display */
        switch(i)
        {

        case 0:

            /* first hour digit (0-2) */
            if(num>=0 && num<=2)
            {
                arr[i]=num;

                cmdLCD(GOTO_LINE_TWO+0);
                numberLCD(num);

                i++;
            }
            break;


        case 1:

            /* second hour digit */
            if((arr[0]==2 && num<=3) || (arr[0]<2 && num<=9))
            {
                arr[i]=num;

                cmdLCD(GOTO_LINE_TWO+1);
                numberLCD(num);

                i++;
            }
            break;


        case 2:

            /* minute tens digit */
            if(num<=5)
            {
                arr[i]=num;

                /* print colon */
                cmdLCD(GOTO_LINE_TWO+2);
                charLCD(':');

                cmdLCD(GOTO_LINE_TWO+3);
                numberLCD(num);

                i++;
            }
            break;


        case 3:

            /* minute units digit */
            if(num<=9)
            {
                arr[i]=num;

                cmdLCD(GOTO_LINE_TWO+4);
                numberLCD(num);

                i++;
            }
            break;


        case 4:

            /* seconds tens digit */
            if(num<=5)
            {
                arr[i]=num;

                /* print colon */
                cmdLCD(GOTO_LINE_TWO+5);
                charLCD(':');

                cmdLCD(GOTO_LINE_TWO+6);
                numberLCD(num);

                i++;
            }
            break;


        case 5:

            /* seconds units digit */
            if(num<=9)
            {
                arr[i]=num;

                cmdLCD(GOTO_LINE_TWO+7);
                numberLCD(num);

                i++;
            }
            break;
        }
    }


    /* combine digits to form actual time values */

    *hour = arr[0]*10 + arr[1];
    *min  = arr[2]*10 + arr[3];
    *sec  = arr[4]*10 + arr[5];


    /* check duplicate medication time */

    for(i=0;i<*medcnt;i++)
    {
        if(medhour[i]==*hour && medmin[i]==*min)
        {
            cmdLCD(CLEAR_DISPLAY);

            cmdLCD(GOTO_LINE_ONE);
            stringLCD("Already exist");

            delay_MS(500);

            (*medcnt)--;

            return;
        }
    }


    /* confirmation message */

    cmdLCD(CLEAR_DISPLAY);

    cmdLCD(GOTO_LINE_ONE);
    stringLCD("Time updated");

    delay_MS(1000);
}

/* 
Function used to adjust Date, Month and Year using keypad input.
User can navigate between DD, MM, YYYY fields and increase/decrease values.
Updated values are displayed on LCD and stored through pointer variables.
*/
void adjustDate(int *date, int *month, int *year){

    //variable used to track which field (DD/MM/YYYY) is selected
	int rowcnt=1,num;

    //default year initialization
	*year=2026;

    //clear LCD display
	cmdLCD(CLEAR_DISPLAY);

    //move cursor to first line
	cmdLCD(GOTO_LINE_ONE);

    //display left arrow symbol for navigation
	charLCD(2);

    //move cursor to end of first line
	cmdLCD(GOTO_LINE_ONE+15);

    //display right arrow symbol for navigation
	charLCD(3);

    //move cursor to position of date field
	cmdLCD(GOTO_LINE_ONE+3);

    //display "DD" indicator showing date field
	stringLCD("DD");

    //move cursor to second line
	cmdLCD(GOTO_LINE_TWO);

    //display down arrow symbol
	charLCD(0);

    //move cursor to end of second line
	cmdLCD(GOTO_LINE_TWO+15);

    //display up arrow symbol
	charLCD(1);

    //display current date, month and year on LCD
	displayDate(date,month,year);

    //infinite loop for keypad interaction
	while(1){

    //scan keypad to get pressed key value
	num=keySCAN();

    //if ENTER key pressed exit from date setup
	if(num==11)
	break;

    /* 
    If RIGHT key pressed:
    move selection to next field (DD ? MM ? YYYY)
    */
	else if(num==12){
		rowcnt++;
		if(rowcnt>3) rowcnt=1;

        //clear previous indicator
		cmdLCD(GOTO_LINE_ONE+1);
		stringLCD("             ");

        //highlight selected field
		if(rowcnt==1){
		cmdLCD(GOTO_LINE_ONE+3);
		stringLCD("DD");
		}
		else if(rowcnt==2){
			cmdLCD(GOTO_LINE_ONE+6);
			stringLCD("MM");
		}
		else if(rowcnt==3){
			cmdLCD(GOTO_LINE_ONE+9);
			stringLCD("YYYY");
		}
	}

    /* 
    If LEFT key pressed:
    move selection to previous field
    */
	else if(num==13){
		rowcnt--;
		if(rowcnt<1) rowcnt=3;

        //clear previous indicator
		cmdLCD(GOTO_LINE_ONE+1);
		stringLCD("             ");

        //highlight selected field
		if(rowcnt==1){
			cmdLCD(GOTO_LINE_ONE+3);
			stringLCD("DD");
		}
		else if(rowcnt==2){
			cmdLCD(GOTO_LINE_ONE+6);
			stringLCD("MM");
		}
		else if(rowcnt==3){
			cmdLCD(GOTO_LINE_ONE+9);
			stringLCD("YYYY");
		}
	}

    /* 
    If UP key pressed:
    increase selected field value
    */
	else if(num==15){

        //increase date
		if(rowcnt==1){
			(*date)++;
			if(*date>31) *date=1;
		}

        //increase month
		else if(rowcnt==2){
		(*month)++;
		if(*month>12) *month=1;
		}

        //increase year
		else if(rowcnt==3){
		(*year)++;
		}
	}

    /* 
    If DOWN key pressed:
    decrease selected field value
    */
	else if(num==14){

        //decrease date
		if(rowcnt==1){
		(*date)--;
		if(*date<1) *date=31;
	}

        //decrease month
	else if(rowcnt==2){
		(*month)--;
		if(*month<1) *month=12;
	}

        //decrease year
		else if(rowcnt==3){
		(*year)--;
	}
	}

    //update LCD with modified date values
	displayDate(date,month,year);
	}

    //clear LCD after exiting setup
	cmdLCD(CLEAR_DISPLAY);

    //move cursor to first line
	cmdLCD(GOTO_LINE_ONE);

    //display confirmation message
	stringLCD("Date Updated");

    //wait 2 seconds before returning
	delay_MS(1000);
}

/* select day of week */
void selectDayOfWeek(int *dow){

	int cnt=0;
	int num;

	/* weekday names */
	char dowarr[7][4]={"SUN","MON","TUE","WED","THU","FRI","SAT"};

	cmdLCD(CLEAR_DISPLAY);

	cmdLCD(GOTO_LINE_ONE);
	charLCD(2);

	cmdLCD(GOTO_LINE_ONE+7);
	stringLCD(dowarr[cnt]);

	cmdLCD(GOTO_LINE_ONE+15);
	charLCD(3);

	while(1){

		num=keySCAN();

		/* next day */
		if(num==12){
			cnt++;
			if(cnt==7) cnt=0;
		}

		/* previous day */
		else if(num==13){
			cnt--;
			if(cnt<0) cnt=6;
		}

		/* select day */
		else if(num==11)
		break;

		cmdLCD(GOTO_LINE_ONE+7);
		stringLCD(dowarr[cnt]);
	}

	/* store selected day */
	*dow=cnt;

	cmdLCD(CLEAR_DISPLAY);

	cmdLCD(GOTO_LINE_ONE);
	stringLCD("Day of week");

	cmdLCD(GOTO_LINE_TWO);
	stringLCD("Updated");

	delay_MS(1000);
}
