#pragma once


#include <windows.h>
#include <objbase.h>

#include "WebClient.h"
#include "resource.h"

#include "include\cef_client.h"
#include "include/cef_base.h"
#include "include/cef_app.h"
#include "include/internal/cef_win.h"
#include <atlstr.h>
#include <vector>
#include "TbManager.h"
#include "client_app_renderer.h"

using namespace std;

using namespace TBClient;
#define  WM_CREATE_NEW  WM_USER+302
#define  WM_DUI_TABTAG	WM_USER+400
#define  WM_DUI_SELTABTAG WM_USER+401


class CCefDuiLibTest : public WindowImplBase
{
public:
	CCefDuiLibTest();
	virtual ~CCefDuiLibTest();
private:
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/);
	virtual void Notify(TNotifyUI& msg);
	virtual void OnFinalMessage(HWND hWnd);
	virtual void OnClick(TNotifyUI& msg);

private:
	void  GoNewUrl(std::wstring Url);
	void  GoNewUrl(); //ת���µ�ַ
	void  MaxMinCefWindow(int offsetx, int offsety); //�����С�����Ŵ��ڴ�С
	void  NewPage(std::wstring Url,bool Isrender=false);//����ҳǩ
	void  RefreshUrl();//ˢ�µ�ǰҳ��
	void  TitleChage(const CefString &str);//����ı�
	void  TableDel();//ɾ��ҳǩ
	void  OnInitWnd();
	void  SearchGo();//����
	void  NewTable(std::wstring url, bool Isrender = false);
	bool GetWorkWeb();
	void TestDomWeb(const CefString& url);
	//virtual bool OnProcessMessageReceived(
	//	CefRefPtr<CCefDuiLibTest> app,
	//	CefRefPtr<CefBrowser> browser,
	//	CefProcessId source_process,
	//	CefRefPtr<CefProcessMessage> message) {
	//	return false;
	//}
private:
	CTabLayoutUI*			m_pTabLayoutUI;		//��ǩ����
	CHorizontalLayoutUI*	m_pTabSwitchHor;	//��ǩ��ť����
	CButtonUI*				m_BackBtn;			//ǰ����ť
	CButtonUI*				m_ForwardBtn;		//���˰�ť
	int						m_indexSwitch;		//��ǩ����
	vector<CefRefPtr<CWebClient>>   m_cWebClient;
	vector<CDuiString>      m_UEdit;	//��ǩҳ��ַ
	CEditUI* m_pSearchEdit;
	vector<wstring> m_vecWorkURL;
	vector<wstring> m_vecBanURL;
private:
	//IMPLEMENT_REFCOUNTING(CCefDuiLibTest);
};


CControlUI& ApplyAttributeList(CControlUI& ctrUI, LPCTSTR pstrList)
{
	CDuiString sItem;
	CDuiString sValue;
	while (*pstrList != _T('\0'))
	{
		sItem.Empty();
		sValue.Empty();
		while (*pstrList != _T('\0') && *pstrList != _T('='))
		{
			LPCTSTR pstrTmp = ::CharNext(pstrList);
			while (pstrList < pstrTmp)
			{
				sItem += *pstrList++;
			}
		}
		ASSERT(*pstrList == _T('='));
		if (*pstrList++ != _T('='))
			return ctrUI;
		ASSERT(*pstrList == _T('\"'));
		if (*pstrList++ != _T('\"'))
			return ctrUI;
		while (*pstrList != _T('\0') && *pstrList != ('\"'))
		{
			LPTSTR pstrTmp = ::CharNext(pstrList);
			while (pstrList < pstrTmp)
			{
				sValue += *pstrList++;
			}
		}
		ASSERT(*pstrList == _T('\"'));
		if (*pstrList++ != _T('\"'))
			return ctrUI;
		ctrUI.SetAttribute(sItem, sValue);
		if (*pstrList++ != _T(' '))
			return ctrUI;
	}
	return ctrUI;
}

