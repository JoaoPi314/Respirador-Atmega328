#ifndef LCD_H
#define LCD_H

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/twi.h>

#define FONT_SIZE 5     //this value is in the comments above the font table

#define OLED_SLA7 0x3C  //where nn is 7 bit address of the OLED I2C address


#define SSD1306_CHAR_SIZE        6
#define SSD1306_OLED_SLA7              0x3C     //  SSD1306 controller,  I2C slave address
#define SSD1306_LASTLINE                0x07
#define SSD1306_COMMAND                 0x00
#define SSD1306_DATA                    0xC0
#define SSD1306_DATA_CONTINUE           0x40
#define SSD1306_SET_CONTRAST_CONTROL    0x81
#define SSD1306_DISPLAY_ALL_ON_RESUME   0xA4
#define SSD1306_DISPLAY_ALL_ON          0xA5
#define SSD1306_NORMAL_DISPLAY          0xA6
#define SSD1306_INVERT_DISPLAY          0xA7
#define SSD1306_DISPLAY_OFF             0xAE
#define SSD1306_DISPLAY_ON              0xAF
#define SSD1306_NOP                     0xE3
#define SSD1306_HORIZONTAL_SCROLL_RIGHT 0x26
#define SSD1306_HORIZONTAL_SCROLL_LEFT  0x27
#define SSD1306_SCROLL_VERT_AND_RIGHT   0x29
#define SSD1306_SCROLL_VERT_AND_LEFT    0x2A
#define SSD1306_DEACTIVATE_SCROLL       0x2E
#define SSD1306_ACTIVATE_SCROLL         0x2F
#define SSD1306_SET_VERT_SCROLL_AREA    0xA3
#define SSD1306_SET_LOWER_COLUMN        0x00
#define SSD1306_SET_HIGHER_COLUMN       0x10
#define SSD1306_MEMORY_ADDR_MODE        0x20
#define SSD1306_SET_COLUMN_ADDR         0x21
#define SSD1306_SET_PAGE_ADDR           0x22
#define SSD1306_SET_START_LINE          0x40
#define SSD1306_SET_SEGMENT_REMAP       0xA0
#define SSD1306_SET_MULTIPLEX_RATIO     0xA8
#define SSD1306_COM_SCAN_DIR_INC        0xC0
#define SSD1306_COM_SCAN_DIR_DEC        0xC8
#define SSD1306_SET_DISPLAY_OFFSET      0xD3
#define SSD1306_SET_COM_PINS            0xDA
#define SSD1306_CHARGE_PUMP             0x8D
#define SSD1306_SET_CLOCK_DIV_RATIO     0xD5
#define SSD1306_SET_PRECHARGE_PERIOD    0xD9
#define SSD1306_SET_VCOM_DESELECT       0xDB
/*##############################################################*/

//  global variables that need to be unchanged between each call to OLED_SetCursor() and OLED_DisplayChar().

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

//  here are the functions for using the SSD1306, followed by documentation.

//~~~~~~~~~~~~~~~~~~~~~~~LOW-level OLED_I2C  @ 180 lines of code ~~~~
void i2c_initialise();
void OLED_sendCommand(uint8_t myCmd);
void OLED_clear();
void OLED_Init(void);
void OLED_SetCursor(uint8_t myLineNumber,uint8_t myColumnPosition);
void OLED_GoToLine(uint8_t myLine);
void OLED_GoToNextLine();
void OLED_DisplayChar(uint8_t myDisplayChar);
void OLED_DisplayString(uint8_t *ptr);
void displayBigDigit(uint8_t myDigit, uint8_t myDigitRow, uint8_t myDigitColumn);

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */


/* OLED display techniques:  numbers; flash strings, bitmaps;
-----------
Numbers:  Values that change and need to be updated:
-----------
            uint8_t myBuffer[22];

            OLED_SetCursor(0, 11 * CHAR_SIZE); //=6   top line 0, mid-screen char (0-20)
            itoa(integer_Variable_Name, myBuffer, 10);   // use dtostrf() for floats
            OLED_DisplayString(myBuffer);
-----------
Individual Characters
-----------OLED_DisplayChar(uint8_t myDisplayChar);

-----------
SRAM strings
-----------OLED_DisplayString(uint8_t *ptr);
 
Constant Number values in program memory:
----------- uint16_t workFMfreq, myFMpresetFreq;
            float displayFMfrequency ;
            byte highFMreg3, lowFMreg3, myBuffer[8];

            OLED_SetCursor(7, 16 * CHAR_SIZE); //=6   bottom line 7, right screen char (0-20)
            myFMpresetFreq = pgm_read_word(&FMstationPreset[currentFMstationIndex]);
            displayFMfrequency  =  (float) myFMpresetFreq / 100;
            dtostrf(displayFMfrequency,4,1,myBuffer); // 4 chars, 1 digit after dec point
            OLED_DisplayString(myBuffer); // dtostrf(value,strSize,decPtCnt,buffer);
-----------
Flash strings:  constant strings that are stored in program memory:
-----------
const char String00[] PROGMEM = "KFBW";   //  FM _The Buzz
const char String01[] PROGMEM = "KLTH";   //  The Eagle
const char String02[] PROGMEM = "KXRY";   //  progressive independent
const char String03[] PROGMEM = "KXJM";   //  Jam'n 1075
const char * const  StringArray[ 4 ] PROGMEM = {String00, String01, String02, String03};

            uint8_t myBuffer[22], *myPtrToFlashString;

            OLED_SetCursor(2,0 * SSD1306_CHAR_SIZE);  // = 6    starting row:char column pixel
            myPtrToFlashString = (char *) pgm_read_word (& StringArray



);
            strcpy_P ((char *) myBuffer, myPtrToFlashString);
            OLED_DisplayString(myBuffer);
-----------
BitMaps:  used for real-time clock's hours:minutes display:
//offset 0-11; row 0-5; BIGDIGITSPACING=15
----------- displayBigDigit(digit_offset, 5, 1 * BIGDIGITSPACING);        */
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~ LOW-level OLED_I2C ~~~~~~~~~~~~~~~~~~~~~~~~~

#endif