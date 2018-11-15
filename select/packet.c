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
#include <sys/stat.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <linux/if_ether.h>

#define MAX_MSG_SIZE 1024

int main(void) {
    unsigned char recvbuf[MAX_MSG_SIZE];
    char* ether;
    int packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    int i;
    struct sockaddr_ll LLAddr;
    int addrlen = sizeof (LLAddr);
    struct packet_mreq Mreq;
    Mreq.mr_type = PACKET_MR_PROMISC;
    setsockopt(packet_socket, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &Mreq, sizeof(Mreq));

    while(1){
        memset(recvbuf,0,MAX_MSG_SIZE);
        recvfrom(packet_socket, recvbuf, sizeof(recvbuf), 0, (struct sockaddr*) &LLAddr,&addrlen);
        for(i=0; i<MAX_MSG_SIZE;i++){
            if(i==0){
                printf("MSG:");
            }

            printf(" %d ", recvbuf[i]);
           if(recvbuf[i]==0 && recvbuf[i+1]==0 && recvbuf[i+2]==0&&
                   recvbuf[i+3]==0 && recvbuf[i+4]==0 && recvbuf[i+5]==0){
               break; // under this circumstance, we think the recv data stops here
           }
        }
       printf("\n PROTOCOL is %d \n",recvbuf[19]);
       printf("S IP is %d:%d:%d:%d\n", recvbuf[22], recvbuf[23], recvbuf[24], recvbuf[25]);
       printf("D IP %d:%d:%d:%d",recvbuf[26],  recvbuf[27], recvbuf[28], recvbuf[29]);
       printf("\n\n");
    }

    return EXIT_SUCCESS;
}


