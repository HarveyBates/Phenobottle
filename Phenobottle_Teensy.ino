// <Phenobottle Teensy 3.6 Script. Controls "The Phenobottles" sensors>
     // Copyright (C) <2020>  <Harvey Bates>

     // This program is free software: you can redistribute it and/or modify
     // it under the terms of the GNU General Public License as published by
     // the Free Software Foundation, either version 3 of the License, or
     // (at your option) any later version.

     // This program is distributed in the hope that it will be useful,
     // but WITHOUT ANY WARRANTY; without even the implied warranty of
     // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     // GNU General Public License for more details.

     // You should have received a copy of the GNU General Public License
     // along with this program.  If not, see <https://www.gnu.org/licenses/>
    
     // For more information contact: harvey.bates@student.uts.edu.au

// Required Imports
#include <SPI.h>
#include <math.h>
#include <Wire.h>
#include <stdlib.h>

// Analog Pins
#define readPin 33
#define odRead 34
#define tempPin 35

// Digital Pins
#define actinicPin 21
#define ODemitPower 23
#define ODdetectPower 22
#define tempPower 20

//RGB Control 
#define Red 36
#define Green 37
#define Blue 38

int baseRead[5];
int microRead [1000];
int milliRead[1000];

int baseCycles = 5;
int microCycles = 1000;
int milliCycles = 1000;

float h[5];
float t[1000]; 
float p[1000];
String command;

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
  pinMode(Red, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(Blue, OUTPUT);
 
  digitalWrite(actinicPin, LOW);
  digitalWrite(ODemitPower, LOW);
  digitalWrite(ODdetectPower, HIGH);
  digitalWrite(Red, LOW);
  digitalWrite(Green, LOW);
  digitalWrite(Blue, LOW);
}

void LED_light_on(int redColor, int redFreq, int greenColor, int greenFreq, int blueColor, int blueFreq){
  analogWrite(redColor, redFreq);
  analogWrite(greenColor, greenFreq);
  analogWrite(blueColor, blueFreq);
  if (redFreq == 0){
    digitalWrite(redColor, LOW);
  }
  if (greenFreq == 0){
    digitalWrite(greenColor, LOW);
  }
  if (blueFreq == 0){
    digitalWrite(blueColor, LOW);
  }
}

void LED_light_off(int color){
  digitalWrite(color, LOW);
}

void rgblightsOFF(){
	digitalWrite(Red, LOW);
	digitalWrite(Green, LOW);
	digitalWrite(Blue, LOW);
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void measureOpticalDensity(){
  digitalWrite(ODemitPower, HIGH);
  analogReference(DEFAULT);
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
  
  digitalWrite(actinicPin, HIGH); 

  long timer = micros();

  for (int i = 0; i < microCycles; i++) 
  {
    microRead[i] = analogRead(readPin);
    t[i] = micros() - timer;
  }
  
  for (int o = 0; o < milliCycles; o++) 
  {
    milliRead[o] = analogRead(readPin);
    p[o] = micros() - timer;
    delay(1);
  }
  
  digitalWrite(actinicPin, LOW); 
  delay(10);
  
  int microMax = microRead[1]; 
    for (int u = 1; u < microCycles; u++){
      if(microRead[u] > microMax){
        microMax = microRead[u];
      }
    }
    
  int milliMax = milliRead[0];
    for (int i = 0; i < milliCycles; i++){
      if(milliRead[i] > milliMax){
        milliMax = milliRead[i];
      }
    }
  
  for (int q = 0; q < microCycles; q++)
  {
   float millCounts = t[q];
   float millicountsConverted = millCounts/1000; 
   float milliReal = millicountsConverted;
   Serial.print(milliReal, 3); 
   Serial.print("\t");
   Serial.println(microRead[q]);
   delay(1);
  }
  
  for (int l = 0; l < milliCycles; l++) 
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
  if(Serial.available()){   
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
    else if(command.equals("RGBOFF")){
      rgblightsOFF();
    }
    else if(command.startsWith("LED_light_ON")){
      String red_col = getValue(command, ';', 1);
      String red_freq = getValue(command, ';', 2); 
      String green_col = getValue(command, ';', 3);
      String green_freq = getValue(command, ';', 4); 
      String blue_col = getValue(command, ';', 5);
      String blue_freq = getValue(command, ';', 6);      
      LED_light_on(red_col.toInt(), red_freq.toInt(), green_col.toInt(), 
      green_freq.toInt(), blue_col.toInt(), blue_freq.toInt());
    }
    else if(command.startsWith("LED_light_OFF")){
      String col = getValue(command, ';', 1);
      LED_light_off(col.toInt());
    }
    else{
      delay(100);
    }
  }
}
