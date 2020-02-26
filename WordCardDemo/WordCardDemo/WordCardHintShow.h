#pragma once

#include <windows.h>
#include <process.h>
#include "resource.h"
#include <comutil.h>
#include <commdlg.h>
#include <mshtml.h>
#include <wininet.h>
#include <sys/stat.h>

#include "XmlWnd.h"
#include "ThreadManager.h"
#include "WordCardMain.h"

class CWordCardMain;
class CWordCardHintShow :
	public CXmlWnd
{
public:
	CWordCardHintShow(WCHAR * pcXmlFileName, WCHAR * pcWndClassName, BOOL Ishotkey);
	~CWordCardHintShow();

public:
	void ThreadProcStart();
	void ThreadProcEnd();
	static UINT WINAPI ThreadProc(void* pParam);
	UINT WINAPI ThreadProc();
public:
	void SetOwner(CWordCardMain* pDlg);
	virtual LONG GetStyle();
	virtual void InitWindow();

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT     SysMessageProc(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	virtual void        OnFinalMessage(HWND hWnd);
	virtual void        Notify(TNotifyUI & msg);
	virtual LRESULT     OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void OnPrepare(TNotifyUI & msg);
	void InitControl();
	LRESULT WndMessageProc_SlidingQuit(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	void UiInit();
	void DataInit();
	void DataUnInit();
private:

	CWordCardMain* m_pOwner;
	int m_uDispTime = 0;
	RECT m_rtMeOrg;
	BOOL m_bPlay;
	BOOL m_hotkey;
	ns_base::CThreadManager     m_ThreadManager;
};

