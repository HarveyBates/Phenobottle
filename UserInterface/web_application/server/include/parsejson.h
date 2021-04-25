#pragma once
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>

using json = nlohmann::json;

class ParseJSON {
	inline static std::string input;

public:
	static void set_input(std::string msg);
	static void parse();

};
