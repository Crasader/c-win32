// testmemory.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


#include <stdio.h>
#include <windows.h>
BOOL FindFirst(DWORD dwValue); // ��Ŀ����̿ռ���е�һ�β���
BOOL CompareAPage(DWORD dwBaseAddr, DWORD dwValue);// �Ƚ�Ŀ������ڴ�һҳ�бȽ�
BOOL FindNext(DWORD dwValue); // ��Ŀ����̿ռ���еڶ����Ĳ���
void ShowList();    // ��ӡ�������ĵ�ַ
DWORD g_arList[1024];   // ��Ų��ҵ��ĵ�ַ�б�
int g_nListCnt;     // ��Ч��ַ����
HANDLE g_hProcess;    // Ŀ����̾��
DWORD dError;     // �������
int main(int argc, char* argv[])
{
	// �������Գ���
	char szFileName[] = "..//02TestMemRepair//debug//02TestMemRepair.exe";
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	PROCESS_INFORMATION pi;
	::CreateProcess(NULL, szFileName, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL,
		&si, &pi);
	::CloseHandle(pi.hThread);
	// �򿪾��Ҫ�����ܶ���Ȩ�ޣ���MSDN֪������PROCESS_VM_READ
	g_hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pi.dwProcessId);

	// ����Ҫ�޸ĵ�ֵ
	int iVal;
	printf(" ��Ҫ�޸ĵ��������ڵ�ֵ�ǣ�");
	scanf("%d", &iVal);
	// ���е�һ�β���
	if (!FindFirst(iVal))
	{
		printf("FindFirstʧ��");
		getchar();
		return FALSE;
	}
	// ��ӡ�������Ľ��
	ShowList();

	// �����������Ψһ���ٴν�������
	while (g_nListCnt > 1)
	{
		printf("/n ���������Ψһ���ı����ݵ�ֵ���ٴ���������ֵΪ��");
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
	printf("/n ��Ҫ�޸ĳɵ�ֵΪ��");
	scanf("%d", &iVal);
	if (::WriteProcessMemory(g_hProcess, (LPVOID)g_arList[0], &iVal, sizeof(iVal), NULL))
		printf("/n �޸ĳɹ���/n");
	else
	{
		printf(" WriteProcessMemoryʧ��");
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
		printf(" Ҫ��ȡ�Ľ��̲�����/n");
		return FALSE;
	}
	// �鿴����ϵͳ���ͣ��Ծ�����ʼ��ַ
	DWORD dwBase;
	OSVERSIONINFO vi = { sizeof(OSVERSIONINFO) };
	::GetVersionEx(&vi);
	if (vi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
	{
		printf(" ����ϵͳΪ98ϵ��/n");
		dwBase = 4 * 1024 * 1024; // 98ϵ�У�4MB=4194304
	}
	else
	{
		printf(" ����ϵͳΪNTϵ��/n");
		dwBase = 65536;// NTϵ�У�64KB=65536
	}

	// �ڿ�ʼ��ַ��2GB�ĵ�ַ�ռ���в���
	for (; dwBase < 2 * dwOneGB; dwBase += dwOnePage)
	{ // ÿ����һҳ�в���

		if (CompareAPage(dwBase, dwValue))
		{
			printf(" CompareAPage�ɹ�/n");
		}
		else
		{
			printf(" CompareAPageʧ��/t%d/n", GetLastError());
			continue;
		}
	}
	return TRUE;
}
BOOL CompareAPage(DWORD dwBaseAddr, DWORD dwValue)
{
	// ��ȡһҳ���ڴ�
	BYTE arByte[4096];
	if (::ReadProcessMemory(g_hProcess, (LPCVOID)dwBaseAddr, arByte, 4096, NULL))
	{
		printf(" ReadProcessMemory�ɹ�/n");
		int* pdw;
		for (int i = 0; i < 4096; i++)
		{
			pdw = (int*)&arByte[i];
			if (*pdw == dwValue) // �������Ҫ���ҵ�ֵ
			{
				if (g_nListCnt >= 1024)
				{
					printf(" �ҵ��ĵ�ַ����/n");
					return FALSE;
				}
				//��ӵ�ȫ�ֱ�����
				g_arList[g_nListCnt++] = dwBaseAddr + i;
			}
		}
	}
	else
	{
		printf(" ReadProcessMemoryʧ��/t%d/n", ::GetLastError());
		return FALSE;  // ��ҳ���ɶ�
	}
	// ����һҳ�в���
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
		if (::ReadProcessMemory(g_hProcess, (LPCVOID)g_arList[m], &dwSecondRead, sizeof(DWORD), NULL))
		{
			if (dwSecondRead == dwValue)
				g_arList[g_nListCnt++] = g_arList[m];
		}
		else
		{
			printf(" ReadProcessMemoryʧ��");
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
	printf("һ���ҵ�%d����ַ/n", g_nListCnt);
}
/****������02MemRepair.cpp�еĴ���****/

/****������02TestMemRepair.cpp�еĴ���****/
#include <stdio.h>
int g_nNum;  // ȫ�ֱ�������
int main(int argc, char* argv[])
{
	
}

