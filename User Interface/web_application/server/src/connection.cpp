#include "connection.h"

const std::string ws_magic_string = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

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
	
	std::string tmp(_data);
	tmp.erase(0, tmp.find("Sec-WebSocket-Key: ") + strlen("Sec-WebSocket-Key: "));
	auto key = tmp.substr(0, tmp.find("\r\n"));

	/* 
	 * Got to do this and return the encoded response:
	 * key_decoded = base64_decode(key64)
	 * magic = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
	 * response = SHA1(append(key_decoded, magic))
	 * response_encoded = base64_encode(response)
	 */

	strncpy(_data, "Response from sever", max_length);
	std::string response = 
		"HTTP 101 Switching Protocols\r\n"
		"Upgrade: websocket\r\n"
		"Connection: Upgrade\r\n"
		"Sec-WebSocket-Accept: "
		+ base64_encode(ws_magic_string) +
		"\r\n\r\n";
	asio::async_write(socket_, asio::buffer(response.c_str(), response.size()),
			[this, self](std::error_code err, std::size_t /*lenght*/){
		if(!err){
			do_read();
		}
	});
}
