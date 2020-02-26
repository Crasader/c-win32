/************************************************************
*  文件名：TalkClient.cpp
*  描述：  客户端入口，界面操作
*************************************************************/
#include <stdio.h>  
#include <Windows.h>  
#include "stdafx.h"
#include "TalkClientService.h"  

void displayMenu();
void exitProc();
void defProc();
void procMenu();

char serverip[6];

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("请输入服务器的IP地址。。。\n");
		return 0;
	}
	strcpy(serverip, argv[1]);
	displayMenu();
	procMenu();
	while (1) Sleep(10000);
}

char ch;     //菜单选择  
void displayMenu()
{
	static int t = 1;
	if (t++ == 1)
	{
		printf("\n\n\n\n\n");
		printf("\n\t      @                                                             @        ");
		printf("\n\t    @ ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ @      ");
		printf("\n\t   @  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^   @    ");
		printf("\n\t  @   **      **  **********  **        **         ********   **  **     @   ");
		printf("\n\t @    **      **  **          **        **        **      **  **  **      @  ");
		printf("\n\t @    **      **  **          **        **        **      **  **  **       @  ");
		printf("\n\t @    **********  **********  **        **        **      **  **  **       @  ");
		printf("\n\t @    **      **  **          **        **        **      **  **  **       @  ");
		printf("\n\t @    **      **  **          **        **        **      **              @  ");
		printf("\n\t  @   **      **  **********  ********  ********   ********   @@  @@     @   ");
		printf("\n\t   @   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^  @    ");
		printf("\n\t    @  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^@      ");
		printf("\n\t      @                                                             @        ");

		printf("\n                                                             -----By Hins    ");
		printf("\n\n\n\n\n\t\tPress 'Enter' to enter the menu......");           /*按任一键进入主菜单*/
	}
	getchar();                                 /*从键盘读取一个字符,但不显示于屏幕*/
	system("cls"); /*清屏*/
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\t************************************************\n");
	printf("\t*                                              *\n");
	printf("\t*         南工2016聊天客户端程序               *\n");
	printf("\t*         Version 2.1     豪华版               *\n");
	printf("\t*                                              *\n");
	printf("\t*                                              *\n");
	printf("\t*                                              *\n");
	printf("\t*           1) 登录(L)                         *\n");
	printf("\t*           2) 退出(e)                         *\n");
	printf("\t*                      by---HinsShwan          *\n");
	printf("\t************************************************\n");
	printf("\n");
	connectServer();
	printf("\t您的选择是:");
	ch = getchar();
}

void procMenu()
{
	switch (ch)
	{
	case 'l':
	case 'L':
	case '1':
		statusConnected();
		break;

	case 'e':
	case 'E':
	case '2':
		exitProc();
		break;

	default:
		defProc();
	}
}

void exitProc()
{
	printf("\t客户端程序已经终止！\n");
	exit(0);
}

void defProc()
{
	printf("\t 输入字符错误！\n");
}