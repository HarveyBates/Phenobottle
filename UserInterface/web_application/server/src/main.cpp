#include "main.h"

bool wsCommand = false;

int main(){
	try{
		asio::io_context io_context;
		Server server(io_context);
		std::thread run_thread([&] {
				io_context.run(); 
				});
		while(true){
			if(wsCommand){
				Serial serial;
				serial.list_ports();
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

