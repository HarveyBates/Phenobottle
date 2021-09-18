#include "lights.h"
#include "motors.h"
#include "parser.h"
#include "temperature.h"

OneWire oneWire(ONE_WIRE_BUS);
Temperature temperature(&oneWire);

Parser parser;
Lights lights;
Motors motors;

void setup() {
  Serial.begin(115200);
  analogWriteResolution(10);
  
  lights.on("Green", 50);
  lights.on("Red", 50);
//  lights.on("Blue", 100);

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
      Serial.println("1");
    }
    else if(input[0] == 'M'){
      parser.motors(motors, input);
      Serial.println("1");
    }
    // Global off
    else if(input[0] == 'O'){
      lights.off();
      motors.off();
      Serial.println("1");
    }
  }
  digitalWrite(13, LOW);
}
