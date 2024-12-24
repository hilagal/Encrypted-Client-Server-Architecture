""" This file contains the encryption-related functions. """

from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP
from Crypto.Random import get_random_bytes
import binascii
import zlib

def generate_aes_key():
    """
    Generates a random 256-bit AES key.
    """
    return get_random_bytes(32)

def encrypt_aes_key_with_rsa(aes_key, public_key):
    """
    Encrypts an AES key using an RSA public key with PKCS1_OAEP padding.
    """
    rsa_key = RSA.import_key(public_key)
    cipher = PKCS1_OAEP.new(rsa_key)
    encrypted_aes_key = cipher.encrypt(aes_key)
    return encrypted_aes_key


def from_hex(hex_key):
    """
    Converts a hex string to a byte string.
    """
    key_bytes = binascii.unhexlify(hex_key)
    return key_bytes.decode('latin1')


def to_hex(byte_string):
    """
    Converts a byte string to a hexadecimal string.
    """
    return binascii.hexlify(byte_string).decode('ascii')


def calculate_crc(data, aes_key):
    """
    Calculates the CRC-32 checksum of combined data and AES key.
    """
    if not isinstance(data, str):
        data = str(data)
    if not isinstance(aes_key, str):
        aes_key = str(aes_key)

    data_bytes = data.encode('ascii')
    aes_key_bytes = aes_key.encode('ascii')
    data_with_key = data_bytes + aes_key_bytes

    crc = zlib.crc32(data_with_key)
    crc_signed = crc & 0xFFFFFFFF

    return crc_signed ^ 0xFFFFFFFF
