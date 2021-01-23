#include <chrono>
#include <optional>
#include <iostream>
#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif
#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

const std::size_t max_length = 2048;
const unsigned short PORT = 15001;

class Connection : public std::enable_shared_from_this<Connection>{

public:
	typedef std::shared_ptr<Connection> pointer;

	static pointer create(asio::io_context& io_context){
		return pointer(new Connection(io_context));
	}

	asio::ip::tcp::socket& socket(){
		return socket_;
	}
	
	void start(){
		do_read();
	}

private:
	Connection(asio::io_context& io_context) : socket_(io_context){}

	void handle_write(const asio::error_code& /*error*/,
			size_t /*bytes_transferred*/){
	}

	void do_read(){
		auto self(shared_from_this());

		socket_.async_read_some(asio::buffer(_data, max_length), 
				[this, self](std::error_code err, std::size_t length){
			if(!err){
				std::cout << _data << std::endl;
				do_write(length);
			}
		});
	}

	void do_write(std::size_t length){
		auto self(shared_from_this());

		strncpy(_data, "Response from sever", max_length);
		std::string response = "This is the server";
		/*std::string response = 
				"HTTP/1.1 101 Switching Protocols\r\n"
				"Upgrade: websocket\r\n"
				"Connection: Upgrade\r\n"
				"Sec-WebSocket-Accept: 258EAFA5-E914-47DA-95CA-C5AB0DC85B11\r\n"
				"Sec-WebSocket-Protocol: echo-protocol\r\n\r\n"; */
		asio::async_write(socket_, asio::buffer(response.c_str(), response.size()),
				[this, self](std::error_code err, std::size_t /*lenght*/){
			if(!err){
				do_read();
			}
		});
	}

	asio::ip::tcp::socket socket_;
	char _data[max_length];
};

class Server{
public:
	Server(asio::io_context& io_context) : io_context_(io_context), 
		acceptor_(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 15001)){
		
		start_accept();
	}

private:
	void start_accept(){
		// Wait for new connetion
		Connection::pointer new_connection = Connection::create(io_context_);
		acceptor_.async_accept(new_connection->socket(), 
				std::bind(&Server::handle_accept, this, new_connection, 
					std::placeholders::_1));
	}

	void handle_accept(Connection::pointer new_connection, 
			const asio::error_code& error){
		// Service client request and starts waiting for connection again
		if(!error){
			new_connection->start();
		}
		start_accept();
	}

	asio::io_context& io_context_;
	asio::ip::tcp::acceptor acceptor_;
};



int main(){
	try{
		asio::io_context io_context;
		Server server(io_context);
		io_context.run();
	}
	catch(std::exception& e){
		std::cerr << e.what() << std::endl;
	}
	return 0;
}

