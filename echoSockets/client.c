#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>

#include <sys/types.h>
#include <unistd.h>

#define PORT 8081
#define MAX_LINE_LENGTH 255

void error(char* msg)
{
	printf("Error: %s\n", msg);
	exit(1);
}

int main(void)
{						
	// Create server socket
	int clientFd = socket(AF_INET, SOCK_STREAM, 0);
	
	// Make sure socket was created successfully
	if(clientFd < 0)
		error("socket");

	// define addresses of client
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if( connect(clientFd, (struct sockaddr*)&address, sizeof(address)) < 0)
		error("connecting");

	char buffer[MAX_LINE_LENGTH];

	while(1){
		// Take user input
		bzero(buffer, MAX_LINE_LENGTH);
		printf("Client: ");
		fgets(buffer, MAX_LINE_LENGTH, stdin);

		if(buffer[0] == 0x0A){
			printf("Disconnecting...\n");
			// writing message to a server to disconecct
			write(clientFd, buffer, strlen(buffer));
			break;
		}

		if(&buffer[MAX_LINE_LENGTH-1] == NULL)
			error("Too large message");

		// Write message to a server
		if( write(clientFd, buffer, strlen(buffer)) < 0 )
			error("writing");	
		
		// receive message form a server
		if ( read(clientFd, buffer, MAX_LINE_LENGTH) < 0)
			error("reading");

		printf("Server: %s", buffer);
	}

	close(clientFd);

	return 0;
}
