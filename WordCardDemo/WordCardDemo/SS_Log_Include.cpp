// ----------------------------------------------------------------------- //
//
//  FILENAME:	SS_Log_Include.cpp
//  AUTHOR:		Steve Schaneville
//  CREATED:	24 Oct 2001, 14:49
//
//  PURPOSE:	
//
//  Copyright (c) 2001
//
// ----------------------------------------------------------------------- //

// ------------------[       Pre-Include Defines       ]------------------ //
// ------------------[          Include Files          ]------------------ //
#include "stdafx.h"
#include "SS_Log_Include.h"

// ------------------[      Macros/Constants/Types     ]------------------ //
// ------------------[         Global Variables        ]------------------ //
// ------------------[         Global Functions        ]------------------ //

#ifdef _SS_LOG_ACTIVE

TCHAR g_szFile[MAX_PATH]; 
int g_nLine;

void SetLogFilePathName(const WCHAR * pcLogFilePathName)
{
    DWORD dwLogFilter = LOGTYPE_LOGTOFILE;
    if (NULL == pcLogFilePathName)
        return;

    g_Log.Filename(pcLogFilePathName);
    g_Log.Filter(dwLogFilter);
}

/// 定义 SWITCH_WRITE_LOG_ENABLE 时，写日志
/// 发行版时，注释掉此开关
#define SWITCH_WRITE_LOG_ENABLE

#ifdef SWITCH_WRITE_LOG_ENABLE
#define ENABLE_WRITE_LOG
#else
#define ENABLE_WRITE_LOG return;
#endif

VOID WriteLog(SS_Log* pLog, DWORD dwFilter, TCHAR* pMsg, va_list* args)
{
    ENABLE_WRITE_LOG
    pLog->WriteLog(g_szFile, g_nLine, dwFilter, pMsg, args);
}

VOID WriteLog(SS_Log* pLog, DWORD dwFilter, TCHAR* pMsg, ...)
{
    ENABLE_WRITE_LOG
    va_list args;
    va_start(args, pMsg);
    
    WriteLog( pLog, dwFilter, pMsg, &args );
    
    va_end(args);
}

VOID WriteLog(SS_Log* pLog, TCHAR* pMsg, ...)
{
    ENABLE_WRITE_LOG
    va_list args;
    va_start(args, pMsg);
    
    WriteLog( pLog, pLog->Filter(), pMsg, &args );
    
    va_end(args);
}

VOID WriteLog(DWORD dwFilter, TCHAR* pMsg, ...)
{
    ENABLE_WRITE_LOG
    va_list args;
    va_start(args, pMsg);
    
    WriteLog( &g_Log, dwFilter, pMsg, &args );
    
    va_end(args);
}

VOID WriteLogEx(TCHAR* pMsg, ...)
{
    ENABLE_WRITE_LOG
    va_list args;
    va_start(args, pMsg);
    
    WriteLog( &g_Log, g_Log.Filter(), pMsg, &args );
    
    va_end(args);
}

VOID LogStoreFileLine(TCHAR* szFile, int nLine)
{
//	_tcscpy(g_szFile, szFile);
	g_nLine = nLine;
}


#else // _SS_LOG_ACTIVE

#define Log

#endif // _SS_LOG_ACTIVE



// ------------------[    Class Function Definitions   ]------------------ //
