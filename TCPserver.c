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
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 80
#define MAX_MSG_SIZE 1024


int main(void) {
	int socket1=socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in ServerAddr;
	ServerAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_port= htons(PORT);

	int bind1=bind(socket1,(struct sockaddr*) &ServerAddr,sizeof(ServerAddr));

	char msg[MAX_MSG_SIZE];
	char echo[MAX_MSG_SIZE]="Get Echo from Server!\n";
	char buf[20];
	int bufbytes;
	int i=0;
	listen(socket1, 5);
	struct sockaddr addr;
	socklen_t addrlen=sizeof(addr);
	struct sockaddr_in* addrin;

	while(1){
		//bzero(msg,MAX_MSG_SIZE);
		bzero(buf,20);
		printf("waiting for connection2...\n");
		int clientfd;
		clientfd=accept(socket1,&addr,&addrlen);
		FILE *f;
		f=fdopen(clientfd,"r");
		fgets(buf,20,f);
		printf("request%d is %s\n",i,buf);
		i++;
		fclose(f);
		//printf("waiting for message...\n");
		//recv(clientfd,msg,sizeof(msg),0);

		//addrin=(struct sockaddr_in *) &addr;

		//printf("from Client %s:%u\n",inet_ntoa(addrin->sin_addr),addrin->sin_port);
		//i++;
		//printf("message%d:%s",i,msg);
		//send(clientfd,echo,strlen(echo),0);
		close(clientfd);

	}

	close(socket1);
	return EXIT_SUCCESS;
}
