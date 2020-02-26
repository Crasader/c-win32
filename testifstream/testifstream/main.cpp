
#include <iostream>
#include <tchar.h>
#include <windows.h>
#include <fstream>
#include <string>
#include <locale>

using namespace std;
struct WebInfo
{
	wchar_t URL[256];
	wchar_t Confirm[2];
	WebInfo() {
		
	}
};
int main()
{
	////ofstream
	//wofstream outfile("BPwebList.txt",ios::out|ios::binary);
	////outfile.open;
	//WebInfo tmptext;
	//tmptext.URL.assign(_T("www"));
	//tmptext.Confirm.assign(_T("Y"));
	//WebInfo* info1 = new WebInfo;
	//for (int i = 0; i < 10; i++)
	//{
	//	//outfile << tmptext.URL << tmptext.Confirm;
	//	//outfile.write( tmptext.URL.c_str(), tmptext.URL.size());
	//	//outfile << std::endl;
	//	
	//	info1->URL.assign(_T("www.baidu.com"));
	//	info1->Confirm.assign(_T("Y"));
	//	outfile.write((wchar_t*)info1, sizeof(WebInfo));
	//}
	//outfile.close();


	//std::wcout.imbue(std::locale("chs"));
	//wifstream datafile("BPwebList.txt", ios::in | ios::binary);
	//if (!datafile.is_open())
	//{
	//	//cout << "Error opening file"; ;
	//	
	//}
	//
	//else
	//{
	//	WebInfo* info2 = new WebInfo;
	//	while (!datafile.eof())
	//	{
	//		
	//		//wstring info1;
	//		datafile.read((wchar_t*)(info2), sizeof(WebInfo));
	//		
	//		wcout << info2->URL <<info2->Confirm << std::endl;
	//	}
	//}

	//datafile.close();
	//return 0;
	MessageBox(NULL, _T("Test"),_T("Test"), NULL);
	//HANDLE m_hFile;

	////m_hFile = CreateFileW(L"BPwebList.dat", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
	////	CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	////if (m_hFile != INVALID_HANDLE_VALUE)
	////{
	////	for (int i = 0; i <10;i++)
	////	{
	////		WebInfo* Infotmp=new WebInfo;
	////		wcscpy_s(Infotmp->URL, _T("www.bilibili.com"));
	////		wcscpy_s(Infotmp->Confirm, _T("Y"));
	////		//memcpy(tmp, m_domain.back(), sizeof(m_domain.back()));
	////		//memcpy(Infotmp.Confirm, m_desc.back(), sizeof(m_desc.back()));
	////		/*Infotmp.URL = m_domain.back();
	////		Infotmp.Confirm = m_desc.back();*/
	////		/*lstrcpy(Infotmp.URL, m_domain.back());
	////		lstrcpy(Infotmp.Confirm, m_domain.back());*/
	////		
	////		DWORD dwRes;
	////		WriteFile(m_hFile, Infotmp, sizeof(WebInfo), &dwRes, NULL);
	////	}
	////}
	////CloseHandle(m_hFile);


	//bool ret = false;
	//
	//m_hFile = CreateFileW(L"BPwebList.dat", GENERIC_READ, FILE_SHARE_READ,
	//	NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//if (m_hFile != INVALID_HANDLE_VALUE)
	//{
	//	DWORD dwFilePos, dwFileSize;
	//	dwFileSize = GetFileSize(m_hFile, NULL);
	//	if (dwFileSize != 0)
	//	{
	//		do
	//		{
	//			WebInfo* info=new WebInfo;
	//			DWORD dwRes;
	//			ReadFile(m_hFile, info, sizeof(WebInfo), &dwRes, NULL);
	//			dwFilePos = SetFilePointer(m_hFile, 0, NULL, FILE_CURRENT);
	//			SetFilePointer(m_hFile, dwFilePos, NULL, FILE_BEGIN);
	//			wcout << info->URL << info->Confirm<<std::endl;
	//		} while (dwFilePos < dwFileSize);
	//		ret = true;
	//	}
	//}
	//CloseHandle(m_hFile);

	return 0;
	////return ret;
}