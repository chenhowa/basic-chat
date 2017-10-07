/*
 * Filename: client.c
 * Author: Howard Chen
 * Date Created: 7-18-2017
 * Description: Implements the client side of an application to connect to
 * 	a server and exchange messages
 *
 * Citation: Heavy uses of Beej's guide to help me set up the sockets and connections
 * 	using the appropriate C libraries
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h> /* for open and create functions*/
#include <unistd.h> /*for the close function */



/* gets a single line of input from an opened input file
 * args: [1] file, a pointer to an open file
 * pre: file is an open file, where every line ends in a newline
 * post: a single line will be read from the file
 * post: returned c-string MUST be freed
 * post: newline is not included in the returned string
 * ret: a pointer to the line that was read, which was allocated on the heap.
 *
 * NOTE: this method comes from the lecture slides of CS 344 that describe
 * 	how to use getline to get a string of input from an open File.
 */
char* getInput(FILE* file) {
	int numChars = -5;
	size_t bufferSize = 0;
	char* line = NULL;

	/*Get a line of user input ending in '\n' */
	numChars = getline(&line, &bufferSize, file);
	
	/*Remove the newline from the end of the string */
	line[numChars - 1] = '\0';
	
	return line;
}



int main (int argc, char* argv[]) {
	char* hostname;
	char* port;

	int status; /*Holds status of attempt to get server info */
	struct addrinfo hints;
	struct addrinfo *serverinfo;

	int sockfd; /*Holds file descriptor for the client socket connected to server */

	char *username = NULL;
	char *userinput = NULL;

	char prompt[15];
	char message[520];
	int message_size;
	int bytes_sent;
	char received[520];
	int bytes_rec;

	memset(received, '\0', sizeof(received));

	if( argc != 3 ) {
		printf("\nError. Need 2 arguments, got %i. Need <hostname> <port number>\n\n", argc - 1);
		return 1;
	}

	hostname = argv[1];
	port = argv[2];
	printf("Hostname: %s \nPort: %s \n\n", hostname, port);

	/*First, get the address info for the specified hostname/port that the 
 		client is going to contact*/
	memset(&hints, 0, sizeof(hints)); /*initialize the struct to be empty*/
	hints.ai_family = AF_UNSPEC;	/*Either IPv4 or IPv6 is acceptable FOR NOW*/	
	hints.ai_socktype = SOCK_STREAM; /* Specify that I'm looking for a TCP stream socket*/
	hints.ai_flags = AI_PASSIVE;  /*fill in the client's IP automatically please */

	/*Next, attempt to get contact information of the specified server process.
 		if it fails, exit with error*/
	status = getaddrinfo(hostname, port, &hints, &serverinfo);
	if(status != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		return 1;
	}

	/*If it succeeds, create the socket based on the server info returned by
 		getaddrinfo, and connect the socket to the server. The OS will bind
		the socket to a local port for us -- we don't need to manage it */
	sockfd = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol);
	/*fcntl(sockfd, F_SETFL, O_NONBLOCK);*/ /* make the socket non-blocking */
	status = connect(sockfd, serverinfo->ai_addr, serverinfo->ai_addrlen);

	/*Notice that this will not catch all errors. Even if no server process 
 	is set up to handle the connection, if the connection isn't closed, the program
 	will run forever!*/
	if(status == -1) {
		fprintf(stderr, "connect error!\n ");
		return 1;
	}

	/*If connection was successful, get user chat name for client.
 * 		Username is at least 1 char and at most 10 chars*/
	printf("Connection was successful!\n");
	printf("\n\nEnter your chat username: ");
	username = getInput(stdin);
	while(strlen(username) == 0 || strlen(username) > 10) {
		free(username);
		username = NULL;
		printf("ERROR. Username must be at most 10 characters, and at least 1 character");
		printf("\n\nEnter your chat username: ");
		username = getInput(stdin);
	}

	/*Get an the initial message */
	memset(prompt, '\0', sizeof(prompt));
	strcpy(prompt, username);
	strcat(prompt, "> ");

	printf("%s", prompt);
	userinput = getInput(stdin);
	while(strlen(userinput) > 500) {
		free(userinput);
		userinput = NULL;
		printf("Message too long. Keep it under 500 characters!");
		userinput = getInput(stdin);
	}

	/*While neither the client nor the server have chosen to quit */
	while(strcmp(userinput, "\\quit") != 0 && strcmp(received, "\\quit") != 0)  {
		/*Send a message */
		memset(message, '\0', sizeof(message));
		strcpy(message, prompt);
		strcat(message, userinput);
		message_size = strlen(message);
		bytes_sent = send(sockfd, message, message_size, 0);

		/*Receive a message */
		memset(received, '\0', sizeof(received));
		bytes_rec = recv(sockfd, received, sizeof(received), 0);	
		printf("%s\n", received);

		/*If the received message wasn't to quit, get more user input
 * 			for the next message */
		if(strcmp(received, "\\quit") != 0) {
			free(userinput);
			userinput = NULL;
			printf("%s", prompt);
			userinput = getInput(stdin);
			while(strlen(userinput) > 500) {
				free(userinput);
				userinput = NULL;
				printf("Message too long. Keep it under 500 characters!\n");
				printf("%s", prompt);
				userinput = getInput(stdin);
			}
		}
	}
	
	/*If client chose to quit, tell the server that the client is closing the connection */
	if(strcmp(userinput, "\\quit") == 0) {
		memset(message ,'\0', sizeof(message));
		strcpy(message, "\\quit");
		message_size = strlen(message);
		bytes_sent = send(sockfd, message, message_size, 0);
	}

	/*done with connection, so close it. Also free memory allocated to username*/
	close(sockfd);

	free(username);
	free(userinput);
	return 0;

}
