#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "Arduino.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 21
#define TEMPERATURE_RESOLUTION 12

/**
 * Inherits from DallasTemperature to read, control and return temperature related information.
 * 
 * @param ONE_WIRE_BUS Specifies the microcontrollers one wire bus pin.
 * @param TEMPERATURE_RESOLUTION Resolves the bit resolution of the temperature reading.
 */
class Temperature : public DallasTemperature{
  DallasTemperature sensors;
  double tempC = 0.00;
public:
  Temperature(OneWire* oneWire);
  double measure();
  double get();
};

#endif
