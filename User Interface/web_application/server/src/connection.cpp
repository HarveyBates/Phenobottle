#include "connection.h"
#include <string.h>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <stdio.h>

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

std::string xor_decrypt(uint16_t* decimal){
	char toDecrypt[10];
	for(int i = 0; i < 10; i++){
		toDecrypt[i] = static_cast<char>(decimal[i]);
	}
	std::string strDecrypt = std::string(toDecrypt);
	std::string output = strDecrypt;
	
	char key1 = toDecrypt[2];
	char key2 = toDecrypt[3];
	char key3 = toDecrypt[4];
	char key4 = toDecrypt[5];
	char keys[4] = {key1, key2, key3, key4};
	
	for(int i = 0; i < 10; i++){
		output[i] = toDecrypt[i] ^ keys[i % (sizeof(keys) / sizeof(char))];
	}
	std::cout << "Input: " << std::string(toDecrypt) << std::endl;
	std::cout << "Keys: " << std::string(keys) << std::endl;
	std::cout << "Ouput: " << output << std::endl; 
	return std::string(keys);
}

int GetBit(const char * data, unsigned int idx)
{
	unsigned int arIdx = idx / 8;
	unsigned int biIdx = 7 - (idx % 8);
	return (data[arIdx] >> (7 - biIdx)) & 1;
}

void Connection::do_read(){
	auto self(shared_from_this());
	std::memset(_data, 0, max_length);
	socket_.async_read_some(asio::buffer(_data, max_length), 
			[this, self](std::error_code err, std::size_t length){
		if(!err){
			std::cout << "Recieved data: " << std::endl;

			char* inBuffer = _data;
			char* buffer;
			unsigned int packet_length = 0;
			unsigned char mask[4];

		//	assert(inBuffer[0] == '0x81');
			packet_length = ((unsigned char) inBuffer[1]) & 0x7f;
			if(packet_length == 6){

				mask[0] = inBuffer[2];
				mask[1] = inBuffer[3];
				mask[2] = inBuffer[4];
				mask[3] = inBuffer[5];
				std::cout << "Packet Length: " << packet_length << std::endl;
				std::cout << "Mask: " << mask << std::endl;
			
				for(unsigned int i = 0; i < packet_length; i++){
					inBuffer[6+i] ^= mask[i % 4];
				}
				std::cout << "Message from client: " << inBuffer << std::endl;
			}
			//	xor_decrypt(decimal);

			/* 
			 * Read bits 9-15 (inclusive) and interpret that as an unsigned integer. 
			 * If it's 125 or less, then that's the length; you're done. 
			 * If it's 126, go to step 2. If it's 127, go to step 3.
			 * 
			 * Read the next 16 bits and interpret those as an unsigned integer. You're done.
			 *
			 * Read the next 64 bits and interpret those as an unsigned integer. 
			 * (The most significant bit must be 0.) You're done.
			 *
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

	asio::async_write(socket_, asio::buffer(response.c_str(), response.size()),
			[this, self](std::error_code err, std::size_t /*lenght*/){
		if(!err){
			do_read();
		}
	});
}
