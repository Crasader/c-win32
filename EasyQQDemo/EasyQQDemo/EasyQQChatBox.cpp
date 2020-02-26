#include "stdafx.h"

#include "EasyQQChatBox.h"

const TCHAR* const kNickNameTextControlName = _T("nicknametext");

const TCHAR* const kViewRichEditControlName = _T("view_richedit");

const TCHAR* const kInputRichEditControlName = _T("input_richedit");

const TCHAR* const kAccountLogoControlName = _T("accountLogo");

const TCHAR* const kCloseButtonControlName = _T("closebtn");

CEasyQQChatBox::CEasyQQChatBox(ISLOGIN login, const FriendListItemInfo& friend_info):m_clientInfo(login),m_friends(friend_info)
{
}


CEasyQQChatBox::~CEasyQQChatBox()
{

}

LPCTSTR CEasyQQChatBox::GetWindowClassName() const
{
	return _T("DioGuiChatBoxUI");
}

void CEasyQQChatBox::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void CEasyQQChatBox::InitWindow()
{
	CTextUI* pTextUI = static_cast<CTextUI*> (m_PaintManager.FindControl(kNickNameTextControlName));
	DWORD Nicknamefont = pTextUI->GetFont();
	pTextUI->SetText(m_friends.nick_name);
	pTextUI->SetFont(Nicknamefont);
	pTextUI->SetTextColor(0xFFFFFFF);
}

LRESULT CEasyQQChatBox::ResponseDefaultKeyEvent(WPARAM wParam)
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

CDuiString CEasyQQChatBox::GetSkinFile()
{
	return _T("EasyQQchatbox.xml");
}

CDuiString CEasyQQChatBox::GetSkinFolder()
{
	return (CPaintManagerUI::GetInstancePath());
}

UILIB_RESOURCETYPE CEasyQQChatBox::GetResourceType() const
{
	return UILIB_FILE;
}

LRESULT CEasyQQChatBox::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return 0;
}

LRESULT CEasyQQChatBox::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	return LRESULT();
}

LRESULT CEasyQQChatBox::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CEasyQQChatBox::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg, wParam, lParam);
}


void CEasyQQChatBox::SendEditMsg()
{
	CRichEditUI* pRichEdit = static_cast<CRichEditUI*> (m_PaintManager.FindControl(kInputRichEditControlName));
	if (pRichEdit == NULL)
		return;
	pRichEdit->SetFocus();
	CDuiString sText = pRichEdit->GetTextRange(0, pRichEdit->GetTextLength());
	if (sText.IsEmpty())
		return;
	pRichEdit->SetText(_T(""));

	LoginData senddata;
	senddata.type = EASYQQCHAT;
	senddata.loginstate = m_clientInfo;
	wcscpy_s(senddata.user_account, m_clientInfo.user_account);
	wcscpy_s(senddata.Message, sText.GetData());
	wcscpy_s(senddata.Addfriend_account, m_friends.nick_name);
	g_qqSocket.send(&senddata, sizeof(senddata));

	pRichEdit = static_cast<CRichEditUI*> (m_PaintManager.FindControl(kViewRichEditControlName));
	if (pRichEdit == NULL)
		return;
	long lSelBegin = 0, lSelEnd = 0;
	CHARFORMAT2 cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT2));
	cf.cbSize = sizeof(cf);
	cf.dwReserved = 0;
	cf.dwMask = CFM_COLOR | CFM_LINK | CFM_UNDERLINE | CFM_UNDERLINETYPE;
	cf.dwEffects = CFE_LINK;
	cf.bUnderlineType = CFU_UNDERLINE;
	cf.crTextColor = RGB(255, 255, 255);

	lSelEnd = lSelBegin = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelEnd, lSelEnd);
	pRichEdit->ReplaceSel(m_clientInfo.user_account, false);

	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);

	lSelEnd = lSelBegin = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelEnd, lSelEnd);
	pRichEdit->ReplaceSel(_T("˵:\t2011-07-30 08:30:12\n"), false);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = RGB(255, 255, 255);
	cf.dwEffects = 0;
	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);

	PARAFORMAT2 pf;
	ZeroMemory(&pf, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = 0;
	pRichEdit->SetParaFormat(pf);

	lSelEnd = lSelBegin = pRichEdit->GetTextLength();

	pRichEdit->SetSel(-1, -1);
	pRichEdit->ReplaceSel(sText.GetData(), false);
	

	pRichEdit->SetSel(-1, -1);
	pRichEdit->ReplaceSel(_T("\n\n"), false);

	cf.crTextColor = RGB(255, 255, 255);
	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);

	ZeroMemory(&pf, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = 220;
	pRichEdit->SetParaFormat(pf);

	pRichEdit->EndDown();
}

void CEasyQQChatBox::SendMsg(const LoginData info)
{
	CRichEditUI* pRichEdit = static_cast<CRichEditUI*> (m_PaintManager.FindControl(kViewRichEditControlName));
	if (pRichEdit == NULL)
		return;
	long lSelBegin = 0, lSelEnd = 0;
	CHARFORMAT2 cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT2));
	cf.cbSize = sizeof(cf);
	cf.dwReserved = 0;
	cf.dwMask = CFM_COLOR | CFM_LINK | CFM_UNDERLINE | CFM_UNDERLINETYPE;
	cf.dwEffects = CFE_LINK;
	cf.bUnderlineColor = CFU_UNDERLINE;
	cf.crTextColor = RGB(220, 0, 0);

	lSelEnd = lSelBegin = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelEnd, lSelEnd);
	pRichEdit->ReplaceSel(info.Addfriend_account, false);

	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);

	lSelEnd = lSelBegin = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelEnd, lSelEnd);
	pRichEdit->ReplaceSel(_T("˵:\t2011-07-30 08:30:12\n"), false);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = RGB(255,255,255);
	cf.dwEffects = 0;
	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);

	PARAFORMAT2 pf;
	ZeroMemory(&pf, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = 0;
	pRichEdit->SetParaFormat(pf);

	lSelEnd = lSelBegin = pRichEdit->GetTextLength();

	pRichEdit->SetSel(-1, -1);
	pRichEdit->ReplaceSel(info.Message, false);

	pRichEdit->SetSel(-1, -1);
	pRichEdit->ReplaceSel(_T("\n"), false);

	cf.crTextColor = RGB(255,255,255);
	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);

	ZeroMemory(&pf, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = 220;
	pRichEdit->SetParaFormat(pf);

	pRichEdit->EndDown();
}

void CEasyQQChatBox::Notify(TNotifyUI & msg)
{
	if (_tcsicmp(msg.sType,_T("windowinit"))==0)
	{
		OnPrepare(msg);
	}
	else if(_tcsicmp(msg.sType,_T("click"))==0)
	{
		if (_tcsicmp(msg.pSender->GetName(),kCloseButtonControlName)==0)
		{
			OnExit(msg);
		}
	}
	else if (_tcsicmp(msg.sType, _T("return")) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(),kInputRichEditControlName)==0)
		{
			SendEditMsg();
		}
	}
	else if (_tcsicmp(msg.sType,_T("timer"))==0)
	{
		return OnTimer(msg);
	}

}

void CEasyQQChatBox::OnPrepare(TNotifyUI & msg)
{
	hRecvThread = CreateThread(NULL, 0, RecvMsg, this, 0, NULL);
}

void CEasyQQChatBox::OnExit(TNotifyUI & msg)
{
	CloseHandle(hRecvThread);
	Close();
}

void CEasyQQChatBox::OnTimer(TNotifyUI & msg)
{
}

DWORD CEasyQQChatBox::RecvMsg(LPVOID lpParm)
{
	CEasyQQChatBox* pThis = (CEasyQQChatBox*)lpParm;
	while (true)
	{
		static LoginData info;
		std::vector<LoginData>::iterator iter;
		g_qqSocket.WaitForRecvMessage(info,iter);
		if (info.loginstate.Islogin==LOGIN_SUCCESS)
		{
			if (wcscmp(info.Addfriend_account,pThis->m_friends.nick_name)==0)
			{
				if (info.type == EASYQQCHAT)
				{
					pThis->SendMsg(info);
					g_qqSocket.EraseFirstRecvMsg(iter);
				}
			}

		}
	}
	return 0;
}
