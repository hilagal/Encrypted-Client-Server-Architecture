/*
* This is the main header file for the client-side application of a file transfer system.
*
* Purpose: Facilitates secure communication with a server, handling operations like registration, key exchange, file encryption, and CRC validation.
* Key Features:
*		- Uses Boost.Asio for networking.
*		- Implements Crypto++ for cryptographic operations.
*		- Designed to ensure secure and reliable data transmission.
*/

#ifndef MAIN_H
#define MAIN_H

// Standard library includes
#include <iostream>
#include <boost/asio.hpp> // For network-related functionality
#include <boost/system/system_error.hpp> // For handling errors in Boost
#include <fstream> // For file handling (if used in main function)

// Boost ASIO types used in the code
using boost::asio::ip::tcp;
using boost::asio::io_context;

// Include all other necessary header files
#include "rsa_keys.h"
#include "socketHandler.h"
#include "fileHandler.h"
#include "clientRequest.h"

/*
 * Opens a socket, connects to the server, and handles user input.
 *
 * This function performs the following actions:
 * 1. Opens a socket and connects to the server using information from a file.
 * 2. Processes the `me.info` file before handling user requests.
 * 3. Continuously reads and processes user input in an infinite loop.
 */
void run();

#endif // MAIN_H