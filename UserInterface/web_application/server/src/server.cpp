#include "server.h"


/* Server constructor with a defined io_context.
 * If asio::ip::tcp::v4() is used any IPv4 address can connect at a specific PORT number.
 * Alternatively a specific IP address can be used here.
 */
Server::Server(asio::io_context& io_context) : io_context_(io_context), 
	acceptor_(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), PORT)){
	start_accept();
}


/* Async connection request function with a shared pointer for each unique connection.
 * std::placeholders::_1 represents the non-boost version of asio's
 * std::placeholders::error.
 */
void Server::start_accept(){
	Connection::pointer new_connection = Connection::create(io_context_);
	acceptor_.async_accept(new_connection->socket(), 
			std::bind(&Server::handle_accept, this, new_connection, 
				std::placeholders::_1));
}


/* Services client conneciton and starts new connection should another arise. */
void Server::handle_accept(Connection::pointer new_connection, 
		const asio::error_code& error){
	if(!error){
		new_connection->start();
	}
	start_accept();
}
