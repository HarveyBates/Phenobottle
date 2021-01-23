#pragma once

#include <iostream>
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

const std::size_t max_length = 2048;

class Connection : public std::enable_shared_from_this<Connection>{

public:
	typedef std::shared_ptr<Connection> pointer;

	static pointer create(asio::io_context& io_context);

	asio::ip::tcp::socket& socket();
	
	void start();

private:
	Connection(asio::io_context& io_context);

	void handle_write(const asio::error_code& /*error*/,
			size_t /*bytes_transferred*/);

	void do_read();

	void do_write(std::size_t length);

	asio::ip::tcp::socket socket_;
	char _data[max_length];
};

