/*
* Implements networking functionality using Boost Asio. 
* Handles sending and receiving data to/from the server, manages socket setup, and ensures reliable communication.
*/

#include "socketHandler.h"

bool open_socket() {
	try {
		// Create an io_context
		boost::asio::io_context io_context;

		// Get ip address and port
		handle_file();
		std::string ip = get_ip();
		std::string port = get_port();
		std::string userName = get_username();

		// Resolve the server address and port
		tcp::resolver resolver(io_context);
		tcp::resolver::results_type endpoints = resolver.resolve(ip, port);

		// Create a socket and connect to the server
		tcp::socket socket(io_context);
		boost::asio::connect(gsocket, endpoints);

	}
	catch (boost::system::system_error& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return false;
	}

	return true;
}

void sendData(tcp::socket& socket, std::string data)
{
	// Send data to the server
	boost::asio::write(socket, boost::asio::buffer(data));

	// Receive acknowledgment from the server
	boost::array<char, 3> buffer;
	int bytes_received = boost::asio::read(socket, boost::asio::buffer(buffer));
	std::string ack(buffer.data(), bytes_received);
}

std::string read_from_server(std::size_t size)
{
	std::vector<char> buffer(size); // buffer to store received data
	boost::system::error_code error;

	std::size_t bytes_transferred = boost::asio::read(gsocket, boost::asio::buffer(buffer), error);

	return std::string(buffer.data(), bytes_transferred);
}

std::string get_info_from_server(boost::asio::ip::tcp::socket& socket, int size)
{
	// Receive data from the server
	boost::array<char, 1024> buffer;
	int bytes_received = socket.read_some(boost::asio::buffer(buffer));

	// Return the received information
	return std::string(buffer.data(), bytes_received);
}