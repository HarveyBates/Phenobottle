#include "lights.h"

Lights::Lights(){
  /**
   * Initalise microcontroller pins as outputs upon startup.
   */
  pinMode(redIN1, OUTPUT);
  pinMode(redIN2, OUTPUT);
  pinMode(greenIN1, OUTPUT);
  pinMode(greenIN2, OUTPUT);
  pinMode(blueIN1, OUTPUT);
  pinMode(blueIN2, OUTPUT);
}

void Lights::on(const char* color){
  /**
   * Turn on specific color of light at a preset intensity and frequency. 
   * @param color Color of the LED.
   */
  if(strcmp(color, "Red") == 0){
    analogWriteFrequency(redIN1, redFrequency);
    analogWriteFrequency(redIN2, redFrequency);
    digitalWrite(redIN2, LOW);
    analogWrite(redIN1, redIntensity);
  }
  else if(strcmp(color, "Green") == 0){
    analogWriteFrequency(greenIN1, greenFrequency);
    analogWriteFrequency(greenIN2, greenFrequency);
    digitalWrite(greenIN2, LOW);
    analogWrite(greenIN1, greenIntensity);
  }
  else if(strcmp(color, "Blue") == 0){
    analogWriteFrequency(blueIN1, blueFrequency);
    analogWriteFrequency(blueIN2, blueFrequency);
    digitalWrite(blueIN2, LOW);
    analogWrite(blueIN1, blueIntensity);
  }
}

void Lights::on(const char* color, int intensity){
  /**
   * Turn on specific color of light with custom intensity and a preset frequency.
   * @param color Color of the LED.
   * @param intensity Intensity (in Bits) of the PWM LED emission.
   */
  if(strcmp(color, "Red") == 0){
    redIntensity = intensity;
    analogWriteFrequency(redIN1, redFrequency);
    analogWriteFrequency(redIN2, redFrequency);
    digitalWrite(redIN2, LOW);
    analogWrite(redIN1, redIntensity);
  }
  else if(strcmp(color, "Green") == 0){
    greenIntensity = intensity;
    analogWriteFrequency(greenIN1, greenFrequency);
    analogWriteFrequency(greenIN2, greenFrequency);
    digitalWrite(greenIN2, LOW);
    analogWrite(greenIN1, greenIntensity);
  }
  else if(strcmp(color, "Blue") == 0){
    blueIntensity = intensity;
    analogWriteFrequency(blueIN1, blueFrequency);
    analogWriteFrequency(blueIN2, blueFrequency);
    digitalWrite(blueIN2, LOW);
    analogWrite(blueIN1, blueIntensity);
  }
}

void Lights::set_frequency(const char* color, int freq){
  /**
   * Set the freqeuency (in Hz) of a specific colors LED emission.
   * 
   * This requires initalising this change by running on()
   * 
   * @param color Color of the LED.
   * @param freq Frequency (in Hz) of the PWM emission.
   */
  if(strcmp(color, "Red") == 0){
    redFrequency = freq;
  }
  else if(strcmp(color, "Green") == 0){
    greenFrequency = freq;
  }
  else if(strcmp(color, "Blue") == 0){
    blueFrequency = freq;
  }
}

void Lights::set_intensity(const char* color, int intensity){
  /**
   * Set the intensity (in Bits) of a specific colors LED emission.
   * 
   * This requires initalising this change by running on()
   * 
   * @param color Color of the LED.
   * @param intensity Intensity (in Bits) of the PWM LED emission.
   * 
   * @todo convert this to percentage
   */
  if(strcmp(color, "Red") == 0){
    redIntensity = intensity;
  }
  else if(strcmp(color, "Green") == 0){
    greenIntensity = intensity;
  }
  else if(strcmp(color, "Blue") == 0){
    blueIntensity = intensity;
  }
}

void Lights::off(){
  /**
   * Turn off all LED's and reset intensity to zero.
   */
  redIntensity = 0;
  greenIntensity = 0;
  blueIntensity = 0;
  
  digitalWrite(redIN2, LOW);
  analogWrite(redIN1, redIntensity);

  digitalWrite(greenIN2, LOW);
  analogWrite(greenIN1, greenIntensity);

  digitalWrite(blueIN2, LOW);
  analogWrite(blueIN1, blueIntensity);

}

void Lights::off(const char* color){
  /**
   * Turn off specific color of LED.
   * 
   * @param color Color of the LED.
   */
  if(strcmp(color, "Red") == 0){
    redIntensity = 0;
  }
  else if(strcmp(color, "Green") == 0){
    greenIntensity = 0;
  }
  else if(strcmp(color, "Blue") == 0){
    blueIntensity = 0;
  }
}
