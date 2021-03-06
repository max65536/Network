#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 16000
#define MAX_MSG_SIZE 1024


int main(void) {

	int socket0=socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in ServerAddr;
	ServerAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_port= htons(PORT);


	char msg[MAX_MSG_SIZE];
	char echo[MAX_MSG_SIZE];


	struct sockaddr addr;
	socklen_t addrlen=sizeof(struct sockaddr);
	connect(socket0,(struct sockaddr*) &ServerAddr, addrlen);
	printf("connecting...\n");
//	while(1){

		bzero(msg,MAX_MSG_SIZE);
		printf("Please write a message:\n");
		fgets(msg,MAX_MSG_SIZE,stdin);

		send(socket0,msg,strlen(msg),0);

		bzero(echo,MAX_MSG_SIZE);
		recv(socket0,echo,sizeof(echo),0);

		puts(echo);

	//}
	close(socket0);

	return EXIT_SUCCESS;
}
