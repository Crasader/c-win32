/// @file		XmlWnd.cpp

#include "stdafx.h"
#include <algorithm>
#include "XmlWnd.h"

#define IDR_BIN_SKIN_ZIP                129

CXmlWnd::CXmlWnd(WCHAR * pcXmlFileName, WCHAR * pcWndClassName)
{                  
    EntryUiInit(FALSE);
    EntryUiDestory(FALSE);

    m_iCmdShow = SW_SHOW;
	m_strSkinFile = L"";
	m_strWindowClassName = L"";

	if (NULL != pcXmlFileName)
		m_strSkinFile = pcXmlFileName;

	if (NULL != pcWndClassName)
		m_strWindowClassName = pcWndClassName;
}

CXmlWnd::~CXmlWnd()
{
}

CDuiString CXmlWnd::GetSkinFolder(void)
{
    std::wstring strSkinFolder = L"";

   GetPathName_Me(strSkinFolder);
    strSkinFolder += L"skin\\";

    return strSkinFolder.c_str();
}

CDuiString CXmlWnd::GetSkinFile()
{
	return m_strSkinFile.c_str();
}

LPCTSTR CXmlWnd::GetWindowClassName(void) const
{
	return m_strWindowClassName.c_str();
}

UILIB_RESOURCETYPE CXmlWnd::GetResourceType() const
{
	return UILIB_FILE;
}

LPCTSTR CXmlWnd::GetResourceID() const
{
    return MAKEINTRESOURCE(IDR_BIN_SKIN_ZIP);
}

CDuiString CXmlWnd::GetZIPFileName() const
{
    /// 从资源载入skin.zip, 不需要经过这里
    _ASSERT(0);
    return L"";
}

LRESULT CXmlWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    POINT           pt;
    RECT            rcClient;
    RECT            rcCaption;
    CControlUI *    pControl = NULL;

    rcCaption = m_PaintManager.GetCaptionRect();
    GetClientRect(m_PaintManager.GetPaintWindow(), &rcClient);
    pt.x = GET_X_LPARAM(lParam);
    pt.y = GET_Y_LPARAM(lParam);

    ::ScreenToClient(m_PaintManager.GetPaintWindow(), &pt);

    /// xml中描述bottom为-1 或默认没设置时，整个窗口区域都可以拖动
    if ((-1 == rcCaption.bottom)
        || (0 == rcCaption.bottom))
    {
        rcCaption.bottom = rcClient.bottom;
    }

    if ((pt.x >= rcClient.left)
        && (pt.x < rcClient.right)
        && (pt.y >= rcCaption.top) 
        && (pt.y < rcCaption.bottom)) 
    {
        pControl = m_PaintManager.FindControl(pt);
        if (IsInStaticControl(pControl))
        {
            return HTCAPTION;
        }
    }

    return __super::OnNcHitTest(uMsg, wParam, lParam, bHandled);
}

BOOL CXmlWnd::GetPathName_Me(std::wstring & strPathName)
{
	BOOL bRc = FALSE;
	std::wstring strTmp = L"";
	std::wstring::size_type pos = std::wstring::npos;

	do
	{
		strPathName = L"";
		if (!GetFilePathName_Me(strTmp))
		{
			break;
		}

		pos = strTmp.rfind(L'\\');
		if (std::wstring::npos == pos)
			break;

		strPathName = strTmp.substr(0, pos);
		if (!IsStringLastCharMatch(strPathName.c_str(), L'\\'))
		{
			strPathName += L"\\";
		}

		bRc = TRUE;
	} while (0);

	return bRc;
}

BOOL CXmlWnd::IsInStaticControl(CControlUI * pControl)
{
	CDuiString strClassName;
	std::vector<CDuiString> vctStaticName;
	std::vector<CDuiString>::iterator it;

	if (NULL == pControl)
		return FALSE;

	strClassName = pControl->GetClass();
	strClassName.MakeLower();
	vctStaticName.push_back(L"controlui");
	vctStaticName.push_back(L"textui");
	vctStaticName.push_back(L"labelui");
	vctStaticName.push_back(L"containerui");
	vctStaticName.push_back(L"horizontallayoutui");
	vctStaticName.push_back(L"verticallayoutui");
	vctStaticName.push_back(L"tablayoutui");
	vctStaticName.push_back(L"childlayoutui");
	vctStaticName.push_back(L"dialoglayoutui");

	it = std::find(vctStaticName.begin(), vctStaticName.end(), strClassName);
	return (it != vctStaticName.end());
}

LRESULT CXmlWnd::WndMessageProc (UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    POINTS          pt;
    int             iWidth = 0;
    int             iHeight = 0;
    WINDOWPOS*      pWndPos = NULL;

    DWORD   dwMsgSn = 0;
    LRESULT lRc = 0;

    switch (uMsg)
    {
    case WM_SYSCOMMAND:
        {
            dwMsgSn = wParam & 0xfff0; 
            switch (dwMsgSn)
            {
            case SC_MAXIMIZE: /// not allow maximize, because after maximize, ui layout is wrong 
                return S_OK;
                break;

            case SC_KEYMENU: ///< 禁止 ALT+SPACE 在程序主界面上出现 系统菜单
                return S_OK;
                break;

            default:
                break;
            }
        }
        break;

    case WM_SIZE:
        {
            if ((SIZE_RESTORED == wParam)
                || (SIZE_MAXIMIZED == wParam)
                || (SIZE_MAXSHOW == wParam))
            {
                iWidth = GET_X_LPARAM(lParam);
                iHeight = GET_Y_LPARAM(lParam);
            }
        }
        break;

    case WM_MOVE:
        {
            pt = MAKEPOINTS(lParam);
        }
        break;

    default:
        break;
    }

    /// 防止编辑框 CEditUI 不能输入字符
//     if (m_PaintManager.SysMessageProc(uMsg, wParam, lParam, lRc)) 
//     {
//         return lRc;
//     }

    return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CXmlWnd::SysMessageProc(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
    return SysMessageProc(uMsg, wParam, lParam, bHandled);
}

void CXmlWnd::OnFinalMessage(HWND hWnd)
{
    __super::OnFinalMessage(hWnd);
}

void CXmlWnd::GetMsgDispArea(RECT & rt)
{
    ::ZeroMemory(&rt, sizeof(RECT));
}

void CXmlWnd::ShowWindowEx(bool bShow /*= true*/, bool bTakeFocus /*= false*/)
{
    ShowWindow(bShow, bTakeFocus);
    m_iCmdShow = bShow ? SW_SHOW : SW_HIDE;
}
