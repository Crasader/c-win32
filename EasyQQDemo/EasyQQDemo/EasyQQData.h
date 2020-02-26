#pragma once
#include <time.h>
#define MAX_LEN 255


#pragma pack(1)
typedef enum LOGINRES
{
	LOGIN_SUCCESS,
	LOGIN_ERR_HASLOGIN,
	LOGIN_ERR_WRONGPASSWORD,
	LOGIN_NOTLOGIN
}LoginRes;
#pragma pack

#pragma pack(1)
typedef enum EASYQQDATATYPE
{
	EASYQQLOGIN,
	EASYQQCHAT,
	EASYQQACCOUNTINFO,
	EASYQQREGISTER,
	EASYQQADDFRIENDS,
	EASYQQNULL,
	
}Datatype;
#pragma pack

#pragma pack(1)
struct ISLOGIN
{
	LoginRes Islogin = LOGIN_NOTLOGIN;
	wchar_t user_account[20];
	wchar_t user_friendlist[10][10];
	ISLOGIN()
	{
		memset(user_friendlist, 0, sizeof(user_account));
		memset(user_account, 0, sizeof(user_account));
	}
	//ISLOGIN& ISLOGIN::operator=(ISLOGIN& info)
	//{
	//	Islogin = info.Islogin;
	//	wcscpy_s(user_account, info.user_account);
	//	for (size_t i = 0; i < 10; i++)
	//	{
	//		wcscpy_s(user_friendlist[i], info.user_friendlist[i]);
	//	}
	//	return *this;
	//}
};
#pragma pack


#pragma pack(1)
struct LoginData
{
	Datatype type ;
	wchar_t user_account[20];
	wchar_t user_code[20];
	wchar_t Addfriend_account[20];
	tm send_time;
	wchar_t Message[MAX_LEN];
	ISLOGIN loginstate;
	wchar_t chatfriendaccount[20];
	LoginData()
	{
		type = EASYQQNULL;
		memset(Addfriend_account, 0, sizeof(Addfriend_account));
		memset(user_code, 0, sizeof(user_code));
		memset(user_account, 0, sizeof(user_account));
		memset(Message, 0, MAX_LEN);
	}
	LoginData& LoginData::operator=(LoginData& info)
	{
		type = info.type;
		wcscpy_s(Addfriend_account, info.Addfriend_account);
		wcscpy_s(user_account, info.user_account);
		wcscpy_s(user_code, info.user_code);
		send_time = info.send_time;
		//wcscpy_s(send_time, info.send_time);
		wcscpy_s(Message, info.Message);
		loginstate = info.loginstate;
		return *this;
	}
};
#pragma pack





#pragma pack(1)
struct AccountInfo
{
	wchar_t user_account[20];
	wchar_t user_code[20];
	ISLOGIN loginstate;
};
#pragma pack