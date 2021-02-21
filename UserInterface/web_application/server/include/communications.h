#pragma once
#include <iostream>
#include <string>

class Communications{
	std::string command;
	public:
		void set_command(std::string message);
		const char* get_command();
};



