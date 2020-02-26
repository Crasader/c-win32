// HotKeyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HotKey.h"
#include "HotKeyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 热键的标识符
#define hotKeyID GlobalAddAtom(_T("MyHotkey"))



/////////////////////////////////////////////////////////////////////////////
// CHotKeyDlg dialog

CHotKeyDlg::CHotKeyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHotKeyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHotKeyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHotKeyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHotKeyDlg)
	DDX_Control(pDX, IDC_HOTKEY1, m_HotKey);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHotKeyDlg, CDialog)
	//{{AFX_MSG_MAP(CHotKeyDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_HOTKEY,OnHotKey)  // 热键的消息映射
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHotKeyDlg message handlers

BOOL CHotKeyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHotKeyDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHotKeyDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


LRESULT CHotKeyDlg::OnHotKey(WPARAM wParam,LPARAM lParam)
{
	if(hotKeyID == (int)wParam)
	{
		AfxMessageBox(_T("按下"));
	}

	return TRUE;
}


/************************************************************************/
/* 保存按钮                                                             */
/************************************************************************/
void CHotKeyDlg::OnBtnSave() 
{
	// 注销原有的热键
	::UnregisterHotKey(m_hWnd,hotKeyID);
	// 获取新的热键
	WORD wvk,wmod;
	m_HotKey.GetHotKey(wvk,wmod);
	// 注册新的热键
	BOOL result=RegisterHotKey(m_hWnd,hotKeyID,wmod,wvk);
	if(!result)
	{
		MessageBox(_T("注册热键失败！"));
	}
}

void CHotKeyDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// 注销热键
	::UnregisterHotKey(m_hWnd,hotKeyID);
}
