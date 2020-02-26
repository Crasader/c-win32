; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CHotKeyDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "HotKey.h"

ClassCount=2
Class1=CHotKeyApp
Class2=CHotKeyDlg

ResourceCount=3
Resource2=IDR_MAINFRAME
Resource3=IDD_HOTKEY_DIALOG

[CLS:CHotKeyApp]
Type=0
HeaderFile=HotKey.h
ImplementationFile=HotKey.cpp
Filter=N

[CLS:CHotKeyDlg]
Type=0
HeaderFile=HotKeyDlg.h
ImplementationFile=HotKeyDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CHotKeyDlg



[DLG:IDD_HOTKEY_DIALOG]
Type=1
Class=CHotKeyDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_BTN_SAVE,button,1342242816
Control5=IDC_HOTKEY1,msctls_hotkey32,1350631424

