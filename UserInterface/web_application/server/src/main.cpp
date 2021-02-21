#include "main.h"

bool wsCommand = false;

Serial serial;
Communications comms;

int main(){ 
	serial.send("LG&I:100&F:20000");
	try{
		asio::io_context io_context;
		Server server(io_context);
		std::thread run_thread([&] {
				io_context.run(); 
				});
		while(true){
			if(wsCommand){
				std::cout << "Getting command: " << comms.get_command() << std::endl;
				serial.send(comms.get_command());
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

