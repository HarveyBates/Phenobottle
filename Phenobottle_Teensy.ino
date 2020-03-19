// Required Imports
#include <SPI.h>
#include <math.h>
#include <Wire.h>

// Analog Pins
#define readPin 33
#define odRead 34
#define tempPin 35

// Digital Pins
#define actinicPin 21
#define ODemitPower 25
#define ODdetectPower 22
#define tempPower 20

int baseRead[5]; //baseline pin readings
int microRead [1000]; //microsecond pin readings
int milliRead[1000]; //millisecond pin readings

int baseCycles = 5; //amount of baseline data points
int microCycles = 1000; //amount of microsecond data points
int milliCycles = 1000; //amount of millisecond data points

float h[5]; //array size for baseline data points
float t[1000]; //array size for microsecond data points 
float p[1000]; //array size for millisecond data points
String command; //used to read the serial teminal input

int microMax = 0;
int milliMax = 0;
float foread = 0;

int ODMeasure;
float Fo;
float Fm;
float VarFluoro;
float transmittance;
float FvoverFm;
double od_real;

float initalOD = 800;

void setup() {
  Serial.begin(115200);
  
  analogReadResolution(12);
  analogReference(DEFAULT);
  
  pinMode(tempPower, OUTPUT);
  pinMode(ODemitPower, OUTPUT);
  pinMode(ODdetectPower, OUTPUT);
  pinMode(actinicPin, OUTPUT);

  digitalWrite(actinicPin, LOW);
  digitalWrite(ODemitPower, LOW);
  digitalWrite(ODdetectPower, HIGH);
}

void measureOpticalDensity(){
  digitalWrite(ODemitPower, HIGH);
  analogReference(INTERNAL1V1);
  delay(1000);
  analogRead(odRead);
  delay(100);
  ODMeasure = analogRead(odRead);
  Serial.println(ODMeasure);
  delay(10);
  digitalWrite(ODemitPower, LOW);
  delay(100);
}

void measureFluoro() {
  analogReference(DEFAULT);
  delay(10);
  
  analogRead(readPin);
  delay (1000);
  
  digitalWrite(actinicPin, HIGH); //turns on actinic light

  long timer = micros();

  for (int i = 0; i < microCycles; i++) //second array loop for microsecond values
  {
    microRead[i] = analogRead(readPin);
    t[i] = micros() - timer;
  }
  
  for (int o = 0; o < milliCycles; o++) //third array loop for millisecond values
  {
    milliRead[o] = analogRead(readPin);
    p[o] = micros() - timer;
    delay(1);
  }
  
  digitalWrite(actinicPin, LOW); //switches OFF the light
  delay(10);
  
  int microMax = microRead[1]; //finds the maximum of the microsecond array values
    for (int u = 1; u < microCycles; u++){
      if(microRead[u] > microMax){
        microMax = microRead[u];
      }
    }
    
  int milliMax = milliRead[0]; //finds the maximum of the millisecond array values
    for (int i = 0; i < milliCycles; i++){
      if(milliRead[i] > milliMax){
        milliMax = milliRead[i];
      }
    }
  
  for (int q = 0; q < microCycles; q++) //prints the values of the ojip curve for microsecond array values
  {
   float millCounts = t[q]; //converts timeseries into a float (this allows math to be used)
   float millicountsConverted = millCounts/1000; //converts microseconds into milliseconds
   float milliReal = millicountsConverted;
   Serial.print(milliReal, 3); 
   Serial.print("\t");
   Serial.println(microRead[q]);
   delay(1);
  }
  
  for (int l = 0; l < milliCycles; l++) //prints the values of the ojip curve for millisecond array values
  {
   float milliTime = p[l];
   float millitimeConverted = milliTime/1000;
   float milliReal = millitimeConverted;
   Serial.print(milliReal, 3); 
   Serial.print("\t");
   Serial.println(milliRead[l]);
   delay(1);
  }

}

void testADC(){
  for (int i = 0; i < 2000; i++){
    int value = analogRead(14);
    Serial.println(value);
    delay(100);
  }
}

void calibrateFo(){
  analogReference(DEFAULT);
  delay(10);
  analogRead(readPin);
  delay(10);
  for (int k = 0; k < 5; k++){
    digitalWrite(actinicPin, HIGH);
    delayMicroseconds(20);
    for (int i = 0; i <= 2; i++){
      foread = analogRead(readPin);
      Serial.println((foread/4096) * 3.3);
    }
    digitalWrite(actinicPin, LOW);
    Serial.print("Final Fo = ");
    Serial.println((foread/4096) * 3.3);
    delay(2000);
    }
}

void calibrateRise(){
  for (int i = 0; i < 200; i++){
    digitalWrite(actinicPin, HIGH);
    delayMicroseconds(100);
    digitalWrite(actinicPin, LOW);
    delay(200);
  }
}

void measureLight(){
  digitalWrite(actinicPin, HIGH);
  delay(3000);
  digitalWrite(actinicPin, LOW);
  delay(20);
}

void loop(){
  if(Serial.available()){                     //the next section waits until a command is entered in the serial monitor before measuring OJIP
    command = Serial.readStringUntil('\n');
    if(command.equals("MeasureOpticalDensity")){
      measureOpticalDensity();
    }
    else if(command.equals("MeasureFluorescence")){
      measureFluoro();
    }
    else if(command.equals("MF")){
      measureFluoro();
    }
    else if(command.equals("CFo")){
      calibrateFo();
    }
    else if(command.equals("Measure Light")){
      measureFluoro();
    }
    else if(command.equals("ML")){
      measureLight();
    }
    else if(command.equals("c")){
      calibrateRise();
    }
    else if(command.equals("t")){
      testADC();
    }
    else{
      delay(100);
    }
  }
}
