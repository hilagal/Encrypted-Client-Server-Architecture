/*
* Defines functions for handling client-side requests. 
* Includes the readInput() function that processes user commands, as well as other 
* request-related functions like sending registration data, public keys, and files 
* to the server.
*/

#ifndef CLIENTREQUEST_H
#define CLIENTREQUEST_H

// Standard libraries
#include <string>

// Boost libraries
#include <boost/asio.hpp>
#include <boost/algorithm/string/trim.hpp> // For trimming whitespace in strings

// CryptoPP libraries
#include <aes.h>
#include <hex.h>

/*
 * Sends a registration request to the server and handles the server's response.
 *
 * This function sends a request to register a user on the server. It sends the user name and waits for
 * a response. Depending on the server's response, it either indicates that the user is already registered
 * or successfully registers the user and generates an RSA public key.
 *
 * @return True if the registration process is successful or the user is already registered;
 *         False if the registration fails for another reason.
 */
bool sendRegistrationRequest();

/*
 * Sends the public key to the server and retrieves the AES key.
 *
 * This function performs the following steps:
 * 1. Sends a registration request to the server with the request number "1101" and the username.
 * 2. Converts the public key to a hexadecimal string using the `toHex()` function.
 * 3. Sends the hexadecimal public key to the server.
 * 4. Waits for a response from the server to confirm the successful receipt of the public key.
 * 5. If the server responds with "2102", it requests the AES key length, retrieves the AES key,
 *    and stores it in the `stringAESkeyHex` variable.
 * 6. Outputs a success message if the public key was sent successfully and the AES key arrived.
 *
 * Returns:
 * - `true` if the public key was sent successfully and the encrypted AES key arrived.
 * - `false` if the server response is not "2102" indicating failure.
 */
bool sendPublicKey();

/*
 * Encrypts a file using an AES key and sends it to the server.
 *
 * This function first sends a request to the server, then decrypts the AES key from a hexadecimal string
 * and saves it. The file is encrypted using the AES key and sent to the server. Finally, the function checks
 * the server's response to confirm whether the file was successfully sent.
 *
 * @return True if the file was encrypted and sent successfully to the server; otherwise, false.
 */
bool sendFile();

/*
 * Sends a file to the server over a socket connection.
 *
 * This function opens the specified file, reads its contents into a buffer, and then sends the file size
 * followed by the file data to the server using the socket connection.
 *
 * @param file_path The path of the file to be sent to the server.
 */
void sendFileToServer(const std::string& file_path);

/*
 * Check the CRC checksum by sending the user name and file name to the server,
 * and comparing the calculated CRC with the one received from the server.
 *
 * This function sends the user name and file name to the server, calculates the CRC-32 checksum
 * for the file combined with the AES key, and compares it with the checksum received from the server.
 * If the calculated checksum matches the server's checksum, it returns true; otherwise, false.
 *
 * @return true if the CRC matches the server's checksum, false otherwise
 */
bool checkCRC();
int crc_cnt; // Global variable for CRC count

/*
 * Calculates the CRC-32 checksum for a file combined with an AES key.
 *
 * @param filepath: The path to the file whose contents are included in the checksum.
 * @param aes_key: The AES key to be appended to the file contents.
 * @return: The CRC-32 checksum of the combined file data and AES key.
 */
unsigned int calculate_crc(const std::string& filepath, const std::string& aes_key);

/*
 * Encrypts a file using AES encryption with the provided AES key.
 *
 * This function reads the contents of a file, encrypts the data using AES encryption in CBC mode,
 * and writes the encrypted data back to the file. The AES encryption uses the key passed as a string
 * parameter and operates in CBC mode. The AES key string is converted into a byte vector before encryption.
 *
 * @param aes_key_str The AES key in string format used for encryption.
 */
void encryptFileWithAES(const std::string& aes_key_str);

/*
 * Converts a hexadecimal string to its ASCII representation.
 *
 * This function takes a string of hexadecimal characters, processes each pair of characters, converts them
 * to their integer values, and then to their corresponding ASCII characters. The resulting string is the ASCII
 * representation of the original hexadecimal input.
 *
 * @param hexString A string representing the hexadecimal input to be converted.
 *
 * @return A string representing the ASCII output corresponding to the given hexadecimal string.
 */
std::string from_hex(const std::string& hexString);

/*
 * Reads user input to determine a request number and processes the corresponding action.
 *
 * The function prompts the user to enter a request number and performs the following actions based on the input:
 * 1. `1100`: Sends a registration request.
 * 2. `1101`: Sends the public key to the server.
 * 3. `1103`: Sends a file to the server.
 * 4. `1104`: Checks the CRC value and prints the result.
 * 5. `1105`: Checks the CRC value once, with a counter condition.
 * 6. `1106`: Checks the CRC value for the fourth time and handles the result.
 */
void readInput();

#endif // CLIENTREQUEST_H
