#include "motors.h"

Motors::Motors(){
  pinMode(mixingIN1, OUTPUT);
  pinMode(mixingIN2, OUTPUT);
  
  pinMode(fanIN1, OUTPUT);
  pinMode(fanIN2, OUTPUT);
  
  pinMode(pumpIN1, OUTPUT);
  pinMode(pumpIN2, OUTPUT);
}


void Motors::on(const char* type){
  /**
   * Turn on specific type of motor at a preset intensity and frequency. 
   * @param type Type of motor.
   */
  if(strcmp(type, "Mixing") == 0){
    analogWriteFrequency(mixingIN1, mixingFrequency);
    analogWriteFrequency(mixingIN2, mixingFrequency);
    digitalWrite(mixingIN2, LOW);
    analogWrite(mixingIN1, mixingIntensity);
  }
  else if(strcmp(type, "Fan") == 0){
    analogWriteFrequency(fanIN1, fanFrequency);
    analogWriteFrequency(fanIN2, fanFrequency);
    digitalWrite(fanIN2, LOW);
    analogWrite(fanIN1, fanIntensity);
  }
  else if(strcmp(type, "Pump") == 0){
    analogWriteFrequency(pumpIN1, pumpFrequency);
    analogWriteFrequency(pumpIN2, pumpFrequency);
    digitalWrite(pumpIN2, LOW);
    analogWrite(pumpIN1, pumpIntensity);
  }
}

void Motors::on(const char* type, int intensity){
  /**
   * Turn on specific motor with custom intensity and a preset frequency.
   * @param type Type of motor.
   * @param intensity Intensity (in Bits) of PWM.
   */
  if(strcmp(type, "Mixing") == 0){
    mixingIntensity = intensity;
    analogWriteFrequency(mixingIN1, mixingFrequency);
    analogWriteFrequency(mixingIN2, mixingFrequency);
    digitalWrite(mixingIN2, LOW);
    analogWrite(mixingIN1, mixingIntensity);
  }
  else if(strcmp(type, "Fan") == 0){
    fanIntensity = intensity;
    analogWriteFrequency(fanIN1, fanFrequency);
    analogWriteFrequency(fanIN2, fanFrequency);
    digitalWrite(fanIN2, LOW);
    analogWrite(fanIN1, fanIntensity);
  }
  else if(strcmp(type, "Pump") == 0){
    pumpIntensity = intensity;
    analogWriteFrequency(pumpIN1, pumpFrequency);
    analogWriteFrequency(pumpIN2, pumpFrequency);
    digitalWrite(pumpIN2, LOW);
    analogWrite(pumpIN1, pumpIntensity);
  }
}


void Motors::set_frequency(const char* type, int freq){
  /**
   * Set the freqeuency (in Hz) of a specific motor.
   * 
   * This requires initalising this change by running on()
   * 
   * @param type Type of motor.
   * @param freq Frequency (in Hz) of the PWM emission.
   */
  if(strcmp(type, "Mixing") == 0){
    mixingFrequency = freq;
  }
  else if(strcmp(type, "Fan") == 0){
    fanFrequency = freq;
  }
  else if(strcmp(type, "Pump") == 0){
    pumpFrequency = freq;
  }
}


void Motors::set_intensity(const char* type, int intensity){
  /**
   * Set the intensity (in Bits) of a specific motor.
   * 
   * This requires initalising this change by running on()
   * 
   * @param type Type of motor.
   * @param intensity Intensity (in Bits) of the PWM LED emission.
   * 
   * @todo convert this to percentage
   */
  if(strcmp(type, "Mixing") == 0){
    mixingIntensity = intensity;
  }
  else if(strcmp(type, "Fan") == 0){
    fanIntensity = intensity;
  }
  else if(strcmp(type, "Pump") == 0){
    pumpIntensity = intensity;
  }
}



void Motors::off(){
  /**
   * Turn off all Motors and reset intensity to zero.
   */
  mixingIntensity = 0;
  
  digitalWrite(mixingIN2, LOW);
  analogWrite(mixingIN1, mixingIntensity);

  digitalWrite(fanIN2, LOW);
  analogWrite(fanIN1, fanIntensity);

  digitalWrite(pumpIN2, LOW);
  analogWrite(pumpIN1, pumpIntensity);
}


void Motors::off(const char* type){
  /**
   * Turn off specific motor.
   * 
   * @param type Type of motor.
   */
  if(strcmp(type, "Mixing") == 0){
    mixingIntensity = 0;
  }
  else if(strcmp(type, "Fan") == 0){
    fanIntensity = 0;
  }
  else if(strcmp(type, "Pump") == 0){
    pumpIntensity = 0;
  }
}
