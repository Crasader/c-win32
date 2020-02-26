#pragma once
#include "Resource.h"
#include "stdafx.h"
#include "D:\po\project\EasyQQDemo\DuiLib\Core\UIBase.h"
using namespace DuiLib;

class CEasyQQRegister:
	public CWindowWnd, public INotifyUI
{
public:
	CEasyQQRegister();
	 ~CEasyQQRegister();
	void Init()
	{
		m_pBtnAffirm = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("Affirm")));
		m_pEditAccountBox = static_cast<CEditUI*> (m_PaintManager.FindControl(_T("AccountBox")));
		m_pEditAffirmCodeBox = static_cast<CEditUI*> (m_PaintManager.FindControl(_T("CodeBox")));
		m_pBtnCancel = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("Cancel")));
		m_pEditCodeBox= static_cast<CEditUI*>(m_PaintManager.FindControl(_T("AffirmBox")));
	}
	 LPCTSTR GetWindowClassName() const { return _T("UIRegisterFrame"); }

	void OnFinalMessage(HWND /*hWnd*/)
	{
		//m_PaintManager.RemovePreMessageFilter(this);
		delete this;
	};

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~WS_CAPTION;
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		m_PaintManager.Init(m_hWnd);
		//m_PaintManager.AddPreMessageFilter(this);
		CDialogBuilder builder;
		CControlUI* pRoot = builder.Create(_T("EasyQQRegisterSkin.xml"), (UINT)0, NULL, &m_PaintManager);
		ASSERT(pRoot&&"Failed to parse XML");
		if (NULL == pRoot)
		{
			MessageBox(NULL, TEXT("Cant not Find the skin!"), NULL, MB_ICONHAND);
			return 0;
		}
		m_PaintManager.AttachDialog(pRoot);
		m_PaintManager.AddNotifier(this);
		Init();
		return 0;
	}
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
		::ScreenToClient(*this, &pt);

		RECT rcClient;
		::GetClientRect(*this, &rcClient);

		RECT rcCaption = m_PaintManager.GetCaptionRect();
		if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
			&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom) {
			CControlUI* pControl = static_cast<CControlUI*>( m_PaintManager.FindControl(pt));
			if (pControl && _tcscmp(pControl->GetClass(), DUI_CTR_BUTTON) != 0)
				return HTCAPTION;
		}

		return HTCLIENT;
	}

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		SIZE szRoundCorner = m_PaintManager.GetRoundCorner();
		if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) {
			CDuiRect rcWnd;
			::GetWindowRect(*this, &rcWnd);
			rcWnd.Offset(-rcWnd.left, -rcWnd.top);
			rcWnd.right++; rcWnd.bottom++;
			HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
			::SetWindowRgn(*this, hRgn, TRUE);
			::DeleteObject(hRgn);
		}

		bHandled = FALSE;
		return 0;
	}

	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (::IsIconic(*this)) bHandled = FALSE;
		return (wParam == 0) ? TRUE : FALSE;
	}

	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	 void    Notify(TNotifyUI& msg)
	{
		if (msg.sType == _T("windowinit"))
			//OnPrepare();
			;
		else if (msg.sType == _T("click"))
		{
			if (msg.pSender == m_pBtnCancel)
			{
				Close(IDCLOSE);
				return;
			}
			if (msg.pSender == m_pBtnAffirm)
			{
				if (m_pEditAccountBox->GetText().IsEmpty() || m_pEditCodeBox->GetText().IsEmpty()|| m_pEditCodeBox->GetText().IsEmpty())
				{
					MessageBox(NULL, TEXT("请输入账号密码"), NULL, MB_ICONHAND);
					return;
				}
				if (m_pEditCodeBox->GetText().Compare(m_pEditAffirmCodeBox->GetText().GetData()))
				{
					MessageBox(NULL, TEXT("请输入相同的密码"), NULL, MB_ICONHAND);
					return;
				}
				else
				{
					LoginData* data=new LoginData;
					data->type = EASYQQREGISTER;
					wcscpy_s(data->user_account, m_pEditAccountBox->GetText().GetData());
					wcscpy_s(data->user_code, m_pEditCodeBox->GetText().GetData());
					g_qqSocket.send(data, sizeof(LoginData));
					delete data;
				}
				
				Close(IDCLOSE);
				return;
			}
		}


	}

	 LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;
		switch (uMsg) {
		case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam); break;
		case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
		case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
		case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
		case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
		case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
		default:
			bHandled = FALSE;
		}




		//if (uMsg == WM_CREATE)
		//{
		//	OnCreate(uMsg, wParam, lParam);
		//	return lRes;
		//}
		//// 以下3个消息WM_NCACTIVATE、WM_NCCALCSIZE、WM_NCPAINT用于屏蔽系统标题栏
		//else if (uMsg == WM_NCACTIVATE)
		//{
		//	if (!::IsIconic(m_hWnd))
		//	{
		//		return (wParam == 0) ? TRUE : FALSE;
		//	}
		//}
		//else if (uMsg == WM_NCCALCSIZE)
		//{
		//	return 0;
		//}
		//else if (uMsg == WM_NCPAINT)
		//{
		//	return 0;
		//}

		if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes))
		{
			return lRes;
		}

		return __super::HandleMessage(uMsg, wParam, lParam);
	}

	 	
public:
	CPaintManagerUI m_PaintManager;

private:
	CButtonUI* m_pBtnAffirm;
	CButtonUI* m_pBtnCancel;
	//CButtonUI* m_pBtnMin;
	//CButtonUI* m_pBtnLogin;
	CEditUI* m_pEditAccountBox;
	CEditUI* m_pEditCodeBox;
	CEditUI* m_pEditAffirmCodeBox;
};

