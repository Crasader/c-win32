// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�: 
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "../DuiLib/UIlib.h"
#ifdef _DEBUG
#	ifdef _UNICODE
#		pragma comment(lib,"..\\bin\\DuiLib_ud.lib")
#	else
#		pragma comment(lib,"..\\bin\\DuiLib_d.lib")
#	endif
#else
#	ifdef _UNICODE
#		pragma comment(lib,"..\\bin\\DuiLib_d.lib")
#	else
#		pragma comment(lib,"..\\bin\\DuiLib.lib")
#	endif
#endif
// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <process.h>

#include "resource.h"
#include "WordCardData.h"
extern CWordCardData g_WordCard;
// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
