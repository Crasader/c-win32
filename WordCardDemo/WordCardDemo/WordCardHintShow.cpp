#include "stdafx.h"
#include "WordCardHintShow.h"
#include "constDefine_BusinessLayer.h"



CWordCardHintShow::CWordCardHintShow(WCHAR * pcXmlFileName, WCHAR * pcWndClassName,BOOL Ishotkey):
	CXmlWnd(pcXmlFileName,pcWndClassName),m_hotkey(Ishotkey)
{
	DataInit();
}

CWordCardHintShow::~CWordCardHintShow()
{
	DataUnInit();
}

void CWordCardHintShow::SetOwner(CWordCardMain * pDlg)
{
	m_pOwner = pDlg;
}

LONG CWordCardHintShow::GetStyle()
{
	long dwStyle = __super::GetStyle();

	dwStyle &= ~WS_MAXIMIZEBOX;
	return dwStyle;
}

void CWordCardHintShow::InitWindow()
{
	UiInit();
}

void CWordCardHintShow::ThreadProcStart()
{
	if (!m_ThreadManager.IsNeedQuitThread()
		&& !m_ThreadManager.IsThreadRunning())
	{
		m_ThreadManager.SetThreadHandle((HANDLE)_beginthreadex(NULL, 0, &CWordCardHintShow::ThreadProc, (void*)this, 0, NULL));
	}
}

void CWordCardHintShow::ThreadProcEnd()
{
	m_ThreadManager.StopThread(TRUE, L"m_ThreadManager");
}

UINT CWordCardHintShow::ThreadProc(void * pParam)
{
	UINT    uRc = S_FALSE;

	do
	{
		if (NULL == pParam)
			break;

		uRc = ((CWordCardHintShow*)pParam)->ThreadProc();
	} while (0);

	return uRc;
}

UINT CWordCardHintShow::ThreadProc()
{
	do
	{
		::Sleep(100);

		if (m_ThreadManager.IsNeedQuitThread())
			break;

		/// 定时发出消息, 让消息处理函数
	
		::PostMessageW(this->GetHWND(), WM_SLIDING_QUIT, 0, 0);
	} while (1);
	return S_OK;
}

LRESULT CWordCardHintShow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL    bNeedDefaultProcess = FALSE;
	do
	{
		switch (uMsg)
		{
		case WM_CREATE:
			bNeedDefaultProcess = TRUE;
			break;

		case WM_CLOSE:
			EntryUiDestory(TRUE);
			ThreadProcEnd();
			bNeedDefaultProcess = TRUE;
			break;

		case WM_SLIDING_QUIT:
			WndMessageProc_SlidingQuit(uMsg, wParam, lParam);
			bNeedDefaultProcess = FALSE;
			break;

		default:
			bNeedDefaultProcess = TRUE;
			break;
		}

		if (bNeedDefaultProcess)
			return __super::WndMessageProc(uMsg, wParam, lParam);
		else
			return S_OK;
	} while (0);

	return S_OK;
}

LRESULT CWordCardHintShow::SysMessageProc(UINT uMsg, WPARAM wParam, LPARAM lParam, bool & bHandled)
{
	return __super::SysMessageProc(uMsg, wParam, lParam, bHandled);
}

void CWordCardHintShow::OnFinalMessage(HWND hWnd)
{
	m_pOwner->SubDlgDestory();
	__super::OnFinalMessage(hWnd);
	delete this;
}

void CWordCardHintShow::Notify(TNotifyUI & msg)
{
	if (_tcsicmp(msg.sType, _T("windowinit")) == 0)
	{
		OnPrepare(msg);
	}
	return;
}

LRESULT CWordCardHintShow::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	return __super::OnNcHitTest(uMsg,wParam,lParam,bHandled);
}

void CWordCardHintShow::OnPrepare(TNotifyUI & msg)
{

	InitControl();
}

void CWordCardHintShow::InitControl()
{
	if (m_hotkey)
	{
		CLabelUI* pLabelUI = static_cast<CLabelUI*> (m_PaintManager.FindControl(_T("HintText")));

		DWORD headerfont = pLabelUI->GetFont();

		pLabelUI->SetText(_T("STOP MEMORY"));
		pLabelUI->SetFont(headerfont);
		pLabelUI->SetTextColor(0xFFFFFFF);
	}
}

LRESULT CWordCardHintShow::WndMessageProc_SlidingQuit(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	do 
	{
		if (m_uDispTime>0)
		{
			SetLayeredWindowAttributes(m_hWnd, NULL, m_uDispTime, LWA_ALPHA);
			UpdateWindow(m_hWnd);
			Sleep(10);
			m_uDispTime-=20;
		}
		else
		{
			m_bPlay = FALSE;
		}
		
		if (!m_bPlay)
		{
			::PostMessageW(this->GetHWND(), WM_CLOSE, 0, 0);
		}
	
	} while (0);
	return S_OK;
}

void CWordCardHintShow::UiInit()
{
	_ASSERT(NULL != m_pOwner);
	::GetClientRect(this->GetHWND(), &m_rtMeOrg);
	ThreadProcStart();
}

void CWordCardHintShow::DataInit()
{
	m_pOwner = NULL;
	m_bPlay = TRUE;
	m_uDispTime = 240;
}

void CWordCardHintShow::DataUnInit()
{
}


