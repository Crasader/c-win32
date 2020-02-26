/************************************************************
*  文件名：TalkClientService.cpp
*  描述：  客户端业务实现
*************************************************************/
#include "stdafx.h"
#include "MyTCP.h"  

#include "MyTools.h"  
#include "TalkClientService.h"  
void myRecvThread();
#define PORT 5001  


extern char serverip[6];

char qq[6] = { 0 };
char fqq[6] = { 0 };
int sock;

int status = STUS_START;

void mainProc()
{
	while (1)
	{

		switch (status)
		{
		case STUS_CONNECTED:
			statusConnected();
			break;

		case STUS_WAIT_LOGIN_RES:
			printMsg("等待服务器响应..........\n");
			Sleep(1000);
			break;

		case STUS_LOGINED:
			statusLogined();
			break;

		case STUS_WAIT_QUERY_FQQ_RES:
			printMsg("等待服务器响应..........\n");
			Sleep(1000);
			break;

		case STUS_TALK:
			statusTalk();
			break;
		case STUS_WAIT_QUERY_ALLQQ_RES:
			printMsg("等待服务器响应..........\n");
			Sleep(1000);
			break;
		}
	}
}

void statusTalk()
{
	printMsg("退出聊天(Exit) 我说>>");
	setbuf(stdin, NULL);   //清空键盘缓冲区  

	char buf[1024] = { 0 };
	gets_s(buf);
	if (strcmp(buf, "Exit") == 0)
	{
		printMsg("客户端已经退出！\n");
		statusLogined();
	}

	else
	{
		struct TalkMsg msg;

		msg.id = TALK_MSG;
		strcpy(msg.qq, qq);
		strcpy(msg.fqq, fqq);
		strcpy(msg.info, buf);
		status = 5;
		tcpSend(sock, (const char *)&msg, sizeof(msg));
	}

	return;
}

void statusLogined()
{
	char buf0[10] = { 0 };
	int rt = 0;
	while (rt == 0)
	{
		memset(buf0, 0, sizeof(buf0));
		printMsg("显示在线好友(L)|找好友聊天(T)|退出(E) 默认(T)>>");
		setbuf(stdin, NULL);   //清空键盘缓冲区  
		gets_s(buf0);
		if (strcmp(buf0, "T") == 0 || strcmp(buf0, "t") == 0)
		{
			printMsg("选择聊天好友......\n");
			while (1)
			{
				printMsg("输入聊天好友QQ号(最多不超过5位)：  退出请输入“quit”");
				char tmp[1024];
				scanf("%s", tmp);
				if (strlen(tmp) > 5)
				{
					printMsg("您输入的qq号长度大于5个，请重新输入\n");
				}
				else if (strcmp(tmp, qq) == 0)
				{
					printMsg("您输入的qq号是自己号码，难道您要自言自语吗？::) 请重新输入\n");
				}
				else if (strcmp(tmp, "quit") == 0)
					break;
				else
				{   //发送要聊天的好友QQ号给服务器  
					struct FriendQqMsg msg;
					msg.id = QUERY_FQQ;
					strcpy(msg.qq, tmp);
					strcpy(fqq, tmp);
					status = STUS_WAIT_QUERY_FQQ_RES;
					tcpSend(sock, (const char *)&msg, sizeof(msg));
					rt = 1;
					break;
				}
			}
		}
		else if (strcmp(buf0, "L") == 0 || strcmp(buf0, "l") == 0)
		{
			char buf2[10] = { 0 };
			memset(buf2, 0, sizeof(buf2));
			struct RequestAllFriend res;
			res.id = REQUEST_ALL_FRD;
			memcpy(buf2, &res, sizeof(res));
			tcpSend(sock, (const char*)buf2, sizeof(buf2));
			status = STUS_WAIT_QUERY_ALLQQ_RES;
			Sleep(100);
		}
		else if (strcmp(buf0, "E") == 0 || strcmp(buf0, "e") == 0)
		{
			printMsg("客户端已经退出！\n");
			exit(0);
		}
		else
		{
			printMsg("输入内容错误（手抖了吧-_-)，请重新输入\n");

		}


	};

}

void connectServer()
{
	printMsg("开始连接服务器......\n");
	initSock(0);  //客户端  
	sock = tcpConnect(serverip, PORT);
	printMsg("连接服务器成功！\n");

	//创建一个线程用于接收服务器发送的信息  
	DWORD threadId;
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)myRecvThread, NULL, 0, &threadId);
}

//创建接收线程专用于接收并处理服务器返回的收数据  
void myRecvThread()
{
	char buf[1024] = { 0 };
	while (1)
	{
		memset(buf, 0, sizeof(buf));
		tcpRecv(sock, buf, sizeof(buf));
		unsigned char id = (unsigned char)buf[0];
		switch (id)
		{
		case LOGIN_MSG_RES:
			procLoginMsgResponse((struct LoginMsgResponse *)buf);
			break;

		case QUERY_FQQ_RES:
			procFriendQqMsgResponse((struct FriendQqMsgResponse *)buf);
			break;

		case TALK_MSG:
			procTalkMsg((struct TalkMsg *)buf);
			break;
		case APPLY_ALL_FRD:
			procApplyAllFriend((struct AllFriendMsg *)buf);
			break;

		}
	}
	sprintf(buf, "结束myRecvThread线程:%d.........\n");
	printMsg(buf);
	exit(0);
}

void statusConnected()
{
	printMsg("\n\t请输入登录信息\n");

	while (1)
	{
		printMsg("本机的QQ号(最多不超过5位数字)：");
		char tmp[1024];
		scanf("%s", tmp);
		int  i;
		if (strlen(tmp) > 5)
		{
			printMsg("您输入的qq号长度大于5个，请重新输入\n");
			continue;
		}

		for (i = 0; i < strlen(tmp); i++)
		{
			if (tmp[i] >= '0' && tmp[i] <= '9')
			{
				continue;
			}
			else
				break;
		}

		if (i == strlen(tmp))
		{
			strcpy(qq, tmp);
			break;

		}
		else
		{
			printMsg("输入信息有误，请重新输入\n");
			continue;
		}

	}

	struct LoginMsg msg;
	msg.id = LOGIN_MSG;
	status = STUS_WAIT_LOGIN_RES;
	strcpy(msg.qq, qq);
	tcpSend(sock, (const char *)&msg, sizeof(msg));
	mainProc(); //进入主处理程序  
	return;
}

void procLoginMsgResponse(struct LoginMsgResponse *msg)
{
	if (msg->isOK == 1)
	{
		printMsg("登录成功！\n");
		status = STUS_LOGINED;//已登录状态  
	}
	else
	{
		printMsg("登录失败！\n");
		printMsgExit(msg->reason);
		printf("\n");
	}
}

void procFriendQqMsgResponse(struct FriendQqMsgResponse *msg)
{
	if (msg->isOK == 1)
	{
		printMsg("好友目前在线，可以开始聊天...\n");
		status = STUS_TALK;
	}
	else
	{
		printMsg("好友不存在啊，再换个人试试！\n");
		status = STUS_LOGINED;
	}
}

void procTalkMsg(struct TalkMsg *msg)
{
	char buf[1024] = { 0 };

	if (strcmp(msg->qq, fqq) == 0)
	{//表示是自己发起的聊天，仅仅显示信息即可  
		printf("\n");
		sprintf(buf, "%s说>>%s", fqq, msg->info);
		printMsg(buf);
	}
	else
	{//是陌生人发起的聊天  
		printf("\n");
		sprintf(buf, "%s说>>%s", msg->qq, msg->info);
		printMsg(buf);
	}
}

void procApplyAllFriend(struct AllFriendMsg *msg)
{

	char buf[100] = { 0 };
	sprintf(buf, "%s", msg->qq);
	printMsg("在线的QQ好友：   \n");
	printMsg(buf);
	printf("\n");
	status = STUS_LOGINED;
}