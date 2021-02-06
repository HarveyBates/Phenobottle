#include <iostream>
#include <cpr/cpr.h>


class Server{
	int port;
	std::string address; 
	std::string url;
public:
	Server(std::string add, int prt){
		address = add;
		port = prt;
		url = set_url();
	}
	int get_port(){ return port; }
	void set_port(int prt) { port = prt; } 
	
	std::string get_address(){ return address; }
	void set_address(std::string add) { address = add; }
	
	std::string get_url() { return url; }
	std::string set_url(){
		std::string url = address + ":" + std::to_string(port);
		return url;
	}
};



std::string weatherKey = std::getenv("WEATHERAPIKEY");
const char* location = "Sydney";

int main(int argc, char** argv) {
	Server server("localhost", 5000);
	std::cout << server.get_url() << std::endl;
	cpr::Response r = cpr::Get(cpr::Url{"api.openweathermap.org/data/2.5/weather"},
		cpr::Parameters{{"q", "sydney"}, {"appid", weatherKey.c_str()}});
	std::cout << r.text << std::endl;
}


