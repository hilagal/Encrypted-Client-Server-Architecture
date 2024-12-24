/* 
* The entry point of the program. 
* It initializes the client application, sets up the socket connection using Boost Asio,
* and calls readInput() to process user commands.
*/

#include "main.h"

int main()
{
	run();
	return 0;
}

void run() {
	// open socket & connect to server from file info
	if (!open_socket()) {
		return;
	}
	handleMeFile(); // first handle me.info file, then continue to user requests
	// read user input in infinite loop
	for (;;) {
		readInput();
	}
}