#pragma once  
/************************************************************
*  文件名：TalkClientService.h
*  描述：  客户端业务层函数声明
*************************************************************/
#ifndef _TALKCLIENTSERVICE_H_  
#define _TALKCLIENTSERVICE_H_  
#include <Windows.h>  
#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include "MyMsg.h"  

void connectServer();
void procLoginMsgResponse(struct LoginMsgResponse *msg);
void procFriendQqMsgResponse(struct FriendQqMsgResponse *msg);
void procTalkMsg(struct TalkMsg *msg);
void statusConnected();
void statusLogined();
void statusTalk();
void procApplyAllFriend(struct AllFriendMsg *msg);

/** 客户端状态 **/
enum {
	STUS_START,
	STUS_CONNECTED,
	STUS_WAIT_LOGIN_RES,
	STUS_LOGINED,
	STUS_WAIT_QUERY_FQQ_RES,
	STUS_TALK,
	STUS_WAIT_QUERY_ALLQQ_RES
};

#endif  