
#include "server.h"

Server::Server(asio::io_context& io_context) : io_context_(io_context), 
	acceptor_(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), PORT)){
	
	start_accept();
}

void Server::start_accept(){
	// Wait for new connetion
	Connection::pointer new_connection = Connection::create(io_context_);
	acceptor_.async_accept(new_connection->socket(), 
			std::bind(&Server::handle_accept, this, new_connection, 
				std::placeholders::_1));
}

void Server::handle_accept(Connection::pointer new_connection, 
		const asio::error_code& error){
	// Service client request and starts waiting for connection again
	if(!error){
		new_connection->start();
	}
	start_accept();
}
