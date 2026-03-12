//include setup header file containing function declarations
#include "setup_meditionAdd.h"


/*
   setup menu function
   this function displays configuration menu on lcd
   user can select options like time setup add medicine slot reset slots or exit
   keypad input determines which menu option will be executed
*/

void SetupMenu(int *hour, int *min, int *sec,int *date, int *month, int *year,
               int medhour[], int medmin[], int *medcnt, int *exitflag, int *dow)
{

	//variable used for yes/no decision when adding medicine slots
	int flag;

	//variable storing keypad choice
    int choice;

	//dummy variable used to capture seconds while entering medicine time
    int dummysec;

	//timeout counter (approx 60 seconds)
	int count = 100;


	//clear lcd before displaying menu
    cmdLCD(CLEAR_DISPLAY);

	//display first line menu options
    cmdLCD(GOTO_LINE_ONE);
    stringLCD("1.setup  3.Reset");

	//display second line menu options
    cmdLCD(GOTO_LINE_TWO);
    stringLCD("2.addslot 4.exit");


	//wait for keypad input or timeout
	while(count--)
    {
		//check if any key is pressed
        if(is_keyPressed())
        {
			//scan pressed key
            choice = keySCAN();
            break;
        }

		//delay to slow loop execution
        delay_MS(100);
    }


	//if timeout occurs
    if(count <= 0)
    {
		//clear lcd
        cmdLCD(CLEAR_DISPLAY);

		//display timeout message
        cmdLCD(GOTO_LINE_ONE);
        stringLCD("Timeout..!");

		//wait before exiting
        delay_S(1);

		//clear lcd
		cmdLCD(CLEAR_DISPLAY);

        return;
    }


	//clear lcd before executing selected option
    cmdLCD(CLEAR_DISPLAY);


	//check user choice
    switch(choice)
    {

		//case 1 : setup time date and weekday
        case 1:

			//display setup message
            cmdLCD(GOTO_LINE_ONE);
            stringLCD("Time-Date setup");

			//wait for user to read message
            delay_MS(1000);

			cmdLCD(CLEAR_DISPLAY);

			//ask user to enter date
            cmdLCD(GOTO_LINE_ONE);
            stringLCD("enter time:");
            cmdLCD(GOTO_LINE_TWO);

			//call function to adjust date
			adjustDate(date,month,year);

			delay_S(1);

			cmdLCD(CLEAR_DISPLAY);

			//ask user to select weekday
            cmdLCD(GOTO_LINE_ONE);
			stringLCD("enter day of week:");
            cmdLCD(GOTO_LINE_TWO);

			//call weekday selection function
			selectDayOfWeek(dow);

			delay_S(1);


			cmdLCD(CLEAR_DISPLAY);

			//ask user to enter time
            cmdLCD(GOTO_LINE_ONE);
            stringLCD("enter time:");
            cmdLCD(GOTO_LINE_TWO);

			//call function to edit time
            adjustTime(hour, min, sec);

			//update rtc registers with new values
            editRTCTIME_DATE(*hour, *min, *sec, *date, *month, *year,*dow);
            break;


		//case 2 : add medicine reminder slots
		case 2:
			while(1)
			{
				//clear lcd
				cmdLCD(CLEAR_DISPLAY);

				//display message
				cmdLCD(GOTO_LINE_ONE);
				stringLCD("Add Med Time");
				delay_MS(1500);
				//check if maximum slots reached
				if(*medcnt >= MAXMED)
				{
					cmdLCD(CLEAR_DISPLAY);

					cmdLCD(GOTO_LINE_ONE);
					stringLCD("Slots Full!");

					delay_S(2);

					break;
				}


				//ask user to enter medicine time
				cmdLCD(CLEAR_DISPLAY);

				cmdLCD(GOTO_LINE_ONE);
				stringLCD("Set Med Time:");
				cmdLCD(GOTO_LINE_TWO);
				//collect medicine time from keypad
				inputTimeFromKeypad(&medhour[*medcnt],&medmin[*medcnt],&dummysec,medhour,medmin,medcnt);
				//increase medicine slot count
				(*medcnt)++;
				cmdLCD(CLEAR_DISPLAY);
				cmdLCD(GOTO_LINE_ONE);
				stringLCD("Add One More?");
				cmdLCD(GOTO_LINE_TWO);
				stringLCD("Yes- ");

				//display arrow symbol
				charLCD(1);
				stringLCD(" / No-ENT");
				
				//read user response
				flag = keySCAN();

				//if user chooses not to continue
				if(flag != 15)
				{
					break;
				}
			}
			break;

		//case 3 : reset all medicine slots
		case 3:

			//check if slots are already empty
			if(*medcnt == 0)
			{
				cmdLCD(GOTO_LINE_ONE);
				stringLCD("All Slots");

				cmdLCD(GOTO_LINE_TWO);
				stringLCD("are Empty");

				delay_MS(1000);
			}

			else
			{
				//clear slot count
				*medcnt = 0;

				cmdLCD(GOTO_LINE_ONE);
				stringLCD("All Slots");

				cmdLCD(GOTO_LINE_TWO);
				stringLCD("Erased");

				delay_MS(1000);
			}

			break;



		//case 4 : exit setup menu
        case 4:

			//set exit flag
            *exitflag = 1;

			cmdLCD(GOTO_LINE_ONE);
            stringLCD("exit");

			delay_MS(1000);

            break;



		//invalid key pressed
        default:

            cmdLCD(GOTO_LINE_ONE);
            stringLCD("invalid!");

            delay_S(1);
    }


	//clear lcd before returning to main program
    cmdLCD(CLEAR_DISPLAY);
}


