#include <iostream>
#include <boost/asio.hpp>
#include <boost/system/system_error.hpp>
#include <fstream>
#include <sys/stat.h>
#include <sstream>
#include <string>

// rsa keys
#include "cryptlib.h"
#include <osrng.h>
#include <rsa.h>
#include <aes.h>
#include <hex.h>
#include <base64.h>
#include <integer.h>
#include <filters.h>
#include <vector>
#include <modes.h>
#include <iomanip>

//clientrequest.h
#include <algorithm>
#include <boost/algorithm/string/trim.hpp>

// sockethandler.h
#include <boost/asio/detail/io_uring_descriptor_service.hpp>
#include <cstring>
#include <boost/array.hpp>


// rsa keys

CryptoPP::AutoSeededRandomPool prng;

using CryptoPP::RSA;
using CryptoPP::AutoSeededRandomPool;
using CryptoPP::Base64Encoder;
using CryptoPP::StringSink;
using CryptoPP::HexEncoder;
using CryptoPP::Exception;


// socket
boost::asio::io_context io_context;
boost::asio::ip::tcp::socket gsocket(io_context);

using boost::asio::ip::tcp;
using boost::asio::io_context; 

void run();

// rsa keys
const unsigned int RSA_KEY_BYTES = 160; // key size in bytes
CryptoPP::RSA::PrivateKey privateKey;
CryptoPP::RSA::PublicKey publicKey;
void generateRSAPublicKey();
void generateRSAKeyPair();
void toHex();

std::string decrypt_aes_rsa(std::string cipher_text);

// spckethandler.h
bool open_socket();
void sendData(tcp::socket& socket, std::string data);
//std::string receive_data(tcp::socket& socket, size_t len);
//std::string getData(tcp::socket& socket, int size);
std::string read_from_server(std::size_t size);
std::string get_info_from_server(boost::asio::ip::tcp::socket& socket, int size);


// filehandler.h for transfer.info
std::string ip, port, userName, fileName, uuid, stringPrivateKey, stringPublicKey, stringPublicKeyHex;
std::string aeskey_encryptedwithpkey, stringAESkey, stringAESkeyHex;
void handle_file();
void handleMeFile();
std::string get_ip();
std::string get_port();
std::string get_username();


void readInput();
// clientrequest.h
std::string receive_data(tcp::socket& socket, size_t len);
bool sendRegistrationRequest();
bool sendPublicKey();
bool sendFile();
bool checkCRC();
int crc_cnt = 0;
unsigned int calculate_crc(const std::string& filepath, const std::string& aes_key);

void encryptFileWithAES(const std::string& aes_key_str);
std::string from_hex(const std::string& hexString);
/*/	bool sendFileToServer(const std::string& cid, const std::string& size, const std::string& fname, const std::string& mcontent);
	bool checkFirstCRC(const std::string& cid, const std::string& fname);
	bool checkSecondCRC(const std::string& cid, const std::string& fname);
	bool checkFourthCRC(const std::string& cid, const std::string& fname);*/
