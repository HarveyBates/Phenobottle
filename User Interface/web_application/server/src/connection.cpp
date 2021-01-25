#include "connection.h"

/* Magic string as provided by WebSocket protocol */
const std::string ws_magic_string = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

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

std::string xor_decrypt(uint16_t* decimal){
	/* [Reminder] This is where I will decode the incoming requests depending 
	 * on their package length
	 */
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


void Connection::do_read(){
	/* Shared ptr to keep connection */
	auto self(shared_from_this());

	/* Reset _data array with zeros
	 * [Reminder] There is proberbly a better way to clear the array
	 */
	std::memset(_data, 0, max_length);
	
	/* Read incoming data */
	socket_.async_read_some(asio::buffer(_data, max_length), 
			[this, self](std::error_code err, std::size_t length){
		if(!err){
			std::cout << "Recieved data: " << std::endl;

			char* inBuffer = _data;
			char* buffer;
			unsigned int packet_length = 0;
			unsigned char mask[4];
			
		//	assert(inBuffer[0] == '0x81');
			
			/* Get the length of the incoming data */
			packet_length = ((unsigned char) inBuffer[1]) & 0x7f;
			
			/* [Reminder] Need to have two functions:
			 * 1. For incoming connection request
			 * 2. For incoming data
			 */
			if(packet_length == 6){

				/* [Reminder] Mask is composed of four bytes.
				 * The position of these bytes change with the size of the packet.
				 * Packet size = 126 (length of data is given by the following 2 bytes)
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
				 * If I want to read longer strings (JSON data) I need to implement
				 * both of these methods.
				 */
				mask[0] = inBuffer[2];
				mask[1] = inBuffer[3];
				mask[2] = inBuffer[4];
				mask[3] = inBuffer[5];
				std::cout << "Packet Length: " << packet_length << std::endl;
				std::cout << "Mask: " << mask << std::endl;
			
				/* Use mask(key) to decode message from client 
				 * Offset of 6 as this is the bytes after the mask
				 * 
				 * [Reminder] This offset (6) will change if the packet_length changes
				 */
				for(unsigned int i = 0; i < packet_length; i++){
					inBuffer[6+i] ^= mask[i % 4];
				}

				/* Take decoded input buffer (inBuffer) and extract only the data 
				 * (not the mask)
				 */
				std::string output = std::string(inBuffer).substr(6, std::string(inBuffer).length());
				std::cout << "Message from client: " << output << std::endl;
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
