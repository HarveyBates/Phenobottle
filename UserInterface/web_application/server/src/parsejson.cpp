#include "parsejson.h"

json j; // Construct nlohmann JSON

void ParseJSON::set_input(std::string msg){
	input = msg;
	parse();
}


void ParseJSON::parse(){
	if(input[0] == '{'){
		// JSON msg
		j = json::parse(input);
		std::cout << j.dump(4) <<std::endl;
	}
}

