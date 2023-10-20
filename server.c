#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4444

int main(int argc, char *argv[]){

	if(argc < 2){
		printf("Error!! The port number is not provided. Program terminated!\n");
		exit(1);
	}

	int sockfd, ret, port_no, newSocket;
	int clients = 0;
	struct sockaddr_in serverAddr, cli_addr;
	
	
	socklen_t addr_size;
	
	char buffer[1024]; 
	pid_t childpid;
	
	//get the commandline input and parse it to integer
	port_no = atoi(argv[1]);
	
	FILE *fptr;
	//create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("Error in creating server socket. \n");
		exit(1);
	}
	
	//client is connected successfully
	printf("Server socket created successfully!\n");
	
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //address of server
	
	//bind ip address and the port
	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	
	if(ret < 0){
		printf("Error in binding ip address to the port. Please reconnect and choose a differernt port.\n");
		exit(1);
	}
	
	
	printf("Bind to port %d\n", port_no);
	
	//connect at most 20 clients to the server
	if(listen(sockfd, 20) == 0){
		printf("Server is listening ... \n");
	} else {
		printf("Error in binding.\n");
	}
	
	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&cli_addr, &addr_size); 
		if(newSocket < 0){
			printf("Error on accepting connection!\n");
			exit(1);
		}
		//print address and port of the client
		printf("Connection accepted from  %s:%d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
		
		//increment number of clients connected.
		clients++;
		
		//clear the buffer
		bzero(buffer, sizeof(buffer));
		
		if((childpid = fork()) ==0){
			close(sockfd);
			
			while(1){
				//receive message from client
				recv(newSocket, buffer, 1024, 0);
				
				//echo the message to client
				if(strlen(buffer)>0){
					printf("Client: %s\n", buffer);
					send(newSocket, buffer, strlen(buffer), 0);
				} else {
					break;
				}
				
				//write the message to file.
				//open the file
				fptr = fopen("chats.txt", "a");				
				//write to the file
				fprintf(fptr, "%s\n", buffer);				
				//close the file
				fclose(fptr);
					
				//clear the buffer
				bzero(buffer, sizeof(buffer));			
			}
		}
		 
	}
	
	
	close(newSocket);
	
	return 0;
	
	
}
