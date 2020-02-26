#include "MainWnd.h"

#include "include\cef_browser.h"

CMainWnd::CMainWnd(void): m_handler(new SimpleHandler)
{
}


CMainWnd::~CMainWnd(void)
{
}

void CMainWnd::InitWindow()
{
	m_pBtnClose = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"btn_close"));
	m_pURL = static_cast<CEditUI*>(m_PaintManager.FindControl(L"edit_url"));
	m_pBtnGo = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"btn_go"));
	m_pBtnBack = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"btn_back"));
	m_pBtnCallJs = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"btn_calljs"));

	RECT rt;
	::GetClientRect(m_hWnd, &rt);
	rt.top += 100;
	rt.bottom -= 3;
	rt.left += 3;
	rt.right -= 3;

	CefWindowInfo info;
	info.SetAsChild(m_hWnd, rt);

	CefBrowserSettings settings;
	CefBrowserHost::CreateBrowser(info, m_handler.get(), L"http://www.baidu.com", settings, NULL);
}

void CMainWnd::OnFinalMessage( HWND hWnd )
{
	while(m_handler->GetBrowser())
	{
		Sleep(10);
	}
	::PostQuitMessage(0);
}

void CMainWnd::Notify(TNotifyUI& msg)
{
	if(msg.sType == DUI_MSGTYPE_CLICK)
	{
		if(msg.pSender == m_pBtnClose)
		{
			//
			Close();
		}
		else if(msg.pSender == m_pBtnCallJs)
		{
			CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("callback_func");
			CefRefPtr<CefListValue> args = msg->GetArgumentList();
			args->SetString(0, "call js call back function");
			args->SetInt(1, 10);
			m_handler->GetBrowser()->SendProcessMessage(PID_RENDERER, msg);
		}
		else if(msg.pSender == m_pBtnGo)
		{
			if(m_pURL->GetText().Compare(L"") == 0)
			{
				m_handler->GetBrowser()->GetMainFrame()->ExecuteJavaScript(L"alert(\"url is empty\");", m_handler->GetBrowser()->GetMainFrame()->GetURL(), 0);
				return ;
			}
			std::wstring strUrl = m_pURL->GetText();
			if(!m_pURL->GetText().IsEmpty() /*&& m_pURL->GetText().Compare(L"") != 0*/)
				m_handler->GetBrowser()->GetMainFrame()->LoadURL(strUrl);
		}
		else if(msg.pSender == m_pBtnBack)
		{
			//m_handler->GetBrowser()->GoBack();
			CallJsFunc(L"GoBack()");
		}
	}
}

LRESULT CMainWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(uMsg == WM_SIZE)
	{
		HWND hwnd = ::FindWindowEx(m_hWnd, NULL, L"CefBrowserWindow", NULL);
		::MoveWindow(hwnd, 3, 100, LOWORD(lParam)-6, HIWORD(lParam)-103, TRUE);
	}
	else if(uMsg == WM_DROPFILES)
	{
		WCHAR wcFile[MAX_PATH] = {0};
		UINT count = DragQueryFile((HDROP)wParam, 0xFFFFFFFF, NULL, 0);
		if(count)
		{
			DragQueryFile((HDROP)wParam, 0, wcFile, MAX_PATH);
			wstring str = wcFile;
			m_handler->GetBrowser()->GetMainFrame()->LoadURL(str);
			m_pURL->SetText(wcFile);
		}
	}
	else if(uMsg == WM_COPYDATA)
	{
		COPYDATASTRUCT* pData = (COPYDATASTRUCT*)lParam;
		::MessageBox(NULL, (LPCWSTR)pData->lpData, L"js func", MB_OK);
	}
	return 0;
}

void CMainWnd::CallJsFunc(LPCWSTR strJSFuncName)
{
	if(strJSFuncName[0] == L'\0' || strJSFuncName == NULL)
		return;

	m_handler->GetBrowser()->GetMainFrame()->ExecuteJavaScript(strJSFuncName, m_handler->GetBrowser()->GetMainFrame()->GetURL(), 0);
}

void CMainWnd::CallJsCBFunc(LPCWSTR str)
{
	
}
