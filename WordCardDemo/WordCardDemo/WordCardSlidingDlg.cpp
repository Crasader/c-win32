#include "stdafx.h"
#include "WordCardSlidingDlg.h"

#include "constDefine_BusinessLayer.h"

CWordCardSlidingDlg::CWordCardSlidingDlg()
{
}


CWordCardSlidingDlg::~CWordCardSlidingDlg()
{
}

LPCTSTR CWordCardSlidingDlg::GetWindowClassName() const
{
	return _T("DioWordCardSlidingDlg");
}

void CWordCardSlidingDlg::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void CWordCardSlidingDlg::InitWindow()
{
}

LRESULT CWordCardSlidingDlg::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

CDuiString CWordCardSlidingDlg::GetSkinFile()
{
	return _T("SlidingDlg.xml");
}

CDuiString CWordCardSlidingDlg::GetSkinFolder()
{
	return (CPaintManagerUI::GetInstancePath());
}

UILIB_RESOURCETYPE CWordCardSlidingDlg::GetResourceType() const
{
	return UILIB_FILE;
}

CControlUI * CWordCardSlidingDlg::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

LRESULT CWordCardSlidingDlg::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return 0;
}

LRESULT CWordCardSlidingDlg::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	return LRESULT();
}

LRESULT CWordCardSlidingDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CWordCardSlidingDlg::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL    bNeedDefaultProcess = FALSE;
	do
	{
		switch (uMsg)
		{
		case WM_SLIDING_SHOW:
//			WndMessageProc_SlidingShow(uMsg, wParam, lParam);
			bNeedDefaultProcess = FALSE;
			break;

		default:
			bNeedDefaultProcess = TRUE;
			break;
		}

	} while (0);

	return WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}
//
//LRESULT CWordCardSlidingDlg::WndMessageProc_SlidingShow(UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	/// ʵ��һ����ģ̬���ڴ�ֱ������ʾ��ָ�����ؼ���������
//	/// * ���µ��ϣ�����С�߶ȵ����߶�
//	/// * Ȼ���ȶ���ʾx��
//	/// * Ȼ����ϵ���, �����߶ȵ���С�߶�
//	/// * ��ʧ
//
//	RECT    rtMain;         ///< ������size
//	RECT    rtLayout;       ///< 100%��ʾ�ڸ��ؼ��·��Ĵ�С
//	RECT    rtLayoutRate;   ///< ���ڻ���ʱ, ��ͨ�����µ���ʾsize
//	const   UINT uDispTimeMax = 1000 * 6;   ///< ������ʾ���ȶ���ʾʱ��
//
//											/// ��ֱ��ʾ���ʲ���ֵ, ���ƴ��ڻ����ٶ�
//											/// �����趨��ÿ10MS, ִ��һ�� Play()
//	const   float fDispStep = 0.03f;
//	const   UINT uMinHeight = 10;           ///< ��С���ڸ߶�
//
//											/// ��С�߶����Ƶ�Ӧ��, ��ʼ��ʾ�ͽ�����ʾ������С�߶�
//	const   float fRateMin = 0.05f;    ///< fRateMin ���ʶ�Ӧ��С���ڸ߶�
//	static  UINT uDispTimeTotal = 0;    ///< �ȶ���ʾ��ʱ�����
//
//	do
//	{
//		if (!m_bPlay)
//			break;
//
//		if (eDispStatus_inc == m_Status)
//		{
//			m_fRate += fDispStep;
//			if (m_fRate > 1.0f)
//			{
//				m_fRate = 1.0f;
//				uDispTimeTotal = 0;
//				m_Status = eDispStatus_disp;
//			}
//			else if (m_fRate < fRateMin)
//				m_fRate = fRateMin;
//		}
//		else if (eDispStatus_disp == m_Status)
//		{
//			uDispTimeTotal += TIMER_DELAY_PLAY;
//			if (uDispTimeTotal >= uDispTimeMax)
//				m_Status = eDispStatus_dec;
//		}
//		else if (eDispStatus_dec == m_Status)
//		{
//			m_fRate -= fDispStep;
//			if (m_fRate <= fRateMin)
//			{
//				m_fRate = fRateMin;
//				uDispTimeTotal = 0;
//				m_Status = eDispStatus_inc;
//				m_bPlay = FALSE;
//			}
//		}
//
//		// ::GetWindowRect(m_pOwner->GetHWND(), &rtMain);
//		::GetClientRect(m_pOwner->GetHWND(), &rtMain);
//
//		rtLayout.right = rtMain.right;
//		rtLayout.bottom = rtMain.bottom;
//		rtLayout.left = rtLayout.right - (m_rtMeOrg.right - m_rtMeOrg.left);
//		rtLayout.top = rtLayout.bottom - (m_rtMeOrg.bottom - m_rtMeOrg.top);
//
//		rtLayoutRate.left = rtLayout.left;
//		rtLayoutRate.right = rtLayout.right;
//		rtLayoutRate.bottom = rtLayout.bottom;
//		rtLayoutRate.top = rtLayout.bottom - 1.0f * (rtLayout.bottom - rtLayout.top) * m_fRate;
//
//		/// ���������ư� ? �޷����óɺܵ͵ĸ߶�, ����С�߶�����...
//		/// ���� OnGetMinMaxInfo Ҳû��
//		/// ͨ������, ������С�߶�Ϊ10����
//
//		/// ��Ϊ�����ߵķ��Ӵ���(~UI_WNDSTYLE_CHILD), ��SetWindowPos
//		/// ����Ƿ��Ӵ���, λ��ҲҪ����ʵ��һ��, ������::GetClientRect.
//		/// Ҫ��::GetWindowRect
//		//         ::SetWindowPos(
//		//             this->GetHWND(), 
//		//             m_pOwner->GetHWND(), 
//		//             rtLayoutRate.left, 
//		//             rtLayoutRate.top, 
//		//             rtLayoutRate.right - rtLayoutRate.left, 
//		//             rtLayoutRate.bottom - rtLayoutRate.top, 
//		//             SWP_SHOWWINDOW);
//
//		/// ��Ϊ�����ߵ��Ӵ���(UI_WNDSTYLE_CHILD), ��MoveWindow85294614
//		::MoveWindow(
//			this->GetHWND(),
//			rtLayoutRate.left,
//			rtLayoutRate.top,
//			rtLayoutRate.right,
//			rtLayoutRate.bottom,
//			TRUE);
//
//		::ShowWindow(this->GetHWND(), m_bPlay ? SW_SHOW : SW_HIDE);
//		if (!m_bPlay)
//		{
//			::PostMessageW(this->GetHWND(), WM_CLOSE, 0, 0);
//		}
//	} while (0);
//
//	return S_OK;
//}


//void CWordCardSlidingDlg::Notify(TNotifyUI & msg)
//{
//	if (_tcsicmp(msg.sType,_T("windowinit"))==0)
//	{
//		OnPrepare(msg);
//	}
//	//if (_tcsicmp(msg.pSender->GetName()))
//	//{
//	//}
//}

void CWordCardSlidingDlg::OnPrepare(TNotifyUI & msg)
{
}

void CWordCardSlidingDlg::OnExit(TNotifyUI & msg)
{
	Close();
}

void CWordCardSlidingDlg::OnTimer(TNotifyUI & msg)
{
}


