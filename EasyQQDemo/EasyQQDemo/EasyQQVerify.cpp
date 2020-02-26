#include "stdafx.h"
#include "EasyQQVerify.h"

const TCHAR* const kCloseButtonControlName = _T("closebtn");

const TCHAR* const kAcceptButtonControlName = _T("AcceptBtn");

const TCHAR* const kRejectButtonControlName = _T("RejectBtn");

const TCHAR* const kUserNameTextName = _T("username");

const TCHAR* const kDescriptionControlName = _T("description");

CEasyQQVerify::CEasyQQVerify(wchar_t* username,ISLOGIN& clientInfo):m_username(username),m_clientInfo(clientInfo)
{
}


CEasyQQVerify::~CEasyQQVerify()
{

}

LPCTSTR CEasyQQVerify::GetWindowClassName() const
{
	return _T("DioGuiAddFriendUI");
}

void CEasyQQVerify::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void CEasyQQVerify::InitWindow()
{
	m_pTextUserName = static_cast<CTextUI*>(m_PaintManager.FindControl(kUserNameTextName));
	CTextUI* pEditUI = static_cast<CTextUI*>(WindowImplBase::m_PaintManager.FindControl(kDescriptionControlName));
	m_pTextUserName->SetFont(pEditUI->GetFont());
	m_pTextUserName->SetTextColor(0xFFFFFFFF);
	m_pTextUserName->SetText(m_username.c_str());
}

LRESULT CEasyQQVerify::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam==VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

CDuiString CEasyQQVerify::GetSkinFile()
{
	return _T("EasyQQVerifyFriend.xml");
}

CDuiString CEasyQQVerify::GetSkinFolder()
{
	return (CPaintManagerUI::GetInstancePath());
}

UILIB_RESOURCETYPE CEasyQQVerify::GetResourceType() const
{
	return UILIB_FILE;
}

CControlUI * CEasyQQVerify::CreateControl(LPCTSTR pstrClass)
{
	return nullptr;
}

LRESULT CEasyQQVerify::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return 0;
}

LRESULT CEasyQQVerify::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	return LRESULT();
}

LPCTSTR CEasyQQVerify::GetResourceID() const
{
	return MAKEINTRESOURCE(102);
}

LRESULT CEasyQQVerify::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	bHandled = FALSE;
	return 0;
}

void CEasyQQVerify::Notify(TNotifyUI & msg)
{
	if (_tcsicmp(msg.sType,_T("windowinit"))==0)
	{
		OnPrepare(msg);
	}
	else if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(),kCloseButtonControlName)==0)
		{
			OnExit(msg);
		}
		if (_tcsicmp(msg.pSender->GetName(), kAcceptButtonControlName) == 0)
		{
			//如果确认加为好友，发送一个ISLOGIN给服务端，原因如下
			//1、使得自己数据被更新2、使得对方也添加到朋友
			LoginData info;
			wcscpy_s(info.Addfriend_account, m_username.c_str());
			for (int i=0;i<sizeof(g_clientInfo.user_friendlist)/sizeof(g_clientInfo.user_friendlist[0]);i++)
			{
				if (wcslen(g_clientInfo.user_friendlist[i]) == 0)
				{
					wcscpy_s(g_clientInfo.user_friendlist[i], m_username.c_str());
					break;
				}
			}
			info.type = EASYQQADDFRIENDS;
			info.loginstate = g_clientInfo;
			g_qqSocket.send(&info, sizeof(LoginData));
			OnExit(msg);
			//info.
			//g_qqSocket.send()
		}
		if (_tcsicmp(msg.pSender->GetName(), kRejectButtonControlName) == 0)
		{
			OnExit(msg);
		}
	}
}

void CEasyQQVerify::OnPrepare(TNotifyUI & msg)
{

}

void CEasyQQVerify::OnExit(TNotifyUI & msg)
{
	Close();
}

void CEasyQQVerify::OnTimer(TNotifyUI & msg)
{
}
