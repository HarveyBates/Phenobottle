#include "lights.h"

Lights::Lights(){
  pinMode(redIN1, OUTPUT);
  pinMode(redIN2, OUTPUT);
  pinMode(greenIN1, OUTPUT);
  pinMode(greenIN2, OUTPUT);
  pinMode(blueIN1, OUTPUT);
  pinMode(blueIN2, OUTPUT);
}

void Lights::on(const char* color){
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
