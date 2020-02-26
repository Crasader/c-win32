#pragma once

#include "Resource.h"
#include "stdafx.h"
#include "D:\po\project\EasyQQDemo\DuiLib\Core\UIBase.h"
#include "EasyQQRegister.h"
#include "EasyQQData.h"
#include "EasyQQSocket.h"
#include "EasyQQMain.h"
using namespace DuiLib;

extern CEasyQQClient g_qqSocket;


class CEasyQQLogin :
	public CWindowWnd,public INotifyUI
{
public:
	CEasyQQLogin();
	~CEasyQQLogin();
	void Init()
	{
		m_pBtnClose = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("BtnExit")));
		m_pEditAccount = static_cast<CEditUI*> (m_PaintManager.FindControl(_T("AccountNum")));
		m_pEditCode= static_cast<CEditUI*> (m_PaintManager.FindControl(_T("Accountcode")));
		m_pBtnSign = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("Sign")));
		m_pLabCreateAccount = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("AccountCreate")));
	}
	virtual LPCTSTR GetWindowClassName() const { return _T("DUIMainFrame"); }

	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
		::ScreenToClient(*this, &pt);

		RECT rcClient;
		::GetClientRect(*this, &rcClient);

		if (!::IsZoomed(*this))
		{
			RECT rcSizeBox = m_PaintManager.GetSizeBox();
			if (pt.y < rcClient.top + rcSizeBox.top)
			{
				if (pt.x < rcClient.left + rcSizeBox.left) return HTTOPLEFT;
				if (pt.x > rcClient.right - rcSizeBox.right) return HTTOPRIGHT;
				return HTTOP;
			}
			else if (pt.y > rcClient.bottom - rcSizeBox.bottom)
			{
				if (pt.x < rcClient.left + rcSizeBox.left) return HTBOTTOMLEFT;
				if (pt.x > rcClient.right - rcSizeBox.right) return HTBOTTOMRIGHT;
				return HTBOTTOM;
			}

			if (pt.x < rcClient.left + rcSizeBox.left) return HTLEFT;
			if (pt.x > rcClient.right - rcSizeBox.right) return HTRIGHT;
		}

		RECT rcCaption = m_PaintManager.GetCaptionRect();
		if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
			&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom) {
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(pt));
			if (pControl && _tcsicmp(pControl->GetClass(), DUI_CTR_BUTTON) != 0 &&
				_tcsicmp(pControl->GetClass(), DUI_CTR_OPTION) != 0 &&
				_tcsicmp(pControl->GetClass(), DUI_CTR_TEXT) != 0)
				return HTCAPTION;
		}

		return HTCLIENT;
	}


	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~WS_CAPTION;
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		m_PaintManager.Init(m_hWnd);
		CDialogBuilder builder;
		CControlUI* pRoot = builder.Create(_T("EasyQQSkin.xml"), (UINT)0, NULL, &m_PaintManager);
		ASSERT(pRoot&&"Failed to parse XML");
		if (NULL==pRoot)
		{
			MessageBox(NULL, TEXT("Cant not Find the skin!"), NULL, MB_ICONHAND);
			return 0;
		}
		m_PaintManager.AttachDialog(pRoot);
		m_PaintManager.AddNotifier(this);
		Init();
		return 0;
	}
	virtual void    Notify(TNotifyUI& msg)
	{
		if (msg.sType == _T("windowinit"))
			//OnPrepare();
			;
		else if (msg.sType==_T("click"))
		{
			if (msg.pSender==m_pBtnClose)
			{
				PostQuitMessage(0);
				return;
			}
			if (msg.pSender==m_pBtnSign)
			{
				if (m_pEditAccount->GetText().IsEmpty()||m_pEditCode->GetText().IsEmpty())
				{
					MessageBox(NULL, TEXT("请输入账号密码"), NULL, MB_ICONHAND);
					return;
				}
				LoginData* logindata=new LoginData;
				wcscpy_s(logindata->user_account, m_pEditAccount->GetText().GetData());
				wcscpy_s(logindata->user_code , m_pEditCode->GetText().GetData());
				logindata->type = EASYQQLOGIN;
				g_qqSocket.send(logindata, sizeof(LoginData));
				ISLOGIN info;
				g_qqSocket.receive(&info, sizeof(ISLOGIN));
				switch (info.Islogin)
				{
				case LOGIN_SUCCESS:
					{
						CEasyQQMain* pFrame = new CEasyQQMain(info);
						pFrame->Create(NULL, _T("DIO2017"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
						pFrame->CenterWindow();
						pFrame->ShowWindow(true);
					}
					break;
				case LOGIN_ERR_HASLOGIN:
					{
						MessageBox(NULL, TEXT("已经登录"), NULL, MB_ICONHAND);
						return;
					}
					break;
				case LOGIN_ERR_WRONGPASSWORD:
					{
						MessageBox(NULL, TEXT("密码错误"), NULL, MB_ICONHAND);
						return;
					}
					break;
				case LOGIN_NOTLOGIN:
					{
						MessageBox(NULL, TEXT("没找到账号"), NULL, MB_ICONHAND);
						return;
					}
					break;
				default:
					break;
				}
				delete logindata;
				Close(IDCLOSE);
			}
			if (msg.pSender==m_pLabCreateAccount)
			{
				CEasyQQRegister* duiFrame;
				duiFrame = new CEasyQQRegister();
				duiFrame->Create(m_hWnd, _T("dui"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
				duiFrame->CenterWindow();
				//duiFrame->ShowWindow(true);
				duiFrame->ShowModal();
			}
			
		}
	
	}

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;
		if (uMsg == WM_CREATE)
		{
			OnCreate(uMsg, wParam, lParam);
			return lRes;
		}
		// 以下3个消息WM_NCACTIVATE、WM_NCCALCSIZE、WM_NCPAINT用于屏蔽系统标题栏
		else if (uMsg == WM_NCACTIVATE)
		{
			if (!::IsIconic(m_hWnd))
			{
				return (wParam == 0) ? TRUE : FALSE;
			}
		}
		else if (uMsg == WM_NCCALCSIZE)
		{
			return 0;
		}
		else if (uMsg == WM_NCPAINT)
		{
			return 0;
		}
		else if (uMsg== WM_NCHITTEST)
		{
			lRes= OnNcHitTest(uMsg, wParam, lParam, bHandled); 
			return lRes;
		}

		if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes))
		{
			return lRes;
		}

		return __super::HandleMessage(uMsg, wParam, lParam);
	}
public:
	CPaintManagerUI m_PaintManager;

private:
	CButtonUI* m_pBtnClose;
	//CButtonUI* m_pBtnMin;
	//CButtonUI* m_pBtnLogin;
	CEditUI* m_pEditAccount;
	CEditUI* m_pEditCode;
	CButtonUI* m_pBtnSign;
	CButtonUI* m_pLabCreateAccount;
private:


};

