#include "temperature.h"

Temperature::Temperature(OneWire* oneWire): sensors(oneWire){
  sensors.begin();
  sensors.setResolution(TEMPERATURE_RESOLUTION);
}

double Temperature::measure(){
  /**
   * Get the current temperature inside the Phenobottle.
   * Current temperature is printed to the serial output for reading by user or external script.
   * 
   * @return tempC The current temperature in degrees celsius
   */
  sensors.requestTemperatures();
  tempC = sensors.getTempCByIndex(0); // Index 0 for single Temperature Probe
  Serial.println(tempC);
  
  return tempC;
}

double Temperature::get(){
   /**
   * Get the last temperature reading.
   * 
   * @returns The last temperature reading in degrees celsius.
   */
  return tempC;
}
