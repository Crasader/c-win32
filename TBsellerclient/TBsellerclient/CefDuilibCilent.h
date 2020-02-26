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
	void  GoNewUrl(); //转换新地址
	void  MaxMinCefWindow(int offsetx, int offsety); //最大最小化缩放窗口大小
	void  NewPage(std::wstring Url,bool Isrender=false);//新增页签
	void  RefreshUrl();//刷新当前页面
	void  TitleChage(const CefString &str);//标题改变
	void  TableDel();//删除页签
	void  OnInitWnd();
	void  SearchGo();//搜索
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
	CTabLayoutUI*			m_pTabLayoutUI;		//标签布局
	CHorizontalLayoutUI*	m_pTabSwitchHor;	//标签按钮布局
	CButtonUI*				m_BackBtn;			//前进按钮
	CButtonUI*				m_ForwardBtn;		//后退按钮
	int						m_indexSwitch;		//标签索引
	vector<CefRefPtr<CWebClient>>   m_cWebClient;
	vector<CDuiString>      m_UEdit;	//标签页网址
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

