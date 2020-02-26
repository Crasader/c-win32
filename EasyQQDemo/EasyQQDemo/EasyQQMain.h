#pragma once
#include "D:\po\project\EasyQQDemo\DuiLib\Utils\WinImplBase.h"
#include "EasyQQFriendList.h"
using namespace DuiLib;

class CEasyQQMain :
	public WindowImplBase
{
public:
	CEasyQQMain(ISLOGIN& logininfo);
	~CEasyQQMain();


public:
	LPCTSTR GetWindowClassName() const;
	virtual void OnFinalMessage(HWND hWnd);
	virtual void InitWindow();
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LPCTSTR GetResourceID() const;
	//virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual CDuiString GetZIPFileName() const;


protected:
	void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);
	void OnExit(TNotifyUI& msg);
	void OnTimer(TNotifyUI& msg);

private:
	void RenewFriendList();
	void UpdateFriendsList();
	static DWORD WINAPI RecvAddFriend(LPVOID lpParm);
	void CreateVerifyWindows(wchar_t* username);
private:
	int bk_image_index_;
	HANDLE hRecvThread;
	FriendListItemInfo myself_info_;
	std::vector<FriendListItemInfo> friends_;
	ISLOGIN m_clientInfo;
	CFriendsUI* m_pFriendsList;
	Node* m_root_parent;
};

