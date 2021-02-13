#include "lights.h"
#include "parser.h"
#include "temperature.h"

OneWire oneWire(ONE_WIRE_BUS);
Temperature temperature(&oneWire);
Parser parser;
Lights lights;

void setup() {
  Serial.begin(115200);
  analogWriteResolution(10);
  
  lights.on("Green", 100);
  lights.on("Red", 100);

  pinMode(13, OUTPUT);
}

void loop() {
  if(Serial.available()){
    digitalWrite(13, HIGH);
    char input[MAX_INPUT_SIZE];
    Serial.readBytes(input, MAX_INPUT_SIZE);
    if(input[0] == 'T'){
      temperature.measure();
    }
    else if(input[0] == 'L'){
      parser.lights(lights, input);
    }
    // Global off
    else if(input[0] == 'O'){
      lights.off();
    }
  }
  digitalWrite(13, LOW);
}
