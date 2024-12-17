import socket
import sqlite3
import os
from Crypto.Random import get_random_bytes
from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP
import binascii
import zlib

""" AES and encryption """

def generate_aes_key():
    # Generate a random AES key
    return get_random_bytes(32)

def encrypt_aes_key_with_rsa(aes_key, public_key):
    # Load the RSA public key
    rsa_key = RSA.import_key(public_key)
    
    # Encrypt the AES key using RSA and the PKCS1_OAEP padding scheme
    cipher = PKCS1_OAEP.new(rsa_key)
    encrypted_aes_key = cipher.encrypt(aes_key)
    
    return encrypted_aes_key

def from_hex(hex_key):
    # Convert the hex string to bytes
    key_bytes = binascii.unhexlify(hex_key)

    # Return the key as a string
    return key_bytes.decode('latin1')

def to_hex(byte_string):
    # Convert the byte string to hex and return as a string
    return binascii.hexlify(byte_string).decode('ascii')

def calculate_crc(data, aes_key):
    if not isinstance(data, str):
        data = str(data)
    if not isinstance(aes_key, str):
        aes_key = str(aes_key)

    # Convert the input strings to bytes
    data_bytes = data.encode('ascii')
    aes_key_bytes = aes_key.encode('ascii')

    # Combine the data and AES key
    data_with_key = data_bytes + aes_key_bytes

    # Calculate the CRC-32 checksum using the zlib library
    crc = zlib.crc32(data_with_key)

    # Convert the unsigned integer to a signed integer
    crc_signed = crc & 0xFFFFFFFF

    # Flip the bits of the signed integer and return it
    return crc_signed ^ 0xFFFFFFFF


""" Data Base """

def delete_database():
    os.remove("clients.db")
import sqlite3

def create_database():
    conn = sqlite3.connect('clients.db')
    c = conn.cursor()
    c.execute('''
        CREATE TABLE IF NOT EXISTS clients (
            uuid TEXT UNIQUE,
            name TEXT UNIQUE,
            publickey TEXT UNIQUE,
            aeskey TEXT UNIQUE,
            fileData TEXT UNIQUE
        )
    ''')
    conn.commit()
    conn.close()

def add_user_by_name_and_uuid(name, uuid):
    conn = sqlite3.connect('clients.db')
    c = conn.cursor()

    c.execute('INSERT INTO clients (uuid, name) VALUES (?, ?)', (uuid, name))
    
    conn.commit()
    conn.close()

def check_user_by_name(name):
    conn = sqlite3.connect('clients.db')
    c = conn.cursor()
    c.execute("SELECT * FROM clients WHERE name=?", (name,))
    result = c.fetchone()
    conn.close()
    return result is not None

def find_name_return_file(name):
    conn = sqlite3.connect('clients.db')
    c = conn.cursor()
    c.execute("SELECT fileData FROM clients WHERE name=?", (name,))
    result = c.fetchone()
    conn.close()
    return result

def find_aesKey(name):
    conn = sqlite3.connect('clients.db')
    c = conn.cursor()
    c.execute("SELECT aeskey FROM clients WHERE name=?", (name,))
    result = c.fetchone()
    conn.close()
    return result

def add_users_publickey(name, publickey):
    conn = sqlite3.connect('clients.db')
    c = conn.cursor()
    c.execute("UPDATE clients SET publickey=? WHERE name=?", (publickey, name))
    conn.commit()
    conn.close()

def add_users_file(uuid, file_data):
    conn = sqlite3.connect('clients.db')
    c = conn.cursor()
    c.execute("UPDATE clients SET fileData=? WHERE name=?", (file_data, name))
    conn.commit()
    conn.close()

def add_users_aeskey(name, aeskey):
    conn = sqlite3.connect('clients.db')
    c = conn.cursor()
    c.execute("UPDATE clients SET aeskey=? WHERE name=?", (aeskey, name))
    conn.commit()
    conn.close()

def print_all_info():
    conn = sqlite3.connect('clients.db')
    c = conn.cursor()

    c.execute("SELECT * FROM clients")
    rows = c.fetchall()
    for row in rows:
        print(row)

    conn.close()


""" uuid """ 
def create_uuid(name):
    output_str = ""
    for char in name:
        output_str += format(ord(char), "02x")
    return output_str

delete_database()
create_database()

# Create a socket object
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Get the local host name
host = "127.0.0.1"

# Set the port number
try:
    with open('port.info', 'r') as f:
        port = int(f.readline().strip())
except FileNotFoundError:
    port = 1234


# Bind the socket to the port
server_socket.bind((host, port))

# Start listening for incoming connections
server_socket.listen()

print("Listening for incoming connections on port %d..." % port)

# Accept an incoming connection
client_socket, client_address = server_socket.accept()

print("Received incoming connection from %s:%d" % (client_address[0], client_address[1]))

# Waiting in infinite loop for client requests
while True:
    
    # Receive request number from the client
    data = client_socket.recv(1024).decode()
    client_socket.send("ACK".encode())
    
    # handle requests

    # registration request
    if data == "1100":
        # Receive data from the client
        name = client_socket.recv(1024).decode()
        # Send an acknowledgment to the client
        client_socket.send("ACK".encode())

        if check_user_by_name(name): # user is already registered
            client_socket.send("2101".encode('utf-8')) # return false - 2101
        else:
            uuid = create_uuid(name)
            add_user_by_name_and_uuid(name, uuid)
      
            if check_user_by_name(name): # if user had been registered
                client_socket.send("2100".encode('utf-8')) # return true - 2100
                # send uuid to client client_socket.send(uuid.encode())
            else:
                client_socket.send("2101".encode('utf-8')) # return false - 2101
    
                
    # send public key to server request
    elif data == "1101":
        # Receive user name from the client
        name = client_socket.recv(1024).decode()
        # Send an acknowledgment to the client
        client_socket.send("ACK".encode())

        # Receive public key from the client
        publicKeyHex = client_socket.recv(1280).decode()
        # Send an acknowledgment to the client
        client_socket.send("ACK".encode())

        aeskey = generate_aes_key()
        # add aes key to data base
        add_users_aeskey(name, str(aeskey))

        publicKey = from_hex(publicKeyHex)
        # add public key to data base
        add_users_publickey(name, publicKey)
 
        encrypted_aeskey = encrypt_aes_key_with_rsa(aeskey, publicKey)
        aeskeyHexEncrypted = to_hex(encrypted_aeskey)
        
        client_socket.send("2102".encode('utf-8')) # return true - 2102
        
        length = len(aeskeyHexEncrypted)
        lenStr = str(length)
        
        aeskeyHex = to_hex(aeskey)
        client_socket.send(lenStr.encode('utf-8'))
        client_socket.send(aeskeyHex.encode('utf-8'))
        

    # send file
    if data == "1103":
        # Receive data from the client
        name = client_socket.recv(1024).decode()
        # Send an acknowledgment to the client
        client_socket.send("ACK".encode())

        # Receive the file size
        size_bytes = client_socket.recv(4)  # assuming 4-byte file size
        file_size = int.from_bytes(size_bytes, byteorder='big')

        # Receive the file data
        file_data = client_socket.recv(file_size)
        
        # Write the file data to a new file
        with open('received_file.txt', 'wb') as f:
            f.write(file_data)

        # add file path to data base
        add_users_file(name, file_data)

        client_socket.send("2103".encode('utf-8')) # return true - 2103

    if (data == "1104" or data == "1105" or data == "1106"):
        # Receive uuid from the client
        name = client_socket.recv(1024).decode()
        # Send an acknowledgment to the client
        client_socket.send("ACK".encode())

        # Receive file name from the client
        fileName = client_socket.recv(1280).decode()
        # Send an acknowledgment to the client
        client_socket.send("ACK".encode())

        fileData = find_name_return_file(name)
        aesKey = find_aesKey(name)
        checkCRC = calculate_crc(fileData, aesKey)
        response = str(checkCRC)
        client_socket.send(response.encode('utf-8'))

    # Print all the users in the database
    #print_all_info()


    # Close the client socket
    # client_socket.close()

    # Close the server socket
    # server_socket.close()

 

# see what's up with localhost ip address later on!!!

# function to read port number from file - default port is 12345