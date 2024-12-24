/*
* Manages socket-based communication with the server, including sending
* and receiving data using Boost Asio. It abstracts socket operations 
* to streamline network communications.
*/

#ifndef SOCKETHANDLER_H
#define SOCKETHANDLER_H

// Boost libraries
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/asio/detail/io_uring_descriptor_service.hpp> // Needed for socket management
#include <cstring>

// Standard libraries
#include <string>

using boost::asio::ip::tcp;
using boost::asio::io_context;


/*
 * Attempts to establish a TCP connection with the server using the specified IP address, port, and username.
 *
 * Parameters:
 * - None directly; uses `handle_file()`, `get_ip()`, `get_port()`, and `get_username()` to obtain necessary information.
 *
 * Returns:
 * - `true` if the connection is successfully established.
 * - `false` if there is an error during the connection process.
 */
bool open_socket();

/*
 * Sends data to the server and waits for an acknowledgment.
 *
 * Parameters:
 * - socket: The TCP socket used for communication.
 * - data: The data to be sent to the server.
 */
void sendData(tcp::socket& socket, std::string data);

/*
 * Reads data from the server and returns it as a string.
 *
 * Parameters:
 * - `size` (std::size_t): The number of bytes to read from the server.
 *
 * Returns:
 * - A 'std::string` containing the received data.
 */
std::string read_from_server(std::size_t size);

/*
 * Receives a specified amount of data from the server.
 *
 * Parameters:
 * - socket: The TCP socket used for communication with the server.
 * - size: The number of bytes to receive from the server.
 *
 * Returns:
 * - A string containing the received data.
 */
std::string get_info_from_server(boost::asio::ip::tcp::socket& socket, int size);

#endif // SOCKETHANDLER_H
