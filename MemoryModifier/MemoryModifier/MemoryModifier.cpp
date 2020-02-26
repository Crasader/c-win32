// MemoryModifier.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <windows.h>
#include <tlhelp32.h>
#include <time.h>
#include <map>
#pragma warning(disable:4996)

HMODULE fnGetProcessBase(DWORD PID);//��ȡ���̻�ַ
DWORD GetProcessID(WCHAR *FileName);//��ȡ����id
BOOL FindFirst(DWORD dwValue);//��Ŀ��ռ�ĵ�һ�β���
BOOL FindNext(DWORD dwValue);//��Ŀ��ռ���еڶ��������Ĵβ���
BOOL ComparePage(DWORD dwBaseAddr, DWORD dwValue);//��һҳ���ݶԱ�
void ShowList();//��ӡĿ���ַ
DWORD GetUsedMemory(HANDLE hProcess);//��ѯ�ڴ�ռ�����


std::map<DWORD,size_t> g_tagBlock;
HANDLE hProcess;//Ŀ�������
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
			printf(" ��Ҫ�޸ĵ��������ڵ�ֵ�ǣ�");
			scanf("%d", &iVal);
			clock_t begin = clock();
			// ���е�һ�β���
			if (!FindFirst(iVal))
			{
				printf("FindFirstʧ��");
				getchar();
				return FALSE;
			}
			// ��ӡ�������Ľ��
			clock_t end = clock();
			ShowList();
			printf("����ʱ��%f s", (double)(end - begin)/ CLOCKS_PER_SEC);

			// �����������Ψһ���ٴν�������
			while (g_nListCnt > 1)
			{
				printf("\n ���������Ψһ���ı����ݵ�ֵ���ٴ���������ֵΪ��");
				scanf("%d", &iVal);
				if (FindNext(iVal))
					ShowList();
				else
				{
					printf(" FindNextʧ��");
					return FALSE;
				}
			}
			// �������Ψһ�������޸�
			printf("\n ��Ҫ�޸ĳɵ�ֵΪ��");
			scanf("%d", &iVal);
			if (::WriteProcessMemory(hProcess, (LPVOID)g_arList[0], &iVal, sizeof(iVal), NULL))
				printf("\n �޸ĳɹ���\n");
			else
			{
				printf(" WriteProcessMemoryʧ��");
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
	//ͨ��CreateToolhelp32Snapshot���߳�ID����ȡ���̿���  
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);
	if (hSnapShot == INVALID_HANDLE_VALUE)
	{
		printf("�޷���������");
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
	//���н��̿���
	myhProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //TH32CS_SNAPPROCESS�������н���
																  //��ʼ���̲���
	mybRet = Process32First(myhProcess, &mype);
	//ѭ���Ƚϣ��ó�ProcessID
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
		printf("Ҫ��ȡ�Ľ��̲�����\n");
		return FALSE;
	}
	DWORD dwBase;
	OSVERSIONINFO vi = { sizeof(OSVERSIONINFO) };
	::GetVersionEx(&vi);
	if (vi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
	{
		printf(" ����ϵͳΪ98ϵ��\n");
		dwBase = 4 * 1024 * 1024;
	}
	else
	{
		printf(" ����ϵͳΪNTϵ��\n");
		dwBase = 65536;
	}
	//for (; dwBase < 2 * dwOneGB; dwBase += dwOnePage)
	//{ // ÿ����һҳ�в���

	//	if (ComparePage(dwBase, dwValue))
	//	{
	//		//printf(" CompareAPage�ɹ�\n");
	//	}
	//	else
	//	{
	//		//printf(" CompareAPageʧ��\t%d\n", GetLastError());
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
				printf(" CompareAPage�ɹ�\n");
			}
		}
	}
	return TRUE;
}

BOOL FindNext(DWORD dwValue)
{
	int nOrgCnt = g_nListCnt;
	g_nListCnt = 0;
	DWORD dwSecondRead;  // �洢�ӵ�ַ��ȡ����ֵ������ֵdwValue�Ƚ�
						 // ���Ѿ����������ĵ�ַ���ٴβ���
	for (int m = 0; m < nOrgCnt; m++)
	{
		if (::ReadProcessMemory(hProcess, (LPCVOID)g_arList[m], &dwSecondRead, sizeof(DWORD), NULL))
		{
			//printf(" ReadProcessMemory�ɹ�\n");
			if (dwSecondRead == dwValue)
				g_arList[g_nListCnt++] = g_arList[m];
		}
		else
		{
			//printf(" ReadProcessMemoryʧ��");
			return FALSE;
		}
	}
	return TRUE;
}

BOOL ComparePage(DWORD dwBaseAddr, DWORD dwValue)
{
	// ��ȡһҳ���ڴ�
	BYTE arByte[4096];
	if (::ReadProcessMemory(hProcess, (LPVOID)dwBaseAddr, arByte, 4096, NULL))
	{
		
		int* pdw;
		for (int i = 0; i < 4096; i++)
		{
			pdw = (int*)&arByte[i];
			if (*pdw == dwValue) // �������Ҫ���ҵ�ֵ
			{
				//��ӵ�ȫ�ֱ�����
				g_arList[g_nListCnt++] = dwBaseAddr + i;
			}
		}
	}
	else
	{
		printf(" ReadProcessMemoryʧ��\t%d\n", ::GetLastError());
		return FALSE;  // ��ҳ���ɶ�
	}
	// ����һҳ�в���
	return TRUE;
}

void ShowList()
{
	for (int i = 0; i < g_nListCnt; i++)
	{
		printf("%d:%08X\n", i + 1, g_arList[i]);
	}
	printf("һ���ҵ�%d����ַ\n", g_nListCnt);
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
