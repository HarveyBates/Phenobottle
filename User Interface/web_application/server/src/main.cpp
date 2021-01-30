#include "main.h"


int main(){
	try{
		asio::io_context io_context;
		Server server(io_context);
		io_context.run();
	}
	catch(std::exception& e){
		std::cerr << e.what() << std::endl;
	}
	// TODO add main event loop here
	return 0;
}

