#pragma once
#include "stdafx.h"
#include "D:\po\project\EasyQQDemo\DuiLib\Utils\WinImplBase.h"
#include "EasyQQFriendList.h"

using namespace DuiLib;
class CEasyQQChatBox :
	public WindowImplBase
{
public:
	CEasyQQChatBox(ISLOGIN login, const FriendListItemInfo& friend_info);
	~CEasyQQChatBox();

public:
	LPCTSTR GetWindowClassName() const;
	virtual void OnFinalMessage(HWND hWnd);
	virtual void InitWindow();
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void SendEditMsg();
	void SendMsg(const LoginData info);
private:
	void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);
	void OnExit(TNotifyUI& msg);
	void OnTimer(TNotifyUI& msg);
	static DWORD WINAPI RecvMsg(LPVOID lpParm);
private:
	ISLOGIN m_clientInfo;
	FriendListItemInfo m_friends;
	HANDLE hRecvThread;
};

