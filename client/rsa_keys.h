/*
* Declares cryptographic functions for encrypting and decrypting data using AES and RSA,
* as well as managing cryptographic keys. Integrates with the Crypto++ library.
*/

#ifndef RSA_KEYS_H
#define RSA_KEYS_H

// Standard libraries
#include <string>
#include <iostream>

// CryptoPP libraries
#include <cryptlib.h>
#include <osrng.h>
#include <rsa.h>
#include <aes.h>
#include <hex.h>
#include <base64.h>
#include <integer.h>
#include <filters.h>
#include <modes.h>
#include <iomanip>

using CryptoPP::RSA;
using CryptoPP::AutoSeededRandomPool;
using CryptoPP::Base64Encoder;
using CryptoPP::StringSink;
using CryptoPP::HexEncoder;
using CryptoPP::Exception;

/*
 * Generates or loads an RSA public key. If the public key is not already generated,
 * it creates a new RSA key pair and stores the keys.
 *
 * If the public key exists, it loads it from `stringPublicKey`.
 */
void generateRSAPublicKey();

/*
 * Generates an RSA key pair and stores it in `stringPrivateKey` and `stringPublicKey`.
 * If the private key is already generated, it loads it and regenerates the public key.
 *
 * This function uses `CryptoPP::RSA` to generate and manage keys.
 */
void generateRSAKeyPair();

/*
 * Converts the public key string to a hexadecimal string representation.
 *
 * This function converts a public key (stored as a string) to a hexadecimal string. The string is first
 * converted to a vector of bytes, and then each byte is converted to its hexadecimal equivalent.
 * The resulting hexadecimal string is stored in the `stringPublicKeyHex` variable.
 */
void toHex();

#endif // RSA_KEYS_H
