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
#include <signal.h>

#define PORT 16000
#define MAX_MSG_SIZE 1024
int socket1;
struct sockaddr_in ServerAddr;
void WorkProcess(int sig);
int main(void) {
        struct sigaction act,oldact;

        socket1=socket(AF_INET,SOCK_DGRAM,0);
        int j=0;
	ServerAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_port= htons(16000);

	int bind1=bind(socket1,(struct sockaddr*) &ServerAddr,sizeof(ServerAddr));

        act.sa_handler = WorkProcess;
        act.sa_flags = 0;
        sigemptyset(&act.sa_mask);
        sigaction(SIGUSR1, &act ,0);
        int pid=0;
        while(1)
        {
            printf("no connection\n");
            for(j=0;j<1000000000;j++)
            {;}
            pid=fork();
            if(pid==0)
            {
                kill(getpid(),SIGUSR1);

            }
        }


	return EXIT_SUCCESS;
}
void WorkProcess(int sig)
{

        int i=0;
        char msg[MAX_MSG_SIZE];
        struct sockaddr addr;
        socklen_t addrlen=sizeof(addr);
        struct sockaddr_in* addrin;


    bzero(msg,MAX_MSG_SIZE);
    recvfrom(socket1,msg,sizeof(msg),0,&addr,&addrlen);
    addrin=(struct sockaddr_in *) &addr;
    printf("from Client %s:%u\n",inet_ntoa(addrin->sin_addr),addrin->sin_port);
    i++;
    printf("message%d:%s",i,msg);
    sendto(socket1,msg,strlen(msg),0,(struct sockaddr*) &addr,addrlen);
}
