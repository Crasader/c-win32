/************************************************************
*  文件名：MyTCP.cpp
*  描述：  TCP/IP通信socket封装
*************************************************************/
#include <stdio.h>  
#include "stdafx.h"
#include <WinSock2.h>  
#include "MyTools.h"  
#pragma comment (lib, "ws2_32.lib")  

#define PORT 5001  
int ls;   //侦听套接字  
int flag;//记录服务器或是客户端的标志  

		 /*   描述: 初始化服务器或客户端, 该函数在使用中必需首先调用,客户端和服务器 */
int initSock(int IsServer)
{
	WSADATA data;

	if (WSAStartup(1, &data) < 0)
		printMsgExit("call WSAStartup() failure!");

	ls = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ls < 0)
		printMsgExit("创建套接字失败!");

	flag = IsServer;
	if (IsServer != 0)
	{//服务器  
		struct sockaddr_in servAddr;
		memset(&servAddr, 0, sizeof(servAddr));
		servAddr.sin_family = AF_INET;
		servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servAddr.sin_port = htons(PORT);

		if (bind(ls, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
			printMsg("bind套接字失败!\n");
			return -1;
		}

		if (listen(ls, 10) < 0) {
			printMsg("listen套接字失败!\n");
			return -1;
		}
	}

	return 0;
}

/*   描述: 仅用于客户端, 连接服务器 */
int tcpConnect(const char *serverIP, unsigned short port)
{
	struct sockaddr_in servAddr;

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(serverIP);
	servAddr.sin_port = htons(port);

	if (connect(ls, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
	{
		printMsgExit("连接服务器失败！\n");
	}

	return ls;
}

/* 描述: 发送数据 */
int tcpSend(unsigned int sock, const char *sendBuf, int sendBufLen)
{
	int len = send(sock, sendBuf, sendBufLen, 0);
	return len;
}

/* 描述: 接收数据 */
int tcpRecv(unsigned int sock, char *recvBuf, int recvBufLen)
{
	int len = recv(sock, recvBuf, recvBufLen, 0);
	return len;
}

void tcpClose(unsigned int sock)
{
	closesocket(sock);
	WSACleanup();
}