#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8000
#define MAX_MSG_SIZE 1024


int main(void) {

	int socket0=socket(AF_INET,SOCK_DGRAM,0);

	struct sockaddr_in ClientAddr;
	ClientAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	ClientAddr.sin_family=AF_INET;
	ClientAddr.sin_port= htons(8001);

	struct sockaddr_in ServerAddr;
	ServerAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_port= htons(8000);

	int bind0=bind(socket0,(struct sockaddr*) &ClientAddr,sizeof(ClientAddr));

	char msg[MAX_MSG_SIZE];
	char echo[MAX_MSG_SIZE];
	char ip4[32]; 
	char ip[33];
	
	inet_ntop(AF_INET,&(ServerAddr.sin_addr.s_addr),ip4,32);
	sprintf(ip,":%s",ip4);
        printf("IP IS %s \n",ip);

	struct sockaddr addr;
	while(1){
		bzero(msg,MAX_MSG_SIZE);
		printf("writing...(you cannot write the symbol :)\n");
		fgets(msg,MAX_MSG_SIZE,stdin);
		strcat(msg,ip);

		sendto(socket0,msg,strlen(msg),0,(struct sockaddr*) &ServerAddr,sizeof(ServerAddr));

		recvfrom(socket0,echo,sizeof(echo),0,(struct sockaddr*) &addr,sizeof(addr));

		puts(echo);
	}


	return EXIT_SUCCESS;
}
