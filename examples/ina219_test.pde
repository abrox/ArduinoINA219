/**********************************************
* INA219 library example
* 10 May 2012 by johngineer
*
* this code is public domain.
**********************************************/


#include <Wire.h>
#include <ina219.h>

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
INA219 monitor;


void setup()
{
  Serial.begin(115200);
  monitor.begin();
  config = default_config;
  ina219.configure( INA219::RANGE_16V, INA219::GAIN_1_40MV, INA219::ADC_64SAMP, INA219::ADC_64SAMP, INA219::CONT_SH_BUS );
  ina219.calibrate( config.r_shunt, config.v_shunt_max, config.v_bus_max, config.i_max_expected);
    
  

}

void loop()
{

  Serial.println("******************");
  
  Serial.print("raw shunt voltage: ");
  Serial.println(monitor.shuntVoltageRaw());
  
  Serial.print("raw bus voltage:   ");
  Serial.println(monitor.busVoltageRaw());
  
  Serial.println("--");
  
  Serial.print("shunt voltage: ");
  Serial.print(monitor.shuntVoltage() * 1000, 4);
  Serial.println(" mV");
  
  Serial.print("shunt current: ");
  Serial.print(monitor.shuntCurrent() * 1000, 4);
  Serial.println(" mA");
  
  Serial.print("bus voltage:   ");
  Serial.print(monitor.busVoltage(), 4);
  Serial.println(" V");
  
  Serial.print("bus power:     ");
  Serial.print(monitor.busPower() * 1000, 4);
  Serial.println(" mW");
  
  Serial.println(" ");
  Serial.println(" ");
  
  delay(10000);

}


