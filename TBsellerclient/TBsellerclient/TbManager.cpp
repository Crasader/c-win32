#include "stdafx.h"
#include "TbManager.h"
#include <iostream>
#include <fstream>
#include <string>

CTbDuilibManager::CTbDuilibManager()
{
	std::wcout.imbue(std::locale("chs"));
	//LoadFile();
	
}

CTbDuilibManager::~CTbDuilibManager()
{
	delete this;
}

CDuiString CTbDuilibManager::GetSkinFolder()
{
 return _T("skin\\test_01");
 }
CDuiString CTbDuilibManager::GetSkinFile()
{
	return _T("test_02.xml");
}

LPCTSTR CTbDuilibManager::GetWindowClassName(void) const
{
	return _T("DIOWinClass");

}


LRESULT CTbDuilibManager::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	LRESULT lRes = 0;
	switch (uMsg)
	{
	case WM_ADDITEM:
		//lRes = OnAddListItem(uMsg, wParam, lParam, bHandled);
	{
		CListTextElementUI* pListElement = (CListTextElementUI*)lParam;
		CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("domainlist"));
		pList->Add(pListElement);
		pListElement->SetText(1, m_domain.back().c_str());
		pListElement->SetText(2, m_desc.back().c_str());
	}
		break;
	default:
		break;
	}
	if (bHandled) return lRes;
	if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return __super::MessageHandler(uMsg, wParam, lParam, bHandled);
}


void CTbDuilibManager::Notify(TNotifyUI & msg)
{
	if (msg.sType == _T("windowinit"))
	{
		OnInitWnd();
	}
	 if (msg.sType == _T("click"))
	{
		OnClick(msg);
	}

}

void CTbDuilibManager::OnFinalMessage(HWND hWnd)
{
	CWindowWnd::OnFinalMessage(hWnd);
}


void CTbDuilibManager::OnClick(TNotifyUI & msg)
{
	if (msg.pSender->GetName().Find(_T("closebtn2")) != -1)
	{
		SaveFile();
		Close(IDCLOSE);
		return;
	}
	// 最小化按钮
	else if (msg.pSender->GetName().Find(_T("minbtn1")) != -1) {
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return;
	}
	else if (msg.pSender->GetName().Find(_T("Addbtn"))!=-1)
	{
		AddItem();
		return;
	}

}


void CTbDuilibManager::ShowAllWeb()
{
	CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("domainlist"));
	vector<wstring>::iterator it2 = m_desc.begin();
	for (auto& it:m_domain)
	{
		CListTextElementUI* pListElement = new CListTextElementUI;
		pList->Add(pListElement);
		pListElement->SetText(1, it.c_str());
		it2++;
		pListElement->SetText(2, it2->c_str());
	}
}

bool CTbDuilibManager::LoadFile()
{
	bool ret = false;
	m_hFile = CreateFileW(L"BPwebList.dat", GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFilePos=0, dwFileSize;
		dwFileSize = GetFileSize(m_hFile, NULL);
		if (dwFileSize != 0)
		{
			do
			{
				WebInfo* info=new WebInfo;
				DWORD dwRes;
				ReadFile(m_hFile, info, sizeof(WebInfo), &dwRes, NULL);
				m_domain.push_back(info->URL);
				m_desc.push_back(info->Confirm);
				dwFilePos += sizeof(WebInfo);
			} while (dwFilePos < dwFileSize);
			ret = true;
		}
	}
	CloseHandle(m_hFile);
	return ret;

}

void CTbDuilibManager::SaveFile()
{

	m_hFile = CreateFileW(L"BPwebList.dat", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		int num = m_domain.size();
		for (int i = 0; i < num;i++)
		{
			WebInfo* Infotmp=new WebInfo;
			wcscpy_s(Infotmp->URL, m_domain.back().c_str());
			wcscpy_s(Infotmp->Confirm, m_desc.back().c_str());
			
			DWORD dwRes;
			
			WriteFile(m_hFile, Infotmp, sizeof(WebInfo), &dwRes, NULL);
			m_domain.pop_back();
			m_desc.pop_back();
		}
	}
	CloseHandle(m_hFile);
	//wofstream outfile;
	//outfile.open("BPwebList.txt");
	//auto it = m_domain.end()-1;
	//auto it2 = m_desc.end()-1;
	//for (int i = 0; i < m_domain.size(); i++)
	//{
	//	WebInfo infotmp;
	//	infotmp.URL = *it;
	//	infotmp.Confirm = *it2;
	//	it--;
	//	it2--;
	//	//outfile.write(reinterpret_cast<char*>(&infotmp), sizeof(WebInfo));
	//}
	//outfile.close();
}

void CTbDuilibManager::AddItem()
{

	CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("domainlist"));
	CEditUI* pEdit = (CEditUI*)m_PaintManager.FindControl(_T("URLbox"));
	CEditUI* pEditConfirm = (CEditUI*)m_PaintManager.FindControl(_T("YNBox"));
	
	//CDuiString tmp = pEdit->GetText();
	m_domain.push_back(pEdit->GetText().GetData());
	m_desc.push_back(pEditConfirm->GetText().GetData());

	CListTextElementUI* pListElement = new CListTextElementUI;
	if (pListElement!=NULL)
	{
		::PostMessage(m_hWnd, WM_ADDITEM, 0L, (LPARAM)pListElement);
	}	
}

void CTbDuilibManager::OnInitWnd()
{
	//ShowAllWeb();
	LoadFile();
	CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("domainlist"));
	auto it = m_domain.end() - 1;
	auto it2 = m_desc.end() - 1;
	for (int i = 0; i < m_domain.size(); i++)
	{
		CListTextElementUI* pListElement = new CListTextElementUI;
		pList->Add(pListElement);
		pListElement->SetText(1, it->c_str());
		pListElement->SetText(2, it2->c_str());
		it--;
		it2--;
	}

}

//LRESULT CTbDuilibManager::OnAddListItem(UINT uMsg, WPARAM wParam, LPARAM lParam, bool & bHandled)
//{
//	CListTextElementUI* pListElement = (CListTextElementUI*)lParam;
//	CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("domainlist"));
//	pList->Add(pListElement);
//	pListElement->SetText(1,m_domain.back());
//	pListElement->SetText(2, m_desc.back());
//	return 0;
//}



