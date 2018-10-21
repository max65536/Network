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
	int socket1=socket(AF_INET,SOCK_DGRAM,0);

	struct sockaddr_in ServerAddr;
	ServerAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_port= htons(8000);

	int bind1=bind(socket1,(struct sockaddr*) &ServerAddr,sizeof(ServerAddr));

	char msg[MAX_MSG_SIZE];
	char echo[MAX_MSG_SIZE]="Get Echo from Server!\n";

	struct sockaddr addr;
	socklen_t addrlen=sizeof(addr);
	struct sockaddr_in* addrin;
	int i=0;

	while(1){
		bzero(msg,MAX_MSG_SIZE);
		printf("waiting...\n");
		recvfrom(socket1,msg,sizeof(msg),0,&addr,&addrlen);
		
		addrin=(struct sockaddr_in *) &addr;
/*		printf("addr...\n");
		printf("addr=%u\n",addrin->sin_addr);
		printf("addr=%s\n",inet_ntoa(addrin->sin_addr));
		printf("port=%u\n",addrin->sin_port); 
*/
		printf("from Client %s:%u\n",inet_ntoa(addrin->sin_addr),addrin->sin_port);
		i++;
		printf("message%d:%s",i,msg);
		sendto(socket1,echo,strlen(echo),0,(struct sockaddr*) &addr,addrlen);
	}


	return EXIT_SUCCESS;
}
