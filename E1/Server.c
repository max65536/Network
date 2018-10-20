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
//	ClientAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	ClientAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	ClientAddr.sin_family=AF_INET;
	ClientAddr.sin_port= htons(8001);

	struct sockaddr_in ServerAddr;
//	addr.sin_addr.s_addr=htonl(inet_addr("127.0.0.1"));//INADDR_ANY//LOCALHOST
	ServerAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_port= htons(8000);

	int bind1=bind(socket1,(struct sockaddr*) &ServerAddr,sizeof(ServerAddr));

		printf("Client...\n");
		printf("addr=%u\n",ClientAddr.sin_addr.s_addr);
		printf("addr=%s\n",inet_ntoa(ClientAddr.sin_addr.s_addr));
		printf("port=%u\n",ClientAddr.sin_port);


		printf("Server...\n");
		printf("addr=%u\n",ServerAddr.sin_addr.s_addr);
		printf("addr=%s\n",inet_ntoa(ServerAddr.sin_addr.s_addr));
		printf("port=%u\n",ServerAddr.sin_port);


	char msg[MAX_MSG_SIZE];
//	char echo[MAX_MSG_SIZE]="hello0";
	char* echo="success";
	struct sockaddr_in addr;

	int sizeT=sizeof(addr);
	while(1){
		bzero(msg,MAX_MSG_SIZE);
		printf("waiting...\n");
//		recvfrom(socket1,msg,sizeof(msg),0,(struct sockaddr*) &ClientAddr,sizeof(ClientAddr));
		recvfrom(socket1,msg,sizeof(msg),0,(struct sockaddr*) &addr,sizeof(addr));
		puts(msg);
		
//		printf("%u\n",addr.sa_data);
		printf("addr...\n");
		printf("addr=%u\n",addr.sin_addr.s_addr);
		printf("addr=%s\n",inet_ntoa(addr.sin_addr.s_addr));
		printf("port=%u\n",addr.sin_port); 
		if (msg=="close") {
			close(socket1);
			break;		
		}

		sendto(socket1,echo,strlen(echo),0,(struct sockaddr*) &ClientAddr,sizeof(ClientAddr));
		printf("Client2...\n");
		printf("addr=%u\n",ClientAddr.sin_addr.s_addr);
		printf("addr=%s\n",inet_ntoa(ClientAddr.sin_addr.s_addr));
		printf("port=%u\n",ClientAddr.sin_port);
//		sendto(socket1,echo,strlen(echo),0,(struct sockaddr*) &addr,sizeof(addr));

//		printf("sent to %s\n",ClientAddr.sa_data);
	}


	return EXIT_SUCCESS;
}
