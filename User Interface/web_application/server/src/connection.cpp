#include "connection.h"
#include <string>
#include <cstring>

/* Shared pointer to keep connection between client and server while
 * async functions are running */
typedef std::shared_ptr<Connection> pointer;

pointer Connection::create(asio::io_context& io_context){
	/* Create new connection using a unique io_context */
	return pointer(new Connection(io_context));
}

asio::ip::tcp::socket& Connection::socket(){
	/* Return a socket for a given io_context */
	return socket_;
}
	
void Connection::start(){
	/* Start async function with a read() command that bounces between write
	 * function */
	do_read();
}

/* Connection constructor with a socket */
Connection::Connection(asio::io_context& io_context) : socket_(io_context){}

/* [Reminder] I'm pretty sure I dont need this funciton */
void Connection::handle_write(const asio::error_code& /*error*/,
		size_t /*bytes_transferred*/){
}

void xor_decrypt(char* inBuffer){
	/* [Reminder] This is where I will decode the incoming requests depending 
	 * on their package length
	 */
	char outBuffer[max_length];
	std::fill_n(outBuffer, max_length, 0);
	char* buffer;
	unsigned int packet_length = 0;
	unsigned char mask[4];
	unsigned int offset = 0;			
	std::string output;
	// Ensures the data received is a message
	if((unsigned char)inBuffer[0] == 0x81){

		while(offset + 6 < (sizeof(&inBuffer)/sizeof(char))){

			/* Get the length of the incoming data */
			packet_length = ((unsigned char) inBuffer[1]) & 0x7f;
			std::cout << "Original packet length: " << packet_length << std::endl;

			if(packet_length == 126){
				/*
				 * byte 0: The 0x81 is just an indicator that a message received
				 * byte 1: will be 0xfe
				 * byte 2, 3: the length of the payload as a uint16 number
				 * byte 4, 5, 6, 7: the 4 byte xor key to decrypt the payload
				 * the rest: payload
				 */

				unsigned int lenByte1 = inBuffer[offset + 2];
				unsigned int lenByte2 = inBuffer[offset + 3];

				packet_length = ((lenByte1 << 8) & 0xFF00) + (lenByte2 & 0x00FF);

				// TODO Getting the right length now, but need to fix the mask 
				// for these longer messages
				std::cout << "Oversize packet length: " << packet_length << std::endl;

				mask[0] = inBuffer[offset + 4];
				mask[1] = inBuffer[offset + 5];
				mask[2] = inBuffer[offset + 6];
				mask[3] = inBuffer[offset + 7];

				for(unsigned int i = 0; i < packet_length; i++){
					int arrPos = offset + 8 + i;
					outBuffer[i] = inBuffer[arrPos] ^ mask[i % 4];
					//inBuffer[arrPos] ^= mask[i % 4];
					//std::cout << inBuffer[arrPos] << std::endl;
				}
				
				std::cout << "Input buffer: " << outBuffer << std::endl;

				//output = std::string(inBuffer).substr(8, std::string(inBuffer).length());
				
				//strcpy(outBuffer, output.c_str());

				offset += 8 + packet_length;

			}
			else if(packet_length == 127){
				// TODO add handle for package size: 127			
				size_t payload_length = (((size_t)inBuffer[2] << 56) +
						((size_t)inBuffer[3] << 48) +
						((size_t)inBuffer[4] << 40) + 
						((size_t)inBuffer[5] << 32) +
						((size_t)inBuffer[6] << 24) + 
						((size_t)inBuffer[7] << 16) +
						((size_t)inBuffer[8] << 8) +
						(size_t)inBuffer[9]);
				std::cout << payload_length << std::endl;
			}
			else{
				/*
				 * byte 0: The 0x81 is just an indicator that a message received
				 * byte 1: the 0x8a is the length, substract 0x80 from it, 0x0A == 10
				 * byte 2, 3, 4, 5: the 4 byte xor key to decrypt the payload
				 * the rest: payload
				 */

				mask[0] = inBuffer[offset + 2];
				mask[1] = inBuffer[offset + 3];
				mask[2] = inBuffer[offset + 4];
				mask[3] = inBuffer[offset + 5];
				std::cout << "Packet Length: " << packet_length << std::endl;
				std::cout << "Mask: " << mask << std::endl;

				/* Use mask(key) to decode message from client 
				 * Offset of 6 as this is the bytes after the mask
				 */
				for(unsigned int i = 0; i < packet_length; i++){
					unsigned int arrPos = offset + 6 + i;
					outBuffer[i] = inBuffer[arrPos] ^ mask[i % 4];
					//inBuffer[arrPos] ^= mask[i % 4];
				}
				
				std::cout << "Message: " << outBuffer << std::endl;

				/* Take decoded input buffer (inBuffer) and extract only the data 
				* (not the mask)
				 */
				//output = std::string(inBuffer).substr(6, std::string(inBuffer).length());
				//strcpy(outBuffer, output.c_str());
				
				offset += 6 + packet_length;
				//std::cout << "Message from client: " << output << std::endl;
			}
		}
		std::cout << outBuffer << std::endl;
	}
}


void Connection::do_read(){
	/* Shared ptr to keep connection */
	auto self(shared_from_this());

	/* Reset _data array with zeros
	 * TODO There is proberbly a better way to clear the array
	 */
	std::fill_n(_data, max_length, 0);
	
	/* Read incoming data */
	socket_.async_read_some(asio::buffer(_data, max_length), 
			[this, self](std::error_code err, std::size_t length){
		if(!err){
			std::cout << "Recieved data: " << std::endl;

			char* inBuffer = _data;
			/* TODO Need to have two functions:
			* 1. For incoming connection request
			* 2. For incoming data
			*/
			if(handshake){
				xor_decrypt(inBuffer);
			}
			else{
				std::cout << _data << std::endl;
				handshake = true;
			}
			/* Begin write funciton to keep connection */
			do_write();
		}
	});
}

void Connection::do_write(){
	/* Keeps connection alive as long as async process is functioning */
	auto self(shared_from_this());
	
	std::string tmp(_data);

	std::string response;
		
	// Debug: std::cout << SimpleWeb::Crypto::Base64::decode(response) << std::endl;
	
	/* Check to see if the message from the client is a connection request
	 * containing a websocket key */
	if(tmp.find("Sec-WebSocket-Key: ") != std::string::npos){
		
		// Extract WebSocket-Key from incoming connection request
		tmp.erase(0, tmp.find("Sec-WebSocket-Key: ") + strlen("Sec-WebSocket-Key: "));
		auto key = tmp.substr(0, tmp.find("\r\n"));
	
		// Apply SHA-1 crptographic hash function using the ws magic key string 
		auto sha1Key = SimpleWeb::Crypto::sha1(key + ws_magic_string);

		// Base64 encode the resulting SHA-1 key
		sha1Key = SimpleWeb::Crypto::Base64::encode(sha1Key);

		/* Respond with a switching protocols response (101) to initiale 
		 * WebSocket communications */
		response = 
			"HTTP/1.1 101 Switching Protocols\r\n"
			"Upgrade: websocket\r\n"
			"Connection: Upgrade\r\n"
			"Sec-WebSocket-Accept: " + sha1Key + "\r\n\r\n";
	}
	/* [Reminder] I am not sure if this decode is needed for non-connection request
	 * messages */
	else{
		response = SimpleWeb::Crypto::Base64::decode(_data);
	}
	
	/* Send back response to client with response message and begin read function 
	 * again */
	asio::async_write(socket_, asio::buffer(response.c_str(), response.size()),
			[this, self](std::error_code err, std::size_t /*lenght*/){
		if(!err){
			do_read();
		}
	});
}
