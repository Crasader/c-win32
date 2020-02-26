#include "stdafx.h"
#include "EasyQQMain.h"
#include "EasyQQFriendList.h"
#include "EasyQQAddFriend.h"
#include "EasyQQVerify.h"
#include "EasyQQChatBox.h"

const TCHAR* const kCloseButtonControlName = _T("closebtn");

const TCHAR* const kHeadloginControlName = _T("headlogo");

const TCHAR* const kFriendsListControlName = _T("friends");

const TCHAR* const kAddFriendsControlName = _T("AddFriends");

const TCHAR* const kNickNameControlName = _T("MainNickname");

const TCHAR* const kLoginStateControlName = _T("loginstate");

const TCHAR* const kBackgroundControlName = _T("bg");

CEasyQQMain::CEasyQQMain(ISLOGIN& logininfo)
	: bk_image_index_(0) , m_clientInfo(logininfo)
{

}


CEasyQQMain::~CEasyQQMain()
{
	PostQuitMessage(0);
}



LPCTSTR CEasyQQMain::GetWindowClassName() const
{
	return _T("DioGuiFoundation");
}

void CEasyQQMain::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void CEasyQQMain::InitWindow()
{
	hRecvThread = CreateThread(NULL, 0, RecvAddFriend, this, 0, NULL);
	CTextUI* pTextUI = static_cast<CTextUI*> (m_PaintManager.FindControl(kNickNameControlName));
	DWORD Nicknamefont = pTextUI->GetFont();
	pTextUI->SetText(m_clientInfo.user_account);
	pTextUI->SetFont(Nicknamefont);
	pTextUI->SetTextColor(0xFFFFFFF);
	g_qqSocket.ThreadRecv();
}

LRESULT CEasyQQMain::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

CDuiString CEasyQQMain::GetSkinFile()
{
	return _T("EasyQQUI.xml");
}

CDuiString CEasyQQMain::GetSkinFolder()
{
	//MessageBox(NULL, CDuiString((CPaintManagerUI::GetInstancePath()) + _T("skin\\")).GetData(), NULL, MB_ICONHAND);
	return (CPaintManagerUI::GetInstancePath());
}

UILIB_RESOURCETYPE CEasyQQMain::GetResourceType() const
{
	return UILIB_FILE;
}

CControlUI * CEasyQQMain::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("FriendList")) == 0)
	{
		return new CFriendsUI(m_PaintManager);
	}
	return NULL;
}

LRESULT CEasyQQMain::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return 0;
}

LRESULT CEasyQQMain::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	return 0;
}

LRESULT CEasyQQMain::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	bHandled = FALSE;
	return 0;
}

LPCTSTR CEasyQQMain::GetResourceID() const
{
	return MAKEINTRESOURCE(101);
}


CDuiString CEasyQQMain::GetZIPFileName() const
{
	return _T("EasyQQUI.zip");
}

void CEasyQQMain::Notify(TNotifyUI & msg)
{
	//UpdateFriendsList();
	if (_tcsicmp(msg.sType, _T("windowinit")) == 0)
	{
		OnPrepare(msg);
	}
	else if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), kCloseButtonControlName) == 0)
		{
			OnExit(msg);
		}
		if (_tcsicmp(msg.pSender->GetName(), kAddFriendsControlName)==0)
		{
			CEasyQQAddFriend* pFrame = new CEasyQQAddFriend(m_clientInfo);
			pFrame->Create(NULL, _T("AddFriends"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
			pFrame->CenterWindow();
			pFrame->ShowWindow(true);
		}
	}
	else if (_tcsicmp(msg.sType, _T("itemactivate")) == 0)
	{
		CFriendsUI* pFriendsList = static_cast<CFriendsUI*>(WindowImplBase::m_PaintManager.FindControl(kFriendsListControlName));
		if ((pFriendsList != NULL) && pFriendsList->GetItemIndex(msg.pSender) != -1)
		{
			if (_tcsicmp(msg.pSender->GetClass(), DUI_CTR_LISTCONTAINERELEMENT) == 0)
			{
				Node* node = (Node*)msg.pSender->GetTag();

				if (!pFriendsList->CanExpand(node))
				{
					FriendListItemInfo friend_info;

					for (std::vector<FriendListItemInfo>::const_iterator citer = friends_.begin(); citer != friends_.end(); ++citer)
					{
						if (_tcsicmp(citer->id, node->data().value) == 0)
						{
							friend_info = *citer;
							break;
						}
					}


					CEasyQQChatBox* pChatDialog = new CEasyQQChatBox(m_clientInfo,friend_info);
					if (pChatDialog == NULL)
						return;
#if defined(WIN32) && !defined(UNDER_CE)
					pChatDialog->Create(NULL, _T("ChatDialog"), UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 0, 0);
#else
					pChatDialog->Create(NULL, _T("ChatDialog"), UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 0, 0);
#endif
					pChatDialog->CenterWindow();
					::ShowWindow(*pChatDialog, SW_SHOW);
				}
			}
		}
	}
	else if (_tcsicmp(msg.sType, _T("itemclick")) == 0)
	{
		CFriendsUI* pFriendsList = static_cast<CFriendsUI*>(WindowImplBase::m_PaintManager.FindControl(kFriendsListControlName));
		if ((pFriendsList != NULL) && pFriendsList->GetItemIndex(msg.pSender) != -1)
		{
			if (_tcsicmp(msg.pSender->GetClass(), DUI_CTR_LISTCONTAINERELEMENT) == 0)
			{
				Node* node = (Node*)msg.pSender->GetTag();

				if (pFriendsList->CanExpand(node))
				{
					pFriendsList->SetChildVisible(node, !node->data().child_visible_);
				}
			}
		}
	}

}

void CEasyQQMain::OnPrepare(TNotifyUI & msg)
{
	g_clientInfo = m_clientInfo;
	UpdateFriendsList();
}

void CEasyQQMain::OnExit(TNotifyUI & msg)
{
	CloseHandle(hRecvThread);
	Close();
}

void CEasyQQMain::OnTimer(TNotifyUI & msg)
{
}

void CEasyQQMain::RenewFriendList()
{
	int oldfriendsnum=0, newfriendsnum=0;
	for (auto friendname: m_clientInfo.user_friendlist)
	{
		if (wcslen(friendname) == 0)
			break;
		oldfriendsnum++;
	}
	for (auto friendname : g_clientInfo.user_friendlist)
	{
		if (wcslen(friendname) == 0)
			break;
		newfriendsnum++;
	}
	/*int oldfriendnum = wcslen((wchar_t*)&m_clientInfo.user_friendlist);
	int newfriendnum = wcslen((wchar_t*)g_clientInfo.user_friendlist);*/
	int leftnum = newfriendsnum - oldfriendsnum;
	if (leftnum>0)
	{
		m_clientInfo = g_clientInfo;
		CFriendsUI* pFriendsList = static_cast<CFriendsUI*>(m_PaintManager.FindControl(kFriendsListControlName));
		if (pFriendsList != NULL)
		{
			std::wstring newfriendname;
			for (int i = 0; i < sizeof(m_clientInfo.user_friendlist) / sizeof(m_clientInfo.user_friendlist[0]); i++)
			{
				if (wcslen(m_clientInfo.user_friendlist[i]) == 0)
				{
					if (wcslen(m_clientInfo.user_friendlist[i - 1]) != 0)
					{
						newfriendname = m_clientInfo.user_friendlist[i - 1];
					}
				}
			}
			FriendListItemInfo item;
			item.id = _T("3");
			item.folder = false;
			item.logo = _T("QQ截图20171007150220.png");
			item.nick_name = newfriendname.c_str();
			//item.nick_name = m_clientInfo.user_friendlist;
			item.description = _T("垃圾玩意儿");
			pFriendsList->AddNode(item, m_root_parent);
			friends_.push_back(item);
		}
	}
	
}

void CEasyQQMain::UpdateFriendsList()
{
	m_pFriendsList = static_cast<CFriendsUI*>(m_PaintManager.FindControl(kFriendsListControlName));
	if (m_pFriendsList != NULL)
	{
		if (!friends_.empty())
			friends_.clear();
		if (m_pFriendsList->GetCount() > 0)
			m_pFriendsList->RemoveAll();

		FriendListItemInfo item;

		item.id = _T("0");
		item.folder = true;
		item.empty = false;
		item.nick_name = _T("我的好友");

		Node* m_root_parent = m_pFriendsList->AddNode(item, NULL);
		friends_.push_back(item);

		for (int i = 0; i<sizeof(m_clientInfo.user_friendlist) / sizeof(m_clientInfo.user_friendlist[0]); i++)
		{
			if (wcslen(m_clientInfo.user_friendlist[i])!=0)
			{
				item.id = _T("2");
				item.folder = false;
				item.logo = _T("QQ截图20171007150220.png");
				item.nick_name = m_clientInfo.user_friendlist[i];
				item.description = _T("垃圾玩意儿");
				m_pFriendsList->AddNode(item, m_root_parent);
				friends_.push_back(item);
			}
			else
			{
				break;
			}
		}
	}
}

DWORD CEasyQQMain::RecvAddFriend(LPVOID lpParm)
{
	CEasyQQMain* pThis = (CEasyQQMain*)lpParm;
	while (true)
	{
		static LoginData info;
		std::vector<LoginData>::iterator iter;
		g_qqSocket.WaitForRecvMessage(info,iter);
		if (info.loginstate.Islogin==LOGIN_SUCCESS)
		{
			if (info.type==EASYQQACCOUNTINFO)
			{
				pThis->CreateVerifyWindows(info.loginstate.user_account);
				pThis->RenewFriendList();
				g_qqSocket.EraseFirstRecvMsg(iter);
			}
			else if(info.type==EASYQQADDFRIENDS)
			{
				g_clientInfo = info.loginstate;
				pThis->RenewFriendList();
				g_qqSocket.EraseFirstRecvMsg(iter);
			}
			else if (info.type==EASYQQCHAT)
			{
				continue;
			}
			else
			{
				MessageBox(NULL, TEXT("已发送给用户"), NULL, MB_ICONINFORMATION);
				g_qqSocket.EraseFirstRecvMsg(iter);
			}
			/*CEasyQQVerify* pFrame = new CEasyQQVerify(info.loginstate.user_account);
			pFrame->Create(NULL, _T("VerifyFriends"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
			pFrame->CenterWindow();
			pFrame->ShowWindow(true);*/
			//MessageBox(NULL, info.loginstate.user_account, NULL, MB_ICONINFORMATION);
		}
		if (info.loginstate.Islogin == LOGIN_NOTLOGIN)
		{
			MessageBox(NULL, TEXT("找不到用户或还未上线"), NULL, MB_ICONINFORMATION);
			g_qqSocket.EraseFirstRecvMsg(iter);
		}
	}
	return 0;
}

void CEasyQQMain::CreateVerifyWindows(wchar_t * username)
{
	CEasyQQVerify* pFrame = new CEasyQQVerify(username,m_clientInfo);
	pFrame->Create(m_hWnd, _T("VerifyFriends"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	pFrame->CenterWindow();
	pFrame->ShowModal();
}



