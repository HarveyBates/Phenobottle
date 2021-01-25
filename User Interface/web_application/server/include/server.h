#pragma once

#include <iostream>
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include "connection.h"

/* Port number used for server.
 * Typically port 80 for unsecured comms or 443 for secured.
 * Other ports can be used, however, problems with firewalls may 
 * occur. */
const unsigned short PORT = 443;

class Server{
public:
	Server(asio::io_context& io_context);

private:
	void start_accept();

	void handle_accept(Connection::pointer new_connection, 
			const asio::error_code& error);

	asio::io_context& io_context_; // Unique io_context for async functions
	asio::ip::tcp::acceptor acceptor_; 
};
