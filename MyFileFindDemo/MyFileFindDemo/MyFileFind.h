#ifndef _CMYFILEFIND_H_
#define _CMYFILEFIND_H_

#include <vector>
#include "System.h"
namespace Dio
{
	class CMyFileFind
	{
		class CThread
		{

		public:
			typedef unsigned(__stdcall *ThreadFunc) (void*);
			CThread(ThreadFunc func,void *lParam)
				:m_Func(func),m_lParam(lParam)
			{}
			~CThread()
			{
				if (m_hThread!=INVALID_HANDLE_VALUE)
					CloseHandle(m_hThread);
			}
			void Run();
			HANDLE* GetThreadHandle();
		private:
			ThreadFunc m_Func;
			void* m_lParam;
			HANDLE m_hThread;
		};

	public:
		CMyFileFind(const std::wstring& wstrBeginDirName, const std::wstring& wstrSearch, const std::wstring& wstrFilter = L"*.*", const unsigned uMaxThreadNum = 0)
			:m_wstrSearch(wstrSearch),
			m_wstrFilter(wstrFilter),
			m_MaxThreadNUm(uMaxThreadNum == 0 ? Dio::CSystem::GetSystemCPUNum() * 2 : uMaxThreadNum),
			m_uWorkThreadNum(m_MaxThreadNUm)
		{
			m_vecDirNames.push_back(wstrBeginDirName);
			m_hHasPushedEvent = CreateEvent(nullptr, TRUE,FALSE,nullptr);
			m_hHasData = CreateEvent(nullptr, FALSE, FALSE, nullptr);
			InitializeCriticalSection(&g_cs);
		}
		~CMyFileFind()
		{
			for (auto m_vec_thread:m_vecThreads)
			{
				delete m_vec_thread;
			}
			CloseHandle(m_hHasPushedEvent);
			CloseHandle(m_hHasData);
			DeleteCriticalSection(&g_cs);
		}

		unsigned Start(bool bIsBlock = false);
		
		BOOL GetNextSearchData(OUT std::wstring& wstrNextData);
		BOOL WaitforObject();

		std::vector<std::wstring> GetSearchData();
	private:
		std::vector<std::wstring> m_vecDirNames;
		std::wstring m_wstrSearch;
		std::wstring m_wstrFilter;
		const unsigned m_MaxThreadNUm;
		HANDLE m_hHasData;
		CRITICAL_SECTION g_cs;
		HANDLE m_hHasPushedEvent;
		std::vector<CThread*> m_vecThreads;
		std::vector<std::wstring> m_vecFindedPaths;
		unsigned m_uWorkThreadNum;

		static unsigned __stdcall ThreadBegin(void* lParam);
		unsigned FuncBegin();
		std::wstring MakeStandardName(const std::wstring &wstDirName);
	
		static unsigned __stdcall ThreadFindFile(void* lParam);
	};
}


#endif