#ifndef MOTORS_H
#define MOTORS_H

#include "arduino.h"

// Mixing Pins (Heater 1)
#define mixingIN1 8
#define mixingIN2 9

// Fan Pins (Fan 2)
#define fanIN1 24
#define fanIN2 25
 
// Pump Pins (Heater 2)
#define pumpIN1 10
#define pumpIN2 11

/**
 * Functions that control the intensity and frequency of various motors.
 */
class Motors{
  int16_t mixingIntensity = 0, fanIntensity = 0, pumpIntensity = 0;
  int16_t mixingFrequency = 20000, fanFrequency = 20000, pumpFrequency = 20000;

  public:
    Motors();
    void on(const char* type);
    void on(const char* type, int intensity);
    void set_frequency(const char* type, int freq);
    void set_intensity(const char* type, int intensity);
    void off();
    void off(const char* type);
};


#endif
