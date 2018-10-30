/*
 ============================================================================
 Name        : IPv6_1.c
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

#define PORT 16002
#define MAX_HEAD_SIZE 1024
void clienterror(int fd, char *errnum,
		 char *shortmsg, char *longmsg);
void send2client(int fd, char *code,
		 char *status,char *msg);
void do_service(int fd);


int main(void) {
	int socketfd = socket(AF_INET6,SOCK_STREAM,0);

	struct sockaddr_in6 ServerAddr;
	ServerAddr.sin6_family = AF_INET6;
	ServerAddr.sin6_addr = in6addr_any;
	ServerAddr.sin6_port = htons(PORT);

	int bind1=bind(socketfd,(struct sockaddr*) &ServerAddr,sizeof(ServerAddr));

	char buf[MAX_HEAD_SIZE];
	char msg[MAX_HEAD_SIZE];
	char ip6[128];
	int i=0;
	listen(socketfd, 5);
	struct sockaddr_in6 addr;
	socklen_t addrlen=sizeof(addr);
	struct sockaddr_in6* addrin;
	int clientfd;
	FILE *f;
	char Head[MAX_HEAD_SIZE];
	int child_pid=0;
	while(1){

		printf("waiting for connection...\n");
		clientfd=accept(socketfd,(struct sockaddr*)&addr,&addrlen);
		printf("----------------------------------------\n");
		printf("connection from %s:%d\n", inet_ntop(AF_INET6,&addr.sin6_addr,ip6,addrlen),ntohs(addr.sin6_port) );

		child_pid=fork();
		printf("child_pid=%d\n",child_pid);
		if ((child_pid)==0){
			close(socketfd);
			f=fdopen(clientfd,"r");
			do_service(clientfd);
			fclose(f);
		}


		close(clientfd);

	}

	close(socketfd);

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
void send2client(int fd, char *code,
		 char *status, char *msg)
{
    char buf1[MAX_HEAD_SIZE], body1[MAX_HEAD_SIZE];
    int j;
    /* Build the HTTP response body */
    // msg="<html><body>Hello</body></html>"
    sprintf(body1, msg);

    /* Print the HTTP response */
    sprintf(buf1, "HTTP/1.1 %s %s\r\n", code, status);
    sprintf(buf1, "%sContent-length: %d\r\n", buf1,(int)strlen(body1));
    sprintf(buf1,"%sConnection: close\r\n",buf1);
    sprintf(buf1, "%sContent-type: text/html\r\n\r\n",buf1);
    printf("\n%s",buf1);
    send(fd, buf1, strlen(buf1),0);
    send(fd, body1, strlen(body1),0);
}
void do_service(int fd){
	char buf[1024];
	int i;
	FILE *f;
	while (1)
	{
		memset(buf, 0, sizeof(buf));
		f=fdopen(fd,"r");
		for (i=0;i<3;i++){
			fgets(buf,1024,f);
			printf("%s",buf);
		}

		send2client(fd,"200","OK","<html><body>Hello</body></html>");
	}
}
