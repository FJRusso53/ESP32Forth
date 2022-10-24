
/* userwords2.h 
FJRusso 220615
 To simplify calling C functions, you can also refer to elements on the stack with the types positional names:

n10 n9 n8 n7 n6 n5 n4 n3 n2 n1 n0 - Access stack as cell_t integer values
                   c4 c3 c2 c1 c0 - Access stack as char* values
                   b4 b3 b2 b1 b0 - Access stack as uint8_t* byte values
             a6 a5 a4 a3 a2 a1 a0 - Access stack
Ln 1489 ( Stack Convience )
: .. sp0 sp! ; ( Clear Stack )
*/
//
// USER DEFINED FUNCTIONS
// #include "thermistor.h"
#ifndef THERMISTOR_SUPPORT 
float TCT(int _na) {return -1.0; };
void TCSetup() { } ;
#endif
//
// #include "MAX6675-collect.h"
#ifndef MAX6675_SUPPORT
void MAXTCsetup(int _na0, int _na1, int _na2, int _na3){ }; 
float TCData(int _na4){return 0.0; };
#endif
//
// #include "DS18B20-R.h"
#ifndef DS_Support 
void DSsetup(int){ };
float DSTemp(int) {return 0.0; };
#endif
//
/* #include <LiquidCrystal_I2C.h>
 #include <wire.h>
 LiquidCrystal_I2C lcd(0x27,16,2); // ( address, cols, rows ) 
 #ifndef LED1602_Support 
 void Wire.begin(uint8_t, uint8_t);     // Define SDA pin 13 / SCL pin 14
 void lcd.init();                       // LCD driver initialization
 void lcd.backlight();                  // Open the backlight
 void lcd.setCursor(uint8_t, uint8_t);  // Move the cursor to row 0, column 0
 void lcd.print(const char[]);          // The print content is displayed on the LCD
 void lcd.clear();                      // Clear Display
 #endif
*/
#define USER_WORDS2 \
  Y(RROTATE, n2 = n1; n1 = ( (n1 >> n0) | (n2 << (32-n0) ) ); DROP )\
  Y(LROTATE, n2 = n1; n1 = ( (n1 << n0) | (n2 >> (32-n0) ) ); DROP )\
  Y(STRLEN, DUP; n0 = strlen(c0))\
  Y(STRCMP,  w = strcmp(c1, c0); DROP; n0 = w )\
  Y(STRSTR, c1 = strstr(c1, c0); DROP )\
  Y(STRCHR, c1 = strchr(c1, n0); DROP )\
  Y(TCBEGIN, TCSetup(); )\
  Y(TCTemp, n0 = TCT( n0) * 10; )\
  Y(MAXBEGIN, MAXTCsetup(n3, n2, n1, n0); DROPn(4) )\
  Y(MAXTCData, n0 = TCData(n0)* 10; )\
  Y(DSstart, DSsetup(n0); )\
  Y(DStempF, n0 = DSTemp(n0)* 10; )\
  Y(ESPrestart, ESP.restart(); DROP ) 
/* Y(LCDINIT, Wire.begin(n1, n0); lcd.init(); lcd.backlight(); DROPn(2) )\
   Y(LCDSetCursor, lcd.setCursor(n1, n0); DROPn(2))\
   Y(LCDPRINT, lcd.print(c0); DROP )\
   Y(LCDCLEAR, lcd.clear(); )\
   Y(LCDBKLIGHT, lcd.backlight(); )\
*/
// -- END OF FILE --
