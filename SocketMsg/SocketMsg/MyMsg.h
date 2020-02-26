#pragma once  
/************************************************************
*  文件名：MyMsg.h
*  描述：  服务器和客户端共用的头文件，消息定义
*************************************************************/
#ifndef _MYMSG_H_  
#define _MYMSG_H_  

enum {
	LOGIN_MSG,           //登录  
	LOGIN_MSG_RES,       //登录响应  
	QUERY_FQQ,           //查询单个好友QQ  
	QUERY_FQQ_RES,       //查询单个好友QQ的响应  
	TALK_MSG,             //聊天  
	REQUEST_ALL_FRD,      //请求所有qq好友信息  
	APPLY_ALL_FRD        //应答所有qq好友信息  
};

struct LoginMsg {
	unsigned char id;  //消息编号  
	char qq[6];        //登录ID号  
};

struct LoginMsgResponse {
	unsigned char id;  //消息编号  
	unsigned char isOK;
	char reason[100];    //如果失败，则在此写入失败原因  
};

struct FriendQqMsg {
	unsigned char id;  //消息编号  
	char qq[6];
};

struct FriendQqMsgResponse {
	unsigned char id;  //消息编号  
	unsigned char isOK;
};

struct TalkMsg {
	unsigned char id;  //消息编号  
	char qq[6];
	char fqq[6];
	char info[200];
};

struct RequestAllFriend {
	unsigned char id;   //消息编号  
};

struct AllFriendMsg {
	unsigned char id;  //消息编号  
	char qq[6];
};

#endif  
