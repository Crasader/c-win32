// EasyQQServe.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../EasyQQDemo/EasyQQSocket.h"

int main()
{
	CEasyQQServer QQServer(10086);
	QQServer.Init();
	QQServer.ClientConnectLoop();
	//QQServer.Init();

	//printf("success!");
    return 0;
}

