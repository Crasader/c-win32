// testmemory.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include <stdio.h>
#include <windows.h>
BOOL FindFirst(DWORD dwValue); // 在目标进程空间进行第一次查找
BOOL CompareAPage(DWORD dwBaseAddr, DWORD dwValue);// 比较目标进程内存一页中比较
BOOL FindNext(DWORD dwValue); // 在目标进程空间进行第二三四查找
void ShowList();    // 打印搜索出的地址
DWORD g_arList[1024];   // 存放查找到的地址列表
int g_nListCnt;     // 有效地址个数
HANDLE g_hProcess;    // 目标进程句柄
DWORD dError;     // 错误代码
int main(int argc, char* argv[])
{
	// 启动测试程序
	char szFileName[] = "..//02TestMemRepair//debug//02TestMemRepair.exe";
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	PROCESS_INFORMATION pi;
	::CreateProcess(NULL, szFileName, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL,
		&si, &pi);
	::CloseHandle(pi.hThread);
	// 打开句柄要给予能读的权限，据MSDN知最少是PROCESS_VM_READ
	g_hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pi.dwProcessId);

	// 输入要修改的值
	int iVal;
	printf(" 想要修改的数据现在的值是：");
	scanf("%d", &iVal);
	// 进行第一次查找
	if (!FindFirst(iVal))
	{
		printf("FindFirst失败");
		getchar();
		return FALSE;
	}
	// 打印搜索出的结果
	ShowList();

	// 若搜索结果不唯一，再次进行搜索
	while (g_nListCnt > 1)
	{
		printf("/n 搜索结果不唯一，改变数据的值后再次搜索，新值为：");
		scanf("%d", &iVal);
		if (FindNext(iVal))
			ShowList();
		else
		{
			printf(" FindNext失败");
			return FALSE;
		}
	}
	// 搜索结果唯一，进行修改
	printf("/n 想要修改成的值为：");
	scanf("%d", &iVal);
	if (::WriteProcessMemory(g_hProcess, (LPVOID)g_arList[0], &iVal, sizeof(iVal), NULL))
		printf("/n 修改成功！/n");
	else
	{
		printf(" WriteProcessMemory失败");
		return FALSE;
	}

	::CloseHandle(g_hProcess);
	return 0;
}
BOOL FindFirst(DWORD dwValue)
{
	const DWORD dwOneGB = 1024 * 1024 * 1024; // 1GB=1073741824 
	const DWORD dwOnePage = 4 * 1024;   // 4KB=4096
	if (g_hProcess == NULL)
	{
		printf(" 要读取的进程不存在/n");
		return FALSE;
	}
	// 查看操作系统类型，以决定开始地址
	DWORD dwBase;
	OSVERSIONINFO vi = { sizeof(OSVERSIONINFO) };
	::GetVersionEx(&vi);
	if (vi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
	{
		printf(" 操作系统为98系列/n");
		dwBase = 4 * 1024 * 1024; // 98系列，4MB=4194304
	}
	else
	{
		printf(" 操作系统为NT系列/n");
		dwBase = 65536;// NT系列，64KB=65536
	}

	// 在开始地址到2GB的地址空间进行查找
	for (; dwBase < 2 * dwOneGB; dwBase += dwOnePage)
	{ // 每次在一页中查找

		if (CompareAPage(dwBase, dwValue))
		{
			printf(" CompareAPage成功/n");
		}
		else
		{
			printf(" CompareAPage失败/t%d/n", GetLastError());
			continue;
		}
	}
	return TRUE;
}
BOOL CompareAPage(DWORD dwBaseAddr, DWORD dwValue)
{
	// 读取一页的内存
	BYTE arByte[4096];
	if (::ReadProcessMemory(g_hProcess, (LPCVOID)dwBaseAddr, arByte, 4096, NULL))
	{
		printf(" ReadProcessMemory成功/n");
		int* pdw;
		for (int i = 0; i < 4096; i++)
		{
			pdw = (int*)&arByte[i];
			if (*pdw == dwValue) // 如果等于要查找的值
			{
				if (g_nListCnt >= 1024)
				{
					printf(" 找到的地址过多/n");
					return FALSE;
				}
				//添加到全局变量中
				g_arList[g_nListCnt++] = dwBaseAddr + i;
			}
		}
	}
	else
	{
		printf(" ReadProcessMemory失败/t%d/n", ::GetLastError());
		return FALSE;  // 这页不可读
	}
	// 在这一页中查找
	return TRUE;
}
BOOL FindNext(DWORD dwValue)
{
	int nOrgCnt = g_nListCnt;
	g_nListCnt = 0;
	DWORD dwSecondRead;  // 存储从地址中取出的值，与新值dwValue比较
						 // 在已经搜索出来的地址中再次查找
	for (int m = 0; m < nOrgCnt; m++)
	{
		if (::ReadProcessMemory(g_hProcess, (LPCVOID)g_arList[m], &dwSecondRead, sizeof(DWORD), NULL))
		{
			if (dwSecondRead == dwValue)
				g_arList[g_nListCnt++] = g_arList[m];
		}
		else
		{
			printf(" ReadProcessMemory失败");
			return FALSE;
		}
		return TRUE;
	}
}

void ShowList()
{
	for (int i = 0; i < g_nListCnt; i++)
	{
		printf("%d:%08X/n", i + 1, g_arList[i]);
	}
	printf("一共找到%d个地址/n", g_nListCnt);
}
/****以上是02MemRepair.cpp中的代码****/

/****以下是02TestMemRepair.cpp中的代码****/
#include <stdio.h>
int g_nNum;  // 全局变量测试
int main(int argc, char* argv[])
{
	
}

