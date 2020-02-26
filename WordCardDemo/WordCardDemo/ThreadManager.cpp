/// @file       UtilityHelper\ThreadManager.cpp

#include "stdafx.h"
#include "ThreadManager.h"
#include "SS_Log_Include.h"
 //#include "constDefine.h"

namespace ns_base
{
	CThreadManager::CThreadManager()
	{
		DataInit();
	}

	CThreadManager::~CThreadManager()
	{
		DataUnInit();
	}

	void CThreadManager::DataInit()
	{
        SetThreadSn(-1);
        m_bEntryStopThread = FALSE;
        m_dwThreadId = 0;
		m_hThread = INVALID_HANDLE_VALUE;
		m_hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	void CThreadManager::DataUnInit()
	{
        /// 因为存在线程释放时机问题, 线程的停止要所有者调用本类的StopThread
        /// e.g. 如果线程带窗口操作, 必须在所有者Wnd Destory之前释放线程

		CloseHandle(m_hEvent);
		m_hEvent = NULL;
	}

    void CThreadManager::SetThreadHandle(HANDLE hThread)
    {
        m_bEntryStopThread = FALSE;
        ResetEvent(m_hEvent); ///< 只在设置线程句柄时，将事件Reset
        m_hThread = hThread;
    }

    BOOL CThreadManager::IsThreadRunning(DWORD dwTimeOut)
    {
        if (INVALID_HANDLE_VALUE == m_hThread)
            return FALSE;

        return WaitForSingleObjectTimeOut(m_hThread, dwTimeOut);
    }

    BOOL CThreadManager::IsNeedQuitThread(DWORD dwTimeOut)
    {
        //_ASSERT_EXPR(dwTimeOut==5,L"dwTimeOut==5");
        if (INVALID_HANDLE_VALUE == m_hEvent)
            return FALSE;

        return QueryFlag_IsNeedQuitThread();
    }

    BOOL CThreadManager::WaitForSingleObjectHappen(HANDLE & hObject, DWORD dwTimeOut)
    {
        DWORD   dwRc = ::WaitForSingleObject(hObject, dwTimeOut);
        return (WAIT_OBJECT_0 == dwRc);
    }

    BOOL CThreadManager::WaitForSingleObjectTimeOut(HANDLE & hObject, DWORD dwTimeOut)
    {
        SetLastError(0);
        DWORD dwRc = ::WaitForSingleObject(hObject, dwTimeOut);
        DWORD dwErr = GetLastError();
        return ((WAIT_TIMEOUT == dwRc) && (dwErr != ERROR_INVALID_HANDLE));
    }

    void CThreadManager::StopThread(BOOL bStopNow, const WCHAR* pcTitle)
    {
        /// because log cost time, so only the stop action need log
        /// the pre stop action need not log
        if ((NULL != pcTitle) && bStopNow)
        {
            if (_tcslen(pcTitle) <= 0)
            {
                /// 为了方便调试,定位问题,请一定写上要停止的线程信息
                _ASSERT_EXPR(0, L"(_tcslen(pcTitle) <= 0)");
            }
            //WriteLogEx(L">>CThreadManager::StopThread(%s)", pcTitle);
        }

        m_bEntryStopThread = TRUE;
        StopThreadInternal(m_hEvent, m_hThread, bStopNow);

        /// 线程已经停止了, 这个停止标记也要复位
        /// 防止在线程已经停止的情况下，判断该线程是否需要退出时，造成逻辑错误
        m_bEntryStopThread = FALSE;

        if ((NULL != pcTitle) && bStopNow)
        {
            //WriteLogEx(L"<<CThreadManager::StopThread(%s)", pcTitle);
        }
    }

    BOOL CThreadManager::QueryFlag_IsNeedQuitThread()
    {
        return m_bEntryStopThread;
    }

    void CThreadManager::StopThreadInternal(HANDLE& hEvent, HANDLE& hThread, BOOL bStopNow)
    {
        /// 防止停止已经不存在的线程死掉
        if (INVALID_HANDLE_VALUE == hThread)
        {
            return;
        }

        /// 设置事件后，不再复位事件
        SetEvent(hEvent);
        ::Sleep(0); ///< give control to other threads

        /// if (!bStopNow), we only send stop signal to thread
        if (bStopNow)
        {
            /// wait until thread has stopped
            ::WaitForSingleObject(hThread, INFINITE);

            /// 当线程已经停止后, 重置事件
            /// 防止第二次判断线程是否需要退出逻辑错误.
            ResetEvent(hEvent);

            CloseHandle(hThread);
			hThread = NULL;
        }
    }
}

