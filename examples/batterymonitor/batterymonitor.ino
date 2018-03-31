/**********************************************
* INA219 library example
*
* this code is public domain.
**********************************************/

#include "ina219.h"


#define MAG_ADDRESS        0x40

#define byte unsigned char
#define SHUNT_MAX_V 0.04 /*Rated max for our shunt is 75mv for 50 A current*/
#define BUS_MAX_V   16.0  /* with 12v lead acid battery this should be enough*/
#define MAX_CURRENT 20  /* In our case this is enaugh even shunt is capable to 50 A*/
#define SHUNT_R     0.0015 /*Shunt resistor in ohm*/


static const byte config_rev = 1; /*Config revision, in case change struct below You nust change revision too */
typedef struct 
{
    byte  rev;            ///< Configuration revision.
    float r_shunt;        ///< Value of shunt in Ohms. 
    float v_shunt_max;    ///< Maximum value of voltage across shunt. Rated max for our shunt is 75mv for 50 A current
    float v_bus_max;      ///< Maximum voltage of bus.with 12v lead acid battery 16v should be enough
    float peukeurt;
    float i_max_expected; ///< Maximum current draw of bus + shunt,In our case 20 is enaugh even shunt is capable to 50 A
    int bat_capacity;     ///< Battery size
    byte crc;
} config_t;

const config_t default_config={config_rev,SHUNT_R,SHUNT_MAX_V,BUS_MAX_V,1.1,MAX_CURRENT,60,0};
config_t config={0};
INA219 ina219(INA219::I2C_ADDR_40);

float shuntvoltage = 0;
float busvoltage = 0;
float current = 0;
float buspower = 0;

void sendCurrent(){
   Serial.print("Bus:     "); Serial.print(busvoltage); Serial.print(" V");
   Serial.print("Shunt:   "); Serial.print(shuntvoltage); Serial.print(" mV ");
   Serial.print("Current: "); Serial.print(current); Serial.print(" mA ");
   Serial.print("Power:   "); Serial.print(buspower); Serial.println(" mW ");
}


void meassureCurrent(){
    shuntvoltage = ina219.shuntVoltage()*1000.0;
    busvoltage   = ina219.busVoltage();
    current      = ina219.shuntCurrent();
    buspower     = ina219.busPower()*1000.0;
    sendCurrent();
}

void setup()
{
  config_t config = default_config;
  ina219.begin();
  ina219.configure( INA219::RANGE_16V, INA219::GAIN_1_40MV, INA219::ADC_64SAMP, INA219::ADC_64SAMP, INA219::CONT_SH_BUS );
  ina219.calibrate( config.r_shunt, config.v_shunt_max, config.v_bus_max, config.i_max_expected);
  Serial.begin( 115200 );

}

void loop()
{
    meassureCurrent();
    sendCurrent();
    delay(1000);
}


