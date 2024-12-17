// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
// This program is a client side in a server-client prokect.
// There is a list of request that are possible to do in order to transfer files safely.

#include "main.h"


int main()
{
	run();
	return 0;
}


/* open socket & directs to input handling */
void run() {
	//open socket & connect to server from file info
	if (!open_socket()) {
		return;
	}
	handleMeFile(); // first handle me.info file, then continue to user requests
	// read user input in infintie loop
	for (;;) {
		readInput();
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

void readInput() {
	std::string input;
	std::cout << "Enter request number: ";
	std::getline(std::cin, input);
	int reqnum = std::stoi(input);

	if (reqnum == 1100) { // send registration request
		sendRegistrationRequest();
		return;
	}
	else if (reqnum == 1101) { // send public key to server
		sendPublicKey();
		return;
	}
	else if (reqnum == 1103) { // send file to server
		sendFile();
		return;

	}
	else if (reqnum == 1104) { // check crc
		sendData(gsocket, "1104");
		if (crc_cnt > 2) {
			std::cout << "CRC is >2" << std::endl;
			return;
		}
		if (checkCRC()) {
			std::cout << "CRC is correct." << std::endl;
			crc_cnt = 0;
			return;
		}
		else {
			std::cout << "CRC is incorrect." << std::endl;
			crc_cnt++;
			return;
		}
	}
	else if (reqnum == 1105) { // check crc
		sendData(gsocket, "1105");
		if (crc_cnt == 1) {
			if (checkCRC()) {
				std::cout << "CRC is correct." << std::endl;
				crc_cnt = 0;
				return;
			}
			std::cout << "CRC is incorrect." << std::endl;
			crc_cnt++;
		}
		return;
	}
	else if (reqnum == 1106) { // check crc 4th time
		sendData(gsocket, "1106");
		if (crc_cnt == 3) {
			if (!checkCRC()) {
				std::cout << "CRC is incorrect 4 times." << std::endl;
				crc_cnt = 0;
				return;
			}
			else {
				std::cout << "CRC is correct." << std::endl;
				crc_cnt = 0;
				return;
			}
		}
		return;
	}
}

unsigned int calculate_crc(const std::string& filepath, const std::string& aes_key) {
	// Open the file in binary mode
	std::ifstream infile(filepath, std::ios::binary);

	if (!infile) {
		std::cerr << "Error opening file " << filepath << std::endl;
		return 0;
	}

	// Read the file contents into a buffer
	infile.seekg(0, std::ios::end);
	std::streamsize file_size = infile.tellg();
	infile.seekg(0, std::ios::beg);
	char* buffer = new char[file_size];
	infile.read(buffer, file_size);

	// Combine the file contents and AES key
	std::string data_with_key(buffer, file_size);
	data_with_key += aes_key;

	// Calculate the CRC-32 checksum using the CRC algorithm
	const unsigned int polynomial = 0xEDB88320;
	unsigned int crc = 0xFFFFFFFF;

	for (std::size_t i = 0; i < data_with_key.size(); ++i) {
		crc ^= static_cast<unsigned int>(data_with_key[i]);
		for (std::size_t j = 0; j < 8; ++j) {
			crc = (crc >> 1) ^ (polynomial & (-(static_cast<int>(crc) & 1)));
		}
	}

	// Flip the bits of the CRC-32 checksum and return it
	return ~crc;
}


bool checkCRC() {
	sendData(gsocket, userName);
	sendData(gsocket, fileName);

	// calculate the crc , recieve calculation from server , check if equal
	int crcNum = calculate_crc(fileName, stringAESkey);
	std::string response;
	response = get_info_from_server(gsocket, 128);

	int calc = std::stoi(response);
	if (calc == crcNum) {
		return true;
	}
	return false;
}

// encrypt file with aes key and send to server
void encryptFileWithAES(const std::string& aes_key_str) {
	// Read the file data
	std::ifstream input_file(fileName, std::ios::binary);
	if (!input_file) {
		std::cerr << "Error: failed to open input file " << fileName << std::endl;
		return;
	}
	std::vector<char> file_data((std::istreambuf_iterator<char>(input_file)),
		std::istreambuf_iterator<char>());
	input_file.close();

	// Convert the AES key string to a byte vector
	std::vector<uint8_t> aes_key;
	aes_key.reserve(CryptoPP::AES::DEFAULT_KEYLENGTH);
	for (std::size_t i = 0; i < aes_key_str.length(); ++i) {
		aes_key.push_back(static_cast<uint8_t>(aes_key_str[i]));
	}

	// Encrypt the file data using AES in CBC mode
	CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption aes_encryption(aes_key.data(), CryptoPP::AES::DEFAULT_KEYLENGTH, aes_key.data());
	CryptoPP::StringSource(reinterpret_cast<const uint8_t*>(file_data.data()), file_data.size(), true,
		new CryptoPP::StreamTransformationFilter(aes_encryption, new CryptoPP::ArraySink(reinterpret_cast<uint8_t*>(file_data.data()), file_data.size())));

	// Write the encrypted data back to the file
	std::ofstream output_file(fileName, std::ios::binary);
	if (!output_file) {
		std::cerr << "Error: failed to open output file " << fileName << std::endl;
		return;
	}
	output_file.write(file_data.data(), file_data.size());
	output_file.close();

}



std::string from_hex(const std::string& hexString) {
	std::stringstream ss;

	// Iterate through the hex string two characters at a time
	for (std::size_t i = 0; i < hexString.size(); i += 2) {
		// Convert the current two hex characters to an integer
		int hexVal = std::stoi(hexString.substr(i, 2), nullptr, 16);

		// Convert the integer to its ASCII character representation
		ss << static_cast<char>(hexVal);
	}

	return ss.str();
}

void sendFileToServer(const std::string& file_path) {
	// Open the file
	std::ifstream input_file(file_path, std::ios::binary);
	if (!input_file) {
		std::cerr << "Error: failed to open input file " << file_path << std::endl;
		return;
	}

	// Read the file into a buffer
	std::vector<char> file_data((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());

	// Send the file size to the server
	uint32_t file_size = static_cast<uint32_t>(file_data.size());
	boost::asio::write(gsocket, boost::asio::buffer(&file_size, sizeof(file_size)));

	// Send the file data to the server
	boost::asio::write(gsocket, boost::asio::buffer(file_data));

}



bool sendFile() {
	std::string response;
	sendData(gsocket, "1103");
	sendData(gsocket, userName);

	// decrypt aes key from hex and save it
	stringAESkey = from_hex(aeskey_encryptedwithpkey);

	// encrypt file with aes key and send it
	encryptFileWithAES(stringAESkey);
	sendFileToServer(fileName);
	
	response = get_info_from_server(gsocket, 4);

	if (response == "2103") {
		std::cout << "File was encrypted with AES key and sent succssefully to the server." << std::endl;
		return true;
	}
	return false;
}

bool sendRegistrationRequest() {
	std::string reqnum = "1100";
	std::string response;
	sendData(gsocket, reqnum);
	sendData(gsocket, userName);

	response = get_info_from_server(gsocket, 4);
	
	if (response ==  "2101") {
		std::cout << "Registration failed (user is registered already)." << std::endl;
		return true;
	}
	if (response == "2100") {
		// get uuid from server uuid = get_info_from_server(gsocket, 128);
		generateRSAPublicKey();
		std::cout << "User is now registered" << std::endl;
		return true;
	}
	
	return false;

}

void toHex() {
	// Convert the public key string to a vector of bytes
	std::vector<uint8_t> publicKeyBytes(stringPublicKey.begin(), stringPublicKey.end());

	// Initialize a string stream to hold the hex-encoded output
	std::stringstream hexStream;

	// Output the bytes as hex to the string stream
	for (uint8_t byte : publicKeyBytes) {
		hexStream << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(byte);
	}

	// Return the hex-encoded string
	stringPublicKeyHex = hexStream.str();
}

bool sendPublicKey() {
	std::string response;
	sendData(gsocket, "1101");
	sendData(gsocket, userName);	

	toHex();
	sendData(gsocket, stringPublicKeyHex);

	response = get_info_from_server(gsocket, 4);

	if (response == "2102") {
		// get aes key length from server
		std::string len = get_info_from_server(gsocket, 10);
		
		int aesKeyLen = std::stoi(len);

		stringAESkeyHex = get_info_from_server(gsocket, aesKeyLen);
		std::cout << "Public key was sent succssefully and encrypted AES key arrived." << std::endl;
		return true;
	}
	return false;
}

std::string read_from_server(std::size_t size)
{
	std::vector<char> buffer(size); // buffer to store received data
	boost::system::error_code error;

	std::size_t bytes_transferred = boost::asio::read(gsocket, boost::asio::buffer(buffer), error);
	 
	return std::string(buffer.data(), bytes_transferred);
}


void generateRSAKeyPair() {

	if (stringPrivateKey.empty()) {
		privateKey.GenerateRandomWithKeySize(prng, RSA_KEY_BYTES * 8);
		publicKey = CryptoPP::RSA::PublicKey(privateKey);
		stringPrivateKey.clear();
		privateKey.Save(CryptoPP::StringSink(stringPrivateKey).Ref());
		stringPublicKey.clear();
		publicKey.Save(CryptoPP::StringSink(stringPublicKey).Ref());
	}
	else {
		privateKey.Load(CryptoPP::StringSource(stringPrivateKey, true).Ref());
		publicKey = CryptoPP::RSA::PublicKey(privateKey);
		stringPublicKey.clear();
		publicKey.Save(CryptoPP::StringSink(stringPublicKey).Ref());
	}
}

void generateRSAPublicKey() {

	if (stringPublicKey.empty()) {
		privateKey.GenerateRandomWithKeySize(prng, RSA_KEY_BYTES * 8);
		publicKey = CryptoPP::RSA::PublicKey(privateKey);
		stringPrivateKey.clear();
		privateKey.Save(CryptoPP::StringSink(stringPrivateKey).Ref());
		stringPublicKey.clear();
		publicKey.Save(CryptoPP::StringSink(stringPublicKey).Ref());
	}
	else {
		publicKey.Load(CryptoPP::StringSource(stringPublicKey, true).Ref());
	}

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

std::string get_info_from_server(boost::asio::ip::tcp::socket& socket, int size)
{
	// Receive data from the server
	boost::array<char, 1024> buffer;
	int bytes_received = socket.read_some(boost::asio::buffer(buffer));
	
	// Return the received information
	return std::string(buffer.data(), bytes_received);
}

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

/* Handle file 'transfer.info'
 * the function reads the information from the file and checks that it is valid,
 * if not - the default ip addres is 127.0.0.1 and default port number is 12345.
 * saves ip address, port number, username and file path */
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

std::string get_ip() {
    return ip;
}

std::string get_port() {
    return port;
}

std::string get_username() {
    return userName;
}
