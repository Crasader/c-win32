#include "stdafx.h"
#include "WordCardMain.h"
#include "constDefine_BusinessLayer.h"
#include "ThreadManager.h"

const TCHAR* const kCloseButtonControlName = _T("closebtn");

const TCHAR* const kStartFuncControlName = _T("startfuncbtn");

const TCHAR* const kTgtTimeControlName = _T("tgttimeedit");

const TCHAR* const kWordCardListControlName = _T("wordcardlist");

const TCHAR* const kHideWordCardMeanBtnControlName = _T("hidewordmeanbtn");

const TCHAR* const kHideWordBtnControlName = _T("hidewordbtn");

const TCHAR* const kResortBtnControlName = _T("reshowbtn");

CWordCardData g_WordCard;

std::vector<std::string> domain;

std::vector<std::string>  desc;

#define WM_ADDLISTITEM WM_USER + 50

#define hotKeyID GlobalAddAtom(_T("MyHotkey"))


struct Prama
{
	HWND hWnd;
	CListUI* pList;
	
};


CWordCardMain::CWordCardMain()
{
}


CWordCardMain::~CWordCardMain()
{
	PostQuitMessage(0);
}

void CWordCardMain::ThreadProcStart()
{
	if (!m_ThreadManager.IsNeedQuitThread()
		&& !m_ThreadManager.IsThreadRunning())
	{
		m_ThreadManager.SetThreadHandle((HANDLE)_beginthreadex(NULL, 0, &CWordCardMain::ThreadProc, (void*)this, 0, NULL));
	}
}

void CWordCardMain::ThreadProcEnd()
{
	m_ThreadManager.StopThread(TRUE, L"m_ThreadManager");
}

UINT CWordCardMain::ThreadProc(void * pParam)
{
	UINT    uRc = S_FALSE;

	do
	{
		if (NULL == pParam)
			break;

		uRc = ((CWordCardMain*)pParam)->ThreadProc();
	} while (0);

	return uRc;
}

UINT CWordCardMain::ThreadProc()
{
	do 
	{
		::Sleep(TIMER_DELAY_PLAY);

		if (m_ThreadManager.IsNeedQuitThread())
			break;
		if (!CheckFullScreen())
		{
			if (m_func)
			{
				if (m_memoryFunc.IsShowtime())
				{
					WordCardBtnGroup btngroup;
					WordCardShow wordcardshow, newwordcard;
					time_t wordtime;
					CreateGroupBtn(btngroup);
					CreateDlgSlidingShow(btngroup);
					newwordcard.index = m_wordindex;
					time(&wordtime);
					newwordcard.memorytime = wordtime;
					newwordcard.memorycount = 0;
					m_memoryFunc.InsertWordCard(newwordcard);
					m_memoryFunc.ClassifyWordCard();

					while (m_memoryFunc.ShowNextWordCard(wordcardshow))
					{
						WordCardBtnGroup otherbtngroup;
						CreateGroupTgtBtn(wordcardshow.index, otherbtngroup);
						CreateDlgSlidingShow(otherbtngroup);
					}
				}
			}
		}
		
		Sleep(10);
		//if()
	} while (1);
	return S_OK;
}

LPCTSTR CWordCardMain::GetWindowClassName() const
{
	return _T("DioWordCard");
}

void CWordCardMain::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void CWordCardMain::InitWindow()
{
}

LRESULT CWordCardMain::ResponseDefaultKeyEvent(WPARAM wParam)
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

CDuiString CWordCardMain::GetSkinFile()
{
	return _T("wordcardmain.xml");
}

CDuiString CWordCardMain::GetSkinFolder()
{
	return (CPaintManagerUI::GetInstancePath());
}

UILIB_RESOURCETYPE CWordCardMain::GetResourceType() const
{
	return UILIB_FILE;
}

CControlUI * CWordCardMain::CreateControl(LPCTSTR pstrClass)
{
	//if (_tcsicmp(pstrClass, _T("WordCardList")) == 0)
	//{
	//	//return new CWordCardUI(m_PaintManager);
	//}
	return NULL;
}

LRESULT CWordCardMain::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return 0;
}

LRESULT CWordCardMain::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	return LRESULT();
}

LRESULT CWordCardMain::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	bHandled = FALSE;
	return 0;
}

void CWordCardMain::UpdateWordCardList()
{
	struct Prama * prama = new Prama;

	CListUI* pList = static_cast<CListUI*> (m_PaintManager.FindControl(kWordCardListControlName));
	pList->RemoveAll();
	
	DWORD dwThreadID = 0;
	pList->SetTextCallback(this);

	prama->hWnd = GetHWND();
	prama->pList = pList;

	HANDLE hThread = CreateThread(NULL, 0, &CWordCardMain::ShowWordCardList, (LPVOID)prama, 0, &dwThreadID);

}

DWORD CWordCardMain::ShowWordCardList(LPVOID lpParameter)
{
	try
	{
		struct Prama* prama = (struct Prama*)lpParameter;
		CListUI* pList = prama->pList;
		int i = 0;
		for (auto m_wordcard:g_WordCard.GetData())
		{
			domain.push_back(m_wordcard.word);
			desc.push_back(m_wordcard.wordmean);
			CListTextElementUI* pListElement = new CListTextElementUI;
			pListElement->SetTag(i);
			if (pListElement!=NULL)
			{
				::PostMessage(prama->hWnd, WM_ADDLISTITEM, 0L, (LPARAM)pListElement);
			}
			i++;
		}
		delete prama;

		return 0;
	}
	catch (...)
	{
		
		return 0;
	}
}

LPCTSTR CWordCardMain::GetItemText(CControlUI * pControl, int iIndex, int iSubItem)
{
	TCHAR szBuf[MAX_PATH] = { 0 };
	switch (iSubItem)
	{
	//case 0:
	//	_stprintf(szBuf, _T("%d"), iIndex);
	//	break;
	case 0:
	{
#ifdef _UNICODE	
		if (!m_IsEng)
		{
			int iLen = domain[iIndex].length();
			if (iLen != 0)
			{
				LPWSTR lpText = new WCHAR[iLen + 1];
				::ZeroMemory(lpText, (iLen + 1) * sizeof(WCHAR));
				::MultiByteToWideChar(CP_ACP, 0, domain[iIndex].c_str(), -1, (LPWSTR)lpText, iLen);
				_stprintf(szBuf, lpText);
				delete[] lpText;
#else
		_stprintf(szBuf, domain[iIndex].c_str());

#endif
			}
	}
		else
		{
			_stprintf(szBuf, L"");
		}
		

	}
	break;
	case 1:
	{
#ifdef _UNICODE		
		if (!m_IsChs)
		{
			int iLen = desc[iIndex].length();
			if (iLen != 0)
			{
				LPWSTR lpText = new WCHAR[iLen + 1];
				::ZeroMemory(lpText, (iLen + 1) * sizeof(WCHAR));
				::MultiByteToWideChar(CP_ACP, 0, desc[iIndex].c_str(), -1, (LPWSTR)lpText, iLen);
				_stprintf(szBuf, lpText);
				delete[] lpText;
#else
		_stprintf(szBuf, desc[iIndex].c_str());
#endif
			}
		}
		else
		{
			_stprintf(szBuf, L"");
		}
	}
	break;
	}
	pControl->SetUserData(szBuf);
	return pControl->GetUserData();
}

LRESULT CWordCardMain::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg)
	{
	case WM_CREATE:			lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:			lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:		lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
#if defined(WIN32) && !defined(UNDER_CE)
	case WM_NCACTIVATE:		lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:		lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:		lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:		lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_GETMINMAXINFO:	lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	case WM_MOUSEWHEEL:		lRes = OnMouseWheel(uMsg, wParam, lParam, bHandled); break;
#endif
	case WM_SIZE:			lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_CHAR:		lRes = OnChar(uMsg, wParam, lParam, bHandled); break;
	case WM_SYSCOMMAND:		lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
	case WM_KEYDOWN:		lRes = OnKeyDown(uMsg, wParam, lParam, bHandled); break;
	case WM_KILLFOCUS:		lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); break;
	case WM_SETFOCUS:		lRes = OnSetFocus(uMsg, wParam, lParam, bHandled); break;
	case WM_LBUTTONUP:		lRes = OnLButtonUp(uMsg, wParam, lParam, bHandled); break;
	case WM_LBUTTONDOWN:	lRes = OnLButtonDown(uMsg, wParam, lParam, bHandled); break;
	case WM_MOUSEMOVE:		lRes = OnMouseMove(uMsg, wParam, lParam, bHandled); break;
	case WM_MOUSEHOVER:	lRes = OnMouseHover(uMsg, wParam, lParam, bHandled); break;
	case WM_ADDLISTITEM: lRes = OnAddListItem(uMsg, wParam, lParam, bHandled); break;
	case WM_HOTKEY: lRes = OnHotKey(uMsg, wParam, lParam, bHandled); break;
	default:				bHandled = FALSE; break;
	}
	if (bHandled) return lRes;

	lRes = HandleCustomMessage(uMsg, wParam, lParam, bHandled);
	if (bHandled) return lRes;

	if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes))
		return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CWordCardMain::OnAddListItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	CListTextElementUI* pListElement = (CListTextElementUI*)lParam;
	CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("wordcardlist")));
	if (pList) pList->Add(pListElement);
	return 0;
}

LRESULT CWordCardMain::OnHotKey(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	if (wParam == hotKeyID)
	{
		if (m_func==TRUE)
		{
			//MessageBoxW(NULL,_T)

			m_func = FALSE;
			m_hotkey = TRUE;
		}
		else
		{
			m_func = TRUE;
			m_hotkey = FALSE;

		}
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		m_pWordCardHintShow = new CWordCardHintShow(
			XML_FILE_NAME_dlgHint_MainDlg,
			WND_CLASS_NAME_digHint_MainDlg, m_hotkey);
		m_pWordCardHintShow->SetOwner(this);
		DWORD dwStyle = WS_SYSMENU | WS_POPUP/*| UI_WNDSTYLE_CHILD*/;
		DWORD dwExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_LAYERED;
		m_pWordCardHintShow->Create(
			NULL,
			WND_DISP_NAME_dlgSlidingShow_MainDlg,
			dwStyle, dwExStyle
		);
		m_pWordCardHintShow->CenterWindow();
		m_pWordCardHintShow->ShowModal();
	
	}
	
	return LRESULT();
}

LRESULT CWordCardMain::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	
	RegisterHotKey(m_hWnd, hotKeyID, MOD_ALT, '1');
	return WindowImplBase::OnCreate(uMsg, wParam, lParam,bHandled);
}

LRESULT CWordCardMain::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	::UnregisterHotKey(m_hWnd, hotKeyID);
	return WindowImplBase::OnDestroy(uMsg,wParam,lParam,bHandled);
}

void CWordCardMain::CreateDlgSlidingShow(_In_ const WordCardBtnGroup& btngroup)
{
	
	do
	{
		RECT    rtMain;
		if (NULL != m_pDlgSlidingShow)
			break;
		/*WordCardBtnGroup wordcardgroup;
		CreateGroupBtn(wordcardgroup);*/
		m_pDlgSlidingShow = new CDlgSlidingShow(
			btngroup,
			XML_FILE_NAME_dlgSlidingShow_MainDlg,
			WND_CLASS_NAME_dlgSlidingShow_MainDlg);

		if (NULL == m_pDlgSlidingShow)
			break;

		m_pDlgSlidingShow->SetOwner(this);
		DWORD dwStyle = WS_SYSMENU | WS_POPUP/*| UI_WNDSTYLE_CHILD*/;
		DWORD dwExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
		m_pDlgSlidingShow->Create(
			NULL,
			WND_DISP_NAME_dlgSlidingShow_MainDlg,
			dwStyle,dwExStyle
			);
		//m_pDlgSlidingShow->CenterWindow();
		/// 先藏起来, 让滑动框自己滑动和显示隐藏, 要不起始位置不对  
		/// 所有者没必要之道创建对象的起始条件
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rtMain, NULL);
		::SetWindowPos(m_pDlgSlidingShow->GetHWND(), HWND_TOP, rtMain.right, rtMain.bottom,
			-1,-1, SWP_HIDEWINDOW| SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

		m_pDlgSlidingShow->ShowModal();
	
	} while (0);
}

void CWordCardMain::Notify(TNotifyUI & msg)
{
	if (_tcsicmp(msg.sType, _T("windowinit")) == 0)
	{
		OnPrepare(msg);
	}
	else if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), kCloseButtonControlName) == 0)
		{
			OnExit(msg);
		}
		if (_tcsicmp(msg.pSender->GetName(),kStartFuncControlName)==0)
		{
			CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Tgttimeedit")));
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
			m_func = TRUE;
			
		/*	WordCardBtnGroup btngroup;
			CreateGroupBtn(btngroup);
			CreateDlgSlidingShow(btngroup);*/
#if defined(UNDER_CE)
			::ShowWindow(m_hWnd, SW_MINIMIZE);
#else
			

#endif
		}
		if (_tcsicmp(msg.pSender->GetName(),kHideWordCardMeanBtnControlName)==0)
		{
			m_IsChs = TRUE;
			m_IsEng = FALSE;
			UpdateWordCardList();
		}
		if (_tcsicmp(msg.pSender->GetName(), kHideWordBtnControlName) == 0)
		{
			m_IsEng = TRUE;
			m_IsChs = FALSE;
			UpdateWordCardList();
		}
		if (_tcsicmp(msg.pSender->GetName(), kResortBtnControlName) == 0)
		{
			m_IsChs = FALSE;
			m_IsEng = FALSE;
			UpdateWordCardList();
		}
	}
}

void CWordCardMain::OnPrepare(TNotifyUI & msg)
{
	g_WordCard.ReadFile();
	UpdateWordCardList();
	ThreadProcStart();
	m_hotkey = FALSE;
	m_wordindex = m_memoryFunc.WordCardDataSize();
}

void CWordCardMain::OnExit(TNotifyUI & msg)
{
	Close();
}

void CWordCardMain::OnTimer(TNotifyUI & msg)
{
}

void CWordCardMain::OnFuncWork(std::wstring time)
{

}



void CWordCardMain::SubDlgDestory()
{
	m_pDlgSlidingShow = NULL;
}

void CWordCardMain::CreateGroupBtn(WordCardBtnGroup& btngroup)
{
	
	/*srand((unsigned)time(NULL));
	int index = rand()%desc.size();*/
	m_wordindex++;
	btngroup.word = domain[m_wordindex];
	int randnum = (rand() % 4)+1;
	switch (randnum)
	{
	case 1:		//btn1
		btngroup.btn1.wordmean = desc[m_wordindex];
		btngroup.btn1.IsRight = TRUE;
		break;
	case 2:		//btn2
		btngroup.btn2.wordmean = desc[m_wordindex];
		btngroup.btn2.IsRight = TRUE;
		break;
	case 3:		//btn3
		btngroup.btn3.wordmean = desc[m_wordindex];
		btngroup.btn3.IsRight = TRUE;
		break;
	case 4:		//btn4
		btngroup.btn4.wordmean = desc[m_wordindex];
		btngroup.btn4.IsRight = TRUE;
		break;
	default:
		break;
	}
	GetLeftBtn(btngroup,randnum, m_wordindex);
}

void CWordCardMain::CreateGroupTgtBtn(int index, WordCardBtnGroup & btngroup)
{
	btngroup.word = domain[index];
	int randnum = (rand() % 4) + 1;
	switch (randnum)
	{
	case 1:		//btn1
		btngroup.btn1.wordmean = desc[index];
		btngroup.btn1.IsRight = TRUE;
		break;
	case 2:		//btn2
		btngroup.btn2.wordmean = desc[index];
		btngroup.btn2.IsRight = TRUE;
		break;
	case 3:		//btn3
		btngroup.btn3.wordmean = desc[index];
		btngroup.btn3.IsRight = TRUE;
		break;
	case 4:		//btn4
		btngroup.btn4.wordmean = desc[index];
		btngroup.btn4.IsRight = TRUE;
		break;
	default:
		break;
	}
	GetLeftBtn(btngroup, randnum, index);
}

void CWordCardMain::GetLeftBtn(_Out_ WordCardBtnGroup& btngroup, int randbtnnum,int randnum)
{
	srand((unsigned)time(NULL));
	int index = 0;
	for (size_t i=1;i<=4;i++)
	{
		switch (i)
		{
		case 1:	//btn1
			if (i!=randbtnnum)
			{
				do 
				{
					index = rand() % desc.size();
				} while (index== randnum);
				
				btngroup.btn1.wordmean = desc[index];
				btngroup.btn1.IsRight = FALSE;
			}
			break;
		case 2:	//btn2
			if (i != randbtnnum)
			{
				do
				{
					index = rand() % desc.size();
				} while (index == randnum);

				btngroup.btn2.wordmean = desc[index];
				btngroup.btn2.IsRight = FALSE;
			}
			break;
		case 3:	//btn3
			if (i != randbtnnum)
			{
				do
				{
					index = rand() % desc.size();
				} while (index == randnum);

				btngroup.btn3.wordmean = desc[index];
				btngroup.btn3.IsRight = FALSE;
			}
			break;
		case 4:	//btn4
			if (i != randbtnnum)
			{
				do
				{
					index = rand() % desc.size();
				} while (index == randnum);

				btngroup.btn4.wordmean = desc[index];
				btngroup.btn4.IsRight = FALSE;
			}
			break;
		default:
			break;
		}
	}
}

BOOL CWordCardMain::CheckFullScreen()
{
	BOOL bFullScreen = FALSE;
	HWND hWnd = GetForegroundWindow();
	RECT rcApp, rcDesk;
	GetWindowRect(GetDesktopWindow(), &rcDesk);
	if (hWnd!=GetDesktopWindow()&&hWnd!=GetShellWindow())
	{
		GetWindowRect(hWnd, &rcApp);
		if (rcApp.left<=rcDesk.left
			&&rcApp.top<=rcDesk.top
			&&rcApp.right>=rcDesk.right
			&&rcApp.bottom>=rcDesk.bottom)
		{
			bFullScreen = TRUE;
			//MessageBoxW(NULL, _T("全屏了"), _T("全屏了"), MB_OK);
		}
	}
	return bFullScreen;
}
