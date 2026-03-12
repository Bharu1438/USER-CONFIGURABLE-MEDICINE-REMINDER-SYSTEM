//function prototype used to initialize keypad module and configure gpio pins
void initKPM(void);
//function prototype used to scan keypad columns to detect key press
int colSCAN(void);
//function prototype used to determine which row the pressed key belongs to
int rowCHECK(void);
//function prototype used to determine which column the pressed key belongs to
int colCHECK(void);
//function prototype used to perform complete keypad scan and return key value
int keySCAN(void);

//starting gpio pin position for keypad rows in port1
#define ROW 16
//starting gpio pin position for keypad columns in port1
#define COL 20

//macro used to write 4-bit value (nibble) into specific position of register
#define WNIBBLE(WORD,POS,VAL) WORD = ((WORD & ~(0x0f<<POS)) | (VAL << POS))

//macro used to read 4-bit value (nibble) from specific position of register
#define RNIBBLE(WORD,POS) ((WORD>>POS)&15)

//function prototype used to initialize keypad gpio configuration
void initKPM(void);
//function prototype used to check if any key is pressed by scanning column pins
int colSCAN(void);
//function prototype used to identify which row key is pressed
int rowCHECK(void);
//function prototype used to identify which column key is pressed
int colCHECK(void);
//function prototype used to scan keypad and return corresponding key value
int keySCAN(void);
//function prototype used to detect key press for timeout conditions
int is_keyPressed(void);

