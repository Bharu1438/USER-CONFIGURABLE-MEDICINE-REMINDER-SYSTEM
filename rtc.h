/* 
macros related to oscillator frequency configuration
these values are used to derive cpu clock and peripheral clock
rtc prescaler calculation depends on pclk frequency */

 //main crystal oscillator frequency used by the microcontroller
 #define FOSC 12000000

 /* 
cpu clock frequency obtained after pll multiplication
here pll multiplies fosc by 5 
*/
 #define CCLK (FOSC * 5)

 /*
peripheral clock used by rtc and other peripherals
here pclk is cclk divided by 4 
*/
 #define PCLK (CCLK / 4)

 /* 
rtc clock frequency coming from 32.768 khz crystal
this is standard frequency used in real time clocks
*/
 #define RTCCLK 32768


 /* 
 macros used to calculate rtc prescaler values
 tc needs 1 second tick derived from pclk
 prescaler registers divide pclk to generate 1 second 
 */

 /* 
 integer part of prescaler calculation
 stored in rtc preint register 
 */
 #define PRESCALAR_INTEGER (int)((PCLK/RTCCLK)-1)
	 
 /* 
 fractional part of prescaler calculation
 stored in rtc prefrac register 
 */
 #define PRESCALAR_FRACTION (PCLK-((PRESCALAR_INTEGER*32768)+1))


 /* 
 macro used when external rtc configuration is required
uncomment this if using rtc configuration specific to lpc2148 board 
*/
 //#define _2148


//macros related to rtc control register operations
// bit mask used to reset rtc counters and registers
#define RTC_RESET 1<<1

//bit mask used to enable rtc counting operation
#define RTC_ENABLE 1<<0


 /* 
 function used to initialize rtc module
configures prescaler and enables rtc counting
*/

 void initRTC(void);


 /* 
 function used to update rtc registers with new values
 parameters represent hour minute second date month year and weekday 
*/

void editRTCTIME_DATE(int, int , int , int , int , int , int);


 /* 
function used to read current time and date from rtc registers
pointer parameters allow returning values to calling function 
*/

void readRTCTIME_DATE(int *, int *, int *, int *, int *, int *, int *);
 




