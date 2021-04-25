#include "main.h"

using namespace std::chrono_literals; 

bool wsCommand = false;
char* serialCommand;

Serial serial;

int main(){ 
	try{
		asio::io_context io_context;
		Server server(io_context);
		std::thread run_thread([&] {
				io_context.run(); 
				});
		while(true){
			if(wsCommand){
				// This is where we parse the JSON
				//ParseJSON::parse();
				wsCommand = false;
			}
			std::this_thread::sleep_for(1ms); // Slight delay to prevent 100% CPU usage
		}
		run_thread.join();
	}
	catch(std::exception& e){
		std::cerr << e.what() << std::endl;
	}
	return 0;
}

