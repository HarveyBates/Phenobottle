#include "communications.h"

void Communications::set_command(std::string message){
	command = message;
}


const char* Communications::get_command(){
	//todo this is resulting in a segmentation fault
	return command.c_str();
}

