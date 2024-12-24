""" This file contains the database-related functions for managing users and data. """

import sqlite3
import os

def delete_database():
    """
    Deletes databases files.
    """
    os.remove("clients.db")
    os.remove("files.db")


def create_database():
    """
    Creates the 'clients.db' SQLite database and a 'clients' table if they do not exist.
    """
    conn = sqlite3.connect('clients.db')
    c = conn.cursor()
    c.execute('''
        CREATE TABLE IF NOT EXISTS clients (
            uuid TEXT UNIQUE,
            name TEXT UNIQUE,
            publickey TEXT UNIQUE,
            aeskey TEXT UNIQUE
        )
    ''')
    conn.commit()
    conn.close()

    # Create files.db (to store file info)
    conn = sqlite3.connect('files.db')
    c = conn.cursor()

    # Create Files Table (uuid and file name)
    c.execute('''
        CREATE TABLE IF NOT EXISTS files (
            uuid TEXT NOT NULL,
            file_name TEXT NOT NULL
        )
    ''')

    conn.commit()
    conn.close()


def add_user_by_name_and_uuid(name, uuid):
    """
    Adds a user to the 'clients' table by inserting their name and UUID.
    """
    conn = sqlite3.connect('clients.db')
    c = conn.cursor()
    c.execute('INSERT INTO clients (uuid, name) VALUES (?, ?)', (uuid, name))
    conn.commit()
    conn.close()


def check_user_by_name(name):
    """
    Checks if a user with the given name exists in the 'clients' table.
    """
    conn = sqlite3.connect('clients.db')
    c = conn.cursor()
    c.execute("SELECT * FROM clients WHERE name=?", (name,))
    result = c.fetchone()
    conn.close()
    return result is not None


def find_name_return_file(client_name):
    """
    Given the client's name, this function retrieves the corresponding uuid from
    clients.db and then queries files.db to return all the file names associated
    with that uuid.

    parm: client_name: The name of the client whose files are being requested.
    return: A list of file names associated with the client, or an error message.
    """
    # Connect to clients.db and get the uuid for the client name
    conn = sqlite3.connect('clients.db')
    c = conn.cursor()

    # Query to find uuid for the given client name
    c.execute('''SELECT uuid FROM clients WHERE name = ?''', (client_name,))
    result = c.fetchone()
    conn.close()

    # If no client is found with the given name
    if result is None:
        return f"No client found with the name: {client_name}"

    # Get the uuid from the result
    client_uuid = result[0]

    # Connect to files.db and get all file names for the given uuid
    conn = sqlite3.connect('files.db')
    c = conn.cursor()

    # Query to get all file names associated with the uuid
    c.execute('''SELECT file_name FROM files WHERE uuid = ?''', (client_uuid,))
    files = c.fetchall()
    conn.close()

    # If no files are found for the given uuid
    if not files:
        return f"No files found for client with name: {client_name}"

    # Return a list of file names
    return [file[0] for file in files]




def find_aesKey(name):
    """
    Retrieves the AES key associated with a given client name from the 'clients' table.
    """
    conn = sqlite3.connect('clients.db')
    c = conn.cursor()
    c.execute("SELECT aeskey FROM clients WHERE name=?", (name,))
    result = c.fetchone()
    conn.close()
    return result


def add_users_publickey(name, publickey):
    """
    Updates the public key for a client in the 'clients' table by their name.
    """
    conn = sqlite3.connect('clients.db')
    c = conn.cursor()
    c.execute("UPDATE clients SET publickey=? WHERE name=?", (publickey, name))
    conn.commit()
    conn.close()


def add_users_file(uuid, file_data):
    """
    Adds a file for a client in the 'files' table by their UUID.
    """
    # Connect to files.db
    conn = sqlite3.connect('files.db')
    c = conn.cursor()

    # Insert the uuid and file name into files.db
    c.execute('''INSERT INTO files (uuid, file_name) 
                 VALUES (?, ?)''', (uuid, file_name))

    conn.commit()
    conn.close()


def add_users_aeskey(name, aeskey):
    """
    Updates the AES key for a client in the 'clients' table by their name.
    """
    conn = sqlite3.connect('clients.db')
    c = conn.cursor()
    c.execute("UPDATE clients SET aeskey=? WHERE name=?", (aeskey, name))
    conn.commit()
    conn.close()


def print_all_info():
    """
    Retrieves and prints all information from the 'clients' table.
    """
    conn = sqlite3.connect('clients.db')
    c = conn.cursor()
    c.execute("SELECT * FROM clients")
    rows = c.fetchall()
    for row in rows:
        print(row)
    conn.close()


def create_uuid(name):
    """
    Generates a unique identifier (UUID) based on the provided name.
    """
    output_str = ""
    for char in name:
        output_str += format(ord(char), "02x")
    return output_str
