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

#define PORT 16002
#define MAX_HEAD_SIZE 1024
void clienterror(int fd, char *errnum, 
		 char *shortmsg, char *longmsg);

int main(void) {
	int socket1=socket(AF_INET,SOCK_STREAM,0);
	int status;
	struct sockaddr_in ServerAddr;
	ServerAddr.sin_addr.s_addr=INADDR_ANY;
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_port= htons(PORT);

	int bind1=bind(socket1,(struct sockaddr*) &ServerAddr,sizeof(ServerAddr));

	char buf[150];
	char msg[MAX_HEAD_SIZE];
	int i=0;
	listen(socket1, 5);
	struct sockaddr addr;
	socklen_t addrlen=sizeof(addr);
	struct sockaddr_in* addrin;
	int clientfd;
	FILE *f;
	char Head[MAX_HEAD_SIZE];
	char Body[10]="hello!";
	while(1){
		bzero(Head,sizeof(Head));
		bzero(buf,150);
		printf("waiting for connection...\n");

		clientfd=accept(socket1,&addr,&addrlen);
		if (clientfd < 0)
		   printf("ERROR on accept\n");

		//do{
		//bzero(msg,MAX_HEAD_SIZE);
		status=recv(clientfd,msg,sizeof(msg),0);
		//printf("%d\n",status);
		//}while(status > 0);
		clienterror(clientfd, "404", "Not found",
			    "We couldn't find this file");
		f=fdopen(clientfd,"r");
		for (i=0;i<3;i++){
			fgets(buf,150,f);
			printf("%s",buf);
		}
		fclose(f);
		//sprintf(Head,"HTTP1.1 200 Ok/\r\n");
		//sprintf(Head,"Content-Length: 6\r\n",Head);
		//sprintf(Head,"Connection: close\r\n",Head);
		//sprintf(Head,"Content-Type: text/plain\r\n\r\n",Head);

		//printf("%d\n%s",strlen(Head),Head);
		//send(clientfd,Head,strlen(Head),0);


		//printf("%d\n%s",strlen(Body),Body);
		//send(clientfd,Body,strlen(Body),0);

		close(clientfd);

	}

	close(socket1);
	return EXIT_SUCCESS;
}

void clienterror(int fd, char *errnum, 
		 char *shortmsg, char *longmsg) 
{
    char buf1[MAX_HEAD_SIZE], body1[MAX_HEAD_SIZE];
    int j;
    /* Build the HTTP response body */
    sprintf(body1, "<html><body>The Error</body></html>");

    /* Print the HTTP response */
    sprintf(buf1, "HTTP/1.1 %s %s\r\n", errnum, shortmsg);
    sprintf(buf1, "%sContent-length: %d\r\n", buf1,(int)strlen(body1));
    sprintf(buf1,"%sConnection: close\r\n",buf1);
    sprintf(buf1, "%sContent-type: text/html\r\n\r\n",buf1);
    printf("\n%s",buf1);
    send(fd, buf1, strlen(buf1),0);
    send(fd, body1, strlen(body1),0);
}
