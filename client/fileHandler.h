/*
* Handles file-related operations such as reading, writing, and file integrity checks. 
* It provides utilities to support file transfer and CRC validation.
*/

#ifndef FILEHANDLER_H
#define FILEHANDLER_H

// Standard libraries
#include <string>
#include <fstream>

/*
 * Reads and parses the 'transfer.info' file to retrieve the IP address, port number, username, and file path.
 * If the file cannot be opened or the data is invalid, defaults are set: IP address to "127.0.0.1" and port to "12345".
 *
 * Parameters:
 * - None
 *
 * Returns:
 * - None
 */
void handle_file();

/*
 * Handles the "me.info" file by reading or creating it, and performing necessary operations based on its existence.
 *
 * This function does the following:
 * 1. Opens the "me.info" file for reading and writing.
 * 2. If the file exists:
 *    - Reads the user's name, UUID, and private key from the file.
 *    - Generates the RSA public key.
 * 3. If the file does not exist:
 *    - Creates a new "me.info" file and registers the user based on data from the "transfer.info" file.
 *    - Writes the user's name, UUID, and private key to the file.
 *    - Generates an RSA key pair.
 */
void handleMeFile();

/*
 * Returns the IP address stored in the global variable 'ip'.
 *
 * Parameters:
 * - None
 *
 * Returns:
 * - A string containing the IP address.
 */
std::string get_ip();

/*
 * Returns the port number stored in the global variable 'port'.
 *
 * Parameters:
 * - None
 *
 * Returns:
 * - A string containing the port number.
 */
std::string get_port();

/*
 * Returns the username stored in the global variable 'userName'.
 *
 * Parameters:
 * - None
 *
 * Returns:
 * - A string containing the username.
 */
std::string get_username();

#endif // FILEHANDLER_H
