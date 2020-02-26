// HotKey.h : main header file for the HOTKEY application
//

#if !defined(AFX_HOTKEY_H__CAC8E0F1_5634_490B_BF9D_18C5192BA25D__INCLUDED_)
#define AFX_HOTKEY_H__CAC8E0F1_5634_490B_BF9D_18C5192BA25D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CHotKeyApp:
// See HotKey.cpp for the implementation of this class
//

class CHotKeyApp : public CWinApp
{
public:
	CHotKeyApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHotKeyApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CHotKeyApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOTKEY_H__CAC8E0F1_5634_490B_BF9D_18C5192BA25D__INCLUDED_)
