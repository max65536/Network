/*
 ============================================================================
 Name        : Server.c
 Author      : np
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8001
#define MAX_MSG_SIZE 1024


int main(void) {
	int socket1=socket(AF_INET,SOCK_DGRAM,0);///////////

	struct sockaddr_in ClientAddr;
	//ClientAddr.sin_addr.s_addr=inet_addr("127.0.0.1");//INADDR_ANY//LOCALHOST
	ClientAddr.sin_family=AF_INET;
	ClientAddr.sin_port= htons(8001);

	struct sockaddr_in ServerAddr;
	ServerAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_port= htons(8000);

	int bind1=bind(socket1,(struct sockaddr*) &ServerAddr,sizeof(ServerAddr));

	char msg[MAX_MSG_SIZE];
	char echo[MAX_MSG_SIZE]="Get Echo from Server!\n";
	char *msg1;
	char ip4[32]; 

	struct sockaddr addr;


	while(1){
		bzero(msg,MAX_MSG_SIZE);
		printf("waiting...\n");
		recvfrom(socket1,msg,sizeof(msg),0,(struct sockaddr*) &addr,sizeof(addr));

		msg1 = strtok(msg,":");
		printf("received: %s", msg1);
		msg1 = strtok(NULL,":");
		printf("From the IP: %s\n\n ", msg1);
		sprintf(ip4,msg1);
		ClientAddr.sin_addr.s_addr=inet_addr(ip4);
	
		sendto(socket1,echo,strlen(echo),0,(struct sockaddr*) &ClientAddr,sizeof(ClientAddr));
	}


	return EXIT_SUCCESS;
}
