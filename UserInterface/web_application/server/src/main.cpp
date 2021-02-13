#include "main.h"

bool wsCommand = false;

int main(){ 
	std::cout << constants::ID << std::endl;
	try{
		asio::io_context io_context;
		Server server(io_context);
		std::thread run_thread([&] {
				io_context.run(); 
				});
		while(true){
			if(wsCommand){
				Serial serial;
				std::vector<std::string> ports = serial.list_ports();
				serial.send(ports[0].c_str(), "T");
				wsCommand = false;
			}
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(1ms);
		}
		run_thread.join();
	}
	catch(std::exception& e){
		std::cerr << e.what() << std::endl;
	}
	return 0;
}

