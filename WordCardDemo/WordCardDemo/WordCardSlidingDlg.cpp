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
//	/// 实现一个非模态窗口垂直滑动显示在指定父控件窗口里面
//	/// * 从下到上，从最小高度到最大高度
//	/// * 然后稳定显示x秒
//	/// * 然后从上到下, 从最大高度到最小高度
//	/// * 消失
//
//	RECT    rtMain;         ///< 主窗口size
//	RECT    rtLayout;       ///< 100%显示在父控件下方的大小
//	RECT    rtLayoutRate;   ///< 窗口滑动时, 不通比率下的显示size
//	const   UINT uDispTimeMax = 1000 * 6;   ///< 滑动显示窗稳定显示时间
//
//											/// 垂直显示比率步进值, 控制窗口滑动速度
//											/// 这里设定是每10MS, 执行一次 Play()
//	const   float fDispStep = 0.03f;
//	const   UINT uMinHeight = 10;           ///< 最小窗口高度
//
//											/// 最小高度限制的应对, 开始显示和结束显示的是最小高度
//	const   float fRateMin = 0.05f;    ///< fRateMin 比率对应最小窗口高度
//	static  UINT uDispTimeTotal = 0;    ///< 稳定显示的时间计数
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
//		/// 界面库的限制吧 ? 无法设置成很低的高度, 有最小高度限制...
//		/// 重载 OnGetMinMaxInfo 也没用
//		/// 通过试验, 发现最小高度为10左右
//
//		/// 作为所有者的非子窗口(~UI_WNDSTYLE_CHILD), 用SetWindowPos
//		/// 如果是非子窗口, 位置也要重新实验一下, 不能用::GetClientRect.
//		/// 要用::GetWindowRect
//		//         ::SetWindowPos(
//		//             this->GetHWND(), 
//		//             m_pOwner->GetHWND(), 
//		//             rtLayoutRate.left, 
//		//             rtLayoutRate.top, 
//		//             rtLayoutRate.right - rtLayoutRate.left, 
//		//             rtLayoutRate.bottom - rtLayoutRate.top, 
//		//             SWP_SHOWWINDOW);
//
//		/// 作为所有者的子窗口(UI_WNDSTYLE_CHILD), 用MoveWindow85294614
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


