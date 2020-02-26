#pragma once

#include "stdafx.h"

#include<sstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <deque>
#include "ThreadManager.h"
#include "XmlWnd.h"
#include "WordCardMain.h"

class CWordCardMain;
/// ������ʾ��ʾ��
enum    eDispStatus
{
	eDispStatus_inc = 0,
	eDispStatus_disp,
	eDispStatus_dec,
	eDispStatus_unknowns
};

class CMainDlg;
class CDlgSlidingShow :
	public CXmlWnd
{
public:
	CDlgSlidingShow(const WordCardBtnGroup groupbtn,WCHAR * pcXmlFileName, WCHAR * pcWndClassName);
	virtual ~CDlgSlidingShow(void);

public:
	void ThreadProcStart();
	void ThreadProcEnd();
	static UINT WINAPI ThreadProc(void* pParam);
	UINT WINAPI ThreadProc();

	DUI_DECLARE_MESSAGE_MAP()

public:
	void SetOwner(CWordCardMain* pDlg);
	virtual void InitWindow();
	virtual LONG GetStyle();
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	//virtual LRESULT     WndMessageProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT WndMessageProc_SlidingShow(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT     SysMessageProc(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	virtual void        OnFinalMessage(HWND hWnd);
	virtual void        Notify(TNotifyUI & msg);
	virtual void        OnClick(TNotifyUI& msg);
	virtual LRESULT     OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void OnPrepare(TNotifyUI & msg);
	void InitControl();

	LRESULT WndMessageProc_SlidingQuit(UINT uMsg, WPARAM wParam, LPARAM lParam);

	std::wstring s2ws(const std::string& s)

	{

		int len;

		int slength = (int)s.length() + 1;

		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);

		wchar_t* buf = new wchar_t[len];

		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);

		std::wstring r(buf);

		delete[] buf;

		return r.c_str();

	}
private:
	void UiInit();
	void DataInit();
	void DataUnInit();
	void WordCardVerify(const WordCardBtn wcbtn);
private:
	CWordCardMain* m_pOwner;
	ns_base::CThreadManager     m_ThreadManager;
	const WordCardBtnGroup m_btngroup;
	eDispStatus m_Status;
	RECT m_rtMeOrg; ///< ���ڵ�һ��100%��ʾʱ��size get by ::GetWindowRect
	BOOL m_bPlay; ///< �Ƿ�̬������, Play() �Ƿ�ִ�еı�־
	float m_fRate; ///< �����100size�Ĵ��ڱ���(0~100)
	BOOL m_answer;
	BOOL m_IsAnswer;
};



