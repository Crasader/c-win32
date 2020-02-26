// MemoryModifier.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <tlhelp32.h>
#include <time.h>
#include <map>
#pragma warning(disable:4996)

HMODULE fnGetProcessBase(DWORD PID);//获取进程基址
DWORD GetProcessID(WCHAR *FileName);//获取进程id
BOOL FindFirst(DWORD dwValue);//在目标空间的第一次查找
BOOL FindNext(DWORD dwValue);//在目标空间进行第二、三、四次查找
BOOL ComparePage(DWORD dwBaseAddr, DWORD dwValue);//对一页内容对比
void ShowList();//打印目标地址
DWORD GetUsedMemory(HANDLE hProcess);//查询内存占用情况


std::map<DWORD,size_t> g_tagBlock;
HANDLE hProcess;//目标代码句柄
DWORD g_arList[1024*1024];
int g_nListCnt;
int g_tagBlockCnt;

DWORD pid = GetProcessID(L"PlantsVsZombies.exe");
HMODULE hPmodule = fnGetProcessBase(pid);


int main()
{
	
	DWORD dwNumberOfBytesRead;
	int tmp = 0;
	if (pid != 0)
	{
		
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
		GetUsedMemory(hProcess);

		if (hProcess != NULL)
		{

			int iVal;
			printf(" 想要修改的数据现在的值是：");
			scanf("%d", &iVal);
			clock_t begin = clock();
			// 进行第一次查找
			if (!FindFirst(iVal))
			{
				printf("FindFirst失败");
				getchar();
				return FALSE;
			}
			// 打印搜索出的结果
			clock_t end = clock();
			ShowList();
			printf("所用时间%f s", (double)(end - begin)/ CLOCKS_PER_SEC);

			// 若搜索结果不唯一，再次进行搜索
			while (g_nListCnt > 1)
			{
				printf("\n 搜索结果不唯一，改变数据的值后再次搜索，新值为：");
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
			printf("\n 想要修改成的值为：");
			scanf("%d", &iVal);
			if (::WriteProcessMemory(hProcess, (LPVOID)g_arList[0], &iVal, sizeof(iVal), NULL))
				printf("\n 修改成功！\n");
			else
			{
				printf(" WriteProcessMemory失败");
				return FALSE;
			}

			
		}
		CloseHandle(hProcess);
		
	}
	return 0;
}


HMODULE fnGetProcessBase(DWORD PID)
{
	HANDLE hSnapShot;
	//通过CreateToolhelp32Snapshot和线程ID，获取进程快照  
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);
	if (hSnapShot == INVALID_HANDLE_VALUE)
	{
		printf("无法创建快照");
		return NULL;
	}
	MODULEENTRY32 ModuleEntry32;
	ModuleEntry32.dwSize = sizeof(ModuleEntry32);
	if (Module32First(hSnapShot, &ModuleEntry32))
	{
		do
		{
			TCHAR szExt[5];
			wcscpy(szExt, ModuleEntry32.szExePath + wcslen(ModuleEntry32.szExePath) - 4);
			for (int i = 0; i < 4; i++)
			{
				if ((szExt[i] >= 'a') && (szExt[i] <= 'z'))
				{
					szExt[i] = szExt[i] - 0x20;
				}
			}
			if (!wcscmp(szExt, L".EXE"))
			{
				CloseHandle(hSnapShot);
				return ModuleEntry32.hModule;
			}
		} while (Module32Next(hSnapShot, &ModuleEntry32));
	}
	CloseHandle(hSnapShot);
	return NULL;
}

DWORD GetProcessID(WCHAR * FileName)
{
	HANDLE myhProcess;
	PROCESSENTRY32 mype;
	mype.dwSize = sizeof(PROCESSENTRY32);
	BOOL mybRet;
	//进行进程快照
	myhProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //TH32CS_SNAPPROCESS快照所有进程
																  //开始进程查找
	mybRet = Process32First(myhProcess, &mype);
	//循环比较，得出ProcessID
	while (mybRet)
	{
		if (wcscmp(FileName, mype.szExeFile) == 0)
			return mype.th32ProcessID;
		else
			mybRet = Process32Next(myhProcess, &mype);
	}
	return 0;
}

BOOL FindFirst(DWORD dwValue)
{
	//const DWORD dwOneGB = 1024 * 1024*1024 ;
	const DWORD dwOnePage = 4 * 1024;
	if (hProcess==NULL)
	{
		printf("要读取的进程不存在\n");
		return FALSE;
	}
	DWORD dwBase;
	OSVERSIONINFO vi = { sizeof(OSVERSIONINFO) };
	::GetVersionEx(&vi);
	if (vi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
	{
		printf(" 操作系统为98系列\n");
		dwBase = 4 * 1024 * 1024;
	}
	else
	{
		printf(" 操作系统为NT系列\n");
		dwBase = 65536;
	}
	//for (; dwBase < 2 * dwOneGB; dwBase += dwOnePage)
	//{ // 每次在一页中查找

	//	if (ComparePage(dwBase, dwValue))
	//	{
	//		//printf(" CompareAPage成功\n");
	//	}
	//	else
	//	{
	//		//printf(" CompareAPage失败\t%d\n", GetLastError());
	//		continue;
	//	}
	//}
	for (auto block=g_tagBlock.begin();block!=g_tagBlock.end();++block)
	{
		auto tmp = block->first;
		for (;tmp-block->first<=block->second;tmp+= dwOnePage)
		{
			if (ComparePage(tmp, dwValue))
			{
				printf(" CompareAPage成功\n");
			}
		}
	}
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
		if (::ReadProcessMemory(hProcess, (LPCVOID)g_arList[m], &dwSecondRead, sizeof(DWORD), NULL))
		{
			//printf(" ReadProcessMemory成功\n");
			if (dwSecondRead == dwValue)
				g_arList[g_nListCnt++] = g_arList[m];
		}
		else
		{
			//printf(" ReadProcessMemory失败");
			return FALSE;
		}
	}
	return TRUE;
}

BOOL ComparePage(DWORD dwBaseAddr, DWORD dwValue)
{
	// 读取一页的内存
	BYTE arByte[4096];
	if (::ReadProcessMemory(hProcess, (LPVOID)dwBaseAddr, arByte, 4096, NULL))
	{
		
		int* pdw;
		for (int i = 0; i < 4096; i++)
		{
			pdw = (int*)&arByte[i];
			if (*pdw == dwValue) // 如果等于要查找的值
			{
				//添加到全局变量中
				g_arList[g_nListCnt++] = dwBaseAddr + i;
			}
		}
	}
	else
	{
		printf(" ReadProcessMemory失败\t%d\n", ::GetLastError());
		return FALSE;  // 这页不可读
	}
	// 在这一页中查找
	return TRUE;
}

void ShowList()
{
	for (int i = 0; i < g_nListCnt; i++)
	{
		printf("%d:%08X\n", i + 1, g_arList[i]);
	}
	printf("一共找到%d个地址\n", g_nListCnt);
}

DWORD GetUsedMemory(HANDLE hProcess)
{
	//HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
	MEMORY_BASIC_INFORMATION mbi;
	DWORD dwAddr = (DWORD)hPmodule;
	while (sizeof(mbi)==VirtualQueryEx(hProcess,(LPVOID)dwAddr,&mbi,sizeof(mbi)))
	{
		if (MEM_COMMIT==mbi.State )
		{
			g_tagBlock.insert(std::pair<DWORD, size_t>(dwAddr, mbi.RegionSize));
		}
		dwAddr += (DWORD)mbi.RegionSize;
		if (dwAddr>0x7FFFFFFF)
			break;
	}
	return TRUE;
}
