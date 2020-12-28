#include <OneWire.h>
#include <DallasTemperature.h>

#define heater1IN1 8
#define heater1IN2 9
#define heater2IN1 11
#define heater2IN2 10

#define fan1IN1 28
#define fan1IN2 29
#define fan2IN1 25
#define fan2IN2 24

#define mixingIN1 22
#define mixingIN2 23

#define redIN1 0
#define redIN2 1


#define ONE_WIRE_BUS 21
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

unsigned int ledPin = 13;

void setup() {
  Serial.begin(115200);
  sensors.begin();
  sensors.setResolution(12);
  
  pinMode(heater1IN1, OUTPUT);
  pinMode(heater1IN2, OUTPUT);
  pinMode(heater2IN1, OUTPUT);
  pinMode(heater2IN2, OUTPUT);
  pinMode(fan1IN1, OUTPUT);
  pinMode(fan1IN2, OUTPUT);
  pinMode(fan2IN1, OUTPUT);
  pinMode(fan2IN2, OUTPUT);
  pinMode(mixingIN1, OUTPUT);
  pinMode(mixingIN2, OUTPUT);
  pinMode(redIN1, OUTPUT);
  pinMode(redIN2, OUTPUT);
  pinMode(13, OUTPUT);
  analogWriteResolution(10);

  // Red Setup
  analogWriteFrequency(redIN1, 80000);
  analogWriteFrequency(redIN2, 80000);
  digitalWrite(redIN2, LOW);
  analogWrite(redIN1, 0);

// Heater setup
  analogWriteFrequency(heater1IN1, 200000);
  analogWriteFrequency(heater1IN2, 200000);
  digitalWrite(heater1IN2, LOW);
  analogWrite(heater1IN1, 0); //800
  delay(5000);
//  analogWriteFrequency(heater2IN1, 200000);
//  analogWriteFrequency(heater2IN2, 200000);
//  digitalWrite(heater2IN1, LOW);
//  analogWrite(heater2IN2, 0);
//  delay(5000);

  
// Fan Setup
//  digitalWrite(fan1IN2, LOW);
//  analogWrite(fan1IN1, 0);
//  delay(5000);
//  digitalWrite(fan2IN1, LOW);
//  analogWrite(fan2IN2, 0);
//  delay(5000);

// Mixing Setup
  analogWriteFrequency(mixingIN1, 25000);
  analogWriteFrequency(mixingIN2, 25000);
  digitalWrite(mixingIN1, LOW);
  analogWrite(mixingIN2, 1000);
  delay(3);
  analogWrite(mixingIN2, 0); //430
  delay(1000);
}

void measureTemperature(){
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0); // Index 0 for single Temperature Probe
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.print(" \xC2\xB0"); // Degrees symbol
  Serial.println("C");
}

void loop() {
  if(Serial.available() > 0){
    String command = Serial.readStringUntil('\n');
    if(command.equals("T")){
      measureTemperature();
    }
  }
  else{
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    delay(1000);
  }
}
