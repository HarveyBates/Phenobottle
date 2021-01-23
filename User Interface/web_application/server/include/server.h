#pragma once

#include <iostream>
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include "connection.h"

const unsigned short PORT = 15001;

class Server{
public:
	Server(asio::io_context& io_context);

private:
	void start_accept();

	void handle_accept(Connection::pointer new_connection, 
			const asio::error_code& error);

	asio::io_context& io_context_;
	asio::ip::tcp::acceptor acceptor_;
};
