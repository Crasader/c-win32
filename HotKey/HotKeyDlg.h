// HotKeyDlg.h : header file
//

#if !defined(AFX_HOTKEYDLG_H__DA46A67F_EA4B_4C53_8A9E_450D54538524__INCLUDED_)
#define AFX_HOTKEYDLG_H__DA46A67F_EA4B_4C53_8A9E_450D54538524__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CHotKeyDlg dialog

class CHotKeyDlg : public CDialog
{
// Construction
public:
	CHotKeyDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CHotKeyDlg)
	enum { IDD = IDD_HOTKEY_DIALOG };
	CHotKeyCtrl	m_HotKey;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHotKeyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CHotKeyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnHotKey(WPARAM wParam,LPARAM lParam);  // 热键消息响应函数的声明
	afx_msg void OnBtnSave();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOTKEYDLG_H__DA46A67F_EA4B_4C53_8A9E_450D54538524__INCLUDED_)
