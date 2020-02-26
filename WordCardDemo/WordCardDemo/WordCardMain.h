#pragma once
#include "D:\po\project\WordCardDemo\DuiLib\Utils\WinImplBase.h"
#include "WordCardList.h"
#include "WordCardData.h"
#include <sstream>
#include "DlgSlidingShow.h"
#include "WordCardHintShow.h"
#include <time.h>
#include "WordCardMemoryFunc.h"
#include <Shellapi.h>

class CDlgSlidingShow;
class CWordCardHintShow;
using namespace DuiLib;
class CWordCardMain :
	public WindowImplBase, public IListCallbackUI
{
public:
	CWordCardMain();
	~CWordCardMain();

public:
	void ThreadProcStart();
	void ThreadProcEnd();
	static UINT WINAPI ThreadProc(void* pParam);
	UINT WINAPI ThreadProc();

	//DUI_DECLARE_MESSAGE_MAP();


public:
	LPCTSTR GetWindowClassName() const;
	virtual void OnFinalMessage(HWND hWnd);
	virtual void InitWindow();
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void UpdateWordCardList();

	static DWORD WINAPI ShowWordCardList(LPVOID lpParameter);
	virtual LPCTSTR GetItemText(CControlUI* pControl, int iIndex, int iSubItem);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnAddListItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnHotKey(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	

	void CreateDlgSlidingShow(_In_ const WordCardBtnGroup& btngroup);
	void SubDlgDestory();
	void CreateGroupBtn(_Out_ WordCardBtnGroup& btngroup);
	void CreateGroupTgtBtn(int index, _Out_ WordCardBtnGroup& btngroup);
	void GetLeftBtn(_Out_ WordCardBtnGroup& btngroup,int randbtnnum, int randnum);
	BOOL CheckFullScreen();
protected:
	void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);
	void OnExit(TNotifyUI& msg);
	void OnTimer(TNotifyUI& msg);
	void OnFuncWork(std::wstring time);
	

	
private:
	CWordCardMemoryFunc m_memoryFunc;
	ns_base::CThreadManager m_ThreadManager;
	CWordCardUI* m_pWordCardList;
	std::vector<WordCardListItemInfo> m_vecWordCards;

	CDlgSlidingShow* m_pDlgSlidingShow;
	CWordCardHintShow* m_pWordCardHintShow;

	int m_wordindex;
	BOOL m_IsChs;
	BOOL m_IsEng;
	BOOL m_func;
	BOOL m_hotkey;
};

