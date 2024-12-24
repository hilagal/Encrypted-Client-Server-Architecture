# Encrypted Client-Server Architecture

## Overview

This project implements a **client-server** application with **encryption** for secure client and server communication. The project's main goal is to securely transfer files and sensitive information between the client and the server using **RSA** for public-key encryption and **AES** for symmetric encryption.

The client registers with the server, encrypts files with AES, and uploads them securely. The server handles storing user data, keys, and encrypted files in a **SQLite** database.

### **Key Features:**
- **Secure Registration:** The client and server use RSA encryption to exchange keys and register users securely.
- **File Encryption:** Files are encrypted using AES encryption before being sent from the client to the server.
- **Database Storage:** The server stores encrypted files and associated metadata in an SQLite database.
- **Client-Server Communication:** The communication between client and server is secure, ensuring confidentiality of data.

---

## Project Structure

The project consists of the following main components:

1. **client.py:** 
   - The client script handles the registration of new users, file encryption, and file upload.
   - The client interacts with the server over a network to transmit encrypted data.
   
2. **server.py:** 
   - The server listens for incoming requests, processes them, and stores the data in a local SQLite database.
   - The server also handles key management, decrypting files, and managing user data.

3. **users.db:** 
   - This is the SQLite database file that stores user information, RSA public/private keys, and encrypted files.
   
4. **requirements.txt:** 
   - A file listing the required dependencies to run the project (e.g., `pycryptodome`, `sqlite3`).

---

## How it Works

This project demonstrates a client-server architecture where both sides communicate securely using encryption and compression. Below is a step-by-step breakdown of how the client and server interact.

1. **Client Side:**
   - The client creates a connection to the server.
   - It loads the necessary configuration, including encryption keys.
   - The client encrypts a message using RSA encryption.
   - The message is compressed using zlib to reduce its size.
   - The encrypted and compressed message is sent to the server over a secure connection.
   
2. **Server Side:**
   - The server receives the encrypted and compressed message from the client.
   - It decompresses the message using zlib.
   - The server decrypts the message using its private RSA key.
   - The server processes the decrypted message and sends a response back to the client.
   
3. **Communication Protocol:**
   - All communication between the client and server is done securely using encryption and compression to ensure both confidentiality and efficiency.
   - The client and server rely on RSA public-key cryptography to ensure that only the recipient can decrypt the messages.

---

### Example of Client-Server Communication Flow

| Step | Client Action                | Server Action                   |
|------|------------------------------|----------------------------------|
| 1    | Establishes a connection      | Waits for incoming connections   |
| 2    | Encrypts the message         | Receives the encrypted message   |
| 3    | Compresses the message       | Decompresses the message         |
| 4    | Sends the encrypted message  | Decrypts the message             |
| 5    | Waits for server response    | Processes the message and sends a response |
| 6    | Receives the server response | Sends the final response         |

---

### Data Flow Diagram

The data flow between the client and server looks as follows:

1. **Client**
   - The client encrypts the message and compresses it.
2. **Server**
   - The server decrypts the message, decompresses it, processes it, and sends the response.

This flow ensures secure and efficient communication.

---

### Database Tables

The database for this project consists of the following tables:

#### 1. **clients.db**
Stores information about all clients.

| Column      | Data Type | Description               |
|-------------|-----------|---------------------------|
| uuid        | 16 bytes  | Unique ID for each user |
| name        | string    | Username of the user      |
| public_key  | 160 bytes | Public key for encryption/decryption |
| aes_key     | 128 bit   | AES key sent to the client |

#### 2. **Files Table**
Stores metadata for the files uploaded by users.

| Column      | Data Type | Description               |
|-------------|-----------|---------------------------|
| uuid        | 16 bytes  | Unique ID referencing the user who uploaded the file |
| file_name   | string    | Name of the file, unique for each file |

---

### Dependencies

To run this project, you'll need to install the required Python libraries:

## Installation

### Requirements

- Python 3.9.0 and above
- C++ libraries:
    - Boost library
    - Crypto++ library
- `pip` for installing dependencies

### Steps to Set Up

1. Clone the repository or download the project files.

2. Install the required dependencies:
   pip install -r requirements.txt

