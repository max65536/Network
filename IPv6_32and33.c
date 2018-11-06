/*
 ============================================================================
 Name        : IPv6_32.c
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
#include<netdb.h>
#include <unistd.h>
#include <sys/stat.h>

#define PORT 16007
#define MAX_HEAD_SIZE 1024
void clienterror(int fd, char *errnum,
		 char *shortmsg, char *longmsg);
void send2client(int fd, char *code,
		 char *status,char *msg);
int sendString(int fd, char *buf, int* len);
void do_service(int fd);
void *get_in_addr(struct sockaddr *sa);

char* file_type(char* arg) ;
int not_exit(char* arguments);
unsigned long get_file_size(const char *path);
void send_header(int sock,char* content_type, int content_length);
void send_file(char* arguments, int sock);
void handle_req(char* request, int client_sock);

int main(void) {
	int socketfd,clientfd;
	struct addrinfo hints, *res;
	struct sockaddr_storage addr;
	socklen_t addrlen=sizeof(addr);
	int child_pid=0;
	int r;	
	char Head[MAX_HEAD_SIZE];
	char buf[MAX_HEAD_SIZE];
	char msg[MAX_HEAD_SIZE];
	char ip6[128];
	char host[128];
	int i=0;
	FILE *f;

	memset(&hints,0,sizeof hints);
	hints.ai_family = AF_INET6;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	getaddrinfo(NULL, "16007", &hints, &res);

	socketfd = socket(res->ai_family,res->ai_socktype, res->ai_protocol);

	bind(socketfd, res->ai_addr, res->ai_addrlen);


	listen(socketfd, 5);


	while(1){

		printf("waiting for connection...\n");
		clientfd=accept(socketfd,(struct sockaddr*)&addr,&addrlen);
		if (clientfd == -1) {
            		printf("accept error\n");
            		continue;
        	}
		gethostname(host,sizeof host);//ipv6_33
		printf("----------------------------------------\n");
		printf("connection from %s\n", inet_ntop(addr.ss_family, get_in_addr((struct sockaddr*)&addr),ip6,sizeof ip6) );//ipv6_33
		printf("Local is %s\n", host );//ipv6_33

		child_pid=fork();
		printf("child_pid=%d\n",child_pid);
		if ((child_pid)==0){
			close(socketfd);
			memset(buf, 0, sizeof(buf));
                        r=recv(clientfd,buf,1024,0);
                        printf("%s",buf);

            		handle_req(buf,clientfd);
			close(clientfd);
            		exit(0);
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

void *get_in_addr(struct sockaddr *sa){
		if(sa->sa_family == AF_INET){
			return &(((struct sockaddr_in*)sa)->sin_addr);
		}
		return &(((struct sockaddr_in6*)sa)->sin6_addr);

}


int sendString(int socket, char* buf, int* len){
//	int len=strlen(buf);
	int total=0;
	int left=*len;
	int r;
	while(total<*len){
		r=send(socket,buf+total,left,0);
		if (r==-1) break;
		total+=r;
		left-=r;
	}
    // printf("send:%s\n",buf );
	return r==-1?-1:0;
}


char* file_type(char* arg) {
 char * temp;          //临时字符串指针
 if ((temp=strrchr(arg,'.')) != NULL) {    //取得后缀
  return temp+1;
 }
 return "";      //如果请求的文件名中没有. 则返回空串
}
int not_exit(char* arguments) {
    struct stat dir_info;
    return (stat(arguments, &dir_info) == -1);
}

unsigned long get_file_size(const char *path)
{
    unsigned long filesize = -1;
    struct stat statbuff;
    if(stat(path, &statbuff) < 0){
        return filesize;
    }else{
        filesize = statbuff.st_size;
    }
    return filesize;
}


void send_header(int sock,char* content_type, int content_length){
	char head[1024];
    int len=strlen(head);

    memset(head, 0, sizeof(head));
    sprintf(head,"HTTP/1.1 200 OK\r\n");
    sprintf(head,"%sConnection: close\r\n",head);
	// sendString(sock,head,&len);

    if (content_length){
        sprintf(head,"%sContent-Length: %d\r\n",head,content_length);
    }
    if (content_type){
        sprintf(head,"%sContent-Type: %s\r\n\r\n",head,content_type);
        // sendString(sock,tmp,&len);
    }
    len=strlen(head);
    sendString(sock,head,&len);
	printf("\nresponse:\n");
    printf("%s\n", head);
	// printf("%s  ,%s",head,tmp);
}


void send_file(char* arguments, int sock) {
    char* extension = file_type(arguments);    //获得文件后缀名
    char* content_type = "text/plain";     //初始化type='text/plain'
    FILE* read_from;         //本地文件指针从该文件中读取.html .jpg等
    int readed = -1;         //每次读得的字节数
    int len=0;
    int isImage=0;
    printf("sending FILE..............\n");
    if (strcmp(extension, "html") == 0) {    //发送内容为html
        content_type = "text/html";
    }
    if (strcmp(extension, "gif") == 0) {    //发送内容为gif
        content_type = "image/gif";
        isImage=1;
    }
    if (strcmp(extension, "jpg") == 0) {    //发送内容为jpg
        content_type = "image/jpg";
        isImage=1;
    }

    if (isImage){
        read_from = fopen(arguments, "rb");
        printf("arguments=%s\n", arguments);
        int size=get_file_size(arguments);

        send_header(sock, content_type,size);    //发送协议头

        printf("size=%d\n",size);
        char *read_buf=(char*)malloc(sizeof(char)*size);
        fread(read_buf,1,size,read_from);
        if (sendString(sock, read_buf,&size) == -1) { //发送数据
            printf("Sending error!");    //出现发送错误显示到控制台 继续发送
        }
        fclose(read_from);
        return;
    }

    read_from = fopen(arguments, "r");     //打开用户指定的文件准备读取
    if(read_from != NULL) {        //指针不为空
        char read_buf[1024];        //读文件时的字节缓存数组
        memset(read_buf, 0, sizeof(read_buf));
        len=get_file_size(arguments);
        send_header(sock, content_type,len);    //发送协议头

        while(!feof(read_from)) {      //判断文件是否已经结束
            if (fgets(read_buf, 1024, read_from)==NULL) break;   //读取
            int len = strlen(read_buf);
            // printf("read:  %s\n",read_buf);
            if (sendString(sock, read_buf,&len) == -1) { //发送数据
                printf("Sending error!");    //出现发送错误显示到控制台 继续发送
                continue;
            }
        }
        fclose(read_from);
    }else{printf("FILE is NULL!!!!!!!!!!!!!!!!!!!!!!!!\n");}
}

void handle_req(char* request, int client_sock) {
 char command[BUFSIZ];        //保存解析到的命令字段 GET PUT
 char arguments[BUFSIZ];        //保存解析到的请求的文件
 /*
  * 在用户请求前加上当前目录符号
  */
 strcpy(arguments, ".");       //注意该符号在不同操作系统的区别
// arguments[0]=".";
 /*
  * 解析请求
  */
// if (sscanf(request, "%s%s", command, arguments+2) != 2) {
//  return;           //解析出错在返回
// }
 sscanf(request, "%*s%s", arguments+1);

 printf("handle_cmd:    %s\n",command);    //向控制台输出此时的命令
 printf("handle_path:   %s\n",arguments);   //向控制台输出此时的请求路径

// if (strcmp(command, "GET") != 0) {     //请求命令格式是否正确
//  wrong_req(client_sock);
//  return;
// }
 if (not_exit(arguments)) {       //请求的文件是否存在
	 clienterror(client_sock, "404",
	 		 "NOT FOUND", "");
	 printf("NOT FOUND !!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
  return;
 }
 printf("SENDING .......................\n");
 send_file(arguments, client_sock);     //命令格式及请求路径正确则发送数据
 printf("Sending complete\n");

 return;
}
