// SystemMemoryDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>

int main()
{
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	printf("页面大小:%d\r\n", sysInfo.dwPageSize);
	printf("分配颗粒:%d\r\n", sysInfo.dwAllocationGranularity);
	printf("用户区开始地址:0x%x\r\n", sysInfo.lpMinimumApplicationAddress);
	printf("用户最大地址:0x%X\r\n", sysInfo.lpMaximumApplicationAddress);

	MEMORYSTATUS memStatus;
	GlobalMemoryStatus(&memStatus);
	printf("使用比:%d%%\r\n", memStatus.dwMemoryLoad);
	printf("总内存大小:%d", memStatus.dwTotalPhys/1024/1024/1024);
	printf("可用内存大小:%d\a", memStatus.dwAvailPhys / 1024 / 1024 / 1024);

	
    return 0;
}

