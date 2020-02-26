#include "stdafx.h"

#include <Windows.h>
#include <process.h>

#include "MyFileFind.h"
#include "System.h"



unsigned Dio::CMyFileFind::Start(bool bIsBlock )
{
	unsigned uRet = 0;
	if (bIsBlock)
	{
		uRet=FuncBegin();
		//运行查找函数
		//SetEvent(m_hHasData);
	}
	else
	{
		//启动线程运行查找函数
		CMyFileFind::CThread thread(ThreadBegin, this);
		thread.Run();
	}
	return uRet;
}

BOOL Dio::CMyFileFind::GetNextSearchData(OUT std::wstring & wstrNextData)
{
	BOOL bRet = FALSE;
	WaitForSingleObject(m_hHasData, INFINITE);
	//while (m_vecFindedPaths.empty())
	//{
	//}
	EnterCriticalSection(&g_cs);
	if (!m_vecFindedPaths.empty())
	{
		wstrNextData = m_vecFindedPaths.back();
		m_vecFindedPaths.pop_back();
		bRet = TRUE;
	}
	LeaveCriticalSection(&g_cs);
	return bRet;
}

BOOL Dio::CMyFileFind::WaitforObject()
{
	BOOL ret = FALSE;
	//WaitForSingleObject(m_hHasData, INFINITE);
	EnterCriticalSection(&g_cs);
	
	WaitForMultipleObjects(m_vecThreads.size(), (*m_vecThreads.data())->GetThreadHandle(), TRUE, INFINITE);
	//while (m_vecThreads.size()<m_MaxThreadNUm)
	//{
	//}
	//WaitForMultipleObjects(m_vecThreads.size(), (*m_vecThreads.data())->GetThreadHandle(), TRUE, INFINITE);
	LeaveCriticalSection(&g_cs);
	ret = TRUE;
	return ret;
}

std::vector<std::wstring> Dio::CMyFileFind::GetSearchData()
{
	return std::vector<std::wstring>();
}

unsigned Dio::CMyFileFind::ThreadBegin(void * lParam)
{
	CMyFileFind* pThis = (CMyFileFind*)lParam;
	pThis->FuncBegin();
	return 0;
}

unsigned Dio::CMyFileFind::FuncBegin()
{
	for (int i=0;i<m_MaxThreadNUm;i++)
	{
		m_vecThreads.push_back(new CThread(ThreadFindFile, this));
	}
	for (auto m_vec_thread:m_vecThreads)
	{
		m_vec_thread->Run();
	}
	return 0;
}

std::wstring Dio::CMyFileFind::MakeStandardName(const std::wstring & wstDirName)
{
	if (wstDirName.back() != '\\')
		return wstDirName + L'\\';
	return wstDirName;
}

unsigned Dio::CMyFileFind::ThreadFindFile(void * lParam)
{
	CMyFileFind* pThis = (CMyFileFind*)lParam;
	std::wstring pwstrSearch = pThis->m_wstrSearch, wstrDirName;
	
	BOOL bActive = TRUE;
	BOOL bRet = FALSE;
	while (true)
	{

	EnterCriticalSection(&pThis->g_cs);
	if (pThis->m_vecDirNames.empty())
	{
		bActive = FALSE;
		//InterlockedAdd(&g_UnWorkThreadNum, 1);
	}
	else
	{
		wstrDirName = pThis->m_vecDirNames.back();
		pThis->m_vecDirNames.pop_back();
	}
	LeaveCriticalSection(&pThis->g_cs);

	if (!bActive)
	{
		InterlockedAdd((long*)&pThis->m_uWorkThreadNum, -1);
		if (pThis->m_MaxThreadNUm == 0)
			break;
		ResetEvent(pThis->m_hHasPushedEvent);
		WaitForSingleObject(pThis->m_hHasPushedEvent, INFINITE);
		InterlockedAdd((long*)&pThis->m_uWorkThreadNum, 1);

		bActive = TRUE;
		continue;
	}

	WIN32_FIND_DATAW fileFind = { 0 };
	HANDLE hFileFind = FindFirstFileW((pThis->MakeStandardName(wstrDirName) + pThis->m_wstrFilter).c_str(), &fileFind);
	do
	{
		if (wcscmp(fileFind.cFileName, L".") == 0)
			continue;
		if (wcscmp(fileFind.cFileName, L"..") == 0)
			continue;
		if (fileFind.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			EnterCriticalSection(&pThis->g_cs);
			pThis->m_vecDirNames.push_back(pThis->MakeStandardName(wstrDirName) + fileFind.cFileName);
			LeaveCriticalSection(&pThis->g_cs);
			SetEvent(pThis->m_hHasPushedEvent);
		}
		else
		{
			if (wcsstr(fileFind.cFileName, pwstrSearch.c_str()) != nullptr)
			{
				EnterCriticalSection(&pThis->g_cs);
				pThis->m_vecFindedPaths.push_back(pThis->MakeStandardName(wstrDirName) + fileFind.cFileName);
				//printf("Searched File In:%ls\r\n", (pThis->MakeStandardName(wstrDirName) + fileFind.cFileName).c_str());
				LeaveCriticalSection(&pThis->g_cs);
			}
		}
	} while (FindNextFileW(hFileFind, &fileFind));
	SetEvent(pThis->m_hHasPushedEvent);
	}
	SetEvent(pThis->m_hHasData);
	return bRet;
}


void Dio::CMyFileFind::CThread::Run()
{
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, m_Func, m_lParam, 0, nullptr);
}

HANDLE* Dio::CMyFileFind::CThread::GetThreadHandle()
{
	return &m_hThread;
}
