#include "stdafx.h"
#include "EasyQQAddFriend.h"



const TCHAR* const kCloseButtonControlName = _T("Closebtn");

const TCHAR* const kAccountEditControlName = _T("AccoutnEdit");

const TCHAR* const kSendMsgControlName = _T("SendAddMsg");


CEasyQQAddFriend::CEasyQQAddFriend(ISLOGIN& clientinfo):m_clientInfo(clientinfo)
{
}


CEasyQQAddFriend::~CEasyQQAddFriend()
{

}

LPCTSTR CEasyQQAddFriend::GetWindowClassName() const
{
	return _T("DioFriendGuiFoundation");
}

void CEasyQQAddFriend::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void CEasyQQAddFriend::InitWindow()
{
}

LRESULT CEasyQQAddFriend::ResponseDefaultKeyEvent(WPARAM wParam)
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

CDuiString CEasyQQAddFriend::GetSkinFile()
{
	return _T("addfriends.xml");
}

CDuiString CEasyQQAddFriend::GetSkinFolder()
{
	return (CPaintManagerUI::GetInstancePath());
}

UILIB_RESOURCETYPE CEasyQQAddFriend::GetResourceType() const
{
	return UILIB_FILE;
}

CControlUI * CEasyQQAddFriend::CreateControl(LPCTSTR pstrClass)
{
	return nullptr;
}

LRESULT CEasyQQAddFriend::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return 0;
}

LRESULT CEasyQQAddFriend::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	return LRESULT();
}

LRESULT CEasyQQAddFriend::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	bHandled = FALSE;
	return 0;
}

void CEasyQQAddFriend::Notify(TNotifyUI & msg)
{
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
		if (_tcsicmp(msg.pSender->GetName(), kSendMsgControlName) == 0)
		{
			CEditUI* pEditUI = static_cast<CEditUI*>(WindowImplBase::m_PaintManager.FindControl(kAccountEditControlName));
			if (pEditUI!=NULL)
			{
				//g_qqSocket.send()
				LoginData info;
				info.type = EASYQQACCOUNTINFO;
				wcscpy_s(info.user_account, m_clientInfo.user_account);
				LPCTSTR s = pEditUI->GetText().GetData();
				wcscpy_s(info.Addfriend_account, pEditUI->GetText().GetData());
				g_qqSocket.send(&info, sizeof(LoginData));
				/*g_qqSocket.receive(&recvinfo, sizeof(LoginData));
				if (recvinfo.loginstate.Islogin!= LOGIN_SUCCESS)
				{
					MessageBox(NULL, TEXT("找不到用户或还未上线"), NULL, MB_ICONINFORMATION);
					return;
				}
				if (recvinfo.loginstate.Islogin==LOGIN_SUCCESS)
				{
					MessageBox(NULL, TEXT("已发送给用户"), NULL, MB_ICONINFORMATION);
					return;
				}*/
			}
			
		}
	}
}

void CEasyQQAddFriend::OnPrepare(TNotifyUI & msg)
{
}

void CEasyQQAddFriend::OnExit(TNotifyUI & msg)
{
	Close();
}

void CEasyQQAddFriend::OnTimer(TNotifyUI & msg)
{
}

