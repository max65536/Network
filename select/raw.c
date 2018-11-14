/*
 ============================================================================
 Name        : raw.c
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
#include <netinet/ip_icmp.h>

#define PORT 0
#define MAX_MSG_SIZE 1024


struct iphead{
	unsigned char  ip_hl:4,ip_version:4;  //ip_hl,ip_version各占四个bit位。
	unsigned char ip_tos;
	unsigned short int ip_len;
	unsigned short int ip_id;
	unsigned short int ip_off;
	unsigned char ip_ttl;
	unsigned char ip_pro;
	unsigned short int ip_sum;
	unsigned int ip_src;
	unsigned int ip_dst;
};


unsigned short int cksum(char buffer[], int size);

int main(void) {
	int on = 1;
	struct iphead *ip;
	struct icmp *icmp,*recvicmp;
	char sendbuf[1024],recvbuf[1024];
	unsigned char package[8];


	struct sockaddr_in addr;
	struct sockaddr_in recvaddr;
	socklen_t addrlen=sizeof(addr);
	struct sockaddr_in* addrin;
    struct sockaddr_in ServerAddr;
	ServerAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_port= htons(PORT);

	int raw_socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	memset(package, 0, sizeof(package));
	icmp=(struct icmphead*)package;

	/*设置ICMP首部各字段值*/
	icmp->icmp_type = 8;
	icmp->icmp_code = 0;
	icmp->icmp_id = 1;
	icmp->icmp_seq = 1;
	icmp->icmp_cksum = (cksum(package, 8));

	printf("icmp_type=%d\n",icmp->icmp_type);
	printf("address1=%p,addr2=%p\n",icmp,package);
	int r;
	ip= (struct iphead*)recvbuf;
	recvicmp=(struct icmphead*)(recvbuf+32);
	for (int i=0;i<5;i++){
		printf("package=%s\n",package);
		printf("size=%d\n",sizeof(package));
		icmp->icmp_seq++;
		r=sendto(raw_socket,package,sizeof(package),0,(struct sockaddr*) &ServerAddr,addrlen);
		if (r<0){
			perror("send failed: ");
		}else{
			printf("send successful\n");
		}
		memset(recvbuf,0,1024);
		recvfrom(raw_socket,recvbuf,sizeof(recvbuf),0,(struct sockaddr*) &recvaddr,&addrlen);

		printf("recvbuf=%x\n\n",recvbuf[0]);
		printf("iptos=%x\n",ip->ip_len);
		printf("ipversion=%d\n\n",ip->ip_version);

		printf("icmptype=%d\n",recvicmp->icmp_type);
		printf("icmpcode=%d\n",recvicmp->icmp_code);
		printf("icmpid=%x\n",recvicmp->icmp_id);
		printf("icmpseq=%x\n",recvicmp->icmp_seq);
		printf("\n");
		sleep(1);
	}


	return EXIT_SUCCESS;
}

unsigned short int cksum(char buffer[], int size){
    unsigned long sum = 0;
    unsigned short int answer;
    unsigned short int *temp;
    temp = (short int *)buffer;
    for( ; temp<buffer+size; temp+=1){
        sum += *temp;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;
    return answer;
}

