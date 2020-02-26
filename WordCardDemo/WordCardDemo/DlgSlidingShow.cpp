/// @file	MainDlg.cpp

#include "stdafx.h"
#include <windows.h>
#include <process.h>
#include "resource.h"
#include <comutil.h>
#include <commdlg.h>
#include <mshtml.h>
#include <wininet.h>
#include <sys/stat.h>

#include "dlgSlidingShow.h"
#include "constDefine_BusinessLayer.h"

CDlgSlidingShow::CDlgSlidingShow(WordCardBtnGroup groupbtn, WCHAR * pcXmlFileName, WCHAR * pcWndClassName)
	: CXmlWnd(pcXmlFileName, pcWndClassName), m_btngroup(groupbtn)
{
	DataInit();
}

CDlgSlidingShow::~CDlgSlidingShow(void)
{
	DataUnInit();
}

DUI_BEGIN_MESSAGE_MAP(CDlgSlidingShow, CXmlWnd)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

void CDlgSlidingShow::UiInit()
{
	_ASSERT(NULL != m_pOwner);
	::GetClientRect(this->GetHWND(), &m_rtMeOrg);
	ThreadProcStart();
}

void CDlgSlidingShow::DataInit()
{

	m_pOwner = NULL;
	m_bPlay = TRUE;
	m_Status = eDispStatus_inc;
	m_fRate = .0f;
}

void CDlgSlidingShow::DataUnInit()
{
}

void CDlgSlidingShow::WordCardVerify(const WordCardBtn wcbtn)
{
	if (wcbtn.IsRight==TRUE)
	{
		m_answer = TRUE;
		CTextUI* pHeaderTextUI = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("WordText")));
		pHeaderTextUI->SetTextColor(0xFF00FF7F);
	}
	else
	{
		m_answer = FALSE;
		CTextUI* pHeaderTextUI = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("WordText")));
		pHeaderTextUI->SetTextColor(0xFFDC143C);
	}
}

LONG CDlgSlidingShow::GetStyle()
{
	long dwStyle = __super::GetStyle();

	dwStyle &= ~WS_MAXIMIZEBOX;
	return dwStyle;
}

LRESULT CDlgSlidingShow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

		case WM_SLIDING_SHOW:
			WndMessageProc_SlidingShow(uMsg, wParam, lParam);
			bNeedDefaultProcess = FALSE;
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



void CDlgSlidingShow::InitWindow()
{
	UiInit();
}

//LRESULT CDlgSlidingShow::WndMessageProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	BOOL    bNeedDefaultProcess = FALSE;
//	do
//	{
//		switch (uMsg)
//		{
//		case WM_CREATE:
//			bNeedDefaultProcess = TRUE;
//			break;
//
//		case WM_CLOSE:
//			EntryUiDestory(TRUE);
//			ThreadProcEnd();
//			bNeedDefaultProcess = TRUE;
//			break;
//
//		case WM_SLIDING_SHOW:
//			WndMessageProc_SlidingShow(uMsg, wParam, lParam);
//			bNeedDefaultProcess = FALSE;
//			break;
//
//		default:
//			bNeedDefaultProcess = TRUE;
//			break;
//		}
//
//		if (bNeedDefaultProcess)
//			return __super::WndMessageProc(uMsg, wParam, lParam);
//		else
//			return S_OK;
//	} while (0);
//
//	return S_OK;
//}

LRESULT CDlgSlidingShow::SysMessageProc(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	return __super::SysMessageProc(uMsg, wParam, lParam, bHandled);
}

void CDlgSlidingShow::OnFinalMessage(HWND hWnd)
{
	m_pOwner->SubDlgDestory();
	__super::OnFinalMessage(hWnd);
	delete this; ///< ������ɱ����, ����Ҫ֪ͨ�����彫�ҵ�ָ��ѹ�����, ��һ�����ʵ�ʱ��ɾ��.
}

void CDlgSlidingShow::Notify(TNotifyUI & msg)
{
	if (_tcsicmp(msg.sType, _T("windowinit")) == 0)
	{
		OnPrepare(msg);
	}
	else if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		OnClick(msg);
	}
	return;
}

void CDlgSlidingShow::OnClick(TNotifyUI& msg)
{
	BOOL            bNeedDefaultProcess = FALSE;
	std::wstring    strName = L"";

	if (NULL != msg.pSender)
	{
		strName = msg.pSender->GetName().GetData();
	}

	if (strName == L"selectBtn1")
	{
		WordCardVerify(m_btngroup.btn1);
		m_IsAnswer = TRUE;
	}
	else if (strName == L"selectBtn2")
	{
		WordCardVerify(m_btngroup.btn2);
		m_IsAnswer = TRUE;
	}
	else if (strName == L"selectBtn3")
	{
		WordCardVerify(m_btngroup.btn3);
		m_IsAnswer = TRUE;
	}
	else if(strName == L"selectBtn4")
	{
		WordCardVerify(m_btngroup.btn4);
		m_IsAnswer = TRUE;
	
	}
	else
	{
		bNeedDefaultProcess = TRUE;
	}

	if (bNeedDefaultProcess)
		__super::OnClick(msg);
}

LRESULT CDlgSlidingShow::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return __super::OnNcHitTest(uMsg, wParam, lParam, bHandled);
}

void CDlgSlidingShow::OnPrepare(TNotifyUI & msg)
{
	InitControl();
}

void CDlgSlidingShow::InitControl()
{
	CTextUI* pHeaderTextUI = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("WordText")));
	CButtonUI* pButtonUI1 = static_cast<CButtonUI*> (m_PaintManager.FindControl(_T("selectBtn1")));
	CButtonUI* pButtonUI2 = static_cast<CButtonUI*> (m_PaintManager.FindControl(_T("selectBtn2")));
	CButtonUI* pButtonUI3 = static_cast<CButtonUI*> (m_PaintManager.FindControl(_T("selectBtn3")));
	CButtonUI* pButtonUI4 = static_cast<CButtonUI*> (m_PaintManager.FindControl(_T("selectBtn4")));


	DWORD headerfont = pHeaderTextUI->GetFont();
	DWORD btnfont = pButtonUI1->GetFont();
	
	pHeaderTextUI->SetText(s2ws(m_btngroup.word).c_str());
	pHeaderTextUI->SetFont(headerfont);
	pHeaderTextUI->SetTextColor(0xFFFFFFF);

	pButtonUI1->SetText(s2ws(m_btngroup.btn1.wordmean).c_str());
	pButtonUI1->SetFont(btnfont);
	pButtonUI1->SetTextColor(0xFFFFFFF);

	pButtonUI2->SetText(s2ws(m_btngroup.btn2.wordmean).c_str());
	pButtonUI2->SetFont(btnfont);
	pButtonUI2->SetTextColor(0xFFFFFFF);

	pButtonUI3->SetText(s2ws(m_btngroup.btn3.wordmean).c_str());
	pButtonUI3->SetFont(btnfont);
	pButtonUI3->SetTextColor(0xFFFFFFF);

	pButtonUI4->SetText(s2ws(m_btngroup.btn4.wordmean).c_str());
	pButtonUI4->SetFont(btnfont);
	pButtonUI4->SetTextColor(0xFFFFFFF);

}

LRESULT CDlgSlidingShow::WndMessageProc_SlidingQuit(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT    rtMain;         ///< ������size
	RECT    rtLayout;       ///< 100%��ʾ�ڸ��ؼ��·��Ĵ�С
	RECT    rtLayoutRate;   ///< ���ڻ���ʱ, ��ͨ�����µ���ʾsize
	const   UINT uDispTimeMax = 500 *2;   ///< ������ʾ���ȶ���ʾʱ��

											/// ��ֱ��ʾ���ʲ���ֵ, ���ƴ��ڻ����ٶ�
											/// �����趨��ÿ10MS, ִ��һ�� Play()
	const   float fDispStep = 0.20f;
	const   UINT uMinHeight = 10;           ///< ��С���ڸ߶�

											/// ��С�߶����Ƶ�Ӧ��, ��ʼ��ʾ�ͽ�����ʾ������С�߶�
	const   float fRateMin = 0.05f;    ///< fRateMin ���ʶ�Ӧ��С���ڸ߶�
	static  UINT uDispTimeTotal = 0;    ///< �ȶ���ʾ��ʱ�����

	do
	{
		if (!m_bPlay)
			break;

		/*if (eDispStatus_inc == m_Status)
		{
			m_fRate += fDispStep;
			if (m_fRate > 1.0f)
			{
				m_fRate = 1.0f;
				uDispTimeTotal = 0;
				m_Status = eDispStatus_disp;
			}
			else if (m_fRate < fRateMin)
				m_fRate = fRateMin;
		}*/
		if (eDispStatus_disp == m_Status)
		{
			uDispTimeTotal += TIMER_DELAY_PLAY;
			if (uDispTimeTotal >= uDispTimeMax)
				m_Status = eDispStatus_dec;
		}
		else if (eDispStatus_dec == m_Status)
		{
			m_fRate -= fDispStep;
			if (m_fRate <= fRateMin)
			{
				m_fRate = fRateMin;
				uDispTimeTotal = 0;
				m_Status = eDispStatus_inc;
				m_bPlay = FALSE;
			}
		}

		// ::GetWindowRect(m_pOwner->GetHWND(), &rtMain);
		//::GetClientRect(m_pOwner->GetHWND(), &rtMain);
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rtMain, NULL);
		//::GetWindowRect()
		rtLayout.right = rtMain.right;
		rtLayout.bottom = rtMain.bottom;
		rtLayout.left = rtLayout.right - (m_rtMeOrg.right - m_rtMeOrg.left);
		rtLayout.top = rtLayout.bottom - (m_rtMeOrg.bottom - m_rtMeOrg.top);

		rtLayoutRate.left = rtLayout.left;
		rtLayoutRate.right = rtLayout.right;
		rtLayoutRate.bottom = rtLayout.bottom - 1.0f * (rtLayout.bottom - rtLayout.top) * m_fRate + 300;
		rtLayoutRate.top = rtLayout.bottom - 1.0f * (rtLayout.bottom - rtLayout.top) * m_fRate;

		/// ���������ư� ? �޷����óɺܵ͵ĸ߶�, ����С�߶�����...
		/// ���� OnGetMinMaxInfo Ҳû��
		/// ͨ������, ������С�߶�Ϊ10����

		/// ��Ϊ�����ߵķ��Ӵ���(~UI_WNDSTYLE_CHILD), ��SetWindowPos
		/// ����Ƿ��Ӵ���, λ��ҲҪ����ʵ��һ��, ������::GetClientRect.
		/// Ҫ��::GetWindowRect
		//         ::SetWindowPos(
		//             this->GetHWND(), 
		//             m_pOwner->GetHWND(), 
		//             rtLayoutRate.left, 
		//             rtLayoutRate.top, 
		//             rtLayoutRate.right - rtLayoutRate.left, 
		//             rtLayoutRate.bottom - rtLayoutRate.top, 
		//             SWP_SHOWWINDOW);

		/// ��Ϊ�����ߵ��Ӵ���(UI_WNDSTYLE_CHILD), ��MoveWindow
		::SetWindowPos(
			this->m_hWnd, HWND_TOP,
			rtLayoutRate.left,
			rtLayoutRate.top,
			m_rtMeOrg.right - m_rtMeOrg.left,
			m_rtMeOrg.bottom - m_rtMeOrg.top,
			SWP_NOSIZE);

		//SetWindowPos(this->GetHWND(), NULL, rtLayout.left, rtLayoutRate.bottom, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOREDRAW);

		::ShowWindow(this->GetHWND(), m_bPlay ? SW_SHOW : SW_HIDE);
		if (!m_bPlay)
		{
			::PostMessageW(this->GetHWND(), WM_CLOSE, 0, 0);
		}
	} while (0);

	return S_OK;
}

void CDlgSlidingShow::ThreadProcStart()
{
	if (!m_ThreadManager.IsNeedQuitThread()
		&& !m_ThreadManager.IsThreadRunning())
	{
		m_ThreadManager.SetThreadHandle((HANDLE)_beginthreadex(NULL, 0, &CDlgSlidingShow::ThreadProc, (void*)this, 0, NULL));
	}
}

void CDlgSlidingShow::ThreadProcEnd()
{
	m_ThreadManager.StopThread(TRUE, L"m_ThreadManager");
}

UINT WINAPI CDlgSlidingShow::ThreadProc(void* pParam)
{
	UINT    uRc = S_FALSE;

	do
	{
		if (NULL == pParam)
			break;

		uRc = ((CDlgSlidingShow*)pParam)->ThreadProc();
	} while (0);

	return uRc;
}

UINT WINAPI CDlgSlidingShow::ThreadProc()
{
	do
	{
		::Sleep(TIMER_DELAY_PLAY);

		if (m_ThreadManager.IsNeedQuitThread())
			break;

		/// ��ʱ������Ϣ, ����Ϣ������
		if (m_answer&&m_IsAnswer)
		{
			::PostMessageW(this->GetHWND(), WM_SLIDING_QUIT, 0, 0);
		}
		else
		{
			::PostMessageW(this->GetHWND(), WM_SLIDING_SHOW, 0, 0);
		}

	} while (1);
	return S_OK;
}

void CDlgSlidingShow::SetOwner(CWordCardMain* pDlg)
{
	m_pOwner = pDlg;
}

LRESULT CDlgSlidingShow::WndMessageProc_SlidingShow(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/// ʵ��һ����ģ̬���ڴ�ֱ������ʾ��ָ�����ؼ���������
	/// * ���µ��ϣ�����С�߶ȵ����߶�
	/// * Ȼ���ȶ���ʾx��
	/// * Ȼ����ϵ���, �����߶ȵ���С�߶�
	/// * ��ʧ

	RECT    rtMain;         ///< ������size
	RECT    rtLayout;       ///< 100%��ʾ�ڸ��ؼ��·��Ĵ�С
	RECT    rtLayoutRate;   ///< ���ڻ���ʱ, ��ͨ�����µ���ʾsize
	const   UINT uDispTimeMax = 1000 * 6;   ///< ������ʾ���ȶ���ʾʱ��

											/// ��ֱ��ʾ���ʲ���ֵ, ���ƴ��ڻ����ٶ�
											/// �����趨��ÿ10MS, ִ��һ�� Play()
	const   float fDispStep = 0.20f;
	const   UINT uMinHeight = 10;           ///< ��С���ڸ߶�

											/// ��С�߶����Ƶ�Ӧ��, ��ʼ��ʾ�ͽ�����ʾ������С�߶�
	const   float fRateMin = 0.05f;    ///< fRateMin ���ʶ�Ӧ��С���ڸ߶�
	static  UINT uDispTimeTotal = 0;    ///< �ȶ���ʾ��ʱ�����

	do
	{
		if (!m_bPlay)
			break;

		if (eDispStatus_inc == m_Status)
		{
			m_fRate += fDispStep;
			if (m_fRate > 1.0f)
			{
				m_fRate = 1.0f;
				uDispTimeTotal = 0;
				m_Status = eDispStatus_disp;
			}
			else if (m_fRate < fRateMin)
				m_fRate = fRateMin;
		}
		//else if (eDispStatus_disp == m_Status)
		//{
		//	uDispTimeTotal += TIMER_DELAY_PLAY;
		//	if (uDispTimeTotal >= uDispTimeMax)
		//		m_Status = eDispStatus_dec;
		//}
		//else if (eDispStatus_dec == m_Status)
		//{
		//	m_fRate -= fDispStep;
		//	if (m_fRate <= fRateMin)
		//	{
		//		m_fRate = fRateMin;
		//		uDispTimeTotal = 0;
		//		m_Status = eDispStatus_inc;
		//		m_bPlay = FALSE;
		//	}
		//}

		// ::GetWindowRect(m_pOwner->GetHWND(), &rtMain);
		//::GetClientRect(m_pOwner->GetHWND(), &rtMain);
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rtMain, NULL);
		//::GetWindowRect()
		rtLayout.right = rtMain.right;
		rtLayout.bottom = rtMain.bottom;
		rtLayout.left = rtLayout.right - (m_rtMeOrg.right - m_rtMeOrg.left);
		rtLayout.top = rtLayout.bottom - (m_rtMeOrg.bottom - m_rtMeOrg.top);

		rtLayoutRate.left = rtLayout.left;
		rtLayoutRate.right = rtLayout.right;
		rtLayoutRate.bottom = rtLayout.bottom- 1.0f * (rtLayout.bottom - rtLayout.top) * m_fRate+300;
		rtLayoutRate.top = rtLayout.bottom - 1.0f * (rtLayout.bottom - rtLayout.top) * m_fRate;

		/// ���������ư� ? �޷����óɺܵ͵ĸ߶�, ����С�߶�����...
		/// ���� OnGetMinMaxInfo Ҳû��
		/// ͨ������, ������С�߶�Ϊ10����

		/// ��Ϊ�����ߵķ��Ӵ���(~UI_WNDSTYLE_CHILD), ��SetWindowPos
		/// ����Ƿ��Ӵ���, λ��ҲҪ����ʵ��һ��, ������::GetClientRect.
		/// Ҫ��::GetWindowRect
		//         ::SetWindowPos(
		//             this->GetHWND(), 
		//             m_pOwner->GetHWND(), 
		//             rtLayoutRate.left, 
		//             rtLayoutRate.top, 
		//             rtLayoutRate.right - rtLayoutRate.left, 
		//             rtLayoutRate.bottom - rtLayoutRate.top, 
		//             SWP_SHOWWINDOW);

		/// ��Ϊ�����ߵ��Ӵ���(UI_WNDSTYLE_CHILD), ��MoveWindow
		::SetWindowPos(
			this->m_hWnd, HWND_TOP,
			rtLayoutRate.left,
			rtLayoutRate.top,
			m_rtMeOrg.right - m_rtMeOrg.left,
			m_rtMeOrg.bottom - m_rtMeOrg.top,
			SWP_NOSIZE);

		//SetWindowPos(this->GetHWND(), NULL, rtLayout.left, rtLayoutRate.bottom, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOREDRAW);

		::ShowWindow(this->GetHWND(), m_bPlay ? SW_SHOW : SW_HIDE);
		if (!m_bPlay)
		{
			::PostMessageW(this->GetHWND(), WM_CLOSE, 0, 0);
		}
	} while (0);

	return S_OK;
}