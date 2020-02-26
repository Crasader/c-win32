/// @file       UtilityHelper\ThreadManager.h

#ifndef __UTILITY_HELPER__THREAD_MANAGER_H__
#define __UTILITY_HELPER__THREAD_MANAGER_H__

namespace ns_base
{
	class CThreadManager
	{
	public:
		CThreadManager();
		virtual ~CThreadManager();

    public:
        void SetThreadHandle(HANDLE hThread);
        HANDLE GetThreadHandle() {return m_hThread;}

        void SetThreadSn(int iThreadSn) {m_iThreadSn = iThreadSn;}
        int GetThreadSn() {return m_iThreadSn;}

        void SetThreadId(DWORD dwThreadId)
        {
            m_dwThreadId = dwThreadId;
        }
        DWORD GetThreadId() {return m_dwThreadId;}

        BOOL IsThreadRunning(DWORD dwTimeOut = 5);
        BOOL IsNeedQuitThread(DWORD dwTimeOut = 5);
        BOOL QueryFlag_IsNeedQuitThread(); ///< 只是查询退出标记，不真的等事件超时, 等超时很浪费时间

        void StopThread(BOOL bStopNow, const WCHAR* pcTitle);
         operator HANDLE() const
         {
            return m_hThread;
         }
	private:
		void DataInit();
		void DataUnInit();

        BOOL WaitForSingleObjectHappen(HANDLE & hObject, DWORD dwTimeOut);
        BOOL WaitForSingleObjectTimeOut(HANDLE & hObject, DWORD dwTimeOut);

        void StopThreadInternal(HANDLE& hEvent, HANDLE& hThread, BOOL bStopNow);

    private:
        int     m_iThreadSn;
        DWORD   m_dwThreadId;
		HANDLE  m_hThread;
		HANDLE  m_hEvent;

        BOOL    m_bEntryStopThread; ///< 已经被执行过 StopThread, 说明要停止了
	};

    typedef struct _tag_Thread_Manager_Param
    {
        CThreadManager* pThreadManager;
        void* pParam;

        _tag_Thread_Manager_Param()
        {
            pThreadManager = NULL;
            pParam = NULL;
        }

        void copy(_tag_Thread_Manager_Param* pSrc)
        {
            do 
            {
                if (NULL == pSrc)
                    break;

                this->pThreadManager = pSrc->pThreadManager;
                this->pParam = pSrc->pParam;
            } while (0);
        }
    }TAG_THREAD_MANAGER_PARAM;
}

#endif // #ifndef __UTILITY_HELPER__THREAD_MANAGER_H__
