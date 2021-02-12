#include <OneWire.h>
#include <DallasTemperature.h>

#include "lights.h"
#include "parser.h"

#define ONE_WIRE_BUS 21
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

unsigned int ledPin = 13;

Parser parser;
Lights lights;

void setup() {
  Serial.begin(115200);
  sensors.begin();
  sensors.setResolution(12);
  analogWriteResolution(10);
  
  lights.on("Green", 10);
  lights.on("Red", 10);

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
}

void measure_temperature(){
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0); // Index 0 for single Temperature Probe
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.print(" \xC2\xB0"); // Degrees symbol
  Serial.println("C");
}

void loop() {
  if(Serial.available()){
    char input[MAX_INPUT_SIZE];
    Serial.readBytes(input, MAX_INPUT_SIZE);
    if(input[0] == 'T'){
      measure_temperature();
    }
    else if(input[0] == 'L'){
      parser.lights(lights, input);
    }
    // Global off
    else if(input[0] == 'O'){
      lights.off();
    }
  }
}
