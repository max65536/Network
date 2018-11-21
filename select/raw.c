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

#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
// #include <netinet/ip_icmp.h>

#include <netinet/ip.h>
#include <time.h>

#define PORT 0
#define MAX_MSG_SIZE 1024


// struct iphead{
// 	unsigned char  ip_hl:4,ip_version:4;  //ip_hl,ip_version各占四个bit位。
// 	unsigned char ip_tos;
// 	unsigned short int ip_len;
// 	unsigned short int ip_id;
// 	unsigned short int ip_off;
// 	unsigned char ip_ttl;
// 	unsigned char ip_pro;
// 	unsigned short int ip_sum;
// 	unsigned int ip_src;
// 	unsigned int ip_dst;
// };
struct icmp
{
  u_int8_t  icmp_type;	/* type of message, see below */
  u_int8_t  icmp_code;	/* type sub code */
  u_int16_t icmp_cksum;	/* ones complement checksum of struct */
  union
  {
    u_char ih_pptr;		/* ICMP_PARAMPROB */
    struct in_addr ih_gwaddr;	/* gateway address */
    struct ih_idseq		/* echo datagram */
    {
      u_int16_t icd_id;
      u_int16_t icd_seq;
    } ih_idseq;
    u_int32_t ih_void;

    /* ICMP_UNREACH_NEEDFRAG -- Path MTU Discovery (RFC1191) */
    struct ih_pmtu
    {
      u_int16_t ipm_void;
      u_int16_t ipm_nextmtu;
    } ih_pmtu;

    struct ih_rtradv
    {
      u_int8_t irt_num_addrs;
      u_int8_t irt_wpa;
      u_int16_t irt_lifetime;
    } ih_rtradv;
  } icmp_hun;
};
#define	icmp_pptr	icmp_hun.ih_pptr
#define	icmp_gwaddr	icmp_hun.ih_gwaddr
#define	icmp_id		icmp_hun.ih_idseq.icd_id
#define	icmp_seq	icmp_hun.ih_idseq.icd_seq
#define	icmp_void	icmp_hun.ih_void
#define	icmp_pmvoid	icmp_hun.ih_pmtu.ipm_void
#define	icmp_nextmtu	icmp_hun.ih_pmtu.ipm_nextmtu
#define	icmp_num_addrs	icmp_hun.ih_rtradv.irt_num_addrs
#define	icmp_wpa	icmp_hun.ih_rtradv.irt_wpa
#define	icmp_lifetime	icmp_hun.ih_rtradv.irt_lifetime

uint16_t cksum(unsigned short buffer[], int size);

int main(int argc, char *argv[]){
	int on = 1;
	struct ip *iphd;
	struct icmp *icmphd;
	struct icmp *recvicmp;
	// struct icmphdr ic;
	char sendbuf[1024],recvbuf[1024];
	unsigned char package[8];
	clock_t start,finish;

    	struct sockaddr_in ServerAddr;
	struct sockaddr_in addr;
	struct sockaddr_in recvaddr;
	socklen_t addrlen=sizeof(addr);
	struct sockaddr_in* addrin;
	
	char address[20];
	strcpy(address,argv[1]);
//	for (short i=0;i<argc; i++){
//		printf("i=%d:%s\n",i,argv[i]);
//	}
	printf("ping %s...\n",address);

	ServerAddr.sin_addr.s_addr=inet_addr(address);
//	ServerAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_port= htons(PORT);

	int raw_socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	icmphd=(struct icmp*)package;




	// printf("icmp_type=%d\n",icmphd->icmp_type);
	// printf("address1=%p,addr2=%p\n",icmphd,package);
	int r;
	iphd= (struct ip*)recvbuf;
	recvicmp=(struct icmp*)(recvbuf+sizeof(struct ip));

//	for (short i=0;i<argc; i++){
//		printf("%s\n",argv[i]);
//	}

	for (short i=0;i<5;i++){
		// printf("package=%s\n",package);
		// printf("size=%d\n",sizeof(package));
		memset(package, 0, sizeof(package));
		/*设置ICMP首部各字段值*/
		icmphd->icmp_type = 8;
		icmphd->icmp_code = 0;
		icmphd->icmp_id = 123;
		// icmphd->icmp_seq=i;
		icmphd->icmp_seq=i+1;
		icmphd->icmp_cksum = 0;
		icmphd->icmp_cksum =cksum((unsigned short*)icmphd, sizeof(struct icmp));
		memset(recvbuf,0,1024);
		start=clock();
		r=sendto(raw_socket,package,sizeof(package),0,(struct sockaddr*) &ServerAddr,addrlen);
		if (r<0){
			perror("send failed: ");
		}else{
			printf("send successful\n");
		}

		recvfrom(raw_socket,recvbuf,sizeof(recvbuf),0,(struct sockaddr*) &recvaddr,&addrlen);
		finish=clock();
		// printf("recvbuf=%x\n\n",recvbuf[0]);
		// printf("iplen=%x\n",iphd->ip_len);
		// printf("ipversion=%d\n\n",iphd->ip_v);

		// printf("icmptype=%d\n",recvicmp->icmp_type);
		// printf("icmpcode=%d\n",recvicmp->icmp_code);
		// printf("icmpid=%d\n",recvicmp->icmp_id);
		// printf("icmpseq=%d\n",recvicmp->icmp_seq);

		printf("ping response: seq=%d, time=%dms\n",recvicmp->icmp_seq, (finish-start));
		// printf("\n");
		sleep(1);
	}


	return EXIT_SUCCESS;
}


uint16_t cksum(unsigned short *buf, int len)
{
    uint32_t sum = 0;

    while(len > 1) {
        sum += *buf;
        buf++;
        len -= 2;
    }
    if(len)
        sum += (*(uint8_t *)buf);

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum = (sum >> 16) + sum;

    return ~sum;
}
// in 6
//        {
//            /* Compute Internet Checksum for "count" bytes
//             *         beginning at location "addr".
//             */
//        register long sum = 0;

//         while( count > 1 )  {
//            /*  This is the inner loop */
//                sum += * (unsigned short) addr++;
//                count -= 2;
//        }

//            /*  Add left-over byte, if any */
//        if( count > 0 )
//                sum += * (unsigned char *) addr;

//            /*  Fold 32-bit sum to 16 bits */
//        while (sum>>16)
//            sum = (sum & 0xffff) + (sum >> 16);

//        checksum = ~sum;
//    }
