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
#include <fcntl.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#define PORT 16007
#define MAX_HEAD_SIZE 1024
char read_buf[1024];

void clienterror(int fd, char *errnum,
		 char *shortmsg, char *longmsg);
int sendString(int fd, char *buf,
		 int* len);
void do_service(int fd);
//void sendFile(int fd, char *code,
//		 char *status,FILE *f);

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
	printf("HEADER111111111111\n");
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
       // char read_buf[1024];        //读文件时的字节缓存数组
        memset(read_buf, 0, sizeof(read_buf));
        len=get_file_size(arguments);
        send_header(sock, content_type,len);    //发送协议头

        while(!feof(read_from)) {      //判断文件是否已经结束
            if (fgets(read_buf, 1024, read_from)==NULL) break;   //读取
            int len = strlen(read_buf);
            printf("read:  %s\n",read_buf);
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

 sscanf(request, "%*s%s", arguments+1);

 printf("handle_cmd:    %s\n",command);    //向控制台输出此时的命令
 printf("handle_path:   %s\n",arguments);   //向控制台输出此时的请求路径


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


int main(void) {
        FILE *f;
        char Head[MAX_HEAD_SIZE];
        char buf[MAX_HEAD_SIZE];
        char msg[MAX_HEAD_SIZE];
        int i=0;
        int on=1;
	int status;
        int r;
        int result; //select result
        int nread;
        int clientfd,fd;
	struct sockaddr_in ServerAddr;
        struct sockaddr_in addr;
        struct sockaddr_in* addrin;
        struct timeval timeout; //waitting time
        socklen_t addrlen=sizeof(addr);
        fd_set readfds,writefds; //r/w descriptor

        int socketfd=socket(AF_INET,SOCK_STREAM,0);
        setsockopt(socketfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)); //reuseport
        fcntl(socketfd, F_SETFL, O_NONBLOCK); //Setting fd to a nonblocking mode.
        memset(&ServerAddr, 0 , sizeof(struct sockaddr_in));
	ServerAddr.sin_addr.s_addr=INADDR_ANY;
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_port= htons(PORT);

	int bind1=bind(socketfd,(struct sockaddr*) &ServerAddr,sizeof(ServerAddr));

	listen(socketfd, 5);

	while(1){
                timeout.tv_sec=3; //initialize waitting time, 1 second
                timeout.tv_usec=0;
                FD_ZERO(&readfds);;
                FD_SET(socketfd,&readfds);
                result = select(socketfd, &readfds, &writefds, (fd_set*)0, &timeout);
		printf("waiting for connection...\n");
                if(result == -1)
                     continue;
                else if (result) {
                   for(fd=0; fd<FD_SETSIZE; fd++ )
                    {
                        if(FD_ISSET(fd, &readfds))
                        {
                            if(fd = socketfd)
                            {
                                clientfd=accept(socketfd,(struct sockaddr*)&addr,&addrlen);
                                FD_SET(clientfd, &readfds);
                                printf("Adding client on fd %d \n", clientfd);
                            }
                            elseif(fd>=3) //client_fd
                            {
                                ioctl(fd, FIONREAD, &nread);
                                if(nread == 0) //clients request is finished
                                {
                                    close(fd);
                                    FD_CLR(fd, &readfds);
                                    printf("Removing client on fd %d \n", fd);
                                }
                                else
                                {
                                    memset(buf, 0, sizeof(buf));
                                    r=recv(fd,buf,1024,0);
                                    printf("%s",buf);
                                    handle_req(buf, fd);
                                    printf("Serving client on fd %d \n", fd);
                                }
                            }
                            elseif(fd == 0)
                            {
                                ioctl(fd, FIONREAD, &nread);
                                if(nread == 0)
                                {

                                }
                            }
                        }
                    }
                }
                else
                    printf("No data within one second");


                //printf("connection from %s:%d\n", inet_ntoa(addr.sin_addr),ntohs(addr.sin_port) );



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


void do_service(int fd){
	char buf[1024];
	int i,r;
	FILE *f;
	while (1)
	{
		memset(buf, 0, sizeof(buf));


		f=fdopen(fd,"r");

		for (i=0;i<1;i++){
			fgets(buf,1024,f);
			printf("%s",buf);
		}


		fclose(f);

		handle_req(buf, fd);
	}
}

