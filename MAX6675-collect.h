//
// FILE: MAX6675-collect.h
// AUTHOR: Rob Tillaart 
// Modified by FJRusso 220430
// PURPOSE: demo
// DATE: 2022-01-12
// URL: https://github.com/RobTillaart/MAX6675
//
#define MAX6675_SUPPORT
#include "MAX6675.h"
//
// note: pins are slightly different than other examples!
// int dataPin ;  //   = 36 ; // 4;
// int clockPin ; // = 14 ; // 18;
// int selectPin ; //  = 22 ; // 5;
const int nr_samples = 16 ;
//
MAX6675 TC_HLT;  // Hot Water Tank
MAX6675 TC_Mash; // Mash Tank
MAX6675 TC_Boil; // Boil Tank
MAX6675 TCSelect; // To be set to one of the objects above
//
void MAXTCsetup( int, int, int, int); // Prototype definition
float TCData( int); // Prototype definition
//
float _readings = 0.0;
int tcstatus ;
float temp ;
//
void MAXTCsetup( int MaxTCSelect, int clockPin, int selectPin, int dataPin) //  Order of pins is IMPORTANT:  MAXTCsetup ( TC#, Clk, Select, Data --  )
{
  Serial.begin(115200);
  switch ( MaxTCSelect )  
  {
        case 1:
          TC_Mash.begin(clockPin, selectPin, dataPin);
          TC_Mash.setSPIspeed(4000000);
        break;
//
        case 2:
          TC_Boil.begin(clockPin, selectPin, dataPin);
          TC_Boil.setSPIspeed(4000000);
        break;
//
        default: 
          TC_HLT.begin(clockPin, selectPin, dataPin);
          TC_HLT.setSPIspeed(4000000);
        break;
//
  }
}
//
float TCData( int MaxTCSelect)
{
    _readings = 0.0;
    for (int i = 0; i < nr_samples; i++) 
      {
          switch ( MaxTCSelect )  
  {
        case 1:
          tcstatus = TC_Mash.read();
          temp = TC_Mash.getTemperature();
        break;
//
        case 2:
          tcstatus = TC_Boil.read();
          temp = TC_Boil.getTemperature();
        break;
//
        default: 
           tcstatus = TC_HLT.read();
          temp = TC_HLT.getTemperature();
        break;
//
  }
      _readings += temp;
      delay (300);
      }
  _readings /= nr_samples;
  return _readings;
}
// -- END OF FILE --
