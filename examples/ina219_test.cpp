/**********************************************
* INA219 library example
* 10 May 2012 by johngineer
*
* this code is public domain.
**********************************************/


#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sstream>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "simplemqttclient.h"
#include "../ina219.h"


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
INA219 ina219;
SimpleMqttClient *smqtt;


void setup(int file)
{
  config_t config = default_config;
  ina219.begin(file);
  ina219.configure( INA219::RANGE_16V, INA219::GAIN_1_40MV, INA219::ADC_64SAMP, INA219::ADC_64SAMP, INA219::CONT_SH_BUS );
  ina219.calibrate( config.r_shunt, config.v_shunt_max, config.v_bus_max, config.i_max_expected);

  smqtt = new SimpleMqttClient("localhost","ueyystusi");
  smqtt->connectToMqttServer();
}

void loop()
{

  std::ostringstream s1,s2;

  s1 << ina219.shuntCurrent();
  s2 << ina219.busVoltage();


  smqtt->publishToMqttServer("battery/current",s1.str());
  smqtt->publishToMqttServer("battery/voltage",s2.str());

  printf("******************\n");
  printf("raw shunt voltage:%d\n",ina219.shuntVoltageRaw());
  printf("raw bus voltage:  %d mV\n",ina219.busVoltageRaw());
  printf("shunt voltage:    %f mV\n",(ina219.shuntVoltage() * 1000));
  printf("shunt current:    %f mA\n",ina219.shuntCurrent() * 1000);
  printf("bus voltage:      %f V\n",ina219.busVoltage());
  printf("bus power:        %f mW\n", ina219.busPower() * 1000);

  sleep(2);
}


int main(int argc, char *argv[])
{
    const char *buffer;
    const char * devName = "/dev/i2c-4";
    int val = 0;

    // Open up the I2C bus
    int file = open(devName, O_RDWR);
    if (file == -1)
    {
        perror(devName);
        exit(1);
    }
    printf("*** Device '%s' opened successfully.\r\n", devName);
    // Specify the address of the slave device.
    if (ioctl(file, I2C_SLAVE, MAG_ADDRESS) < 0)
    {
        perror("Failed to acquire bus access and/or talk to slave");
        exit(1);
    }

    printf("*** Acquired bus access to a slave device.\r\n");
    setup(file);
    while(1)
    {
      loop();
    }

    close(file);
    delete smqtt;
    return 0;
}
