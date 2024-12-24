/*
* Implements cryptographic functions for securing data. 
* Handles encryption and decryption using AES and RSA, as well as key generation. 
* Integrates with the Crypto++ library for secure cryptographic operations.
*/

#include "rsa_keys.h"

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