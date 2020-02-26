// ----------------------------------------------------------------------- //
//
//  FILENAME:	SS_Log.cpp
//  AUTHOR:		Steve Schaneville
//  CREATED:	23 Oct 2001, 16:07
//
//  PURPOSE:	
//
//  Copyright (c) 2001
//
// ----------------------------------------------------------------------- //

// ------------------[       Pre-Include Defines       ]------------------ //
// ------------------[          Include Files          ]------------------ //
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include "SS_RegistryKey.h"

// we #define __SS_Log_Include_h__ so the SS_Log.h file does not tell is to 
// include SS_Log_Include.h instead
#define __SS_Log_Include_h__ 
#include "SS_Log.h"
#undef __SS_Log_Include_h__

// ------------------[      Macros/Constants/Types     ]------------------ //
typedef struct tagFindWindow
{
    TCHAR   szWindowName[MAX_PATH];
    HWND    hWnd;
} SS_FIND_WINDOW;

// ------------------[         Global Variables        ]------------------ //
#ifdef _SS_LOG_ACTIVE

SS_Log g_Log;
static HANDLE g_hPipe; 

#endif

// ------------------[         Global Functions        ]------------------ //
BOOL CALLBACK EnumWindowsProc( HWND hwnd, LPARAM lParam )
{
    SS_FIND_WINDOW* pFindWindow = (SS_FIND_WINDOW*)lParam;

    TCHAR szText[256];
    GetWindowText(hwnd, szText, 256);
    if( !_tcscmp(szText, pFindWindow->szWindowName) )
    {
        pFindWindow->hWnd = hwnd;
        return FALSE;
    }

    return TRUE;
}

// ------------------[    Class Function Definitions   ]------------------ //

//std constructor
SS_Log::SS_Log()
{
    ::InitializeCriticalSection(&m_cs);
    InitObject();
}

// SS_Log::SS_Log(LPCTSTR szFilename, LPCTSTR szWindowName)
// {
//     InitObject();
// 
//     // set log window name
//     WindowName(szWindowName);
// 
//     // set the log filename, adding ".log" if not there
//     TCHAR szFile[MAX_PATH];
//     _tcscpy(szFile, szFilename);
//     TCHAR* szTmp = _tcstok( szFile, _T(".") );
//     if( !_tcscmp(szFile,szFilename) )
//     {
//         _tcscat(szFile, _T(".log") );
//         Filename(szFile);
//     }
//     else
//         Filename(szFilename);
// }

// std destructor
SS_Log::~SS_Log()
{
    delete[]m_szLogFile;
	m_szLogFile = NULL;
    delete[]m_szLogWindow;
	m_szLogWindow = NULL;
    ::DeleteCriticalSection(&m_cs);
}

// copy constructor
SS_Log::SS_Log( SS_Log& rhs )
{
    *this = rhs;
}

// assignment operator
SS_Log&	SS_Log::operator = (SS_Log& rhs)
{
    Filter( rhs.Filter() );
    Filename( rhs.Filename() );
    WindowName( rhs.WindowName() );
    FileHandle( rhs.FileHandle() );
    
    return *this;
}

// ----------------------------------------------------------------------- //
//  Function:		InitObject
//  Author:			Steve Schaneville
//  Parameters:		none
//  Return Values:	none
//  Notes:			All constructors should call this function.
//                  The defaults are different for release and debug builds.
// ----------------------------------------------------------------------- //
VOID SS_Log::InitObject()
{
    DWORD dwSize = MAX_PATH;
    m_szLogFile = new TCHAR[MAX_PATH];
    GetLogFileFromReg(m_szLogFile, dwSize);
    m_szLogWindow = new TCHAR[MAX_PATH];
    _tcscpy( m_szLogWindow, _T("Default Log Window") );
    FileHandle(NULL);
    
    ASSERT(m_szLogFile);

#ifdef _DEBUG

    Filter( LOGTYPE_DEBUG | LOGTYPE_CRITICAL | LOGTYPE_LOGTOWINDOW );
    
#else // _DEBUG

    Filter( LOGTYPE_RELEASE | LOGTYPE_CRITICAL | LOGTYPE_LOGTOFILE );
    
#endif // _DEBUG

#ifdef _SS_LOG_ACTIVE
    g_hPipe = INVALID_HANDLE_VALUE;
#endif // _SS_LOG_ACTIVE    
}
  
// ----------------------------------------------------------------------- //
//  Function:		SS_Log::GetLogFile
//  Author:			Steve Schaneville
//  Parameters:		szFilenname (out) - a buffer for the filename
//                  dwSize (in, out) - in, size of the filename buffer.
//                                     out, size written to the filename buffer.
//  Return Values:	ERROR_SUCCESS if successful, MS error code if not.
//  Notes:			Get the default filename for the log file, stored in
//                  the registry.
// ----------------------------------------------------------------------- //
LONG SS_Log::GetLogFileFromReg(LPTSTR szFilename, DWORD& dwSize)
{
    LONG lResult = ERROR_SUCCESS;

#ifdef _SS_LOG_ACTIVE
    
    lResult = GetRegValue( SS_LOG_REG_HIVE, SS_LOG_REG_KEY, 
                           SS_LOG_REG_LOGFILE, szFilename, &dwSize);
    if( ERROR_SUCCESS != lResult )
    {
        //SS_RegistryKey key;
        lResult = SetRegValue( SS_LOG_REG_HIVE, SS_LOG_REG_KEY, 
                               SS_LOG_REG_LOGFILE, 
                               SS_LOG_DEFAULT_LOGFILE );
        _tcsncpy(szFilename, SS_LOG_DEFAULT_LOGFILE, dwSize);
    }

#endif // _SS_LOG_ACTIVE

    return lResult;
}

// ----------------------------------------------------------------------- //
//  Function:		SS_Log::GetFilterFromReg
//  Author:			Steve Schaneville
//  Parameters:		dwFilter (out) - the filter in the registry
//  Return Values:	0 if successful, non-zero if not.
//  Notes:			We simply look in the registry for each entry
//                  specified by the SS_LOG_REG_FILTER_* values.  If we
//                  don't find them, we create them and give them default
//                  values.  We place all filters into the provided dwFilter
//                  for returning to the calling function.
// ----------------------------------------------------------------------- //
LONG SS_Log::GetFilterFromReg(DWORD& dwFilter)
{
    LONG lResult = 0;
    DWORD dwFilterIsOn = 0;
    dwFilter = 0;
    
#ifdef _SS_LOG_ACTIVE
    
    // Debug
    lResult += GetRegValue( SS_LOG_REG_HIVE, SS_LOG_REG_KEY, 
        SS_LOG_REG_FILTER_DEBUG, dwFilterIsOn);
    if( ERROR_SUCCESS != lResult )
    {
        dwFilterIsOn = 1;
        lResult += SetRegValue( SS_LOG_REG_HIVE, SS_LOG_REG_KEY, 
            SS_LOG_REG_FILTER_DEBUG, dwFilterIsOn );
    }
    if( dwFilterIsOn )
        dwFilter |= LOGTYPE_DEBUG;
    
    
    // Release
    lResult += GetRegValue( SS_LOG_REG_HIVE, SS_LOG_REG_KEY, 
        SS_LOG_REG_FILTER_RELEASE, dwFilterIsOn);
    if( ERROR_SUCCESS != lResult )
    {
        dwFilterIsOn = 1;
        lResult += SetRegValue( SS_LOG_REG_HIVE, SS_LOG_REG_KEY, 
            SS_LOG_REG_FILTER_RELEASE, dwFilterIsOn );
    }
    if( dwFilterIsOn )
        dwFilter |= LOGTYPE_RELEASE;
    
    
    // Critical
    lResult += GetRegValue( SS_LOG_REG_HIVE, SS_LOG_REG_KEY, 
        SS_LOG_REG_FILTER_CRITICAL, dwFilterIsOn);
    if( ERROR_SUCCESS != lResult )
    {
        dwFilterIsOn = 1;
        lResult += SetRegValue( SS_LOG_REG_HIVE, SS_LOG_REG_KEY, 
            SS_LOG_REG_FILTER_CRITICAL, dwFilterIsOn );
    }
    if( dwFilterIsOn )
        dwFilter |= LOGTYPE_CRITICAL;


    // Warning
    lResult += GetRegValue( SS_LOG_REG_HIVE, SS_LOG_REG_KEY, 
        SS_LOG_REG_FILTER_WARNING, dwFilterIsOn);
    if( ERROR_SUCCESS != lResult )
    {
        dwFilterIsOn = 1;
        lResult += SetRegValue( SS_LOG_REG_HIVE, SS_LOG_REG_KEY, 
            SS_LOG_REG_FILTER_WARNING, dwFilterIsOn );
    }
    if( dwFilterIsOn )
        dwFilter |= LOGTYPE_WARNING;
    
    
    // Normal
    lResult += GetRegValue( SS_LOG_REG_HIVE, SS_LOG_REG_KEY, 
        SS_LOG_REG_FILTER_NORMAL, dwFilterIsOn);
    if( ERROR_SUCCESS != lResult )
    {
        dwFilterIsOn = 1;
        lResult += SetRegValue( SS_LOG_REG_HIVE, SS_LOG_REG_KEY, 
            SS_LOG_REG_FILTER_NORMAL, dwFilterIsOn );
    }
    if( dwFilterIsOn )
        dwFilter |= LOGTYPE_NORMAL;
    
    
    // Trace
    lResult += GetRegValue( SS_LOG_REG_HIVE, SS_LOG_REG_KEY, 
        SS_LOG_REG_FILTER_TRACE, dwFilterIsOn);
    if( ERROR_SUCCESS != lResult )
    {
        dwFilterIsOn = 1;
        lResult += SetRegValue( SS_LOG_REG_HIVE, SS_LOG_REG_KEY, 
            SS_LOG_REG_FILTER_TRACE, dwFilterIsOn );
    }
    if( dwFilterIsOn )
        dwFilter |= LOGTYPE_TRACE;
    
    
    // Log to Window
    lResult += GetRegValue( SS_LOG_REG_HIVE, SS_LOG_REG_KEY, 
        SS_LOG_REG_FILTER_LOGTOWINDOW, dwFilterIsOn);
    if( ERROR_SUCCESS != lResult )
    {
        dwFilterIsOn = 1;
        lResult += SetRegValue( SS_LOG_REG_HIVE, SS_LOG_REG_KEY, 
            SS_LOG_REG_FILTER_LOGTOWINDOW, dwFilterIsOn );
    }
    if( dwFilterIsOn )
        dwFilter |= LOGTYPE_LOGTOWINDOW;
    
    
    // Log to File
    lResult += GetRegValue( SS_LOG_REG_HIVE, SS_LOG_REG_KEY, 
        SS_LOG_REG_FILTER_LOGTOFILE, dwFilterIsOn);
    if( ERROR_SUCCESS != lResult )
    {
        dwFilterIsOn = 1;
        lResult += SetRegValue( SS_LOG_REG_HIVE, SS_LOG_REG_KEY, 
            SS_LOG_REG_FILTER_LOGTOFILE, dwFilterIsOn );
    }
    if( dwFilterIsOn )
        dwFilter |= LOGTYPE_LOGTOFILE;
    
#endif // _SS_LOG_ACTIVE
        
    return lResult;
}

// ----------------------------------------------------------------------- //
//  Function:		SS_Log::WriteLog()
//  Author:			Steve Schaneville
//  Parameters:		szFile      - The name of the file in which the log
//                                originated.  Use __FILE__ in the MS 
//                                compiler.
//                  nLine       - The line number of the file in which the log
//                                originated.  Use __LINE__ in the MS 
//                                compiler.
//                  dwFilterLog - The filter to use for this logged message.
//                                Valid values are those defined in the 
//                                SS_LogFilterType enum in "SS_Log.h"
//                  pMsg        - A sprintf-style format string.
//                  args        - The parameter list for the sprintf arguments.
//  Return Values:	none
//  Notes:			This function takes the following steps:
//                  1. Prep the Filter - this involves determining if any
//                     LOGTYPE_OUTPUTS filters were specified, and if not,
//                     adding the default ones.  We also check if any
//                     LOGTYPE_BUILDS filters were specified, and if not,
//                     add the default ones.  Finally we check if any
//                     LOGTYPE_LEVELS filters were specified, and if not,
//                     add the default ones.  
//                  2. Perform the sprintf (_vstprintf) on the pMsg and args.
//                  3. Get the current local time.
//                  4. Create the level string with a bunch of ifs.  This
//                     really needs to be made differently, because currently,
//                     if we add any new levels, this code will have to be 
//                     changed, but we'd like to be able to add levels without
//                     making changes to this code.
//                  4. Put all info (time, file, line #, level, message) into
//                     a tab-delimited buffer.
//                  5. Check to see if we need to process this message by
//                     comparing the provided filter (dwFilterLog) with the 
//                     filter in the registry.  See comments below for details.
//                  6. If we are processing this message, check to see if
//                     we need to send it to a log file.  If so, sent it to
//                     the file in SS_Log::Filename().
//                  7. If we are processing this message, check to see if
//                     we need to send it to a log window.  If so, first
//                     make sure that the log window specified in 
//                     SS_Log::WindowName() is open, and if not, create it.  
//                     Then send the message to it.
// ----------------------------------------------------------------------- //
VOID SS_Log::WriteLog( TCHAR* szFile, int nLine, DWORD dwFilterLog, 
    TCHAR* pMsg, va_list* args )
{
#ifdef _SS_LOG_ACTIVE

    ASSERT(pMsg);
    ASSERT(args);

    const TCHAR*        pcTimeFormat = _T("%04d-%02d-%02d %02d:%02d:%02d");
    const TCHAR*        pcFinalFormat = _T("[%05d]\t%s\t%s\r\n");

    WCHAR*      pcBuffer = NULL;
    WCHAR*      pcFinalBuffer = NULL;

    int         nchars;
    TCHAR       szTime[MAX_PATH] = {L'\0'};
    SYSTEMTIME  ust;
    SYSTEMTIME  lst;
    DWORD       cbWritten = 0;
    HANDLE      hLog = INVALID_HANDLE_VALUE;

    do 
    {
        // prep the filter... if we are using the default filter, just
        // use it as is.  If not, we need to add the default LOGTYPE_OUTPUTS
        // and default LOGTYPE_BUILDS type filters
        SetFilter(dwFilterLog);

        nchars = _vsctprintf(pMsg, *args);
        if (0 == nchars)
            break;

        pcBuffer = new WCHAR[nchars + 1];
        if (NULL == pcBuffer)
            break;

        ::ZeroMemory(pcBuffer, sizeof(WCHAR) * (nchars + 1));

        pcFinalBuffer = new WCHAR[nchars + MAX_PATH];
        if (NULL == pcFinalBuffer)
            break;

        ::ZeroMemory(pcFinalBuffer, sizeof(WCHAR) * (nchars + MAX_PATH));
        _vstprintf(pcBuffer, pMsg, *args);

        // prep message time
        GetSystemTime(&ust);
        SystemTimeToTzSpecificLocalTime(NULL, &ust, &lst);
        _stprintf(szTime, pcTimeFormat, 
            lst.wYear, lst.wMonth, lst.wDay,
            lst.wHour, lst.wMinute, lst.wSecond);

        // prep the final output buffer
        _stprintf(pcFinalBuffer, pcFinalFormat, ::GetCurrentThreadId(), szTime, pcBuffer);

        // log to file
        Lock();
        hLog = ::CreateFile( Filename(), GENERIC_WRITE, 0, NULL,
            OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 
            NULL );

        if (INVALID_HANDLE_VALUE != hLog) 
        {
            ::SetFilePointer(hLog, 0, NULL, FILE_END);

            ::WriteFile(
                hLog,
                (LPCVOID)pcFinalBuffer, 
                _tcslen(pcFinalBuffer) * sizeof(TCHAR),
                &cbWritten,
                NULL);

            ::FlushFileBuffers(hLog);
            ::CloseHandle(hLog);
        }

        UnLock();
    } while (0);

    SAFE_DELETE_ARRAY(pcBuffer);
    SAFE_DELETE_ARRAY(pcFinalBuffer);

#endif // _SS_LOG_ACTIVE
}

// ----------------------------------------------------------------------- //
//  Function:		SS_Log::CreateLogWindowPipe
//  Author:			Steve Schaneville
//  Parameters:		none
//  Return Values:	true if successful, false if not
//  Notes:			We use the EnumWindows() API function to compare the
//                  names of all open windows with the name specified in
//                  SS_Log::WindowName().  If we don't find a match, we
//                  create a new instance of SS_Log_Window.exe and pass
//                  it the SS_Log::WindowName() name as a commandline param.
//                  IMPORTANT:  SS_Log_Window.exe must be somewhere in the 
//                  path for this function to succeed.
// ----------------------------------------------------------------------- //
BOOL SS_Log::OpenLogWindow()
{

#ifdef _SS_LOG_ACTIVE
    
    // we must create the log window if is it not already running
#ifdef _LCS
    WNDENUMPROC lpEnumFunc = (int(_stdcall*)())EnumWindowsProc;
#else
    WNDENUMPROC lpEnumFunc = EnumWindowsProc;
#endif

    SS_FIND_WINDOW findWindow;
    _tcscpy( findWindow.szWindowName, WindowName() );
    findWindow.hWnd = NULL;
    EnumWindows( lpEnumFunc, (LPARAM)&findWindow );
    if( !findWindow.hWnd )
    {
        PROCESS_INFORMATION pi;
        STARTUPINFO si;
        si.lpDesktop = NULL;
        si.lpTitle = NULL;
        si.dwFlags = STARTF_USESHOWWINDOW;
        si.wShowWindow = TRUE;
        si.cb = sizeof(si);
        si.cbReserved2 = 0;
        si.lpReserved = NULL;
        si.lpReserved2 = NULL;
        
        TCHAR szCommandLine[MAX_PATH];
        _stprintf( szCommandLine, L"SS_Log_Window \"%s\"", WindowName() );
        INT nResult = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, 
            NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);            
        
        Sleep(500);
        if( !nResult )
        {
            ::MessageBox(NULL, _T("Could not start the SS_Log_Window.exe application... does it reside in the system path?"), _T("Error"), MB_OK|MB_ICONSTOP);
            return FALSE;
        }
    }
    
#endif // _SS_LOG_ACTIVE
    
    return TRUE;
}

// ----------------------------------------------------------------------- //
//  Function:		SS_Log::EraseLog()
//  Author:			Steve Schaneville
//  Parameters:		none
//  Return Values:	none
//  Notes:			Erases both the log file and window of current log.
//                  Note that if multiple logs are writing to the same
//                  file or window simultaniously, this function will
//                  erase info from all logs involved, not just the entries
//                  that were created by the log calling this function.
// ----------------------------------------------------------------------- //
VOID SS_Log::EraseLog()
{

#ifdef _SS_LOG_ACTIVE
    
#ifdef _LCS
    WNDENUMPROC lpEnumFunc = (int(_stdcall*)())EnumWindowsProc;
#else
    WNDENUMPROC lpEnumFunc = EnumWindowsProc;
#endif
    
    SS_FIND_WINDOW findWindow;
    _tcscpy( findWindow.szWindowName, WindowName() );
    findWindow.hWnd = NULL;
    EnumWindows( lpEnumFunc, (LPARAM)&findWindow );

    if( (Filter() & LOGTYPE_LOGTOWINDOW) && findWindow.hWnd )
    {
        DWORD dwBytesRead = 0;
        TCHAR szBuffer[] = _T("~~WMSS_LOG_ERASE_LOG~~");
        TCHAR szPipeName[MAX_PATH];
        _stprintf( szPipeName, _T("\\\\.\\pipe\\%s"), WindowName() );

        BOOL bResult = CallNamedPipe(szPipeName, szBuffer, 
            _tcslen(szBuffer)+1, (LPVOID)NULL, 
            0, &dwBytesRead, 
            NMPWAIT_USE_DEFAULT_WAIT);
    }

    if( (Filter() & LOGTYPE_LOGTOFILE) )
        ::DeleteFile(Filename());

#endif // _SS_LOG_ACTIVE
    
}

VOID SS_Log::SetFilter(DWORD& dwFilter)
{

#ifdef _SS_LOG_ACTIVE
    
    if( !(dwFilter & LOGTYPE_OUTPUTS) )
    {
        // no LOGTYPE_OUTPUTS were specified, so we add the default
        dwFilter |= Filter() & LOGTYPE_OUTPUTS;
    }
    
    if( !(dwFilter & LOGTYPE_BUILDS) )
    {
        // no LOGTYPE_BUILDS were specified, so we add the default
        dwFilter |= Filter() & LOGTYPE_BUILDS;
    }

    if( !(dwFilter & LOGTYPE_LEVELS) )
    {
        // no LOGTYPE_LEVELS were specified, so we add the default
        dwFilter |= Filter() & LOGTYPE_LEVELS;
    }

#endif // _SS_LOG_ACTIVE
    
}
