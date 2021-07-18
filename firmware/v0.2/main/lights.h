#ifndef LIGHTS_H
#define LIGHTS_H

#include "Arduino.h"

// Red Pins
#define redIN1 0
#define redIN2 1

// Green Pins
#define greenIN1 2
#define greenIN2 3

// Blue Pins
#define blueIN1 4
#define blueIN2 5

/**
 * Functions that control the intensity and frequency of growth LED's.
 */
class Lights{
  int redIntensity = 0, greenIntensity = 0, blueIntensity = 0;
  int redFrequency = 20000, greenFrequency = 20000, blueFrequency = 20000;
  
public:
  Lights();
  void on(const char* color);
  void on(const char* color, int intensity);
  void set_frequency(const char* color, int freq);
  void set_intensity(const char* color, int intensity);
  void off();
  void off(const char* color);
};

#endif
