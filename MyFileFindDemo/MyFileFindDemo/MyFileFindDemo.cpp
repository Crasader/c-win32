// MyFileFindDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <string>
#include <process.h>
#include <vector>
#include "MyFileFind.h"

std::wstring MakeStandardName(const std::wstring &wstDirName)
{
	if (wstDirName.back() != '\\')
		return wstDirName + L'\\';
	return wstDirName;
}
long nNum = 0;
long g_nFindedFIleNum = 0;
long g_nSearchDirNum = 0;
long g_UnWorkThreadNum = 0;
CRITICAL_SECTION g_cs;
std::vector<HANDLE> g_hThreads;
HANDLE g_hExitEvent;

struct ThreadData
{
	std::wstring wstrDirName;
	std::wstring wstrSearch;
	std::wstring wstrFilter;
};

void MyFileFind(std::wstring &wstrBeginDirName, std::wstring &wstrSearch,std::wstring wstrFilter=L"*.*")
{

	WIN32_FIND_DATAW fileFind = { 0 };
	HANDLE hFileFind = FindFirstFileW((MakeStandardName(wstrBeginDirName)+wstrFilter) .c_str(), &fileFind);
	do
	{
		//printf("File name:%ls\r\n", fileFind.cFileName);
		//如果是一个文件夹，要进去再次查找，
		//如果是一个文件，我就进行对比
		if (wcscmp(fileFind.cFileName,L".")==0)
			continue;
		if (wcscmp(fileFind.cFileName,L"..")==0)
			continue;
		if (fileFind.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			MyFileFind(MakeStandardName(wstrBeginDirName) + fileFind.cFileName, wstrSearch);
			g_nSearchDirNum++;
		}
		else
		{
			if (wcsstr(fileFind.cFileName,wstrSearch.c_str())!=nullptr)
			{
				printf("Searched File In:%ls\r\n", (MakeStandardName(wstrBeginDirName)+fileFind.cFileName).c_str());
				nNum++;
				
			}
			g_nFindedFIleNum++;
		}
	} while (FindNextFileW(hFileFind, &fileFind));


}

//unsigned __stdcall ThreadFileFind(void* lParam)
//{
//	InterlockedAdd(&g_UnWorkThreadNum, 1);
//	ThreadData* pData = (ThreadData*)lParam;
//	WIN32_FIND_DATAW fileFind = { 0 };
//	HANDLE hFileFind = FindFirstFileW((MakeStandardName(pData->wstrDirName) + pData->wstrFilter).c_str(), &fileFind);
//	if (hFileFind != INVALID_HANDLE_VALUE)
//	{
//		do
//		{
//			printf("File name:%ls\r\n", fileFind.cFileName);
//			如果是一个文件夹，要进去再次查找，
//			如果是一个文件，我就进行对比
//			if (wcscmp(fileFind.cFileName, L".") == 0)
//				continue;
//			if (wcscmp(fileFind.cFileName, L"..") == 0)
//				continue;
//			if (fileFind.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
//			{
//				ThreadData* pTempData = new ThreadData;
//				pTempData->wstrDirName = MakeStandardName(pData->wstrDirName) + fileFind.cFileName;
//				pTempData->wstrFilter = pData->wstrFilter;
//				pTempData->wstrSearch = pData->wstrSearch;
//				EnterCriticalSection(&g_cs);
//				HANDLE hThread=(HANDLE)_beginthreadex(nullptr, 0, ThreadFileFind, pTempData, 0, nullptr);
//				if (hThread==0)
//				{
//					printf("Error!Error No:%d", errno);
//				}
//				LeaveCriticalSection(&g_cs);
//
//				InterlockedAdd(&g_nSearchDirNum, 1);
//			}
//			else
//			{
//				if (wcsstr(fileFind.cFileName, pData->wstrSearch.c_str()) != nullptr)
//				{
//					printf("Searched File In:%ls\r\n", (MakeStandardName(pData->wstrDirName) + fileFind.cFileName).c_str());
//					InterlockedAdd(&nNum, 1);
//
//				}
//				InterlockedAdd(&g_nFindedFIleNum, 1);
//			}
//		} while (FindNextFileW(hFileFind, &fileFind));
//	}
//	delete pData;
//	if (g_UnWorkThreadNum!=0)
//		InterlockedAdd(&g_UnWorkThreadNum, -1);
//	if (g_UnWorkThreadNum==0)
//		SetEvent(g_hExitEvent);
//	return 0;
//}
HANDLE g_hPushedDirEvent;
std::vector<HANDLE> g_Threads;
std::vector<std::wstring> g_wstDirNames;

DWORD ThreadsNum;


unsigned __stdcall ThreadFileFind(void* lParam)
{
	std::wstring *pwstrSearch = (std::wstring*)lParam, wstrDirName;
	
	BOOL bActive = TRUE;
	while (true)
	{
		EnterCriticalSection(&g_cs);
		if (g_wstDirNames.empty())
		{
			bActive = FALSE;
			
		}
		else
		{
			wstrDirName = g_wstDirNames.back();
			g_wstDirNames.pop_back();
		}
		LeaveCriticalSection(&g_cs);

		if (!bActive)
		{	
		
			InterlockedAdd(&g_UnWorkThreadNum, -1);
			ResetEvent(g_hPushedDirEvent);
			/*if (g_UnWorkThreadNum>0)
			{
				--ThreadsNum;
				if (ThreadsNum == 0)
					SetEvent(g_hExitEvent);
				break;
			}*/
			if (g_UnWorkThreadNum==0)
				break;
			
			WaitForSingleObject(g_hPushedDirEvent, INFINITE);
			InterlockedAdd(&g_UnWorkThreadNum, 1);
			bActive = TRUE;
			continue;
		}
		WIN32_FIND_DATA fileFind = { 0 };
		HANDLE hFileFind = FindFirstFileW((MakeStandardName(wstrDirName) + L"*.*").c_str(), &fileFind);
		if (hFileFind != INVALID_HANDLE_VALUE)
		{

			do
			{
				if (wcscmp(fileFind.cFileName, L".") == 0)
					continue;
				if (wcscmp(fileFind.cFileName, L"..") == 0)
					continue;
				if (fileFind.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
				{
					EnterCriticalSection(&g_cs);
					g_wstDirNames.push_back(MakeStandardName(wstrDirName) + fileFind.cFileName);
					LeaveCriticalSection(&g_cs);
					SetEvent(g_hPushedDirEvent);
					InterlockedAdd(&g_nSearchDirNum, 1);
				}
				else
				{
					if (wcsstr(fileFind.cFileName, pwstrSearch->c_str()) != nullptr)
					{
						printf("Searched File In:%ls\r\n", (MakeStandardName(wstrDirName) + fileFind.cFileName).c_str());
						InterlockedAdd(&nNum, 1);

					}
					InterlockedAdd(&g_nFindedFIleNum, 1);
				}

			} while (FindNextFile(hFileFind, &fileFind));
			if (!FindClose(hFileFind))
				printf("Error No:%d", GetLastError());
		}
	}
	SetEvent(g_hPushedDirEvent);
	return 0;
}

SYSTEM_INFO Dio::CSystem::m_SystemInfo = { 0 };
BOOL Dio::CSystem::m_bIsInit = FALSE;

int main()
{
	/*DWORD dwBegin = GetTickCount();
	std::wstring wstrBeginDirName = L"C:\\";
	std::wstring wstrSearchName = L"ntdll";
	MyFileFind(wstrBeginDirName, wstrSearchName);
	DWORD dwTime = GetTickCount() - dwBegin;*/
	//InitializeCriticalSection(&g_cs);
	//g_hExitEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	//ThreadData* pTempData = new ThreadData;
	//pTempData->wstrDirName = L"C:\\";
	//pTempData->wstrFilter = L"*.*";
	//pTempData->wstrSearch = L"ntdll";
	//DWORD dwBegin = GetTickCount();
	//g_hThreads.push_back((HANDLE)_beginthreadex(nullptr, 0, ThreadFileFind, pTempData, 0, nullptr));
	//WaitForSingleObject(g_hExitEvent, INFINITE);
	//DWORD dwTime = GetTickCount() - dwBegin;
	//for (auto g_h_thread : g_hThreads)
	//{
	//	CloseHandle(g_h_thread);
	//}
	//DeleteCriticalSection(&g_cs);
	//printf("所花时间:%d,共遍历%d文件夹,找到文件数:%d", dwTime / 1000, g_nSearchDirNum, g_nFindedFIleNum);

	//InitializeCriticalSection(&g_cs);
	//g_hExitEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	//g_hPushedDirEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
	//if (g_hExitEvent == NULL)
	//{
	//	printf("CreateEvent Error:%d", GetLastError());
	//	return 0;
	//}
	//SYSTEM_INFO sysInfo;
	//GetSystemInfo(&sysInfo);
	//DWORD g_ThreadsNum = sysInfo.dwNumberOfProcessors * 2;
	//ThreadsNum = g_ThreadsNum;
	//std::wstring SearchName = L"ntdll";
	//g_wstDirNames.push_back(L"C:");
	////InterlockedAdd(&g_UnWorkThreadNum, -1);
	//g_UnWorkThreadNum = g_ThreadsNum;
	//DWORD dwBegin = GetTickCount();
	//for (DWORD i = g_ThreadsNum; i > 0; --i)
	//{
	//	HANDLE g_Thread = (HANDLE)_beginthreadex(nullptr, 0, ThreadFileFind, &SearchName, 0, nullptr);
	//	if (g_Thread == NULL)
	//		i++;
	//	else
	//		g_Threads.push_back(g_Thread);
	//}
	////WaitForSingleObject(g_hExitEvent, INFINITE);
	//WaitForMultipleObjects(g_Threads.size(), g_Threads.data(), TRUE, INFINITE);

	//for (auto g_Thread : g_Threads)
	//{
	//	CloseHandle(g_Thread);
	//}
	//DeleteCriticalSection(&g_cs);
	//DWORD dwTime = GetTickCount() - dwBegin;
	//printf("所花时间:%d,共遍历%d文件夹,找到文件数:%d", dwTime / 1000, g_nSearchDirNum, g_nFindedFIleNum);
	//CloseHandle(g_hExitEvent);
	//CloseHandle(g_hPushedDirEvent);


	Dio::CMyFileFind fileFinder(L"C:", L"ntdll");
	fileFinder.Start(true);
	std::wstring wstrData;
	fileFinder.WaitforObject();
	while (fileFinder.GetNextSearchData(wstrData))
	{
		printf("%ls\r\n", wstrData.c_str());
	}

    return 0;
}

