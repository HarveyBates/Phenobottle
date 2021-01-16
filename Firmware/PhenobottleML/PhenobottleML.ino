// I2C imports
#include <Wire.h>
// PCA9685 PWM driver imports
#include <Adafruit_PWMServoDriver.h>
// Temperature sensor imports
#include <OneWire.h>
#include <DallasTemperature.h>
// Local imports
#include "settings.h"
#include "temperatureprobe.h"

// PWM setup
Adafruit_PWMServoDriver pwma = Adafruit_PWMServoDriver(0x60, Wire); // I2C Address (a)
Adafruit_PWMServoDriver pwmb = Adafruit_PWMServoDriver(0x61, Wire); // I2C Address (b)
//Adafruit_PWMServoDriver pwmc = Adafruit_PWMServoDriver(0x63, Wire); // I2CAddress (c)

// Temperture setup
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  /*
   * See https://www.pjrc.com/teensy/td_uart.html for suitable baud rates
   */
  Serial.begin(38400);
  pwma.begin(); 
  pwma.setOscillatorFrequency(25000000);
  pwma.setPWMFreq(1600); // between 40 and 1600 Hz
  
  pwmb.begin(); 
  pwmb.setOscillatorFrequency(27000000);
  pwmb.setPWMFreq(1200); // between 40 and 1600 Hz
  Wire.setClock(200000);

  sensors.begin();
  sensors.setResolution(12); // Set resolution of Temperture probe
  globalOff();
  pinMode(13, OUTPUT);
}

void selectLEDColor(char color){
  if(color == 'R'){ // Red
    IN1 = 10;
    IN2 = 9;
    PWM = 8;
  }
  else if(color == 'G'){ // Green
    IN1 = 5;
    IN2 = 6;
    PWM = 7;
  }
  else if(color == 'B'){ //Blue
    IN1 = 4;
    IN2 = 3;
    PWM = 2;
  }
}

void lightOn(char color, int intensity){
  // pwm.setPWM(pin, on, off);
  selectLEDColor(color);
  Serial.println(color);
  if(color == 'R'){ //Red
    pwmb.setPWMFreq(RFreq);
    pwmb.setPin(IN1, 4096); // set pin HIGH
    pwmb.setPin(IN2, 0); // set pin LOW
    pwmb.setPWM(PWM, 0, intensity);
  }
  else if(color == 'G'){ // Green LED is inverse
    pwmb.setPWMFreq(GFreq);
    pwmb.setPin(IN1, 0); // set pin LOW
    pwmb.setPin(IN2, 4096); // set pin HIGH
    pwmb.setPWM(PWM, 0, intensity);
  }
  else{ //Blue
    pwmb.setPWMFreq(BFreq);
    pwmb.setPin(IN1, 4096); // set pin HIGH
    pwmb.setPin(IN2, 0); // set pin LOW
    pwmb.setPWM(PWM, 0, intensity);
  }
}

void lightOff(char color){
  selectLEDColor(color);
  pwmb.setPin(IN1, 0); // set pin OFF
  pwmb.setPin(IN2, 0); // set pin ON
  pwmb.setPin(PWM, 4096);
}

void lightFrequency(char color, int freq){
  if(color == 'R'){
    RFreq = freq; 
  }
  else if(color == 'G'){
    GFreq = freq;
  }
  else if(color == 'B'){
    BFreq = freq;
  }
}

void selectMotor(char motor){
  if(motor == 'M'){ // Mixing
    IN1 = 4;
    IN2 = 3;
    PWM = 2;
  }
}

void motorOn(char motor, int intensity, int reverse){
  selectMotor(motor);
  if(motor == 'M'){
    pwma.setPWMFreq(MFreq);
    if(reverse){ // Reverse
      pwma.setPin(IN1, 0); // set pin LOW
      pwma.setPin(IN2, 4096); // set pin HIGH
      pwma.setPWM(PWM, 0, intensity);
    }
    else{ // Forward
      pwma.setPin(IN1, 4096); // set pin LOW
      pwma.setPin(IN2, 0); // set pin HIGH
      pwma.setPWM(PWM, 0, intensity);
      Serial.println(intensity);
    }
  }
  else{}
}

void motorOff(char motor){
  selectMotor(motor);
  pwma.setPin(IN1, 0); // set pin LOW
  pwma.setPin(IN2, 0); // set pin LOW
  pwma.setPin(PWM, 4096);
}

void motorFrequency(char motor, int freq){
  if(motor == 'M'){
    MFreq = freq;
  }
  else{}
}

void measureTemperature(){
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0); // Index 0 for single Temperature Probe
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.print(" \xC2\xB0"); // Degrees symbol
  Serial.println("C");
}

void globalOff(){
  lightOff('R');
  lightOff('G');
  lightOff('B');
  motorOff('M');
}

void loop() {
  if(Serial.available() > 0){
    String command = Serial.readStringUntil('\n');
    if(command.startsWith("R")){ // Red
      if(command.equals("ROFF")){
        lightOff('R');
      }
      else if(command[1] == 'F'){
        int freq = command.substring(2, command.length()).toInt();
        lightFrequency('R', freq);
      }
      else{
        int intensity = command.substring(1, command.length()).toInt();
        lightOn('R', intensity);
      }
    }
    else if(command.startsWith("G")){ // Green
      if(command.equals("GOFF")){
        lightOff('G');
      }
      else if(command[1] == 'F'){
        int freq = command.substring(2, command.length()).toInt();
        lightFrequency('G', freq);
      }
      else{
        int intensity = command.substring(1, command.length()).toInt();
        lightOn('G', intensity);
      }
    }
    else if(command.startsWith("B")){ // Blue
      if(command.equals("BOFF")){
        lightOff('B');
      }
      else if(command[1] == 'F'){
        int freq = command.substring(2, command.length()).toInt();
        lightFrequency('B', freq);
      }
      else{
        int intensity = command.substring(1, command.length()).toInt();
        lightOn('B', intensity);
      }
    }
    else if(command.startsWith("M")){
      if(command.equals("MOFF")){
        motorOff('M');
      }
      else if(command[1] == 'F'){
        int freq = command.substring(2, command.length()).toInt();
        motorFrequency('M', freq);
      }
      else{
        int intensity = command.substring(1, command.length()).toInt();
        digitalWrite(13, HIGH);
        motorOn('M', intensity, 0);  
      }
    }
    else if(command.startsWith("T")){
      measureTemperature();
    }
    else if(command.equals("OFF")){ // Global OFF Command
      globalOff();
    }
  }
  else{
    delay(50);
  }
}
