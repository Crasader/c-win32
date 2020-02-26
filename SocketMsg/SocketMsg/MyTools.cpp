/************************************************************
*  文件名：MyTools.cpp
*  描述：  共用的文件，工具函数定义
*************************************************************/
#include <stdio.h> 
#include "stdafx.h"
#include <stdlib.h>  
#include "MyTools.h"  

extern int flag;

void printMsg(const char * msg) {
	if (flag)
		printf("\t【南工服务器豪华版】%s", msg);
	else
		printf("\t【南工客户端豪华版】%s", msg);
}

void printInt(int value) {
	if (flag)
		printf("\t【南工服务器豪华版】%d", value);
	else
		printf("\t【南工客户端豪华版】%d", value);
}

void printMsgExit(const char * msg) {
	printMsg(msg);
	exit(0);
}

void printIntExit(int value) {
	printInt(value);
	exit(0);
}