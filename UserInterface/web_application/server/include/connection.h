#pragma once

#include <iostream>
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <asio/ssl.hpp>
#include <string.h>

#include "constants.h"
#include "crypto.hpp"
#include "serial.h"

/* Tests if a new cmd has been recieved */
extern bool wsCommand;

/* Serial command for microcontroller */
extern char* serialCommand;

/* Max payload size in bytes */
const std::size_t max_length = 4096;

/* Magic string as provided by WebSocket protocol */
const std::string ws_magic_string = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

class Connection : public std::enable_shared_from_this<Connection>{
	bool handshake = false;
public:
	typedef std::shared_ptr<Connection> pointer;
	~Connection();
	static pointer create(asio::io_context& io_context);
	asio::ip::tcp::socket& socket();
	void start();
	void set_serial_command(char* cmd);
private:
	Connection(asio::io_context& io_context);
	void handle_write(const asio::error_code& /*error*/,
			size_t /*bytes_transferred*/);
	void do_read();
	void xor_decrypt(char* inBuffer);
	void do_write();
	asio::ip::tcp::socket socket_; // Unique socket for each client
	char _data[max_length]; // To store incoming message from client
};

