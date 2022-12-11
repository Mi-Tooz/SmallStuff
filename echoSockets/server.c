#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <netinet/in.h>
#include <sys/socket.h>

#include <sys/types.h>
#include <unistd.h>

#define PORT 8081
#define MAX_LINE_LENGTH 256

void error(char* msg)
{
	printf("Error: %s\n", msg);
	exit(1);
}

int main(void)
{
// Searching for connection (like we all do)

	// Create server socket
	int serverFd = socket(AF_INET, SOCK_STREAM, 0);
	
	// Make sure socket was created successfully
	if(serverFd < 0)
		error("server socket");

	// Define addresses of the server
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Attach socket to port 8080
	if( bind(serverFd, (struct sockaddr*)&address, sizeof(address))  < 0)
		error("bind");

	// Listen to connetion
	if( listen(serverFd, 3) < 0 )
		error("listen");

	// Accept the connection
	int addrLen = sizeof(address);
	int clientFd = accept(serverFd, (struct sockaddr*)&address, &addrLen);
	if(clientFd < 0)
		error("client socket");

// Making magic with achived connection

	// Initialize message variables
	char buffer[MAX_LINE_LENGTH];
	int numBytes; 

	while(1){
		// receive message 
		bzero(buffer, MAX_LINE_LENGTH);
		numBytes = read(clientFd, buffer, MAX_LINE_LENGTH);

		if(buffer[0] == 0x0A)
			break;

		if(numBytes <= 0)
			error("reading");

		//chane message
		buffer[0] = toupper(buffer[0]);
		printf("Client: %s", buffer);

		// send message
		if( write(clientFd, buffer, numBytes) < 0 )
			error("writing");
	}
	
	close(clientFd);
	close(serverFd);

	return 0;
}
