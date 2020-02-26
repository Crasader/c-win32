#ifndef CSYSTEM_H_
#define CSYSTEM_H_

#include <Windows.h>

namespace Dio
{
	class CSystem
	{
	public:
		CSystem() {}
		~CSystem() {}
		inline static unsigned GetSystemCPUNum()
		{
			if (!m_bIsInit)
			{
				GetSystemInfo(&m_SystemInfo);
				m_bIsInit = TRUE;
			}
			return m_SystemInfo.dwNumberOfProcessors;
		}
	private:
		static SYSTEM_INFO m_SystemInfo;
		static BOOL m_bIsInit;
	};
	
	
}


#endif
