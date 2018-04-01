/**********************************************
* INA219 library example
*
* this code is public domain.
**********************************************/

#include "ina219.h"

INA219 monitor(INA219::I2C_ADDR_40);

void setup()
{
  Serial.begin( 115200 );
  monitor.begin();
  monitor.configure();
  monitor.calibrate();
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

  delay(1000);
}


