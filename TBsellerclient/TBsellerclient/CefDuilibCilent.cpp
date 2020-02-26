// App.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "CefDuilibCilent.h"
#include "WebClient.h"
#include "client_app_renderer.h"
#define OFFSETX 140 //Xƫ��ֵ
#define OFFSETY 110 //����ʱYƫ��ֵ
#define OFFSETY_1 20 //����ʱYƫ��ֵ

using namespace TBClient;

CCefDuiLibTest::CCefDuiLibTest()
	: m_pTabLayoutUI(NULL)
	, m_pTabSwitchHor(NULL)
	, m_BackBtn(NULL)
	, m_ForwardBtn(NULL)
	, m_indexSwitch(0)
{

}
CCefDuiLibTest::~CCefDuiLibTest()
{
	m_cWebClient.clear();
	m_UEdit.clear();
}
void CCefDuiLibTest::GoNewUrl(std::wstring Url)
{
	CDuiString strUrl;
	strUrl = Url.c_str();
	if (strUrl.Compare(_T("")) == 0)
		return;
	// ��ȡ��ǰ��ʾ��tab
	int index = m_pTabLayoutUI->GetCurSel();
	CHorizontalLayoutUI* pHor = (CHorizontalLayoutUI*)m_pTabLayoutUI->GetItemAt(index);
	if (pHor)
	{
		const CefString url(strUrl.GetData());
		m_cWebClient.at(index)->GetBrowser()->GetMainFrame()->LoadURL(url);
		m_UEdit.at(index) = url.c_str();
		// ���ı�ǩ����
		int iCount = m_pTabSwitchHor->GetCount();
		for (int i = 0; i < iCount; i++)
		{
			COptionUI* pOpt = (COptionUI*)m_pTabSwitchHor->GetItemAt(i);
			CHorizontalLayoutUI* pHaveHor = (CHorizontalLayoutUI*)pOpt->GetTag();
			if (pHaveHor == pHor)
			{
				pOpt->SetToolTip(m_cWebClient.at(index)->m_TitleStr.c_str());
				return;
			}
		}
		MaxMinCefWindow(OFFSETX, OFFSETY);
	}
}
//��ҳ��ת
void CCefDuiLibTest::GoNewUrl()
{
	// ��ȡ�������ַ
	CDuiString strUrl;
	CEditUI* pEdit = (CEditUI*)m_PaintManager.FindControl(_T("url"));
	if (pEdit)
	{
		strUrl = pEdit->GetText();

		// �����ַΪ���򷵻�
		if (strUrl.Compare(_T("")) == 0)
			return;
	}

	// ��ȡ��ǰ��ʾ��tab
	int index = m_pTabLayoutUI->GetCurSel();
	CHorizontalLayoutUI* pHor = (CHorizontalLayoutUI*)m_pTabLayoutUI->GetItemAt(index);
	if (pHor)
	{
		const CefString url(strUrl.GetData());
		m_cWebClient.at(index)->GetBrowser()->GetMainFrame()->LoadURL(url);
		m_UEdit.at(index) = url.c_str();
		// ���ı�ǩ����
		int iCount = m_pTabSwitchHor->GetCount();
		for (int i = 0; i < iCount; i++)
		{
			COptionUI* pOpt = (COptionUI*)m_pTabSwitchHor->GetItemAt(i);
			CHorizontalLayoutUI* pHaveHor = (CHorizontalLayoutUI*)pOpt->GetTag();
			if (pHaveHor == pHor)
			{
				pOpt->SetToolTip(m_cWebClient.at(index)->m_TitleStr.c_str());
				return;
			}
		}
		MaxMinCefWindow(OFFSETX, OFFSETY);
	}
}
//ˢ�µ�ǰҳ��
void CCefDuiLibTest::RefreshUrl()
{
	int index = m_pTabLayoutUI->GetCurSel();
	m_cWebClient.at(index)->GetBrowser()->Reload();
}
//����ҳǩ
void CCefDuiLibTest::NewPage(std::wstring Url, bool Isrender)
{
	CDuiString teststr;
	teststr = Url.c_str();
	m_UEdit.push_back(teststr);

	CefRefPtr<CWebClient>client(new CWebClient());
	m_cWebClient.push_back(client);
	client->m_hWnd = m_hWnd;
	if (!Isrender)
		client->TurnRenderMode();
	CefWindowInfo info;
	RECT rect;
	GetClientRect(this->GetHWND(), &rect);
	RECT rectnew = rect;
	rectnew.top = rect.top + OFFSETX;
	rectnew.bottom = rect.bottom - OFFSETY_1;
	rectnew.left = rect.left;
	rectnew.right = rect.right;
	info.SetAsChild(m_hWnd, rectnew);
	CefBrowserSettings browserSettings;
	CefBrowserHost::CreateBrowser(info, static_cast<CefRefPtr<CefClient> >(client), Url.c_str(), browserSettings, NULL);
	for (UINT i = 0; i<m_cWebClient.size(); i++)
	{
		if (m_cWebClient.at(i)->GetBrowser() && !m_cWebClient.at(i)->GetBrowser()->GetMainFrame()->GetURL().empty())
		{
			CefRefPtr<CefBrowserHost> t_host = m_cWebClient.at(i)->GetBrowser()->GetHost();
			CefWindowHandle hwnd = t_host->GetWindowHandle();
			if (i<m_cWebClient.size() - 1)
			{
				MoveWindow(hwnd, 0, 0, 0, 0, false);
			}
			else
			{
				MoveWindow(hwnd, rectnew.left, rectnew.top, rectnew.right-rectnew.left, rectnew.bottom -rectnew.top, true);
			}
		}
	}
	CEditUI* pEdit = (CEditUI*)m_PaintManager.FindControl(_T("url"));
	if (pEdit)
	{
		pEdit->SetText(Url.c_str());
	}
}
//�������ڴ�С
void CCefDuiLibTest::MaxMinCefWindow(int offsetx, int offsety)
{
	RECT rect;
	GetClientRect(this->GetHWND(), &rect);
	RECT rectnew = rect;
	rectnew.top = rect.top + offsetx; //95
	rectnew.bottom = rect.bottom - offsety; //110
	rectnew.left = rect.left;
	rectnew.right = rect.right;
	int t_index = m_pTabLayoutUI->GetCurSel();
	for (UINT i = 0; i<m_cWebClient.size(); i++)
	{
		CefWindowHandle hwnd = m_cWebClient.at(i)->GetBrowser()->GetHost()->GetWindowHandle();
		if (i != t_index)
		{
			MoveWindow(hwnd, 0, 0, 0, 0, false);
		}
		else
		{
			MoveWindow(hwnd, rectnew.left, rectnew.top, rectnew.right - rectnew.left, rectnew.bottom - rectnew.top, true);
		}
	}
}
//����ı�
void CCefDuiLibTest::TitleChage(const CefString &str)
{

	int index = m_pTabLayoutUI->GetCurSel();
	CHorizontalLayoutUI* pHor = (CHorizontalLayoutUI*)m_pTabLayoutUI->GetItemAt(index);
	if (pHor)
	{
		int iCount = m_pTabSwitchHor->GetCount();
		for (int i = 0; i < iCount; i++)
		{
			COptionUI* pOpt = (COptionUI*)m_pTabSwitchHor->GetItemAt(i);
			CHorizontalLayoutUI* pHaveHor = (CHorizontalLayoutUI*)pOpt->GetTag();
			if (pHaveHor == pHor)
			{
				if (str.length()>8)
				{
					wchar_t strtilte[100] = { NULL };
					wcsncpy(strtilte, str.c_str(), 6);
					wcsncat(strtilte, _T("..."), 3);
					pOpt->SetText(strtilte);
				}
				else
				{
					pOpt->SetText(str.c_str());
				}
				pOpt->SetToolTip(str.c_str());
				break;
			}
		}
	}
}
//ɾ��ҳǩ
void CCefDuiLibTest::TableDel()
{
	int t_Index = 0;
	vector<CefRefPtr<CWebClient>>::iterator cWebClientit = m_cWebClient.begin();
	if (m_UEdit.size()>1)
	{
		int t_moveIndex = m_indexSwitch;
		if (m_indexSwitch>0)t_moveIndex = m_indexSwitch - 1;
		COptionUI* pOpt = (COptionUI*)m_pTabSwitchHor->GetItemAt(m_indexSwitch);
		CHorizontalLayoutUI* pHaveHor = (CHorizontalLayoutUI*)pOpt->GetTag();
		m_pTabSwitchHor->Remove(pOpt);
		m_pTabLayoutUI->Remove(pHaveHor);

		COptionUI* p = (COptionUI*)m_pTabSwitchHor->GetItemAt(t_moveIndex);
		p->Selected(true);

		CHorizontalLayoutUI* pHaveHor1 = (CHorizontalLayoutUI*)m_pTabLayoutUI->GetItemAt(t_moveIndex);
		m_pTabLayoutUI->SelectItem(pHaveHor1);
		for (vector<CDuiString>::iterator it = m_UEdit.begin(); it != m_UEdit.end();)   //forѭ���в�Ҫit++
		{
			if (t_Index == m_indexSwitch)
			{
				::MoveWindow(m_cWebClient.at(m_indexSwitch)->GetBrowser()->GetHost()->GetWindowHandle(), 0, 0, 0, 0, true);
				m_cWebClient.at(m_indexSwitch)->CloseBrowser(false);
				m_cWebClient.erase(cWebClientit++);
				m_UEdit.erase(it++);
				MaxMinCefWindow(OFFSETX, OFFSETY);
				::CoUninitialize();
				break;
			}
			else
			{
				it++;
				cWebClientit++;
			}
			t_Index++;
		}
	}
	else
	{
		m_UEdit.at(0) = _T("www.baidu.com");
		m_cWebClient.at(0)->GetBrowser()->GetMainFrame()->LoadURL(m_UEdit.at(0).GetData());
		CEditUI* pEdit = (CEditUI*)m_PaintManager.FindControl(_T("url"));
		if (pEdit)
		{
			pEdit->SetText(m_UEdit.at(0));
		}
	}
}
//����
void CCefDuiLibTest::SearchGo()
{
	//CDuiString sText = m_pSearchEdit->GetText();
	//if (!sText.IsEmpty())
	//{
	//	CDuiString sUrl;
	//	sUrl.Format(_T("https://www.baidu.com/s?ie=utf-8&wd=%s"), sText.GetData());
	//	std::wstring T_urL = sUrl.GetData();
	//	// ������ǩOPTION��ť
	//	/*CDuiString sAttr;
	//	COptionUI* pOpt = new COptionUI;
	//	sAttr.Format(_T("name=\"IE_\" width=\"100\" height=\"30\" normalimage=\"tabbar_normal.png\" hotimage=\"tabbar_hover.png\" \
	//	pushedimage=\"tabbar_pushed.png\" selectedimage=\"tabbar_pushed.png\" group=\"demo_list_header\""));
	//	pOpt->ApplyAttributeList(sAttr);

	//	int iCount = m_pTabSwitchHor->GetCount();
	//	m_pTabSwitchHor->AddAt(pOpt, iCount-1);

	//	// ������ǩ����
	//	CHorizontalLayoutUI* pHor = new CHorizontalLayoutUI;

	//	// �Ѳ�����ӽ���ǩ
	//	m_pTabLayoutUI->Add(pHor);
	//	m_pTabLayoutUI->SetTag((UINT_PTR)pHor);

	//	m_pTabLayoutUI->SelectItem(pHor);//ѡ�б�ǩ��ť����

	//	// �ѱ�ǩ��ť�Ͳ��ֹ�������
	//	pOpt->SetTag((UINT_PTR)pHor);

	//	::PostMessageA(this->m_hWnd, WM_DUI_TABTAG, NULL, NULL);

	//	NewPage(T_urL);*/
	//	NewTable(T_urL);
	//}
}
//����
void CCefDuiLibTest::NewTable(std::wstring url, bool Isrender )
{
	CDuiString sAttr;
	COptionUI* pOpt = new COptionUI;
	sAttr.Format(_T("name=\"IE_\" width=\"100\" height=\"30\" normalimage=\"tabbar_normal.png\" hotimage=\"tabbar_hover.png\" \
					pushedimage=\"tabbar_pushed.png\" selectedimage=\"tabbar_pushed.png\" group=\"demo_list_header\""));
	ApplyAttributeList(*pOpt, sAttr);
	//pOpt->ApplyAttributeList(sAttr);
	int iCount = m_pTabSwitchHor->GetCount();
	m_pTabSwitchHor->AddAt(pOpt, iCount - 1);

	// ������ǩ����
	CHorizontalLayoutUI* pHor = new CHorizontalLayoutUI;

	// �Ѳ�����ӽ���ǩ
	m_pTabLayoutUI->Add(pHor);
	m_pTabLayoutUI->SetTag((UINT_PTR)pHor);

	m_pTabLayoutUI->SelectItem(pHor);//ѡ�б�ǩ��ť����

									 // �ѱ�ǩ��ť�Ͳ��ֹ�������
	pOpt->SetTag((UINT_PTR)pHor);


	::PostMessageA(this->m_hWnd, WM_DUI_TABTAG, NULL, NULL);

	NewPage(url,Isrender);
	pOpt->SetText(url.c_str());
}
bool CCefDuiLibTest::GetWorkWeb()
{
	bool ret = false;
	HANDLE hFile;
	hFile = CreateFileW(L"BPwebList.dat", GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFilePos = 0, dwFileSize;
		dwFileSize = GetFileSize(hFile, NULL);
		if (dwFileSize != 0)
		{
			do
			{
				WebInfo* info = new WebInfo;
				DWORD dwRes;
				ReadFile(hFile, info, sizeof(WebInfo), &dwRes, NULL);
				if (wcscmp(info->Confirm,_T("N"))==0)
				{
					m_vecWorkURL.push_back(info->URL);
				}
				else if (wcscmp(info->Confirm,_T("Y"))==0)
				{
					m_vecBanURL.push_back(info->URL);
				}
				
				dwFilePos += sizeof(WebInfo);
			} while (dwFilePos < dwFileSize);
			ret = true;
		}
	}
	CloseHandle(hFile);
	return ret;

}
void CCefDuiLibTest::TestDomWeb(const CefString& url)
{
	
	NewTable(url);
}
//����¼�
void CCefDuiLibTest::OnClick(TNotifyUI& msg)
{
	// �رհ�ť	
	if (msg.pSender->GetName().Find(_T("closebtn")) != -1)
	{
		//CefQuitMessageLoop();
		Close();
	}
	// ��С����ť
	else if (msg.pSender->GetName().Find(_T("minbtn")) != -1) {
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return;
	}
	// ��󻯰�ť
	else if (msg.pSender->GetName().Find(_T("maxbtn")) != -1)
	{
		CButtonUI* pBtn = (CButtonUI*)m_PaintManager.FindControl(_T("restorebtn"));
		pBtn->SetVisible(true);
		msg.pSender->SetVisible(false);
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		MaxMinCefWindow(OFFSETX, OFFSETY);
		return;
	}
	// ��ԭ��ť
	else if (msg.pSender->GetName().Find(_T("restorebtn")) != -1)
	{
		CButtonUI* pBtn = (CButtonUI*)m_PaintManager.FindControl(_T("maxbtn"));
		pBtn->SetVisible(true);
		msg.pSender->SetVisible(false);
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
		MaxMinCefWindow(OFFSETX, OFFSETY);
		return;
	}
	else if(msg.pSender->GetName().Find(_T("LoginBtn"))!=-1)
	{
		GoNewUrl(_T("https://login.taobao.com/"));
		NewTable(_T("http://ytweb.1tsoft.com/YitaoOKOWebService/user/buyerLogin"));
	}
	else if(msg.pSender->GetName().Find(_T("AuthBtn")) != -1)
	{
		NewTable(_T("http://ytweb.1tsoft.com/YitaoOKOWebService/QNshop/authoOtherShop"));
	}
	else if (msg.pSender->GetName().Find(_T("ProfitsBtn")) != -1)
	{

		const char* cTestMessage = "DOMTest.Message";
		CefRefPtr<CefProcessMessage> message(
			CefProcessMessage::Create(cTestMessage));
		message->GetArgumentList()->SetInt(0, DOM_TEST_MODIFY);
		auto it=m_cWebClient.front();
		it->GetBrowser()->SendProcessMessage(PID_RENDERER, message);
		//it->OnProcessMessageReceived(it->GetBrowser(), PID_RENDERER, message);
		//::MessageBox(NULL, _T("Send����"), L"Send��", MB_OK);
		//NewTable(_T("file:///C:/Users/Administrator/Desktop/%E6%B5%8B%E8%AF%95html.html"));
		
		//NewTable(_T("file:///C:/Users/Administrator/Desktop/%E6%B5%8B%E8%AF%95html.html"),true);
		//NewTable(_T("file:///C:/Users/Administrator/Desktop/%E6%B5%8B%E8%AF%95html.html"));
		//NewTable(_T("http://ytweb.1tsoft.com/YitaoOKOWebService/QNshop/authoOtherShop"));
	}

	else if(msg.pSender->GetName().Find(_T("AdminEntry"))!=-1)
	{
		CTbDuilibManager* duiFrame;
		duiFrame = new CTbDuilibManager();
		duiFrame->Create(this->GetHWND(), _T("CDemoDlg"), UI_WNDSTYLE_FRAME | WS_POPUP, UI_WNDSTYLE_EX_FRAME);
		//duiFrame->Create(m_hWnd, _T("dui"), UI_WNDSTYLE_FRAME | WS_POPUP,UI_WNDSTYLE_EX_FRAME);
		duiFrame->CenterWindow();
		duiFrame->ShowWindow();
	}
	else if (msg.pSender->GetName().Find(_T("WorkBtn"))!=-1)
	{
		GetWorkWeb();
		for (auto& it:m_vecWorkURL)
		{
			NewTable(it.c_str());
		}
		m_vecWorkURL.clear();
	}
	// ���IE��ǩ��option
	else if (msg.pSender->GetName().Find(_T("IE_")) != -1)
	{
		COptionUI* pOpt = (COptionUI*)msg.pSender;

		CHorizontalLayoutUI* pHor = (CHorizontalLayoutUI*)pOpt->GetTag();
		if (pHor)
		{
			RECT rc = pOpt->GetPos();
			if (msg.ptMouse.x > rc.right - 20 && msg.ptMouse.y<rc.top + 15)
			{
				int iCount = m_pTabSwitchHor->GetCount();
				for (int i = 0; i < iCount; i++)
				{
					COptionUI* pOpt1 = (COptionUI*)m_pTabSwitchHor->GetItemAt(i);
					if (pOpt == pOpt1)
					{
						m_indexSwitch = i;
						::PostMessageA(this->m_hWnd, WM_DUI_SELTABTAG, NULL, NULL);
						return;
					}
				}
			}
			m_pTabLayoutUI->SelectItem(pHor);
			RECT rect;
			GetClientRect(this->GetHWND(), &rect);
			RECT rectnew = rect;
			rectnew.top = rect.top + OFFSETX;
			rectnew.bottom = rect.bottom - OFFSETY_1;
			rectnew.left = rect.left;
			rectnew.right = rect.right;
			int t_index = m_pTabLayoutUI->GetCurSel();
			for (UINT i = 0; i<m_cWebClient.size(); i++)
			{
				CefWindowHandle hwnd = m_cWebClient.at(i)->GetBrowser()->GetHost()->GetWindowHandle();
				if (i != t_index)
					MoveWindow(hwnd, 0, 0, 0, 0, false);
				else
				{
					CEditUI* pEdit = (CEditUI*)m_PaintManager.FindControl(_T("url"));
					if (pEdit)
					{
						pEdit->SetText(m_UEdit.at(t_index));
					}
					MoveWindow(hwnd, rectnew.left, rectnew.top, rectnew.right - rectnew.left, rectnew.bottom - rectnew.top, true);
				}
			}
		}
	}
	// ���������ť
	else if (msg.pSender->GetName().Find(_T("Nav")) != -1)
	{
		GoNewUrl();
	}
	// ��ӱ�ǩ
	else if (msg.pSender->GetName().Find(_T("Add")) != -1)
	{

		// ������ǩOPTION��ť
		/*CDuiString sAttr;
		COptionUI* pOpt = new COptionUI;
		sAttr.Format(_T("name=\"IE_\" width=\"100\" height=\"30\" normalimage=\"tabbar_normal.png\" hotimage=\"tabbar_hover.png\" \
		pushedimage=\"tabbar_pushed.png\" selectedimage=\"tabbar_pushed.png\" group=\"demo_list_header\""));
		pOpt->ApplyAttributeList(sAttr);
		int iCount = m_pTabSwitchHor->GetCount();
		m_pTabSwitchHor->AddAt(pOpt, iCount-1);

		// ������ǩ����
		CHorizontalLayoutUI* pHor = new CHorizontalLayoutUI;

		// �Ѳ�����ӽ���ǩ
		m_pTabLayoutUI->Add(pHor);
		m_pTabLayoutUI->SetTag((UINT_PTR)pHor);

		m_pTabLayoutUI->SelectItem(pHor);//ѡ�б�ǩ��ť����

		// �ѱ�ǩ��ť�Ͳ��ֹ�������
		pOpt->SetTag((UINT_PTR)pHor);


		::PostMessageA(this->m_hWnd, WM_DUI_TABTAG, NULL, NULL);

		NewPage(_T("www.hao123.com"));
		pOpt->SetText(_T("http://www.hao123.com"));*/
		NewTable(_T("https://login.taobao.com/"));
	}
	else if (msg.pSender->GetName().Find(_T("refreshbtnr")) != -1)
	{
		RefreshUrl();
	}
	else if (msg.pSender->GetName().Find(_T("Forward")) != -1)
	{
		int index = m_pTabLayoutUI->GetCurSel();
		CHorizontalLayoutUI* pHor = (CHorizontalLayoutUI*)m_pTabLayoutUI->GetItemAt(index);
		if (pHor)
		{
			int iCount = m_pTabSwitchHor->GetCount();
			for (int i = 0; i < iCount; i++)
			{
				COptionUI* pOpt = (COptionUI*)m_pTabSwitchHor->GetItemAt(i);
				CHorizontalLayoutUI* pHaveHor = (CHorizontalLayoutUI*)pOpt->GetTag();
				if (pHaveHor == pHor)
				{
					CefRefPtr<CefBrowser> t_browser = m_cWebClient.at(index)->GetBrowser();
					if (t_browser->CanGoForward())
					{
						t_browser->GoForward();
					}
					break;
				}
			}
		}
	}
	else if (msg.pSender->GetName().Find(_T("Back")) != -1)
	{
		int index = m_pTabLayoutUI->GetCurSel();
		CHorizontalLayoutUI* pHor = (CHorizontalLayoutUI*)m_pTabLayoutUI->GetItemAt(index);
		if (pHor)
		{
			int iCount = m_pTabSwitchHor->GetCount();
			for (int i = 0; i < iCount; i++)
			{
				COptionUI* pOpt = (COptionUI*)m_pTabSwitchHor->GetItemAt(i);
				CHorizontalLayoutUI* pHaveHor = (CHorizontalLayoutUI*)pOpt->GetTag();
				if (pHaveHor == pHor)
				{
					CefRefPtr<CefBrowser> t_browser = m_cWebClient.at(index)->GetBrowser();
					if (t_browser->CanGoBack())
					{
						t_browser->GoBack();
					}
					break;
				}
			}
		}
	}
	//else if (msg.pSender->GetName().Find(_T("Main_Type")) != -1)
	//{
	//	// ������ǩOPTION��ť
	//	/*CDuiString sAttr;
	//	COptionUI* pOpt = new COptionUI;
	//	sAttr.Format(_T("name=\"IE_\" width=\"100\" height=\"30\" normalimage=\"tabbar_normal.png\" hotimage=\"tabbar_hover.png\" \
	//	pushedimage=\"tabbar_pushed.png\" selectedimage=\"tabbar_pushed.png\" group=\"demo_list_header\""));
	//	pOpt->ApplyAttributeList(sAttr);
	//	int iCount = m_pTabSwitchHor->GetCount();
	//	m_pTabSwitchHor->AddAt(pOpt, iCount-1);

	//	// ������ǩ����
	//	CHorizontalLayoutUI* pHor = new CHorizontalLayoutUI;


	//	// �Ѳ�����ӽ���ǩ
	//	m_pTabLayoutUI->Add(pHor);
	//	m_pTabLayoutUI->SetTag((UINT_PTR)pHor);

	//	m_pTabLayoutUI->SelectItem(pHor);//ѡ�б�ǩ��ť����

	//	// �ѱ�ǩ��ť�Ͳ��ֹ�������
	//	pOpt->SetTag((UINT_PTR)pHor);


	//	::PostMessageA(this->m_hWnd, WM_DUI_TABTAG, NULL, NULL);

	//	NewPage(_T("www.hao123.com"));
	//	pOpt->SetText(_T("http://www.hao123.com"));*/
	//	NewTable(_T("www.hao123.com"));
	//}
}

//��Ϣ��Ӧ
void CCefDuiLibTest::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("windowinit"))
	{
		OnInitWnd();
	}
	else if (msg.sType == _T("click"))
	{
		OnClick(msg);
	}
	else if (msg.sType == _T("return"))
	{
		GoNewUrl();
	}
}
//ϵͳ��Ϣ
LRESULT CCefDuiLibTest::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	LRESULT lRes = 0;

	if (uMsg == WM_TITLECHANGE)
	{
		CefString str = *(CefString*)lParam;
		TitleChage(str);
	}
	if (uMsg == WM_LOADSTART)
	{
		int index = m_pTabLayoutUI->GetCurSel();
		CLabelUI * pLable = (CLabelUI*)m_PaintManager.FindControl((_T("statusbar")));
		pLable->SetTextColor(0xff0000);

		std::wstring  str = _T("���ڼ���");
		str = str + m_cWebClient.at(index)->m_url.c_str();
		pLable->SetText(str.c_str());


		CHorizontalLayoutUI* pHor = (CHorizontalLayoutUI*)m_pTabLayoutUI->GetItemAt(index);
		if (pHor)
		{
			int iCount = m_pTabSwitchHor->GetCount();
			for (int i = 0; i < iCount; i++)
			{
				COptionUI* pOpt = (COptionUI*)m_pTabSwitchHor->GetItemAt(i);
				CHorizontalLayoutUI* pHaveHor = (CHorizontalLayoutUI*)pOpt->GetTag();
				if (pHaveHor == pHor)
				{
					m_UEdit.at(index) = m_cWebClient.at(index)->m_url.c_str();
					break;
				}
			}
		}
	}
	else if (uMsg == WM_LOADEND)
	{
		CLabelUI * pLable = (CLabelUI*)m_PaintManager.FindControl((_T("statusbar")));
		pLable->SetTextColor(0xff0000);
		int index = m_pTabLayoutUI->GetCurSel();
		CHorizontalLayoutUI* pHor = (CHorizontalLayoutUI*)m_pTabLayoutUI->GetItemAt(index);
		if (pHor)
		{
			int iCount = m_pTabSwitchHor->GetCount();
			for (int i = 0; i < iCount; i++)
			{
				COptionUI* pOpt = (COptionUI*)m_pTabSwitchHor->GetItemAt(i);
				CHorizontalLayoutUI* pHaveHor = (CHorizontalLayoutUI*)pOpt->GetTag();
				if (pHaveHor == pHor)
				{
					if (m_cWebClient.at(index)->GetBrowser()->CanGoBack())
					{
						m_BackBtn->SetEnabled(true);
					}
					else
					{
						m_BackBtn->SetEnabled(false);
					}
					if (m_cWebClient.at(index)->GetBrowser()->CanGoForward())
					{
						m_ForwardBtn->SetEnabled(true);
					}
					else
					{
						m_ForwardBtn->SetEnabled(false);
					}
					m_UEdit.at(index) = m_cWebClient.at(index)->m_url.c_str();
					break;
				}
			}
		}
		CEditUI* pEdit = (CEditUI*)m_PaintManager.FindControl(_T("url"));
		if (pEdit)
		{
			pEdit->SetText(m_UEdit.at(index));
		}
		pLable->SetText(_T("�������"));
	}

	else if (uMsg == WM_LOADPOPUP)
	{
		std::wstring T_urL = *(std::wstring*)lParam;
		// ������ǩOPTION��ť
		CDuiString sAttr;
		COptionUI* pOpt = new COptionUI;
		sAttr.Format(_T("name=\"IE_\" width=\"100\" height=\"30\" normalimage=\"tabbar_normal.png\" hotimage=\"tabbar_hover.png\" \
			pushedimage=\"tabbar_pushed.png\" selectedimage=\"tabbar_pushed.png\" group=\"demo_list_header\""));
		ApplyAttributeList(*pOpt, sAttr);

		int iCount = m_pTabSwitchHor->GetCount();
		m_pTabSwitchHor->AddAt(pOpt, iCount - 1);

		// ������ǩ����
		CHorizontalLayoutUI* pHor = new CHorizontalLayoutUI;

		// �Ѳ�����ӽ���ǩ
		m_pTabLayoutUI->Add(pHor);
		m_pTabLayoutUI->SetTag((UINT_PTR)pHor);

		m_pTabLayoutUI->SelectItem(pHor);//ѡ�б�ǩ��ť����

										 // �ѱ�ǩ��ť�Ͳ��ֹ�������
		pOpt->SetTag((UINT_PTR)pHor);

		::PostMessageA(this->m_hWnd, WM_DUI_TABTAG, NULL, NULL);

		NewPage(T_urL);
	}
	else if (uMsg == WM_DUI_TABTAG)
	{
		int iCount = m_pTabSwitchHor->GetCount();
		COptionUI* pOpt = (COptionUI*)m_pTabSwitchHor->GetItemAt(iCount - 2);
		if (pOpt)
		{
			pOpt->Selected(true);
		}
	}
	else if (uMsg == WM_DUI_SELTABTAG)
	{

		TableDel();
	}

	return __super::MessageHandler(uMsg, wParam, lParam, bHandled);
}
//��ȡƤ��·��
DuiLib::CDuiString CCefDuiLibTest::GetSkinFolder()
{
	return _T("skin\\test_01");
}
//��ȡƤ������
DuiLib::CDuiString CCefDuiLibTest::GetSkinFile()
{
	return _T("test_01.xml");
}

//��ȡ����
LPCTSTR CCefDuiLibTest::GetWindowClassName(void) const
{

	return _T("LoginWinClass");
}
//�����ؼ�
CControlUI* CCefDuiLibTest::CreateControl(LPCTSTR pstrClass)
{
	//if (_tcscmp(pstrClass,_T("ButtonGif")) == 0) return new CButtonGifUI;
	return NULL;
}

void CCefDuiLibTest::OnFinalMessage(HWND hWnd)
{
	for (auto& it:m_cWebClient)
	{
		while (it->GetBrowser())
		{
			Sleep(10);
		}
	}

	::PostQuitMessage(0);
	delete this;
}
	
	

LRESULT CCefDuiLibTest::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	::PostQuitMessage(0);
	return 0;
}
void CCefDuiLibTest::OnInitWnd()
{
	//��ǩ����
	m_pTabLayoutUI = (CTabLayoutUI*)m_PaintManager.FindControl(_T("switch"));

	// ��ǩ��ť����
	m_pTabSwitchHor = (CHorizontalLayoutUI*)(m_PaintManager.FindControl(_T("tabswitch")));

	//ǰ����ť
	m_ForwardBtn = (CButtonUI*)m_PaintManager.FindControl(_T("Forward"));
	//���˰�ť
	m_BackBtn = (CButtonUI*)m_PaintManager.FindControl(_T("Back"));
	//����
	//m_pSearchEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("search_edit")));

	m_BackBtn->SetEnabled(false);
	m_ForwardBtn->SetEnabled(false);

	CefRefPtr<CWebClient>client(new CWebClient());
	m_cWebClient.push_back(client);
	m_cWebClient.at(0)->m_hWnd = m_hWnd;
	CefWindowInfo info;
	RECT rect;
	GetClientRect(this->GetHWND(), &rect);
	RECT rectnew = rect;
	rectnew.top = rect.top + OFFSETX;
	rectnew.bottom = rect.bottom - OFFSETY_1;
	rectnew.left = rect.left;
	rectnew.right = rect.right;
	info.SetAsChild(m_hWnd, rectnew);
	CefBrowserSettings browserSettings;
	CefBrowserHost::CreateBrowser(info, static_cast<CefRefPtr<CWebClient> >(m_cWebClient.at(0)), "file:///C:/Users/Administrator/Desktop/%E6%B5%8B%E8%AF%95html.html", browserSettings, NULL);
	CDuiString str(_T("file:///C:/Users/Administrator/Desktop/%E6%B5%8B%E8%AF%95html.html"));
	//CDuiString str(_T("https://login.taobao.com/"));
	m_UEdit.push_back(str);


	// ����ҳ�ؼ��ͱ�ǩ���ֹ�������
	CHorizontalLayoutUI* pHor = (CHorizontalLayoutUI*)(m_PaintManager.FindControl(_T("HorBrowser")));
	/*if (pHor)
	{
	CWebBrowserUI* pWer = (CWebBrowserUI*)(m_PaintManager.FindControl(_T("Browser")));
	if (pWer)
	{
	pHor->SetTag((UINT_PTR)pWer);
	}
	}*/

	// �ѵ�һ��option��ť�ͱ�ǩ���ֹ�������
	COptionUI* pOpt = (COptionUI*)(m_PaintManager.FindControl(_T("IE_1")));
	if (pOpt)
	{
		pOpt->SetTag((UINT_PTR)pHor);
	}
}
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());

	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;

	CefSettings cSettings;
	CefSettingsTraits::init(&cSettings);
	cSettings.no_sandbox = true;
	cSettings.multi_threaded_message_loop = true;
	CefRefPtr<CefApp>spApp(new ClientAppRenderer);
	CefMainArgs main_args;
	

	int exit_code = CefExecuteProcess(main_args, NULL, NULL);
	if (exit_code >= 0)
	{
		// The sub-process has completed so return here.
		return exit_code;
	}

	CefInitialize(main_args, cSettings, spApp,NULL);



	CCefDuiLibTest* pFrame = new CCefDuiLibTest();
	if (pFrame == NULL) return 0;  //
	pFrame->Create(NULL, _T("����һ����򵥵Ĳ�����exe���޸�test_01.xml�Ϳ��Կ���Ч��"), WS_MINIMIZEBOX | WS_POPUP | WS_VISIBLE, WS_EX_WINDOWEDGE);
	pFrame->CenterWindow();
	pFrame->ShowWindow(true);

	//CefRunMessageLoop();
	//DuiLib::CPaintManagerUI::MessageLoop();


	if (!cSettings.multi_threaded_message_loop)
	{
		//Run the CEF message loop. This function will block until the application
		//recieves a WM_QUIT message.
		CefRunMessageLoop();

	}
	else
	{
		DuiLib::CPaintManagerUI::MessageLoop();
	}

	
	::CoUninitialize();
	CefShutdown();

	return 0;
}

