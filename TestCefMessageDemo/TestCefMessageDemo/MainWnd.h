#pragma once
#include "simple_handler.h"
#include "Common.h"

class CMainWnd : public WindowImplBase
{
public:
	CMainWnd(void);
	~CMainWnd(void);

	virtual void InitWindow();
	virtual void OnFinalMessage( HWND hWnd );
	virtual void Notify(TNotifyUI& msg);

protected:
	virtual CDuiString GetSkinFolder(){return L"Skin\\";}
	virtual CDuiString GetSkinFile(){return L"MainWnd.xml";}
	virtual LPCTSTR GetWindowClassName(void) const {return L"main_wnd_class";}

public:
	virtual UINT GetClassStyle() const { return CS_DBLCLKS;}
	virtual UILIB_RESOURCETYPE GetResourceType() const {return UILIB_FILE;}
	virtual CDuiString GetZIPFileName() const { return L"";}

	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void CallJsFunc(LPCWSTR strJSFuncName);
	void CallJsCBFunc(LPCWSTR str);

private:
	CButtonUI* m_pBtnClose;
	CEditUI* m_pURL;
	CButtonUI* m_pBtnGo;
	CButtonUI* m_pBtnBack;
	CButtonUI* m_pBtnCallJs;

	CefRefPtr<SimpleHandler> m_handler;
};

