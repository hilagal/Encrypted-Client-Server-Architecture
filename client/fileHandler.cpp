/*
* Implements functions for managing files. Includes logic for reading files from disk,
* checking CRC values, and preparing files for transfer to the server. 
* Also handles error-checking during file operations.
*/

#include "fileHandler.h"

void handle_file()
{
	std::ifstream file("transfer.info");

	if (file.is_open()) {
		std::string line;
		std::getline(file, line);

		ip = line.substr(0, line.find(":")); // get the ip address from the file
		port = line.substr(line.find(":") + 1, line.length()); // get the port number from the file

		std::getline(file, line);
		userName = line;

		std::getline(file, line);
		fileName = line;

		file.close();
	}
	else {
		std::cout << "Unable to open \"transfer.info\" file" << std::endl;
	}
}

void handleMeFile() {
	std::fstream file;
	file.open("me.info", std::ios::in | std::ios::out | std::ios::app);
	if (file.is_open()) {
		std::getline(file, userName);
		std::getline(file, uuid);
		std::getline(file, stringPrivateKey);
		generateRSAPublicKey();
		file.close();
	}
	else { // file does not exist - open one and register user based on transfer.info
		std::cout << "creating file" << std::endl;
		file.open("me.info", std::ios::out | std::ios::trunc);
		file << userName << std::endl;
		sendRegistrationRequest();
		file << uuid << std::endl;
		generateRSAKeyPair();
		file << stringPrivateKey << std::endl;
		file.close();
	}
}


std::string get_ip() {
	return ip;
}

std::string get_port() {
	return port;
}

std::string get_username() {
	return userName;
}