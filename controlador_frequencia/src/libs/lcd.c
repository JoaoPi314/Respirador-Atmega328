#include "lcd.h"

uint8_t         OLEDlineNumber = 0, OLEDcursorPos = 0;

// Next comes two font tables that go into 1K of flash. The first table is for 7x5 chars and
//  it is the same table found in most libraries of graphics-based display controllers.
//   After the 7x5 table is a second table for making big digit
//  chars that are three rows high.  {0..9, plus colon : and space chars)
//  Least-sig-bit on upper top side of char column.  Bytes go left to right.
 // Vertical pixel alignment: 475 bytes in flash.
const uint8_t FontTable[][FONT_SIZE] PROGMEM = {
        {0x00, 0x00, 0x00, 0x00, 0x00},   // space 00   20 hexASCII
        {0x00, 0x00, 0x2f, 0x00, 0x00},   // !     01   21
        {0x00, 0x07, 0x00, 0x07, 0x00},   // "     02   22
        {0x14, 0x7f, 0x14, 0x7f, 0x14},   // #     03   23
        {0x24, 0x2a, 0x7f, 0x2a, 0x12},   // $     04   24
        {0x23, 0x13, 0x08, 0x64, 0x62},   // %     05   25
        {0x36, 0x49, 0x55, 0x22, 0x50},   // &     06   26
        {0x00, 0x05, 0x03, 0x00, 0x00},   // '     07   27
        {0x00, 0x1c, 0x22, 0x41, 0x00},   // (     08   28
        {0x00, 0x41, 0x22, 0x1c, 0x00},   // )     09   29
        {0x14, 0x08, 0x3E, 0x08, 0x14},   // *     10   2a
        {0x08, 0x08, 0x3E, 0x08, 0x08},   // +     11   2b
        {0x00, 0x00, 0xA0, 0x60, 0x00},   // ,     12   2c
        {0x08, 0x08, 0x08, 0x08, 0x08},   // -     13   2d
        {0x00, 0x60, 0x60, 0x00, 0x00},   // .     14   2e
        {0x20, 0x10, 0x08, 0x04, 0x02},   // /     15   2f
        {0x3E, 0x51, 0x49, 0x45, 0x3E},   // 0     16   30
        {0x00, 0x42, 0x7F, 0x40, 0x00},   // 1     17   31
        {0x42, 0x61, 0x51, 0x49, 0x46},   // 2     18   32
        {0x21, 0x41, 0x45, 0x4B, 0x31},   // 3     19   33
        {0x18, 0x14, 0x12, 0x7F, 0x10},   // 4     20   34
        {0x27, 0x45, 0x45, 0x45, 0x39},   // 5     21   35
        {0x3C, 0x4A, 0x49, 0x49, 0x30},   // 6     22   36
        {0x01, 0x71, 0x09, 0x05, 0x03},   // 7     23   37
        {0x36, 0x49, 0x49, 0x49, 0x36},   // 8     24   38
        {0x06, 0x49, 0x49, 0x29, 0x1E},   // 9     25   39
        {0x00, 0x36, 0x36, 0x00, 0x00},   // :     26   3a
        {0x00, 0x56, 0x36, 0x00, 0x00},   // ;     27   3b
        {0x08, 0x14, 0x22, 0x41, 0x00},   // <     28   3c
        {0x14, 0x14, 0x14, 0x14, 0x14},   // =     29   3d
        {0x00, 0x41, 0x22, 0x14, 0x08},   // >     30   3e
        {0x02, 0x01, 0x51, 0x09, 0x06},   // ?     31   3f
        {0x32, 0x49, 0x59, 0x51, 0x3E},   // @     32   40
        {0x7C, 0x12, 0x11, 0x12, 0x7C},   // A     33   41
        {0x7F, 0x49, 0x49, 0x49, 0x36},   // B     34   42
        {0x3E, 0x41, 0x41, 0x41, 0x22},   // C     35   43
        {0x7F, 0x41, 0x41, 0x22, 0x1C},   // D     36   44
        {0x7F, 0x49, 0x49, 0x49, 0x41},   // E     37   45
        {0x7F, 0x09, 0x09, 0x09, 0x01},   // F     38   46
        {0x3E, 0x41, 0x49, 0x49, 0x7A},   // G     39   47
        {0x7F, 0x08, 0x08, 0x08, 0x7F},   // H     40   48
        {0x00, 0x41, 0x7F, 0x41, 0x00},   // I     41   49
        {0x20, 0x40, 0x41, 0x3F, 0x01},   // J     42   4a
        {0x7F, 0x08, 0x14, 0x22, 0x41},   // K     43   4b
        {0x7F, 0x40, 0x40, 0x40, 0x40},   // L     44   4c
        {0x7F, 0x02, 0x0C, 0x02, 0x7F},   // M     45   4d
        {0x7F, 0x04, 0x08, 0x10, 0x7F},   // N     46   4e
        {0x3E, 0x41, 0x41, 0x41, 0x3E},   // O     47   4f
        {0x7F, 0x09, 0x09, 0x09, 0x06},   // P     48   50
        {0x3E, 0x41, 0x51, 0x21, 0x5E},   // Q     49   51
        {0x7F, 0x09, 0x19, 0x29, 0x46},   // R     50   52
        {0x46, 0x49, 0x49, 0x49, 0x31},   // S     51   53
        {0x01, 0x01, 0x7F, 0x01, 0x01},   // T     52   54
        {0x3F, 0x40, 0x40, 0x40, 0x3F},   // U     53   55
        {0x1F, 0x20, 0x40, 0x20, 0x1F},   // V     54   56
        {0x3F, 0x40, 0x38, 0x40, 0x3F},   // W     55   57
        {0x63, 0x14, 0x08, 0x14, 0x63},   // X     56   58
        {0x07, 0x08, 0x70, 0x08, 0x07},   // Y     57   59
        {0x61, 0x51, 0x49, 0x45, 0x43},   // Z     58   5a
        {0x00, 0x7F, 0x41, 0x41, 0x00},   // [     59   5b
        {0x55, 0xAA, 0x55, 0xAA, 0x55},   //       60   5c Backslash (Checker pattern)
        {0x00, 0x41, 0x41, 0x7F, 0x00},   // ]     61   5d
        {0x04, 0x02, 0x01, 0x02, 0x04},   // ^     62   5e
        {0x40, 0x40, 0x40, 0x40, 0x40},   // _     63   5f underscore
        {0x00, 0x03, 0x05, 0x00, 0x00},   // `     64   60 apostrophe
        {0x20, 0x54, 0x54, 0x54, 0x78},   // a     65   61
        {0x7F, 0x48, 0x44, 0x44, 0x38},   // b     66   62
        {0x38, 0x44, 0x44, 0x44, 0x20},   // c     67   63
        {0x38, 0x44, 0x44, 0x48, 0x7F},   // d     68   64
        {0x38, 0x54, 0x54, 0x54, 0x18},   // e     69   65
        {0x08, 0x7E, 0x09, 0x01, 0x02},   // f     70   66
        {0x18, 0xA4, 0xA4, 0xA4, 0x7C},   // g     71   67
        {0x7F, 0x08, 0x04, 0x04, 0x78},   // h     72   68
        {0x00, 0x44, 0x7D, 0x40, 0x00},   // i     73   69
        {0x40, 0x80, 0x84, 0x7D, 0x00},   // j     74   6a
        {0x7F, 0x10, 0x28, 0x44, 0x00},   // k     75   6b
        {0x00, 0x41, 0x7F, 0x40, 0x00},   // l     76   6c
        {0x7C, 0x04, 0x18, 0x04, 0x78},   // m     77   6d
        {0x7C, 0x08, 0x04, 0x04, 0x78},   // n     78   6e
        {0x38, 0x44, 0x44, 0x44, 0x38},   // o     79   6f
        {0xFC, 0x24, 0x24, 0x24, 0x18},   // p     80   70
        {0x18, 0x24, 0x24, 0x18, 0xFC},   // q     81   71
        {0x7C, 0x08, 0x04, 0x04, 0x08},   // r     82   72
        {0x48, 0x54, 0x54, 0x54, 0x20},   // s     83   73
        {0x04, 0x3F, 0x44, 0x40, 0x20},   // t     84   74
        {0x3C, 0x40, 0x40, 0x20, 0x7C},   // u     85   75
        {0x1C, 0x20, 0x40, 0x20, 0x1C},   // v     86   76
        {0x3C, 0x40, 0x30, 0x40, 0x3C},   // w     87   77
        {0x44, 0x28, 0x10, 0x28, 0x44},   // x     88   78
        {0x1C, 0xA0, 0xA0, 0xA0, 0x7C},   // y     89   79
        {0x44, 0x64, 0x54, 0x4C, 0x44},   // z     90   7a
        {0x00, 0x10, 0x7C, 0x82, 0x00},   // {     91   7b
        {0x00, 0x00, 0xFF, 0x00, 0x00},   // |     92   7c
        {0x00, 0x82, 0x7C, 0x10, 0x00},   // }     93   7d
        {0x00, 0x06, 0x09, 0x09, 0x06}};  // ~     94   7e

const uint8_t BigDigitFont[][36] PROGMEM = { // each SSD1306 data byte is 8 vertical pixels.
// 12 columns * 3 rows = 36 bytes per digit [ * 12 digit characters__432 bytes total ]
//         0    1    2    3    4    5    6    7    8    9   10   11
    {0xfc,0xfe,0xfe,0x0f,0x07,0x07,0x07,0x07,0xcf,0xfe,0xfc,0xf8},  // character '0'
    {0xff,0xff,0xff,0xf0,0x78,0x3c,0x1e,0x0f,0x07,0xff,0xff,0xff},  //
    {0x3f,0x7f,0x7f,0xf8,0xf0,0xf0,0xf0,0xf0,0xf8,0x7f,0x7f,0x1f},  //

    {0x00,0x00,0x40,0x60,0x70,0x78,0xfc,0xfe,0xff,0xff,0x00,0x00},  // character '1'
    {0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00},  //
    {0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00},  //

    {0x38,0x3c,0x3e,0x0f,0x07,0x07,0x07,0x0f,0x1f,0xff,0xfe,0xfc},  // character '2'
    {0xc0,0xe0,0xe0,0xf0,0x78,0x78,0x3c,0x1e,0x0f,0x0f,0x07,0x03},  //
    {0xff,0xff,0xff,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0},  //

    {0x38,0x3c,0x1e,0x0f,0x0f,0x0f,0x0f,0x0f,0x1e,0xfc,0xf8,0xf0},  // character '3'
    {0x00,0x00,0x00,0x1e,0x1e,0x1e,0x1e,0x3e,0xff,0xff,0xe7,0x80},  //
    {0x78,0xf8,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf8,0xff,0x7f,0x1f},  //

    {0x00,0x00,0x00,0x00,0x00,0xc0,0xf0,0xf8,0x7e,0xff,0xff,0xff},  // character '4'
    {0xe0,0xf0,0xf8,0xbe,0x9f,0x8f,0x83,0x81,0x80,0xff,0xff,0xff},  //
    {0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0xff,0xff,0xff},  //

    {0xff,0xff,0xff,0x87,0x87,0x87,0x87,0x87,0x87,0x07,0x07,0x07},  // character '5'
    {0x07,0x07,0x03,0x03,0x03,0x03,0x03,0x03,0x07,0xff,0xff,0xfc},  //
    {0x3c,0x7c,0x78,0xf0,0xf0,0xf0,0xf0,0xf0,0x78,0x7f,0x3f,0x1f},  //

    {0xf8,0xfc,0xfe,0x0f,0x07,0x07,0x07,0x07,0x07,0x0f,0x1e,0x1c},  // character '6'
    {0xff,0xff,0xff,0x1e,0x0e,0x0e,0x0e,0x0e,0x1e,0xfe,0xfc,0xf8},  //
    {0x3f,0x7f,0x7f,0xf0,0xe0,0xe0,0xe0,0xe0,0xf8,0x7f,0x3f,0x1f},  //

    {0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0xff,0xff,0xff,0xff},  // character '7'
    {0x00,0x00,0x00,0x00,0x80,0xe0,0xf8,0xfe,0x7f,0x1f,0x0f,0x03},  //
    {0x00,0xc0,0xf0,0xfe,0xff,0x1f,0x07,0x01,0x00,0x00,0x00,0x00},  //

    {0xf8,0xfe,0xff,0x07,0x07,0x07,0x07,0x07,0x8f,0xff,0xfe,0x78},  // character '8'
    {0xe0,0xff,0xff,0x1f,0x0e,0x0e,0x0e,0x1f,0x3f,0xff,0xf9,0xe0},  //
    {0x1f,0x7f,0x7f,0xf0,0xe0,0xe0,0xe0,0xf0,0x78,0x7f,0x3f,0x1f},  //

    {0xfc,0xfe,0xff,0x0f,0x07,0x07,0x07,0x07,0x0f,0xff,0xfc,0xf8},  // character '9'
    {0x1f,0x3f,0x3f,0x7c,0x78,0x78,0x78,0x78,0x3c,0xff,0xff,0xff},  //
    {0x00,0x70,0xf0,0xf0,0xf0,0xf0,0x70,0x78,0x7c,0x3f,0x3f,0x0f},  //

    {0x00,0x00,0x00,0x60,0xf0,0xf8,0xf8,0xf0,0x60,0x00,0x00,0x00},  // character ':'
    {0x00,0x00,0x00,0x00,0x80,0xc1,0xc1,0x80,0x00,0x00,0x00,0x00},  //
    {0x00,0x00,0x00,0x03,0x07,0x0f,0x0f,0x07,0x03,0x00,0x00,0x00},  //

    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // character ' '
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  //
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};


//~~~~~~~~~~~~~~~~~~~~~~~LOW-level OLED_I2C  @ 180 lines of code ~~~~
void i2c_initialise()  {   //  should called at the beginning of main() {Arduino:  by setup() when not using Wire.h}
        TWBR = 12;         //  400KHz   use 0x62 = @80KHz
        TWSR = 0x01;
        TWCR = (1<<TWEN);
}//~~~~~~~~~~~~~~~~~~~~~ LOW-level OLED_I2C ~~~~~~~~~~~~~~~~~~~~~~~~
void OLED_sendCommand(uint8_t myCmd)   {
        TWCR=((1<<TWINT)|(1<<TWSTA)|(1<<TWEN)); // send START
        while(!(TWCR & (1 << TWINT)));  // delay until TWINT set
        TWDR = OLED_SLA7 << 1;   // send SLA    8-bit SLAw = 0x78
        TWCR = ((1<<TWINT)|(1<<TWEN)); while(!(TWCR & (1<<TWINT)));

        TWDR = SSD1306_COMMAND; // send D/C_sentinel byte for command:0x00
        TWCR = ((1<<TWINT)|(1<<TWEN)); while(!(TWCR & (1<<TWINT)));

        TWDR = myCmd;   // I2C write, wait until completed
        TWCR = ((1<<TWINT)|(1<<TWEN)); while(!(TWCR & (1<<TWINT)));

        TWCR = ( (1<<TWINT)|(1<<TWEN)|(1<<TWSTO) ); // I2C STOP
}//~~~~~~~~~~~~~~~~~~~~~ LOW-level OLED_I2C ~~~~~~~~~~~~~~~~~~~~~~~~
void OLED_clear() {
        OLED_sendCommand(SSD1306_SET_COLUMN_ADDR);   // opcode:0x21
        OLED_sendCommand(0);     // start at left edge of screen
        OLED_sendCommand(127);   // end at right edge of screen

        OLED_sendCommand(SSD1306_SET_PAGE_ADDR);     // opcode:0x22
        OLED_sendCommand(0);     // start at the top row
        OLED_sendCommand(7);     // end at the bottom row

        TWCR=((1<<TWINT)|(1<<TWSTA)|(1<<TWEN)); // send START
        while(!(TWCR & (1 << TWINT)));  // delay until TWINT set
        TWDR = OLED_SLA7 << 1;   // send SLA  8-bit SLAw = 0x78
        TWCR = ((1<<TWINT)|(1<<TWEN)); while(!(TWCR & (1<<TWINT)));

        TWDR = SSD1306_DATA_CONTINUE;   // send D/C sentinal byte 0x40
        TWCR = ((1<<TWINT)|(1<<TWEN)); while(!(TWCR & (1<<TWINT)));

        // TWI NOT using Wire.h   128 bytes (8 bits vertical) per row * 8 char rows = 1024
        // Send 64 TWI messages { START:SLAw:0x40:16 bytes of 0x00:STOP }  1024 data bytes total
        for (uint16_t scrClearIndex=0; scrClearIndex<1024; scrClearIndex++) {  // (SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8)
            TWCR=((1<<TWINT)|(1<<TWSTA)|(1<<TWEN)); // send START
            while(!(TWCR & (1 << TWINT)));  // delay until TWINT set
            TWDR = OLED_SLA7 << 1;   // 8-bit SLAw = 0x78
            TWCR = ((1<<TWINT)|(1<<TWEN)); while(!(TWCR & (1<<TWINT))); // TX?done delay

            TWDR = 0x40;  // send D/C_sentinal byte for data stream
            TWCR = ((1<<TWINT)|(1<<TWEN)); while(!(TWCR & (1<<TWINT)));

            for (uint8_t x=0; x<16; x++) {
                TWDR = 0x00;  // all eight pixels in the column are off.
                TWCR = ((1<<TWINT)|(1<<TWEN)); while(!(TWCR & (1<<TWINT)));
                scrClearIndex++;  // 16 bytes per I2C transmission__64 transmissions per screen
            }
            scrClearIndex--;
            TWCR = ( (1<<TWINT)|(1<<TWEN)|(1<<TWSTO) ); // I2C STOP
        }
}//~~~~~~~~~~~~~~~~~~~~~ LOW-level OLED_I2C ~~~~~~~~~~~~~~~~~~~~
void OLED_Init(void) {  // cmd: send START, 8bitSLA, D/C sentinal=0x00, command_value, STOP
        OLED_sendCommand(SSD1306_DISPLAY_OFF);          //   0xAE
        OLED_sendCommand(SSD1306_SET_CLOCK_DIV_RATIO);  //   0xD5
        OLED_sendCommand(0x80);                         //   0x80
        OLED_sendCommand(SSD1306_SET_MULTIPLEX_RATIO);  //   0xA8
        OLED_sendCommand(0x3F);                         //   0x3F
        OLED_sendCommand(SSD1306_SET_DISPLAY_OFFSET);   //   0xD3
        OLED_sendCommand(0x0);                          //   0x00
        OLED_sendCommand(SSD1306_SET_START_LINE | 0x00); //   0x40
        OLED_sendCommand(SSD1306_CHARGE_PUMP);          //   0x8D
        OLED_sendCommand(0x14);                         //   0x14
        OLED_sendCommand(SSD1306_MEMORY_ADDR_MODE);     //   0x20
        OLED_sendCommand(0x00);                         //   0x00
        OLED_sendCommand(SSD1306_SET_SEGMENT_REMAP | 0x01);// 0xA1
        OLED_sendCommand(SSD1306_COM_SCAN_DIR_DEC);     //   0xC8
        OLED_sendCommand(SSD1306_SET_COM_PINS);         //   0xDA
        OLED_sendCommand(0x12);                         //   0x12
        OLED_sendCommand(SSD1306_SET_CONTRAST_CONTROL); //   0x81
        OLED_sendCommand(0xCF);                         //   0xCF
        OLED_sendCommand(SSD1306_SET_PRECHARGE_PERIOD); //   0xD9
        OLED_sendCommand(0xF1);                         //   0xF1
        OLED_sendCommand(SSD1306_SET_VCOM_DESELECT);    //   0xDB
        OLED_sendCommand(0x40);                         //   0x40
        OLED_sendCommand(SSD1306_DISPLAY_ALL_ON_RESUME);//   0xA4
        OLED_sendCommand(SSD1306_NORMAL_DISPLAY);       //   0xA6
        OLED_sendCommand(SSD1306_DISPLAY_ON);           //   0xAF
        OLED_clear();
}//~~~~~~~~~~~~~~~~~~~~~~ LOW-level OLED_I2C ~~~~~~~~~~~~~~~~~~~~~
void OLED_SetCursor(uint8_t myLineNumber,uint8_t myColumnPosition) {
        if( (myLineNumber <= SSD1306_LASTLINE) && (myColumnPosition <= 127) ) {
            OLEDlineNumber = myLineNumber;     // global var: current line number
            OLEDcursorPos =  myColumnPosition; // global var: current cursor position

            OLED_sendCommand(SSD1306_SET_COLUMN_ADDR);
            OLED_sendCommand(myColumnPosition);
            OLED_sendCommand(127);

            OLED_sendCommand(SSD1306_SET_PAGE_ADDR);
            OLED_sendCommand(myLineNumber);
            OLED_sendCommand(7);

            TWCR=((1<<TWINT)|(1<<TWSTA)|(1<<TWEN)); // send START
            while(!(TWCR & (1 << TWINT)));  // delay until TWINT set
            TWDR = OLED_SLA7 << 1;   // 8-bit SLAw = 0x78  7-bitSLA=0x3C  SSD1306 controller
            TWCR = ((1<<TWINT)|(1<<TWEN)); while(!(TWCR & (1<<TWINT)));

            TWDR = SSD1306_DATA_CONTINUE;   // 0x40
            TWCR = ((1<<TWINT)|(1<<TWEN)); while(!(TWCR & (1<<TWINT)));  // TWINT?set delay
        }
}//~~~~~~~~~~~~~~~~~~~~~~ LOW-level OLED_I2C ~~~~~~~~~~~~~~~~~~~~~~~
void OLED_GoToLine(uint8_t myLine) {
        if( myLine < 8) {
            OLEDlineNumber = myLine;
            OLED_SetCursor(OLEDlineNumber, 0);
        }
}//~~~~~~~~~~~~~~~~~~~~~~ LOW-level OLED_I2C ~~~~~~~~~~~~~~~~~~~~~~~
void OLED_GoToNextLine() {      // Increment and roll-over the current line number.
        OLEDlineNumber++;       // global var for current line number
        OLEDlineNumber = OLEDlineNumber & 0x07; // roll-over back to top after 8 lines
        OLED_SetCursor(OLEDlineNumber, 0);
}//~~~~~~~~~~~~~~~~~~~~~~ LOW-level OLED_I2C ~~~~~~~~~~~~~~~~~~~~~~~~~~
void OLED_DisplayChar(uint8_t myDisplayChar) {
        uint8_t charColumnBitMap, charBitMapIndex = 0;

        if(((OLEDcursorPos + FONT_SIZE) >= 128) || (myDisplayChar=='\n')) { // font size=5
//      If the line has no more room for complete chars OR NewLine command,
//      then move the cursor to next line
            OLED_GoToNextLine();
        }
        if(myDisplayChar != '\n') {
            myDisplayChar = myDisplayChar - 0x20; // As the lookup table starts from Space(0x20)
            while(1) {
               charColumnBitMap = pgm_read_byte(&FontTable[myDisplayChar][charBitMapIndex]);
               TWDR = charColumnBitMap;
               TWCR = ((1<<TWINT)|(1<<TWEN)); while(!(TWCR & (1<<TWINT))); // TX?done delay
               OLEDcursorPos++;
               charBitMapIndex++;
               if( charBitMapIndex == FONT_SIZE) {   // Exit the loop after sending five bytes
                  TWDR = 0x00;  // last column of char is one column of space between chars.
                  TWCR = ((1<<TWINT)|(1<<TWEN)); while(!(TWCR & (1<<TWINT))); // TX?done delay
                  OLEDcursorPos++;
                  break;
               }    }    }
}//~~~~~~~~~~~~~~~~~~~~~ LOW-level OLED_I2C ~~~~~~~~~~~~~~~~~~~~~~~~~~
void OLED_DisplayString(uint8_t *ptr) {
        while(*ptr) OLED_DisplayChar(*ptr++);
}//~~~~~~~~~~~~~~~~~~~~~ LOW-level OLED_I2C ~~~~~~~~~~~~~~~~~~~~~~~~~

void displayBigDigit(uint8_t myDigit, uint8_t myDigitRow, uint8_t myDigitColumn) {
        uint8_t charColumnBitMap, columnIndex = 0, rowIndex = 0, fullDigitIndex=0;
        
        if ( (myDigitRow <= 5) && (myDigitColumn <= 116) ) { // full digit gets displayed
            if (myDigit >= 0 && myDigit <= 9 ) {
              for (rowIndex=0; rowIndex<3; rowIndex++) {
                 OLED_SetCursor(myDigitRow + rowIndex, myDigitColumn);
                 for (columnIndex = 0; columnIndex < 12; columnIndex++) {
                    charColumnBitMap = pgm_read_byte(&BigDigitFont[myDigit][fullDigitIndex]);
                    TWDR = charColumnBitMap;
                    TWCR = ((1<<TWINT)|(1<<TWEN)); while(!(TWCR & (1<<TWINT))); // TX?done delay
                    fullDigitIndex++;
                 }
              }
            }
            
            if (myDigit == 10 ) {
               for (rowIndex=0; rowIndex<3; rowIndex++) {
                  OLED_SetCursor(myDigitRow + rowIndex, myDigitColumn);
                  for (columnIndex = 0; columnIndex < 12; columnIndex++) {
                      charColumnBitMap = pgm_read_byte(&BigDigitFont[10][fullDigitIndex]);
                      TWDR = charColumnBitMap;
                      TWCR = ((1<<TWINT)|(1<<TWEN)); while(!(TWCR & (1<<TWINT))); // TX?done delay
                      fullDigitIndex++;
                  }
               }
            }    // myDigit 10
              
            if (myDigit == 11 ) {
               for (rowIndex=0; rowIndex<3; rowIndex++) {
                  OLED_SetCursor(myDigitRow + rowIndex, myDigitColumn);
                  for (columnIndex = 0; columnIndex < 12; columnIndex++) {
                      charColumnBitMap = pgm_read_byte(&BigDigitFont[11][fullDigitIndex]);
                      TWDR = charColumnBitMap;
                      TWCR = ((1<<TWINT)|(1<<TWEN)); while(!(TWCR & (1<<TWINT))); // TX?done delay
                      fullDigitIndex++;
                  }
               }
            }   // myDigit 11
        }   // myDigitRow  5..116
}/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */


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