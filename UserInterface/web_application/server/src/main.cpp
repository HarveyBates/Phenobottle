#include "main.h"

bool wsCommand = false;

Serial serial;

int main(){ 
//	std::vector<std::string> ports = serial.list_ports();
	serial.configure("/dev/tty.usbmodem81500001");
//	serial.open_port("/dev/tty.usbmodem81500001");
	serial.send("/dev/tty.usbmodem81500001", "LB&I:50&F:20000");
	try{
		asio::io_context io_context;
		Server server(io_context);
		std::thread run_thread([&] {
				io_context.run(); 
				});
		while(true){
			if(wsCommand){
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

