// Create "resonably large buffer" to re-use for storing data
std::vector<char> vBuffer(20 * 1024);

void get_data(asio::ip::tcp::socket& socket){
	/* Asyncronous reading of data from address
	ASIO is primed by the async_read_some function
	This will run in the background and run our lambda function when there is data available
	This will continue to run until there is no data left to read */
	socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()), 
			[&](std::error_code ec, std::size_t length){
				if(!ec){
					std::cout << "\n\nRead" << length << " bytes\n\n";

					for(int i = 0; i < length; i++)
						std::cout << vBuffer[i];
				
					get_data(socket);
				}
			}
		);
}

/* Acts to maintain the connection and holds information about the connection */
class Session : public std::enable_shared_from_this<Session>{
public:
		Session(asio::ip::tcp::socket&& socket) : 
			socket(std::move(socket)){
		}

		void start(){
			socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()), 
					[&](std::error_code ec, std::size_t length){
						if(!ec){
							std::cout << "\n\nRead" << length << " bytes\n\n";

							for(int i = 0; i < length; i++)
								std::cout << vBuffer[i];
							
							start();
						}
			});

			/* Read data from socket into buffer and end at newline ('\n') char
			asio::async_read_until(socket, streambuf, '\n',
					[self = shared_from_this()] (asio::error_code ec,
						std::size_t bytes_transferred){
					std::cout << std::istream(&self->streambuf).rdbuf();
			});
			std::string response = 
				"GET /parameters HTTP/1.1\r\n"
				"Connection: close\r\n\r\n";
			socket.write_some(asio::buffer(response.data(), response.size()));
			*/
		}

private:	
	asio::ip::tcp::socket socket;
	asio::streambuf streambuf;
};


class Server{
public:
	Server(asio::io_context& io_context, std::uint16_t port) : 
		io_context(io_context),
			acceptor (io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)){
	}

	void async_accept(){
		socket.emplace(io_context);

		/* Wait for connection, once established assosicate this connection
		 * with this socket */
		acceptor.async_accept(*socket, [&] (asio::error_code ec){
				std::make_shared<Session> (std::move(*socket))->start();
				async_accept();
		});
	}

private:
	asio::io_context& io_context;
	asio::ip::tcp::acceptor acceptor;
	std::optional<asio::ip::tcp::socket> socket;
};


int main(){

	asio::io_context io_context;
	Server srv(io_context, 15001);
	srv.async_accept();
	io_context.run();

//	asio::error_code ec;
	
//	std::uint16_t port = 60000;

//	asio::io_context context;
//	asio::ip::udp::endpoint receiver(asio::ip::make_address("localhost", ec), port);
//	asio::ip::udp::socket socket(context, receiver);
//	if(socket.is_open()){		
//		for(;;){
//			char buffer[65536];
//			asio::ip::udp::endpoint sender;
//			std::size_t bytes_transferred = socket.receive_from(asio::buffer(buffer), sender);
//			socket.send_to(asio::buffer(buffer, bytes_transferred), sender);
//		}
//	}
//	else{
//		std::cout << "Error: " << ec.message() << std::endl;
//	}
	
	// // Define errorcode for future use
	// asio::error_code ec;

	// // Create a context for platform specific access to asio
	// asio::io_context context;

	// // Give the context some idle work do do in the background (as its async)
	// asio::io_context::work idleWork(context);

	// // Start context with the idle work above
	// std::thread thrContext = std::thread([&](){
	// 		context.run();
	// 		});

	// // Construct address to where we want to connect to (endpoint)
	// // TCP = Address is defined by an IP address and a PORT
	// asio::ip::tcp::endpoint endpoint(asio::ip::make_address("localhost", ec), 5000);

	// // Create networking socket (doorway to network)
	// asio::ip::tcp::socket socket(context);

	// // Tell socket to try to connect to endpoint
	// socket.connect(endpoint, ec);

	// if(!ec){
	// 	std::cout << "Connected" << std::endl;
	// }
	// else{ std::cout << "Failed: " << ec.message() << std::endl; }

	// // If connected make a HTTP request
	// if(socket.is_open()){
		
	// 	// Prime async read before we send the request for data
	// 	get_data(socket);
		
	// 	std::string sRequest = 
	// 		"GET /parameters HTTP/1.1\r\n"
	// 		"Connection: close\r\n\r\n";

	// 	// Send request
	// 	// Send as much data as possible (buffer contains array of bytes)
	// 	socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);

	// 	using namespace std::chrono_literals;
	// 	std::this_thread::sleep_for(20000ms);

	// 	context.stop();
	// 	if(thrContext.joinable()) thrContext.join();
	// }
	return 0;
}
