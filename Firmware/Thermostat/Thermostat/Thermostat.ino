// I2C imports
#include <Wire.h>
// PCA9685 PWM driver imports
#include <Adafruit_PWMServoDriver.h>
// Temperature sensor imports
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

// PWM setup
Adafruit_PWMServoDriver pwma = Adafruit_PWMServoDriver(0x60, Wire); // I2C Address (a)
Adafruit_PWMServoDriver pwmb = Adafruit_PWMServoDriver(0x61, Wire); // I2C Address (b)
Adafruit_PWMServoDriver pwmc = Adafruit_PWMServoDriver(0x63, Wire); // I2C Address (c)

// Temperture setup
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int IN1;
int IN2;
int PWM;

void setup() {
  Serial.begin(38400);
  pwma.begin(); 
  pwma.setOscillatorFrequency(27000000);
  pwma.setPWMFreq(1600); // between 40 and 1600 Hz
  
  pwmb.begin(); 
  pwmb.setOscillatorFrequency(27000000);
  pwmb.setPWMFreq(1600); // between 40 and 1600 Hz

  pwmc.begin(); 
  pwmc.setOscillatorFrequency(27000000);
  pwmc.setPWMFreq(1600); // between 40 and 1600 Hz
  
  Wire.setClock(200000);
  sensors.begin(); // Begin temeperature probe
  sensors.setResolution(12); // Set resolution of Temperture probe
  globalOff();
  pinMode(13, OUTPUT);
}


void selectOutput(char module){
  if(module == 'R'){
    Serial.println("Right");
    IN1 = 4;
    IN2 = 3;
    PWM = 2;
  }
  else if(module == 'L'){
    Serial.println("Left");
    IN1 = 10;
    IN2 = 9;
    PWM = 8;
  }
  else if(module == 'F'){
    Serial.println("Cooling");
    IN1 = 11;
    IN2 = 12;
    PWM = 13;
  }
}


void peltierOn(int intensity){
  int fan_speed = 4000;
  // Setup hard limits 
  if(intensity > 2000){
    intensity = 2000;
  }
  if(intensity < -2000){
    intensity = -2000;
  }

  Serial.println(intensity);
  
  if(intensity < 0){
    // Cool
    selectOutput('R');
    pwmc.setPin(IN1, 0); // set pin LOW
    pwmc.setPin(IN2, 4096); // set pin HIGH
    pwmc.setPWM(PWM, 0, (intensity * -1)); // Convert to positive int

    selectOutput('L');
    pwma.setPin(IN1, 0); // set pin LOW
    pwma.setPin(IN2, 4096); // set pin HIGH
    pwma.setPWM(PWM, 0, (intensity * -1)); // Convert to positive int

    selectOutput('F');
    pwma.setPin(IN1, 0); // set pin LOW
    pwma.setPin(IN2, 4096); // set pin HIGH
    pwma.setPWM(PWM, 0, fan_speed);
    pwmb.setPin(IN1, 0); // set pin LOW
    pwmb.setPin(IN2, 4096); // set pin HIGH
    pwmb.setPWM(PWM, 0, fan_speed);
  }
  else if(intensity > 0){
    // Heat
    selectOutput('R');
    pwmc.setPin(IN1, 4096); // set pin LOW
    pwmc.setPin(IN2, 0); // set pin HIGH
    pwmc.setPWM(PWM, 0, intensity);
  
    selectOutput('L');
    pwma.setPin(IN1, 4096); // set pin LOW
    pwma.setPin(IN2, 0); // set pin HIGH
    pwma.setPWM(PWM, 0, intensity);

    selectOutput('F');
    pwma.setPin(IN1, 0); // set pin LOW
    pwma.setPin(IN2, 4096); // set pin HIGH
    pwma.setPWM(PWM, 0, fan_speed);
    pwmb.setPin(IN1, 0); // set pin LOW
    pwmb.setPin(IN2, 4096); // set pin HIGH
    pwmb.setPWM(PWM, 0, fan_speed);
  }
  else{
    globalOff();
  }
}

void globalOff(){
  selectOutput('R');
  pwmc.setPin(IN1, 0); // set pin OFF
  pwmc.setPin(IN2, 0); // set pin ON
  pwmc.setPin(PWM, 4096);

  selectOutput('L');
  pwma.setPin(IN1, 0); // set pin OFF
  pwma.setPin(IN2, 0); // set pin ON
  pwma.setPin(PWM, 4096);

  selectOutput('F');
  pwma.setPin(IN1, 0); // set pin OFF
  pwma.setPin(IN2, 0); // set pin ON
  pwma.setPin(PWM, 4096);
  pwmb.setPin(IN1, 0); // set pin OFF
  pwmb.setPin(IN2, 0); // set pin ON
  pwmb.setPin(PWM, 4096);

  motorOff('M');
}

void measureTemperature(){
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0); // Index 0 for single Temperature Probe
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.print(" \xC2\xB0"); // Degrees symbol
  Serial.println("C");
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
  selectMotor('M');
  pwma.setPin(IN1, 0); // set pin LOW
  pwma.setPin(IN2, 0); // set pin LOW
  pwma.setPin(PWM, 4096);
}

void loop() {
  if(Serial.available() > 0){
      String command = Serial.readStringUntil('\n');
      if(command.startsWith("P")){ // Peltier on
        int intensity = command.substring(1, command.length()).toInt();
        peltierOn(intensity);
      }
      else if(command.equals("T")){
        measureTemperature();
      }
      else if(command.startsWith("M")){
        if(command.equals("MOFF")){
          motorOff('M');
        }
        else{
          int intensity = command.substring(1, command.length()).toInt();
          digitalWrite(13, HIGH);
          motorOn('M', intensity, 0);  
        }
      }
      else if(command.equals("OFF")){ // Global OFF Command
        globalOff();
      }
    else{
      delay(100);
    }
  }
}
  
