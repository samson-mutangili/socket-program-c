#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
	
	if(argc < 2){
		printf("Error!! The port number is not provided. Program terminated!\n");
		exit(1);
	}
	
	int clientSocket, ret, port_no;
	struct sockaddr_in serverAddr;
	char buffer[1024];
	
	//get the commandline input and parse it to integer
	port_no = atoi(argv[1]);
	
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("Error creating a socket. Please try again later. \n");
		exit(1);
	}
	
	//client is connected successfully
	printf("Client socket created successfully!\n");
	
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port_no);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //address of server
	
	
	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	
	if(ret < 0){
		printf("Error in connection.\n");
		exit(1);
	}
	
	printf("Connected to server. \n");
	while(1){
		printf("Client: ");
		//Read message from the client
		scanf("%s", &buffer[0]);
		//send client message to server
		send(clientSocket, buffer, strlen(buffer), 0);
				
		//receive data from the server
		if( recv(clientSocket, buffer, 1024, 0) < 0){
			printf("Error in receiving data!!\n");
		} else{
			//print the message on the server.
			printf("Server: %s\n", buffer);
		}
	}
	return 0;
}
