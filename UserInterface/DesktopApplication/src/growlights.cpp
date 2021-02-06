#include "growlights.h"

GrowLights::GrowLights(void){
	// Nothing
}

void GrowLights::lightOn(const char* color, int intensity){
	if(intensity >= max_intensity){
		intensity = max_intensity;
	}
	std::string combined = color + std::to_string(intensity);
	const char* request = combined.c_str();
	SerialComs serialComs;
	serialComs.sendComs("/dev/ttyACM0", request);
}

void GrowLights::lightOff(std::string color){
	std::string combined = color + "OFF";
	const char* request = combined.c_str();
	SerialComs serialComs;
	serialComs.sendComs("/dev/ttyACM0", request);
}

void GrowLights::setFrequency(const char* color, int frequency){
	// Change frequency of the growth lights
	// Specify color, initalise frequency, set frequency
	std::string combined = color + std::to_string(frequency); // Output example = GF1000
	const char* request = combined.c_str();
	SerialComs serialComs;
	serialComs.sendComs("/dev/ttyACM0", request);
}
