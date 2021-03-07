#include "temperature.h"

Temperature::Temperature(OneWire* oneWire): sensors(oneWire){
  sensors.begin();
  sensors.setResolution(TEMPERATURE_RESOLUTION);
}

void Temperature::measure(){
  /**
   * Get the current temperature inside the Phenobottle.
   * Current temperature is printed to the serial output for reading by user or external script.
   */
  sensors.requestTemperatures();
  tempC = sensors.getTempCByIndex(0); // Index 0 for single Temperature Probe
  Serial.println(tempC);
}

double Temperature::get(){
   /**
   * Get the last temperature reading.
   * @returns The last temperature reading in degrees celcius.
   */
  return tempC;
}
