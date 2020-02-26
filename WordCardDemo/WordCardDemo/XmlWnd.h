/// @file		XmlWnd.h
/// @brief		��Xml�ļ����ɵĴ������

#ifndef __XML_WND_H__
#define __XML_WND_H__
#include "D:\po\project\WordCardDemo\DuiLib\Utils\WinImplBase.h"
#include "WordCardList.h"
#include "WordCardData.h"
#include <sstream>

class CXmlWnd : public WindowImplBase
{
public:
	CXmlWnd(WCHAR * pcXmlFileName, WCHAR * pcWndClassName);
	virtual ~CXmlWnd();

    void EntryUiInit(IN BOOL bIn) {m_bWasEntryProcUiInit = bIn;}
    BOOL IsEntryUiInit() {return m_bWasEntryProcUiInit;}

    void EntryUiDestory(IN BOOL bIn) {m_bWasEntryProcUiDestory = bIn;}
    BOOL IsEntryUiDestory()
    {
        return m_bWasEntryProcUiDestory;
    }

    void ShowWindowEx(bool bShow = true, bool bTakeFocus = false);
    BOOL IsWindowsWasShowed() {return (SW_HIDE != m_iCmdShow);}

	/// ���ص���Դ������Zipʱ, ������GetSkinFolder, ��Ϊ�Ǵ��麯��, Ҫʵ��һ��
	virtual CDuiString GetSkinFolder(void);

	/// ���µ��麯�������ܵ�����
    virtual BOOL IsNeedIeScrollBar() {return FALSE;}

    virtual LRESULT WndMessageProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual LRESULT SysMessageProc(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    virtual void    OnFinalMessage(HWND hWnd);

    virtual void SaveCurWebPage(){}
    virtual void RestoreCurWebPage(){}
    virtual void SetFocusToWebPage(){}

	virtual CDuiString GetZIPFileName() const;
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual UILIB_RESOURCETYPE GetResourceType() const;
    virtual LPCTSTR GetResourceID() const;
    virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    virtual void SubDlgOnFinalMessage(CXmlWnd* pWnd, BOOL bDeleteMe = FALSE) {}
    
    virtual void GetMsgDispArea(RECT & rt);

    virtual BOOL WebBrowserInit(CWebBrowserUI * pActivexUi) {return FALSE;}
    virtual BOOL WebBrowserUnInit(CWebBrowserUI * pActivexUi) {return FALSE;}
    virtual BOOL WebBrowserNavigate(CWebBrowserUI * pActivexUi, const WCHAR * pcUrl, BOOL bCloseBeforeOpen = FALSE) {return FALSE;}
    virtual void NotifyNavigateComplete2(IDispatch *pDisp, VARIANT *&url) {return;}

	BOOL GetPathName_Me(std::wstring & strPathName);
	BOOL IsStringLastCharMatch(const WCHAR* pcSrc, WCHAR cCharLast)
	{
		size_t          nLen = 0;
		WCHAR           cTemp = L'\0';
		std::wstring    strTemp = L"";

		if (NULL == pcSrc)
			return FALSE;

		nLen = _tcslen(pcSrc);
		if (nLen <= 0)
			return FALSE;

		cTemp = *(pcSrc + nLen - 1);
		return (cTemp == cCharLast);
	}
	BOOL GetFilePathName_Me(std::wstring & strPathName)
	{
		WCHAR szModuleMe[MAX_PATH + 1] = { L'\0' };

		::GetModuleFileName(NULL, szModuleMe, MAX_PATH);
		strPathName = szModuleMe;

		return TRUE;
	}
private:
    BOOL IsInStaticControl(CControlUI * pControl);
    
private:
    int             m_iCmdShow; ///< ���� SW_SHOW or SW_HIDE
	std::wstring	m_strSkinFile;
	std::wstring	m_strWindowClassName;

    BOOL        m_bWasEntryProcUiInit;    ///< ����UI��ʼ������
    BOOL        m_bWasEntryProcUiDestory;   ///< ���봰�����ٴ���
};

#endif
