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
    int x=0;
    int flag;
    struct sockaddr_ll LLAddr;
    int addrlen = sizeof (LLAddr);
    struct packet_mreq Mreq;
    Mreq.mr_type = PACKET_MR_PROMISC;
    setsockopt(packet_socket, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &Mreq, sizeof(Mreq));

    while(1){
        memset(recvbuf,0,MAX_MSG_SIZE);
        x=recvfrom(packet_socket, recvbuf, sizeof(recvbuf), 0, (struct sockaddr*) &LLAddr,&addrlen);
	
		for(i=0; i<MAX_MSG_SIZE;i++){
		  //  if(i==0 ){
		    //    printf("MSG:");
		   // }
		   // printf(" %d ", recvbuf[i]);
		    if(recvbuf[i]==69 && recvbuf[i+1]==0 && recvbuf[i+2]==0)
	            {
		       flag=i;
                    }
		   if(recvbuf[i]==0 && recvbuf[i+1]==0 && recvbuf[i+2]==0 &&
		       recvbuf[i+3]==0 && recvbuf[i+4]==0 && recvbuf[i+5]==0)
		   {
		       break; // under this circumstance, we think the recv data stops here
		   }
		}
	       switch(recvbuf[flag+9])
               {
		  case 1: printf("\n PROTOCOL: ICMP "); break;
                  case 2: printf("\n PROTOCOL: IGMP "); break;
                  case 6: printf("\n PROTOCOL: TCP "); break;
                  case 17: printf("\n PROTOCOL: UDP "); break;
                  case 41: printf("\n PROTOCOL: ENCAP "); break;
		  case 89: printf("\n PROTOCOL: OSPF "); break;
 		  case 132: printf("\n PROTOCOL: SCTP "); break;
		  default: printf("\n unknown protocol");
               }
	       //printf("\n protocol is %d", recvbuf[flag+9]);
	       printf(" Source IP is %d:%d:%d:%d ", recvbuf[flag+12], recvbuf[flag+13], recvbuf[flag+14], recvbuf[flag+15]);
	       printf(" Destination IP %d:%d:%d:%d ",recvbuf[flag+16],  recvbuf[flag+17], recvbuf[flag+18], recvbuf[flag+19]);
	       printf("\n\n");
      

    }

    return EXIT_SUCCESS;
}


