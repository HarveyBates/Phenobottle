#include "connection.h"
#include <string.h>
#include <cstdint>

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

std::string xor_decrypt(char* _data){
	char keys[4];
	std::string toDecrypt = std::string(_data);
	std::string output = toDecrypt;
	if(std::string(_data).length() > 125){
		char key1 = _data[4];
		char key2 = _data[5];
		char key3 = _data[6];
		char key4 = _data[7];
		char keys[4] = {key1, key2, key3, key4};
	}
	else{
		char key1 = _data[2];
		char key2 = _data[3];
		char key3 = _data[4];
		char key4 = _data[5];
		char keys[4] = {key1, key2, key3, key4};
	}
	for(int i = 0; i < toDecrypt.size(); i++){
		output[i] = toDecrypt[i] ^ keys[i % (sizeof(keys) / sizeof(char))];
	}
	return output;
}

void Connection::do_read(){
	auto self(shared_from_this());
	std::memset(_data, 0, max_length);
	socket_.async_read_some(asio::buffer(_data, max_length), 
			[this, self](std::error_code err, std::size_t length){
		if(!err){
			std::cout << "Recieved data: " << std::endl;
			std::cout << "Data-size: " << std::string(_data).length() << std::endl;
			/* 
			 * If byte length < 125:
			 * byte 0: The 0x81 is just an indicator that a message received
			 * byte 1: the 0x8a is the length, substract 0x80 from it, 0x0A == 10
			 * byte 2, 3, 4, 5: the 4 byte xor key to decrypt the payload
			 * the rest: payload
			 *
			 * If the byte length > 125:	
			 * byte 0: The 0x81 is just an indicator that a message received
			 * byte 1: will be 0xfe
			 * byte 2, 3: the length of the payload as a uint16 number
			 * byte 4, 5, 6, 7: the 4 byte xor key to decrypt the payload
			 * the rest: payload
			 *
			 */
			for(int i = 0; i < std::string(_data).length(); i++){
				std::cout << std::hex << int(_data[i]);
			}

			std::cout << "\nDecrypted: " << xor_decrypt(_data) << std::endl;

			do_write();
		}
	});
}

void Connection::do_write(){
	auto self(shared_from_this());
	
	std::string tmp(_data);

	std::string response;
		
	std::cout << SimpleWeb::Crypto::Base64::decode(response) << std::endl;

	if(tmp.find("Sec-WebSocket-Key: ") != std::string::npos){
		
		std::cout << "Found WebSocket Key" << std::endl;

		tmp.erase(0, tmp.find("Sec-WebSocket-Key: ") + strlen("Sec-WebSocket-Key: "));
		auto key = tmp.substr(0, tmp.find("\r\n"));
	
		auto sha1Key = SimpleWeb::Crypto::sha1(key + ws_magic_string);
		sha1Key = SimpleWeb::Crypto::Base64::encode(sha1Key);

		response = 
			"HTTP/1.1 101 Switching Protocols\r\n"
			"Upgrade: websocket\r\n"
			"Connection: Upgrade\r\n"
			"Sec-WebSocket-Accept: " + sha1Key + "\r\n\r\n";
	}
	else{
		response = SimpleWeb::Crypto::Base64::decode(_data);
	}

	std::cout << "Response: \n" << response << std::endl;
	asio::async_write(socket_, asio::buffer(response.c_str(), response.size()),
			[this, self](std::error_code err, std::size_t /*lenght*/){
		if(!err){
			std::cout << "Message sent" << std::endl;
			do_read();
		}
	});
}
