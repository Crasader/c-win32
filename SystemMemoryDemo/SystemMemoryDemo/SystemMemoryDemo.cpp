// SystemMemoryDemo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>

int main()
{
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	printf("ҳ���С:%d\r\n", sysInfo.dwPageSize);
	printf("�������:%d\r\n", sysInfo.dwAllocationGranularity);
	printf("�û�����ʼ��ַ:0x%x\r\n", sysInfo.lpMinimumApplicationAddress);
	printf("�û�����ַ:0x%X\r\n", sysInfo.lpMaximumApplicationAddress);

	MEMORYSTATUS memStatus;
	GlobalMemoryStatus(&memStatus);
	printf("ʹ�ñ�:%d%%\r\n", memStatus.dwMemoryLoad);
	printf("���ڴ��С:%d", memStatus.dwTotalPhys/1024/1024/1024);
	printf("�����ڴ��С:%d\a", memStatus.dwAvailPhys / 1024 / 1024 / 1024);

	
    return 0;
}

