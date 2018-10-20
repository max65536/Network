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

	printf("INADDR_ANY=%u\n",INADDR_ANY);

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
//	ServerAddr.sin_port= 8000;

		printf("Client0...\n");
		printf("addr=%u\n",ClientAddr.sin_addr.s_addr);
		printf("addr=%s\n",inet_ntoa(ClientAddr.sin_addr.s_addr));
		printf("port=%u\n",ClientAddr.sin_port);

	int bind0=bind(socket0,(struct sockaddr*) &ClientAddr,sizeof(ClientAddr));

		printf("Client1...\n");
		printf("addr=%u\n",ClientAddr.sin_addr.s_addr);
		printf("addr=%s\n",inet_ntoa(ClientAddr.sin_addr.s_addr));
		printf("port=%u\n",ClientAddr.sin_port);


		printf("Server...\n");
		printf("addr=%u\n",ServerAddr.sin_addr.s_addr);
		printf("addr=%s\n",inet_ntoa(ServerAddr.sin_addr.s_addr));
		printf("port=%u\n",ServerAddr.sin_port);
	char msg[MAX_MSG_SIZE];
	char echo[MAX_MSG_SIZE];
	int count=0;
	struct sockaddr_in addr;
//	int sizeT=sizeof(addr);
//	while(1){
		bzero(msg,MAX_MSG_SIZE);
		printf("writing...\n");
		fgets(msg,MAX_MSG_SIZE,stdin);


		sendto(socket0,msg,strlen(msg),0,(struct sockaddr*) &ServerAddr,sizeof(ServerAddr));
		printf("Server2...\n");
		printf("addr=%u\n",ServerAddr.sin_addr.s_addr);
		printf("addr=%s\n",inet_ntoa(ServerAddr.sin_addr.s_addr));
		printf("port=%u\n",ServerAddr.sin_port);

		count=recvfrom(socket0,echo,sizeof(echo),0,(struct sockaddr*) &addr,sizeof(addr));
		printf("count=%d\n",count);
		printf("addr...\n");
		printf("addr=%u\n",addr.sin_addr.s_addr);
		printf("addr=%s\n",inet_ntoa(addr.sin_addr.s_addr));
		printf("port=%u\n",addr.sin_port);


		puts(echo);
//	}

		close(socket0);
	return EXIT_SUCCESS;
}
