//making 5th pin for buzzer
#define BUZZ 5

/*
function to initialize buzzer gpio pin
configure buzzer pin as output using iodir0 register
buzzer pin number is defined by macro BUZZ
*/
void initBUZZER(void);
/*
function to turn on buzzer
sets buzzer pin high using ioset0 register
when pin becomes high buzzer starts producing sound
*/
void buzz_on(void);
/*
function to turn off buzzer
clears buzzer pin using ioclr0 register
when pin becomes low buzzer stops producing sound
*/
void buzz_off(void);
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
void buzzerAlert(void);

