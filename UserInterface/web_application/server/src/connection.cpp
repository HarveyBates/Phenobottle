#include "connection.h"

extern bool wsCommand;

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
Connection::Connection(asio::io_context& io_context) : socket_(io_context){

}

/* [Reminder] I'm pretty sure I dont need this funciton */
void Connection::handle_write(const asio::error_code& /*error*/,
		size_t /*bytes_transferred*/){
}

void xor_decrypt(char* inBuffer){
	/* Decode frame from incoming message using xor decryption.
	 * A mask (4 byte) sequence of char's are used to decrypt the incoming data.
	 * The positon of the mask varies with the size of the incoming data, 
	 * therefore two functions are provided (<125 & 126).
	 */
	
	// Create output array and fill to delete any previous values
	char outBuffer[max_length];
	std::fill_n(outBuffer, max_length, 0);
	
	uint16_t packet_length = 0; // Length of package data
	unsigned char mask[4]; // Used for xor decryption
	unsigned int offset = 0; // Offset position of data based on data length
	
	// Ensures the data received is a message
	if((unsigned char)inBuffer[0] == 0x81){

		/* Get the length of the incoming data */
		packet_length = ((unsigned char) inBuffer[1]) & 0x7f;

		if(packet_length == 126){
			/* For packet sizes of 126:
			 * byte 0: The 0x81 is just an indicator that a message received
			 * byte 1: will be 0xfe
			 * byte 2, 3: the length of the payload as a uint16 number
			 * byte 4, 5, 6, 7: the 4 byte xor key to decrypt the payload
			 * the rest: payload
			 */
		
			// Get length of larger packet size
			unsigned int lenByte1 = inBuffer[2];
			unsigned int lenByte2 = inBuffer[3];
			packet_length = ((lenByte1 << 8) & 0xFF00) + (lenByte2 & 0x00FF);

			// Get mask for 126 pkt size data
			mask[0] = inBuffer[4];
			mask[1] = inBuffer[5];
			mask[2] = inBuffer[6];
			mask[3] = inBuffer[7];
			
			// Set a 8 byte offset to read mask first
			offset = 8;
	
			// Interpret data after the offset and decrypt based on mask keys
			for(unsigned int i = 0; i < packet_length; i++){
				int arrPos = offset + i;
				outBuffer[i] = inBuffer[arrPos] ^ mask[i % 4];
			}
		}
		else{
			/* For packet sizes < 125
			 * byte 0: The 0x81 is just an indicator that a message received
			 * byte 1: the 0x8a is the length, substract 0x80 from it, 0x0A == 10
			 * byte 2, 3, 4, 5: the 4 byte xor key to decrypt the payload
			 * the rest: payload
			 */
			
			mask[0] = inBuffer[2];
			mask[1] = inBuffer[3];
			mask[2] = inBuffer[4];
			mask[3] = inBuffer[5];
			
			// Set a 6 bit offset (after mask)
			offset = 6;

			/* Use mask(key) to decode message from client 
			 * Offset of 6 as this is the bytes after the mask
			 */
			for(unsigned int i = 0; i < packet_length; i++){
				unsigned int arrPos = offset + i;
				outBuffer[i] = inBuffer[arrPos] ^ mask[i % 4];
			}
		}
		std::cout << "Output buffer: " << outBuffer << std::endl;
		wsCommand = true;
	}
}


void Connection::do_read(){
	/* Shared ptr to keep connection */
	auto self(shared_from_this());

	// Reset _data array with zeros
	std::fill_n(_data, max_length, 0);
	
	/* Read incoming data */
	socket_.async_read_some(asio::buffer(_data, max_length), 
			[this, self](std::error_code err, std::size_t length){
		if(!err){
			// Store data as a char for deryption
			char* inBuffer = _data;

			// If handshake has already occured no need to decrypt this message
			if(handshake){
				xor_decrypt(inBuffer);
			}
			else{
				std::cout << "\nHandshake: \n" << _data << std::endl;
				handshake = true; // Handshake has occured
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
