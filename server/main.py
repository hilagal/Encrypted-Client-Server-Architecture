""" This is the main file, where the server runs and integrates everything. """

import socket
from encryption import generate_aes_key, encrypt_aes_key_with_rsa, from_hex, to_hex, calculate_crc
from database import delete_database, create_database, add_user_by_name_and_uuid, check_user_by_name, create_uuid

# Delete and create the database
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
    
    # Handle requests

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
    elif data == "1103":
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

    elif (data == "1104" or data == "1105" or data == "1106"):
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


    # Close the client socket
    client_socket.close()

    # Close the server socket
    server_socket.close()