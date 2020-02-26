// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#ifndef __TBSELLERCLIENT_H_
#define __TBSELLERCLIENT_H_


#define _CRT_SECURE_NO_WARNINGS
#include "targetver.h"




#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
#include <windowsx.h>
#include <Windows.h>
#undef GetFirstChild
#undef GetNextSibling




// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "..\Duilib\UIlib.h" 
using namespace DuiLib;

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_ud.lib")
#   else
#       pragma comment(lib, "DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_u.lib")
#   else
#       pragma comment(lib, "DuiLib.lib")
#   endif
#endif

#define USE(FEATURE) (defined USE_##FEATURE  && USE_##FEATURE)
#define ENABLE(FEATURE) (defined ENABLE_##FEATURE  && ENABLE_##FEATURE)






#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


// TODO:  在此处引用程序需要的其他头文件
#define WM_AFTER_BROSWER_CREATE					WM_USER+0x1001

#define WM_LOADSTART							WM_USER+0x2000
#define WM_LOADEND								WM_USER+0x2001
#define WM_LOADPOPUP							WM_USER+0x2002
#define WM_TITLECHANGE							WM_USER+0x2003// TODO:  在此处引用程序需要的其他头文件

#define WM_ADDITEM								WM_USER+0x2005



#endif