//#include "stdafx.h"
#include <Winsock2.h>
#include<stdio.h>

#pragma comment (lib, "ws2_32.lib")



int _tmain(int argc, _TCHAR* argv[])
{
    WORD dwVersionReq = MAKEWORD(1,1);
    WSAData wsData = {0};
    //加载套接字库
    if (0 != WSAStartup(dwVersionReq,&wsData))
    {
        WSACleanup();
        return 0;
    }
    //socket版本确认
    if (LOBYTE(wsData.wVersion) != 1 || HIBYTE(wsData.wVersion != 1))
    {
        WSACleanup();
        return 0;
    }

    //创建服务套接字
    SOCKET socketSrv = socket(AF_INET,SOCK_DGRAM,0/*自动选择协议*/);

    //对socket绑定端口号和IP地址
    SOCKADDR_IN addrServer;
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(6000);
    addrServer.sin_addr.S_un.S_addr = htonl(ADDR_ANY);
    bind(socketSrv,(SOCKADDR*)&addrServer,sizeof(addrServer));


    //保存client端socket信息
    SOCKADDR_IN addClient;
    int len = sizeof(SOCKADDR_IN);

    while (1)
    {
        //等待并接收数据
        char szBuffer[100] = {0};
        recvfrom(socketSrv,szBuffer,100,0,(SOCKADDR*)&addClient,&len);
        printf("client->server:%s\n",szBuffer);
        //发送数据
        char szSendBuf[100] = "this is server";
        sendto(socketSrv, szSendBuf, 100,0,(SOCKADDR*)&addClient,len);
    }


    //关闭套接字
    closesocket(socketSrv);

    //关闭winsocket库
    WSACleanup();

    system("pause");

    return 0;
}
