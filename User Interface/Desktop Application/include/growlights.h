#pragma once
#include "SerialComs.h"
#include <string>

class GrowLights{
public:
	const char* color;
	int intensity;
	int max_intensity = 2500; // Percentage of maxiumum
	
	void lightOn(const char* color, int intensity);
	void lightOff(std::string color);
	void setFrequency(const char* color, int frequency);
	GrowLights();
};