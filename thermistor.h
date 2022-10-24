// Support for use of Thermistor
#define THERMISTOR_SUPPORT
#include "esp_adc_cal.h"
#define DEFAULT_VREF    1100                //Default vref
#define NO_OF_SAMPLES   64                  //Multisampling
adc_channel_t channel = ADC_CHANNEL_0;      // ADC1:GPIO36, ADC2:GPIO4
adc_unit_t unit = ADC_UNIT_2;               // ADC2
adc_atten_t atten = ADC_ATTEN_DB_11;        // Full scale 0-3.9V, precision range 150mV-2450mV
esp_adc_cal_characteristics_t *adc_chars;
esp_adc_cal_value_t val_type;
//
float TCV( float) ;   // Prototype declaration
float TCR( float) ;   // Prototype declaration
float TCT( float) ;   // Prototype declaration 
float TCK( float) ;   // Prototype declaration
float TCC( float) ;   // Prototype declaration
float TCF( float) ;   // Prototype declaration
void TCSetup() ;      // Prototype declaration
float TCSample() ;    // Prototype declaration
float TCSample() {
    uint32_t adc_reading = 0;
    //Multisampling
    for (int i = 0; i < NO_OF_SAMPLES; i++) {
        if (unit == ADC_UNIT_1) {
            adc_reading += adc1_get_raw((adc1_channel_t)channel);
        }
        else {
            int raw;
            adc2_get_raw((adc2_channel_t)channel, ADC_WIDTH_BIT_12, &raw);
            adc_reading += raw;
        }
    }
    adc_reading /= NO_OF_SAMPLES;
    //Convert adc_reading to voltage in mV
    return (esp_adc_cal_raw_to_voltage(adc_reading, adc_chars))/ 1000.0f ;
}
void TCSetup() {
    Serial.begin(115200);
    if (unit == ADC_UNIT_1) {
        adc1_config_width(ADC_WIDTH_BIT_12);
        adc1_config_channel_atten((adc1_channel_t)channel, atten);
    }
    else {
        adc2_config_channel_atten((adc2_channel_t)channel, atten);
    }
    //Characterize ADC
    adc_chars = (esp_adc_cal_characteristics_t*)calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
}
//
float TCV() { return TCSample() ; }                                                        // Calc & return Voltage from input of TC
float TCR( float calcV) { return ( 10 * calcV / ( 3.3 - calcV ) ) ; }                       // Calc & return resistance from calc voltage
float TCK( float calcR) { return ( 1 / (1 / (273.15 + 25 ) + log( calcR/10 ) / 3950.0)); }  // Using the calculated resistance determine the temperature �K
float TCC( float calcK) { return ( calcK - 273.15 ); }                                      // Calculate the temperature �C
float TCF( float calcC) { return ( ( calcC * 9 / 5 ) + 32 ); }                              // Calculate temperature �F
float TCT( int DegUnit )  {
      float TempResult = 0.0 ;
      float TCVolt = TCV();
      float TCRes = TCR( TCVolt );
      switch ( DegUnit )  {
        case 0:
          TempResult = TCK( TCRes);
        break;
//
        case 1:
          TempResult = TCK( TCRes);
          TempResult = TCC( TempResult);
        break;
//
        case 2:
          TempResult = TCK( TCRes);
          TempResult = TCC( TempResult);
          TempResult = TCF( TempResult);
        break;
//        
        default: TempResult = 0.0; break;
}
      return TempResult;
}
// End of File
