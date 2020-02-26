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
        /// ��Ϊ�����߳��ͷ�ʱ������, �̵߳�ֹͣҪ�����ߵ��ñ����StopThread
        /// e.g. ����̴߳����ڲ���, ������������Wnd Destory֮ǰ�ͷ��߳�

		CloseHandle(m_hEvent);
		m_hEvent = NULL;
	}

    void CThreadManager::SetThreadHandle(HANDLE hThread)
    {
        m_bEntryStopThread = FALSE;
        ResetEvent(m_hEvent); ///< ֻ�������߳̾��ʱ�����¼�Reset
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
                /// Ϊ�˷������,��λ����,��һ��д��Ҫֹͣ���߳���Ϣ
                _ASSERT_EXPR(0, L"(_tcslen(pcTitle) <= 0)");
            }
            //WriteLogEx(L">>CThreadManager::StopThread(%s)", pcTitle);
        }

        m_bEntryStopThread = TRUE;
        StopThreadInternal(m_hEvent, m_hThread, bStopNow);

        /// �߳��Ѿ�ֹͣ��, ���ֹͣ���ҲҪ��λ
        /// ��ֹ���߳��Ѿ�ֹͣ������£��жϸ��߳��Ƿ���Ҫ�˳�ʱ������߼�����
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
        /// ��ֹֹͣ�Ѿ������ڵ��߳�����
        if (INVALID_HANDLE_VALUE == hThread)
        {
            return;
        }

        /// �����¼��󣬲��ٸ�λ�¼�
        SetEvent(hEvent);
        ::Sleep(0); ///< give control to other threads

        /// if (!bStopNow), we only send stop signal to thread
        if (bStopNow)
        {
            /// wait until thread has stopped
            ::WaitForSingleObject(hThread, INFINITE);

            /// ���߳��Ѿ�ֹͣ��, �����¼�
            /// ��ֹ�ڶ����ж��߳��Ƿ���Ҫ�˳��߼�����.
            ResetEvent(hEvent);

            CloseHandle(hThread);
			hThread = NULL;
        }
    }
}

