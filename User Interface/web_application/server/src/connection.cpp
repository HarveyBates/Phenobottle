#include "connection.h"

typedef std::shared_ptr<Connection> pointer;

pointer Connection::create(asio::io_context& io_context){
	return pointer(new Connection(io_context));
}

asio::ip::tcp::socket& Connection::socket(){
	return socket_;
}
	
void Connection::start(){
	do_read();
}

Connection::Connection(asio::io_context& io_context) : socket_(io_context){}

void Connection::handle_write(const asio::error_code& /*error*/,
		size_t /*bytes_transferred*/){
}

void Connection::do_read(){
	auto self(shared_from_this());

	socket_.async_read_some(asio::buffer(_data, max_length), 
			[this, self](std::error_code err, std::size_t length){
		if(!err){
			std::cout << _data << std::endl;
			do_write(length);
		}
	});
}

void Connection::do_write(std::size_t length){
	auto self(shared_from_this());

	strncpy(_data, "Response from sever", max_length);
	std::string response = "This is the server";
	asio::async_write(socket_, asio::buffer(response.c_str(), response.size()),
			[this, self](std::error_code err, std::size_t /*lenght*/){
		if(!err){
			do_read();
		}
	});
}
