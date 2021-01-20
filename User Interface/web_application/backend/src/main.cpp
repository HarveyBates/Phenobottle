#include <iostream>
#include <cpr/cpr.h>

std::string weatherKey = std::getenv("WEATHERAPIKEY");
const char* location = "Sydney";

int main(int argc, char** argv) {
	cpr::Response r = cpr::Get(cpr::Url{"http://api.openweathermap.org/data/2.5/weather"},
		cpr::Parameters{{"q", "sydney"}, {"appid", weatherKey.c_str()}});
	std::cout << r.text << std::endl;
}


