// tracetool.cpp
//
// Author : Thierry Parent
// Version : 12.4
//
// HomePage :  http://www.codeproject.com/csharp/TraceTool.asp
// Download :  http://sourceforge.net/projects/tracetool/
// See License.txt for license information

//#include "stdafx.h"   // remove precompiled header
#include <windows.h>
#include <stdio.h>
#include <winsock.h>
#include <tchar.h>
#include <assert.h>
#include <vector>
#include <fstream>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#ifndef _WIN32_WCE
    #include <process.h>
#else
    #include <Kfuncs.h>
#endif

#include "tracetool.h"
#pragma comment(lib, "Ws2_32.lib")
#pragma warning (disable: 4311 4312)


namespace
{
	// イニシャライザ
	struct DbgTraceInitializer
	{
#if !defined(_CONSOLE)
		FILE* _fpStdOut;
#endif

		DbgTraceInitializer()
		{
			// 初期化
			//_tsetlocale(LC_ALL, _T("jpn"));
			_tsetlocale(LC_ALL, _T(""));
#if !defined(_CONSOLE)
			// for Windows application
			if (AllocConsole()) {
				_tfreopen_s(&_fpStdOut, _T("CONOUT$"), _T("w"), stdout);
			}
			else {
				// とりあえず失敗は無視
			}
			//DebugTrace(_T("Debug console started.\n"));
			TRACE_SEND(_T("Debug console started."));
#endif
		}

		~DbgTraceInitializer()
		{
			// 後始末
#if !defined(_CONSOLE)
			if (_fpStdOut != NULL) {
				fclose(_fpStdOut);
				_fpStdOut = NULL;
				FreeConsole();
			}
#endif
		}
	};
}


namespace 
{
	DWORD WaitWithMsgQueue(DWORD nCount, const HANDLE* pHandles, BOOL fWaitAll, DWORD dwMilliseconds)
	{
		DWORD dw, tcNow, tcEnd, dwTimeToWait; 
		DWORD dwResult = 0;
		MSG msg;
		DWORD i;
		bool *ba = 0;
		HANDLE myHandles[MAXIMUM_WAIT_OBJECTS];
		unsigned int myCount;

		// Initialize handle array
		for (i = 0; i < nCount; i++)
		{
			myHandles[i] = pHandles[i];
		}
		myCount = nCount;

		tcNow = GetTickCount();
		tcEnd = tcNow + dwMilliseconds;

		bool bRepeat = true;
		while (bRepeat) 
		{
			if ((dwMilliseconds != INFINITE) && (tcNow >= tcEnd))
			{
				dwResult = WAIT_TIMEOUT;
				goto Cleanup;
			}

			if (dwMilliseconds != INFINITE)
			{
				dwTimeToWait = tcEnd - tcNow;
			}
			else
			{
				dwTimeToWait = INFINITE;
			}

			dw = MsgWaitForMultipleObjects(myCount, myHandles, false, dwTimeToWait, QS_ALLINPUT);
			// Object got signaled
			if (dw < (WAIT_OBJECT_0 + myCount))
			{
				if (fWaitAll)
				{
					// Remove object
					i = dw - WAIT_OBJECT_0;
					myCount--;
					for (unsigned int j = i; j < myCount - 1 && myCount > 0; j++)
					{
						myHandles[j] = myHandles[j + 1];
					}

					// Exit if all objects have been signaled
					if (myCount == 0)
					{
						dwResult = WAIT_OBJECT_0;
						goto Cleanup;
					}
				}
				else
				{
					dwResult = dw;
					goto Cleanup;
				}
			}
			// Got message
			else if (dw == (WAIT_OBJECT_0 + nCount))
			{
				while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
				{
					if (msg.message == WM_QUIT)
					{
						dwResult = dw;
						goto Cleanup;
					}

					DispatchMessage(&msg);

					// Check for timeout
					tcNow = GetTickCount();
					if ((dwMilliseconds != INFINITE) && (tcNow >= tcEnd))
					{
						dwResult = WAIT_TIMEOUT;
						goto Cleanup;
					}
				}
			}
			// WAIT timed out
			else if (dw == WAIT_TIMEOUT)
			{
				dwResult = dw;
				goto Cleanup;
			}
			// WAIT failed
			else
			{
				dwResult = dw;
				goto Cleanup;
			}

			tcNow = GetTickCount();
		}

Cleanup:
		if (ba) 
			delete ba;
		return dwResult;
	}

	BOOL LaunchCommand(const std::wstring& command, BOOL waitForEnd, BOOL minimized = FALSE)
	{
		PROCESS_INFORMATION processInfo;
		STARTUPINFOW startupInfo;
		memset(&startupInfo, 0, sizeof(startupInfo));
		startupInfo.cb = sizeof(startupInfo);
		startupInfo.dwFlags = minimized ? STARTF_USESHOWWINDOW : 0;
		startupInfo.wShowWindow = SW_SHOWMINIMIZED;
		int res = CreateProcessW(0,
			const_cast<wchar_t*>(command.c_str()),
			0, 0,
			FALSE,
			CREATE_NO_WINDOW,
			0, 0, &startupInfo, &processInfo);

		if (res == 0)
		{
			return FALSE;
		}

		if (waitForEnd)
		{
			WaitWithMsgQueue(1, &processInfo.hProcess, true, INFINITE);			
		}

		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
		return TRUE;
	}


	void Location_Regedit(const std::wstring& rootkey, const std::wstring& key)
	{

		std::wstring cmd;
		cmd.append(L"regjump.exe ");
		cmd.append(rootkey);
		cmd.append(L"\\");
		cmd.append(key);
		LaunchCommand(cmd, TRUE);
	}
}

namespace cactus
{
	void trace_regkey(const wchar_t* rootkey, const wchar_t* key)
	{
		TTrace::Debug()->SendW(rootkey, key);		
		Location_Regedit(rootkey, key);
	}
}



static const int kMaxLogLen = 16 * 1024;
#define MAX_LEN (kMaxLogLen + 1)

//#include "vld.h"

// static TTrace initialization

struct sockaddr_in TTrace::m_serverSockAddr ;              // Socket adress
CRITICAL_SECTION   TTrace::criticalSection ;
TraceOptions *     TTrace::m_options = new TraceOptions();
WinTrace *         TTrace::m_winTrace = new WinTrace() ;
WinWatch *         TTrace::m_winWatch = new WinWatch() ;
InternalWinTrace * TTrace::DefaultWinTrace = new InternalWinTrace() ;
deque <InternalWinTrace *> * TTrace::FormTraceList = new deque <InternalWinTrace *> ;
deque <CommandList *> * TTrace::setMessageList = new deque <CommandList *> ;
deque <CommandList *> * TTrace::getMessageList ;  // handled by thread
bool               TTrace::isStopping ;

WORD               TTrace::m_DeviceId1 = 0;
WORD               TTrace::m_DeviceId2 = 0;
WORD               TTrace::m_DeviceId3 = 0;
bool               TTrace::m_IsSocketInitialized = false ;
SOCKET             TTrace::m_socketHandle = -1;
DWORD              TTrace::m_ClockSequenceBase = TTrace::Init();  // Initialize the Tracetool system

// thread vars
HANDLE TTrace::CloseEvent ;
HANDLE TTrace::MessageReady ;
HANDLE TTrace::ClosedEvent ;
HANDLE TTrace::HandlesToWaitFor[2] ;
HANDLE TTrace::hThread ;

//==========================================================================
// TTRACE
//==========================================================================

/// <summary>
// Initialize the Tracetool system
/// </summary>
/// <returns>random number used by createTraceId</returns>

DWORD TTrace::Init(void)
{
    InitializeCriticalSection(&criticalSection);   // TTrace::criticalSection
    // The spatially unique node identifier. Because we are not sure there is a network card ,
    // we generate random number instead of unique network card id.
    m_DeviceId1 = LOWORD(GetTickCount() * rand());
    m_DeviceId2 = LOWORD(rand());
    m_DeviceId3 = LOWORD(rand());
    // add the main internalWintrace to list (owner)
    FormTraceList->push_back(DefaultWinTrace) ;
    CloseEvent = CreateEvent(
                     NULL,   // default security attributes
                     TRUE,   // auto-reset event object
                     FALSE,  // initial state is nonsignaled
                     NULL);  // unnamed object
    MessageReady = CreateEvent(
                       NULL,   // default security attributes
                       TRUE,   // auto-reset event object
                       FALSE,  // initial state is nonsignaled
                       NULL);  // unnamed object
    ClosedEvent = CreateEvent(
                      NULL,   // default security attributes
                      TRUE,   // auto-reset event object
                      FALSE,  // initial state is nonsignaled
                      NULL);  // unnamed object
    HandlesToWaitFor[0] = CloseEvent ;
    HandlesToWaitFor[1] = MessageReady ;
    // Create the sender thread.
    isStopping = false ;
    hThread = (HANDLE)_beginthreadex(
                  NULL,                //   void *security,                        // Pointer to a SECURITY_ATTRIBUTES structure that determines whether the returned handle can be inherited by child processes. If NULL, the handle cannot be inherited. Must be NULL for Windows 95 applications.
                  0,                   //   unsigned stack_size,                   // Stack size for a new thread or 0.
                  &MsgThread,          //   unsigned ( *start_address )( void * ), // Start address of a routine that begins execution of a new thread. For _beginthread, the calling convention is either __cdecl or __clrcall; for _beginthreadex, it is either __stdcall or __clrcall.
                  NULL,                //   void *arglist,                         // Argument list to be passed to a new thread or NULL.
                  0,                   //   unsigned initflag,                     // Initial state of a new thread (0 for running or CREATE_SUSPENDED for suspended); use ResumeThread to execute the thread.
                  NULL);          //   unsigned *thrdaddr                     // Points to a 32-bit variable that receives the thread identifier. Might be NULL, in which case it is not used.
    // sequence number
    m_ClockSequenceBase = rand();
    return m_ClockSequenceBase ;
}

//-------------------------------------------------------------------------

/// <summary>
// Stop the Tracetool system. Called by global destructor
/// </summary>
void TTrace::Stop(void)
{
    // check if the Stop method was not already called.
    if (CloseEvent == NULL)
        return ;

    isStopping = true ;
    SetEvent(CloseEvent) ;
    DWORD waitStatus = WaitForSingleObject(ClosedEvent, 1000) ; // wait 1s max for the closed event
    CloseHandle(CloseEvent) ;
    CloseHandle(ClosedEvent) ;
    CloseHandle(MessageReady) ;
    CloseEvent   = NULL ;
    ClosedEvent  = NULL ;
    MessageReady = NULL ;
    // Destroy the thread object.
    CloseHandle(hThread);
    // close socket
    CloseSocket() ;

    if (m_options != NULL)
        delete m_options ;

    m_options = NULL ;

    if (m_winTrace != NULL)
        delete m_winTrace ;

    m_winTrace = NULL ;

    if (m_winWatch != NULL)
        delete m_winWatch ;

    m_winWatch = NULL ;
    // delete FormTraceList content and array (included the main internalWintrace)
    deque <InternalWinTrace *>::const_iterator WBegin;
    deque <InternalWinTrace *>::const_iterator WEnd;
    WEnd  = FormTraceList->end() ;

    for (WBegin = FormTraceList->begin(); WBegin < WEnd ; WBegin++) {
        InternalWinTrace * wintrace = * WBegin ;
        delete wintrace ;
    }

    delete FormTraceList ;
    FormTraceList = NULL ;
    // delete critical section
    DeleteCriticalSection(&criticalSection);  // TTrace::criticalSection
    // delete remaining traces in setMessageList
    deque <CommandList *>::const_iterator CommandBegin;
    deque <CommandList *>::const_iterator CommandEnd;
    // delete the send message list
    CommandEnd  = setMessageList->end() ;

    for (CommandBegin = setMessageList->begin(); CommandBegin < CommandEnd ; CommandBegin++) {
        CommandList * Commands = * CommandBegin ;
        Commands->clear() ;
        delete Commands ;   // the only place where the command is deleted
    }  // next messageList

    setMessageList->clear() ;
    delete setMessageList ;
    setMessageList = NULL ;
    // the getMessageList is handled by the thread
}

//-------------------------------------------------------------------------
/// <summary>
// Flush messages to viewer
/// </summary>

void TTrace::Flush()
{
    Flush(5000) ;
}

//-------------------------------------------------------------------------
/// <summary>
// Flush messages to viewer
/// </summary>

void TTrace::Flush(int FlushTimeOut)
{
    if (m_winTrace == NULL)
        return ;

    // create the event
    HANDLE flushEvent ;
    flushEvent = CreateEvent(
                     NULL,   // default security attributes
                     TRUE,   // auto-reset event object
                     FALSE,  // initial state is nonsignaled
                     NULL);  // unnamed object
    // create the flush event message
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_FLUSH, int(flushEvent));
    TTrace::SendToClient(Commands);
    // wait for the sender read events. (max 5 sec)
    WaitForSingleObject(flushEvent, FlushTimeOut) ;
    // release event
    CloseHandle(flushEvent) ;
}

//-------------------------------------------------------------------------
/// <summary>
// Close viewer connection.<p>
// Replace TTrace destructor
/// </summary>

void TTrace::CloseSocket()
{
    if (m_IsSocketInitialized == true) {
        // close connection
        shutdown(m_socketHandle, 2);  // SD_BOTH
        closesocket(m_socketHandle);
        WSACleanup();
        m_IsSocketInitialized = false ;
    }
}
//-------------------------------------------------------------------------
/// <summary>
// Helper function : convert a C string to widestring .<p>
// The caller is responsive to destroy the new string
/// </summary>
/// <param name="MbsStr">The string to convert</param>
/// <returns>pointer to the new wide string</returns>

wchar_t * TTrace::MbsToWide(const char * MbsStr)
{
    if (MbsStr == NULL)
        return NULL ;

    size_t lenStr = strlen(MbsStr) + 1 ;
    wchar_t * result = (wchar_t *) malloc(lenStr * sizeof(wchar_t)) ;
    mbstate_t psbuf;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
    size_t i ;
    mbsrtowcs_s(&i, result, lenStr, &MbsStr, lenStr, &psbuf);
#else
    mbsrtowcs(result , &MbsStr, lenStr, &psbuf);
#endif
    return result ;
}

//-------------------------------------------------------------------------

/// <summary>
// Helper function : convert a widestring to a C string.<p>
// The caller is responsive to destroy the new string
/// </summary>
/// <param name="WideStr">The wide string to convert</param>
/// <returns>pointer to the new string</returns>

char * TTrace::WideToMbs(const wchar_t * WideStr)
{
    if (WideStr == NULL)
        return NULL ;

    DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, WideStr, -1, NULL, 0, NULL, FALSE);
    size_t lenStr = dwNum;
    char * result = (char *) malloc(lenStr) ;
    WideCharToMultiByte(CP_OEMCP, NULL, WideStr, -1, result, dwNum, NULL, FALSE);
    *(result + strlen(result)) = '\0';
    //size_t lenStr = wcslen(WideStr) + 1 ;
    //
    //char * result = (char *) malloc (lenStr) ;
    //#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
    //size_t i ;
    //wcstombs_s(&i, result, lenStr, WideStr, lenStr );
    //#else
    //wcstombs(result , WideStr, lenStr );
    //#endif
    return result ;
}

//-------------------------------------------------------------------------


/// <summary>
// Create an unique ID.<p>
// For windows CE compatibility reason, we cannot ask microsoft API to return an unique ID.<p>
// Since this function uses random generator instead of computer id it cannot be
// garanted that generated GUID is unique in the world.<p>
// The caller is responsive to destroy the new string
/// </summary>
/// <returns>pointer to a new trace id</returns>
char * TTrace::CreateTraceID()
{
	static DbgTraceInitializer initializer;	// 最初の呼び出し時のみ起動

    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    WORD Sequence ;
    char * bufferId ;
    SYSTEMTIME systemTime;
    FILETIME fileTime;
    GetSystemTime(&systemTime);
    BOOL bResult = SystemTimeToFileTime(&systemTime, &fileTime);

    if (!bResult)
        return NULL;

    //0-3    The low field of the timestamp.
    Data1 = fileTime.dwLowDateTime;
    //4-5    The middle field of the timestamp.
    Data2 = LOWORD(fileTime.dwHighDateTime);
    //6-7    The high field of the timestamp multiplexed
    //       with the version number.
    //       Version number is 0x0002
    Data3 = (HIWORD(fileTime.dwHighDateTime) & 0xFFF0) | 0x0002;
    //8-9    Here we store sequence number
    m_ClockSequenceBase++;
    Sequence = LOWORD(m_ClockSequenceBase);
    //10-15  The spatially unique node identifier.
    //       Because there is no network card we generate random number
    //       instead of unique network card id.
    bufferId = (char*)malloc(32 + 1);
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
    sprintf_s(bufferId, 33, "%08x%04x%04x%04x%04x%04x%04x", Data1, Data2, Data3, Sequence, m_DeviceId1, m_DeviceId2, m_DeviceId3) ;
#else
    sprintf(bufferId, "%08x%04x%04x%04x%04x%04x%04x", Data1, Data2, Data3, Sequence, m_DeviceId1, m_DeviceId2, m_DeviceId3) ;
#endif
    return bufferId;
}

//-------------------------------------------------------------------------

HWND StartDebugWin()
{
	
#ifndef _WIN32_WCE
    // first check if already running
    HWND hWndDBG;
    hWndDBG = FindWindow(TEXT("TFormReceiver"), TEXT("FormReceiver"));

    if (hWndDBG != NULL)
        return hWndDBG ;

    // get the path in the registry
    HKEY hKey;
    DWORD dwBufferSize = MAX_PATH;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW ;
    si.wShowWindow = SW_NORMAL ;
    ZeroMemory(&pi, sizeof(pi));
#ifdef UNICODE
    WCHAR Buffer[MAX_PATH];
#else
    char Buffer[MAX_PATH];
#endif // !UNICODE
    RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\TraceTool"), 0, KEY_READ, &hKey);
    RegQueryValueEx(hKey, TEXT("FilePath"), NULL, NULL, (LPBYTE) Buffer, &dwBufferSize);
    RegCloseKey(hKey);

    // run the tracetool process
    if (CreateProcess(Buffer, NULL, NULL, NULL, false, 0, NULL, NULL, &si, &pi) == false)
        return NULL ;

    // wait for the proces
    WaitForInputIdle(pi.hProcess, 3 * 1000); // wait for 3 seconds to get idle
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    // check if the window is created now
    hWndDBG = FindWindow(TEXT("TFormReceiver"), TEXT("FormReceiver"));

    if (hWndDBG != NULL)
        return hWndDBG ;

#endif
    // stil not found ? Run traceTool first to register it
    return NULL ;
}

//-------------------------------------------------------------------------
/// <summary>
/// Send the trace to the viewer
/// </summary>
/// <param name="Commands">The list of command to send</param>
/// <param name="winWatch">window watch</param>
void TTrace::SendToWinWatchClient(CommandList * Commands, const char * winWatchId)
{
    // add threading info
    if (TTrace::Options()->SendThreadId) {
        Commands->AddFront(CST_THREAD_ID, GetCurrentThreadId());
    }

    SYSTEMTIME Time;
    char buffer [MAX_PATH] ;
    // message time
    GetLocalTime(&Time);

    // Add time
    if (TTrace::Options()->SendDate) {
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
        sprintf_s(buffer, MAX_PATH, "%04d%2d%2d %02d:%02d:%02d:%03d",
                  Time.wYear, Time.wMonth , Time.wDay,
                  Time.wHour, Time.wMinute, Time.wSecond, Time.wMilliseconds);
#else
        sprintf(buffer, "%04d%2d%2d %02d:%02d:%02d:%03d",
                Time.wYear, Time.wMonth , Time.wDay,
                Time.wHour, Time.wMinute, Time.wSecond, Time.wMilliseconds);
#endif
    } else {
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
        sprintf_s(buffer, MAX_PATH, "%02d:%02d:%02d:%03d", Time.wHour, Time.wMinute, Time.wSecond, Time.wMilliseconds);
#else
        sprintf(buffer, "%02d:%02d:%02d:%03d", Time.wHour, Time.wMinute, Time.wSecond, Time.wMilliseconds);
#endif
    }

    Commands->AddFront(CST_MESSAGE_TIME, buffer);

    // Add process name
    if (TTrace::Options()->SendProcessName)
        Commands->AddFront(CST_PROCESS_NAME, TTrace::Options()->CheckProcessName());

    Commands->AddFront(CST_WINWATCH_ID, winWatchId);   // assume that winWatchId is not null
    SendToClient(Commands) ;
}

//-------------------------------------------------------------------------

/// <summary>
/// send the trace to the viewer
/// </summary>/// <param name="Commands">The list of command to send</param>
/// <param name="winTrace">window trace</param>
void TTrace::SendToWinTraceClient(CommandList * Commands, const char * winTraceId)
{
    SendToWinTraceClient(Commands , winTraceId, NULL , NULL) ;
}

//-------------------------------------------------------------------------

/// <summary>
/// send the trace to the viewer
/// </summary>/// <param name="Commands">The list of command to send</param>
/// <param name="winTrace">window trace</param>
void TTrace::SendToWinTraceClient(CommandList * Commands, const char * winTraceId, const char * dateTime, const char * threadName)
{
    // add threading info
    if (TTrace::Options()->SendThreadId) {
        if (threadName == NULL || threadName[0] == NULL)
            Commands->AddFront(CST_THREAD_ID, GetCurrentThreadId());
        else
            Commands->AddFront(CST_THREAD_NAME, threadName);
    }

    if (dateTime == NULL || dateTime[0] == NULL) {
        SYSTEMTIME Time;
        char buffer [MAX_PATH] ;
        // message time
        GetLocalTime(&Time);

        // Add time
        if (TTrace::Options()->SendDate) {
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
            sprintf_s(buffer, MAX_PATH, "%04d%2d%2d %02d:%02d:%02d:%03d",
                      Time.wYear, Time.wMonth , Time.wDay,
                      Time.wHour, Time.wMinute, Time.wSecond, Time.wMilliseconds);
#else
            sprintf(buffer, "%04d%2d%2d %02d:%02d:%02d:%03d",
                    Time.wYear, Time.wMonth , Time.wDay,
                    Time.wHour, Time.wMinute, Time.wSecond, Time.wMilliseconds);
#endif
        } else {
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
            sprintf_s(buffer, MAX_PATH, "%02d:%02d:%02d:%03d", Time.wHour, Time.wMinute, Time.wSecond, Time.wMilliseconds);
#else
            sprintf(buffer, "%02d:%02d:%02d:%03d", Time.wHour, Time.wMinute, Time.wSecond, Time.wMilliseconds);
#endif
        }

        Commands->AddFront(CST_MESSAGE_TIME, buffer);
    } else {
        Commands->AddFront(CST_MESSAGE_TIME, dateTime);
    }

    // Add process name
    if (TTrace::Options()->SendProcessName)
        Commands->AddFront(CST_PROCESS_NAME, TTrace::Options()->CheckProcessName());

    // CST_USE_TREE MUST be inserted at the first position
    if (winTraceId != NULL && winTraceId != "")
        Commands->AddFront(CST_USE_TREE , winTraceId);

    SendToClient(Commands) ;
}

//-------------------------------------------------------------------------

/// <summary>
/// Html encoding : convert special chars to ampersand #nnn;
/// </summary>
/// <param name="source">string to encode</param>
/// <returns>encoded string</returns>
string TTrace::HtmlEncode(string source)
{
    string target = "" ;

    if (source == "")
        return target;

    size_t length = source.length();
    size_t startIndex = 0;
    size_t currentIndex = 0;
    const char * s = source.c_str() ;

    while (currentIndex < length) {
        unsigned char ch = s[currentIndex];

        switch (ch) {
        case '<':
            if (startIndex < currentIndex)
                target.append(s, startIndex, currentIndex - startIndex);

            target.append("&lt;");
            startIndex = currentIndex + 1;
            break;

        case '>':
            if (startIndex < currentIndex)
                target.append(s, startIndex, currentIndex - startIndex);

            target.append("&gt;");
            startIndex = currentIndex + 1;
            break;

        case '"':
            if (startIndex < currentIndex)
                target.append(s, startIndex, currentIndex - startIndex);

            target.append("&quot;");
            startIndex = currentIndex + 1;
            break;

        case '&':
            if (startIndex < currentIndex)
                target.append(s, startIndex, currentIndex - startIndex);

            target.append("&amp;");
            startIndex = currentIndex + 1;
            break;

        default :
            if ((ch < 32) || ((ch >= 160) && (ch < 256))) { // 0 to 31 and 160 to 256
                if (startIndex < currentIndex)
                    target.append(s, startIndex, currentIndex - startIndex);

                char * line = (char*)malloc(20);
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
                sprintf_s(line, 20, "&#%u;", unsigned int(ch));
#else
                sprintf(line, "&#%d;", int(ch));
#endif
                target.append(line);
                free(line);
                startIndex = currentIndex + 1;
            } // else no encoding

            break;
        } // switch

        currentIndex++ ;
    }

    target.append(s, startIndex, length - startIndex);
    return target;
}

//-------------------------------------------------------------------------

InternalWinTrace * TTrace::getInternalTraceForm(const char * TraceWinID , bool doCreate)
{
    string id = TraceWinID ;

    if (strlen(TraceWinID) == 0 || strcmp(TraceWinID, "_") == 0)
        return DefaultWinTrace ;

    deque <InternalWinTrace *>::const_iterator WBegin;
    deque <InternalWinTrace *>::const_iterator WEnd;
    WEnd  = FormTraceList->end() ;

    for (WBegin = FormTraceList->begin(); WBegin < WEnd ; WBegin++) {
        InternalWinTrace * wintrace = * WBegin ;

        if (wintrace->ID == TraceWinID)
            return wintrace ;
    }

    // if the trace window don't exist, create it if needed
    if (doCreate) {
        InternalWinTrace * wintrace = new InternalWinTrace() ;
        wintrace->LogFileName = "" ;
        wintrace->LogFileType = 3 ;   // no log
        wintrace->IsMultiColTree = false ;
        wintrace->ID = TraceWinID ;
        FormTraceList->push_back(wintrace);
        return wintrace ;
    } else
        return NULL ;
}

//-------------------------------------------------------------------------

// Split a string on a given character into a vector of strings
// The vector is passed by reference and cleared each time
// The number of strings split out is returned
size_t split(vector<string>& v, const string& str, char c)
{
    v.clear();
    string::const_iterator s = str.begin();

    while (true) {
        string::const_iterator begin = s;

        while (*s != c && s != str.end()) {
            ++s;
        }

        v.push_back(string(begin, s));

        if (s == str.end()) {
            break;
        }

        if (++s == str.end()) {
            v.push_back("");
            break;
        }
    }

    return v.size();
}

//-------------------------------------------------------------------------

// Split a NULL-terminated character array on a given character into
// a vector of strings
// The vector is passed by reference and cleared each time
// The number of strings split out is returned
size_t split(vector<string>& v, const char* s, char c)
{
    v.clear();

    while (true) {
        const char* begin = s;

        while (*s != c && *s) {
            ++s;
        }

        v.push_back(string(begin, s));

        if (!*s) {
            break;
        }

        if (!*++s) {
            v.push_back("");
            break;
        }
    }

    return v.size();
}

//-------------------------------------------------------------------------

/// <summary>
/// send the trace to the viewer
/// </summary>/// <param name="Commands">The list of command to send</param>
void TTrace::SendToClient(CommandList * Commands)
{
    EnterCriticalSection(&criticalSection) ;    // TTrace::criticalSection
    setMessageList->push_back(Commands) ;
    LeaveCriticalSection(&criticalSection) ;    // TTrace::criticalSection
    SetEvent(MessageReady) ;
}

//-------------------------------------------------------------------------

/// Thread processing
unsigned __stdcall TTrace::MsgThread(void* pArguments)
{
    getMessageList = new deque <CommandList *> ;

    while (isStopping == false) {
        DWORD  dwEvent;
        // wait for a message or the CloseEvent. The Update will wait max 60 sec
        dwEvent = WaitForMultipleObjects(
                      2,                    // number of objects in array
                      HandlesToWaitFor,     // array of objects
                      FALSE,                // wait for any object
                      60000);               // 60 second wait

        switch (dwEvent) {
        case WAIT_OBJECT_0 + 0: // CloseEvent signaled
            // time to exit
            isStopping = true ;
            ResetEvent(CloseEvent) ;
            SetEvent(ClosedEvent) ;    // tell the main thread that the thread can be free
            break;

        case WAIT_OBJECT_0 + 1: // MessageReady signaled : New message was received.
            EnterCriticalSection(&criticalSection) ;    // TTrace::criticalSection
            // swap the 2 list to release the lock as fast as possible
            deque <CommandList *> * tempList ;
            tempList = getMessageList ;          // getMessageList is the empty list
            getMessageList = setMessageList ;    // setMessageList is the list of messages to send
            setMessageList = tempList ;
            LeaveCriticalSection(&criticalSection) ;    // TTrace::criticalSection
            // let other thread setting the ready flag
            ResetEvent(MessageReady) ;
            LoopMessages() ;  // will call also ParseForInternal() for each messages
            break;

        case WAIT_TIMEOUT:
            // time out, perform some jobs...
            break ;

        default:  // This case should never occur.
            break;
        }
    } // main loop

    // delete the message list
    delete getMessageList ;
    getMessageList = NULL ;
    _endthreadex(0);
    return 0;
} // MsgThread


//-------------------------------------------------------------------------

// loop over the getMessageList
void TTrace::LoopMessages(void)
{
    deque <CommandList *>::const_iterator WBegin;
    deque <CommandList *>::const_iterator WEnd;
    char *line = NULL;
    size_t j ; // int j;
    WEnd  = getMessageList->end() ;

    for (WBegin = getMessageList->begin(); WBegin < WEnd ; WBegin++) {
        // detect memory leak
        if (line != NULL)
            free(line);

        line = NULL ;
        CommandList * Commands = * WBegin ;

        if (isStopping) {
            Commands->clear() ;
            delete Commands ;   // the only place where the command is deleted
            continue ;
        }

        // Save the last error. TraceTool must be neutral about the error
        DWORD err = GetLastError() ;
        int parse = ParseForInternal(Commands) ;
        // compute line size and create buffer to send
        j = 0;
        CommandList::const_iterator c1_elem ;
        CommandList::const_iterator c1_end ;
        c1_end  = Commands->end() ;

        for (c1_elem = Commands->begin(); c1_elem < c1_end ; c1_elem++)
            j += (* c1_elem).length() + 1 ;

        line = (char*)malloc(j + 1);
        // create a single buffer line for all commands
        j = 0;

        for (c1_elem = Commands->begin(); c1_elem < c1_end ; c1_elem++) {
            string str = (* c1_elem) ;
            size_t strLen = str.length() + 1 ;
            memcpy(line + j, str.c_str(), strLen);
            j += strLen ;
        }

        Commands->clear() ;
        delete Commands ;   // the only place where the command is deleted
        line[j] = 0;

        if (m_options->sendMode == None)
            continue ;

        if (parse == 1)  // ParseForInternal indicate that the message don't have to be send (flush event)
            continue ;

        if (m_options->sendMode == Socket) {
            int err;
            WSADATA wsaData;

            if (m_IsSocketInitialized == false) {
                err = WSAStartup(MAKEWORD(1, 1), &wsaData);

                if (err != 0)
                    continue;

                memset(&m_serverSockAddr, 0, sizeof(m_serverSockAddr));
                // Try to convert the string as an IP address (e.g., "192.168.55.100")
                m_serverSockAddr.sin_addr.s_addr = inet_addr(m_options->socketHost);  // host to network port

                // If not in IP format, get the address via DSN...
                if (m_serverSockAddr.sin_addr.s_addr == INADDR_NONE) {
                    hostent* lphost;
                    // request the host address
                    lphost = gethostbyname(m_options->socketHost); // lphost is allocated by Windows Sockets

                    if (lphost != NULL) {
                        m_serverSockAddr.sin_addr.S_un.S_addr = ((LPIN_ADDR)lphost->h_addr_list[0])->s_addr;
                        //swprintf (Host_adr, L"%s (%d.%d.%d.%d)", Host,
                        //    serverSockAddr.sin_addr.S_un.S_un_b.s_b1,
                        //    serverSockAddr.sin_addr.S_un.S_un_b.s_b2,
                        //    serverSockAddr.sin_addr.S_un.S_un_b.s_b3,
                        //    serverSockAddr.sin_addr.S_un.S_un_b.s_b4) ;
                    } else {         // else name was invalid (or couldn't be resolved)
                        continue;
                    }
                }  // INADDR_NONE

                m_serverSockAddr.sin_port = htons(m_options->socketPort);             // port to network port
                m_serverSockAddr.sin_family = AF_INET;                                // AF_*** : INET=internet

                // Socket creation
                if ((m_socketHandle = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                    //MessageBox(NULL,_T("Couldn't create socket"),_T("Socket Error"),MB_OK);
                    continue;
                }

                // Open connection
                if (connect(m_socketHandle, (struct sockaddr *)&m_serverSockAddr, sizeof(m_serverSockAddr)) < 0)    {
                    //MessageBox(NULL,_T("Couldn't connect"),_T("Socket Error"),MB_OK);
                    continue;
                }

                m_IsSocketInitialized = true ;
            } // m_IsSocketInitialized == false

            // send the line lenght
            char FAR buffer [100] ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
            sprintf_s(buffer, 100, "%d", j);
#else
            sprintf(buffer, "%d", j);
#endif
            send(m_socketHandle, buffer, (int)strlen(buffer) + 1, 0);
            // send the buffer
            size_t i = send(m_socketHandle, line, (int)j, 0);
        } else if (m_options->sendMode == WinMsg) {
            HWND hWndDBG;
            hWndDBG = StartDebugWin() ; // FindWindow(TEXT("TFormReceiver"), TEXT("FormReceiver"));

            if (hWndDBG != NULL) {
                COPYDATASTRUCT CDS;
                CDS.cbData = DWORD(j + 1);
                CDS.dwData = WMD_TRACETOOL; // identification code 'traceTool'
                CDS.lpData = line; // no need to add #0, because String are null terminated
                SendMessage(hWndDBG, WM_COPYDATA, 0, (LPARAM)(&CDS));  //WParam(Application.Handle)
            }
        } else {  // none : not possibe
        }

        free(line);
        line = NULL ; // for memory leak
        SetLastError(err) ;   // preserve last error
    }  // next messageList

    // detect memory leak
    if (line != NULL)
        free(line);

    getMessageList->clear() ;
}

//------------------------------------------------------------------------------

/// <summary>
/// check if the message contains information for the client
/// </summary>
/// <param name="Commands">The list of command to parse</param>
int TTrace::ParseForInternal(CommandList * Commands)
{
    CommandList::const_iterator c1_elem ;
    CommandList::const_iterator c1_end ;
    string LeftMsg     ;       // Left col
    string RightMsg    ;       // right col
    string TraceID     ;       // the reference of the node : it's a guid
    string ThreadID    ;       // thread id of the sender
    string ProcessName ;       // optional : the name of the process that send traces
    int TreeIcon       ;       // -1 by default, converted to 24
    string ParentId    ;
    string MessageTime ;
    char temp[20];
    string commandParams ;
    int intParam ;
    bool firstCommand = true ;
    bool IsNewNode ;
    string xml ;
    string MemberXml ;
    InternalWinTrace * TraceForm = DefaultWinTrace ;
    LeftMsg     = "" ;       // Left col
    RightMsg    = "" ;       // right col
    TraceID     = "" ;       // the node reference
    ThreadID    = "" ;
    ProcessName = "" ;
    TreeIcon    = -1 ;
    ParentId    = "" ;
    MessageTime = "" ;
    MemberXml   = "" ;
    IsNewNode   = false ;      // check if message is a new node
    //TraceForm   = DefaultWinTrace ;  // traces are send to the master trace form by default
    c1_end  = Commands->end() ;

    for (c1_elem = Commands->begin(); c1_elem < c1_end ; c1_elem++) {
        string str = (* c1_elem) ;
        string strCommand = str.substr(0, 5) ;;
        int command = atoi(strCommand.c_str());
        // get param
        commandParams = "" ;
        size_t paramLength = str.length() - 5 ;

        if (paramLength > 0)
            commandParams = str.substr(5, paramLength) ;

        // special case : the CST_FLUSH is handled by the sender thread and is not send to viewer
        if (command == CST_FLUSH) {
            intParam = atoi(commandParams.c_str()) ;
            SetEvent(HANDLE(intParam));
            return 1 ;
        }

        // to be valid, CST_USE_TREE or CST_USE_MULTICOL_TREE or CST_WINWATCH_ID must be the first command
        if (firstCommand) {
            if (command == CST_USE_TREE) {
                TraceForm = getInternalTraceForm(commandParams.c_str(), false) ;
            } else if (command == CST_WINWATCH_ID) {
                return 0 ;
            } ;
        }

        // stop parsing if the winForm is not registered or the winForm don't need to be saved
        // 3, Local log is disabled
        // 4, Local log enabled. No size limit.
        // 5, Local log enabled. A new file is create each day (CCYYMMDD is appended to the filename)
        if (TraceForm == NULL || TraceForm->LogFileType == 3) {
            return 0;
        }

        switch (command) {
        case CST_WATCH_NAME :
            //   if (paramLength > 0)
            //      free(strParam);
            return 0 ;  // Bypass watches

        case CST_MESSAGE_TIME :
            MessageTime = commandParams ;
            break ;

        case CST_PROCESS_NAME :
            ProcessName = commandParams ;
            break ;

        case CST_THREAD_ID :
            intParam = atoi(commandParams.c_str()) ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)   // visual studio 2005 : deprecated function
            sprintf_s(temp, 20, "0x%X", intParam);
#else
            sprintf(temp, "%X", intParam);
#endif
            ThreadID = temp ;
            break ;

        case CST_THREAD_NAME :
            ThreadID = commandParams ;
            break ;

        case CST_ICO_INDEX :
            intParam = atoi(commandParams.c_str()) ;
            TreeIcon = intParam ;
            break ;

        case CST_NEW_NODE :
            // param1 : Parent Node
            ParentId = commandParams ;
            IsNewNode = true ;
            break ;

        case CST_TRACE_ID :
            TraceID = commandParams ;
            break ;

        case CST_LEFT_MSG :
            LeftMsg = commandParams ;  // param : msg
            break ;

        case CST_RIGHT_MSG :
            RightMsg = commandParams ;    // param : msg
            break ;

        case CST_CREATE_MEMBER :
            MemberXml.append("<Member>").append(HtmlEncode(commandParams)) ;
            break ;

        case CST_MEMBER_COL2 :
            if (paramLength > 0)
                MemberXml.append("<ColB>").append(HtmlEncode(commandParams)).append("</ColB>") ;

            break ;

        case CST_MEMBER_COL3 :
            if (paramLength > 0)
                MemberXml.append("<ColC>").append(HtmlEncode(commandParams)).append("</ColC>") ;

            break ;

        case CST_MEMBER_VIEWER_KIND :
            intParam = atoi(commandParams.c_str()) ;

            if (intParam != CST_VIEWER_NONE)
                MemberXml.append("<ViewerKind>").append(commandParams).append("</ViewerKind>") ;

            break ;

        case CST_ADD_MEMBER :
            MemberXml.append("</Member>") ;
            break ;
        }  // switch

        firstCommand = false ;
    } // next line to interpret

    // if new node then save to log file
    if (IsNewNode == false)
        return 0 ;

    xml = "<Node" ;

    if (ProcessName != "") {
        xml.append(" Process=\"") ;
        xml.append(HtmlEncode(ProcessName)) ;
        xml.append("\"");
    }

    if (MessageTime != "") {
        xml.append(" Time=\"") ;
        xml.append(HtmlEncode(MessageTime)) ;
        xml.append("\"");
    }

    if (ParentId != "") {  // add parent relation if not root
        xml.append(" Parent=\"") ;
        xml.append(HtmlEncode(ParentId)) ;
        xml.append("\"");
    }

    if (TraceID != "") {
        xml.append(" Id=\"") ;
        xml.append(HtmlEncode(TraceID)) ;
        xml.append("\"");
    }

    if (ThreadID != "") {
        xml.append(" ThId=\"") ;
        xml.append(HtmlEncode(ThreadID)) ;
        xml.append("\"");
    }

    // don't save default
    if (TreeIcon != -1 && TreeIcon != 24) {
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)   // visual studio 2005 : deprecated function
        sprintf_s(temp, 20, "%d", TreeIcon);
#else
        sprintf(temp, "%d", TreeIcon);
#endif
        xml.append(" Icon=\"").append(temp).append("\"");
    }

    xml.append(">");   // <Node ...>

    if (TraceForm->IsMultiColTree) {
        //<ColValue Order="2">C3</ColValue>
        vector<string> Columns;
        split(Columns, LeftMsg, '\t');
        int c = 0 ;
        vector <string>::iterator Column;

        for (Column = Columns.begin() ; Column != Columns.end() ; Column++) {
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)   // visual studio 2005 : deprecated function
            sprintf_s(temp, 20, "%d", c);
#else
            sprintf(temp, "%d", c);
#endif
            xml.append("<ColValue Order=\"").append(temp).append("\">") ;
            xml.append(HtmlEncode(*Column)) ;
            xml.append("</ColValue>");
            c++ ;
        }
    } else {
        // save the tree col1
        xml.append(HtmlEncode(LeftMsg));

        // save the tree col 2
        if (RightMsg != "") {
            xml.append("<Col2>") ;
            xml.append(HtmlEncode(RightMsg)) ;
            xml.append("</Col2>");
        }
    }

    // append member to xml
    xml.append(MemberXml);
    xml.append("</Node>\n</Data>");

    if (TraceForm->LogFileName == "")
        TraceForm->LogFileName = "TraceLog.xml";

    //string FileToWrite = "";
    size_t posExtension = TraceForm->LogFileName.find_last_of(".") ;
    TraceForm->LastLocalLogFileName = TraceForm->LogFileName.substr(0, posExtension) ; // file without extension
    string ExtensionWithDot = TraceForm->LogFileName.substr(posExtension, TraceForm->LogFileName.length() - TraceForm->LastLocalLogFileName.length()) ; // include the dot

    if (TraceForm->LogFileType == 3) {            // 3, Local log disabled.
        // should not happens. Detected before parsing
        return 0 ;
    } else if (TraceForm->LogFileType == 4) {     // 4, Local log enabled. No size limit.
        if (TraceForm->CurrentFileNumber != 0) {
            // Append CurrentFileNumber Before extension
            TraceForm->LastLocalLogFileName.append("_") ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
            sprintf_s(temp, 20, "%d", TraceForm->CurrentFileNumber);
#else
            sprintf(temp, "%d", TraceForm->CurrentFileNumber);
#endif
            TraceForm->LastLocalLogFileName.append(temp);
            TraceForm->LastLocalLogFileName.append(ExtensionWithDot) ;
        } else
            TraceForm->LastLocalLogFileName = TraceForm->LogFileName;
    } else {                                     // 5, Local log enabled. A new file is create each day (CCYYMMDD is appended to the filename)
        // append YYYYMMDD
        SYSTEMTIME Time;
        GetLocalTime(&Time);
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
        sprintf_s(temp, 20, "%04d%02d%02d", Time.wYear, Time.wMonth , Time.wDay);
#else
        sprintf(temp, "%04d%02d%02d", Time.wYear, Time.wMonth , Time.wDay);
#endif
        TraceForm->LastLocalLogFileName.append(temp) ;

        // add CurrentFileNumber if <> 0
        if (TraceForm->CurrentFileNumber != 0) {
            TraceForm->LastLocalLogFileName.append("_") ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
            sprintf_s(temp, 20, "%d", TraceForm->CurrentFileNumber);
#else
            sprintf(temp, "%d", TraceForm->CurrentFileNumber);
#endif
            TraceForm->LastLocalLogFileName.append(temp);
        }

        // append file extension (XML)
        TraceForm->LastLocalLogFileName.append(ExtensionWithDot);
    }

    ofstream f ;
    // check if exist
    ifstream iFileToCheck;
    iFileToCheck.open(TraceForm->LastLocalLogFileName.c_str());
    bool isOpen = iFileToCheck.is_open() ;
    iFileToCheck.close() ;

    if (isOpen == false) {
        // f = new FileStream(TraceForm->LastLocalLogFileName, FileMode.Create) ;
        f.open(TraceForm->LastLocalLogFileName.c_str(), ios_base::out | ios_base::trunc);

        // include header in file
        if (TraceForm->IsMultiColTree) {
            string strbBuilder ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)   // visual studio 2005 : deprecated function
            sprintf_s(temp, 20, "%d", TraceForm->MainCol);
#else
            sprintf(temp, "%d", TraceForm->MainCol);
#endif
            strbBuilder.append("<MainColumn>").append(temp).append("</MainColumn>") ;
            vector<string> Columns;
            split(Columns, TraceForm->TitleList, '\t');
            int c = 0 ;
            vector <string>::iterator Column;

            for (Column = Columns.begin() ; Column != Columns.end() ; Column++) {
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)   // visual studio 2005 : deprecated function
                sprintf_s(temp, 20, "%d", c);
#else
                sprintf(temp, "%d", c);
#endif
                xml.append("<ColTitle Order=\"").append(temp).append("\">") ;
                xml.append(HtmlEncode(*Column)) ;
                xml.append("</ColTitle>");
                c++ ;
            }

            xml.insert(0, strbBuilder);
        }

        // http://en.wikipedia.org/wiki/Windows-1252
        xml.insert(0, "<?xml version='1.0' encoding='windows-1252'?><Data>"); // ISO-8859-1 , ASCII , ISO-8859-15 , windows-1252
    } else {  // append only the node
        f.open(TraceForm->LastLocalLogFileName.c_str(), ios_base::in | ios_base::out);
        f.seekp(-7, ios::end); // override the </data> tag
    }

    f << xml ;
    f.close();

    // limit file size
    if (TraceForm->MaxLines != -1) {
        TraceForm->LinesWritten++;

        if (TraceForm->LinesWritten >= TraceForm->MaxLines) {
            TraceForm->CurrentFileNumber++;
            TraceForm->LinesWritten = 0;  // reset counter
        }
    }

    return 0 ;
}

//-------------------------------------------------------------------------

/// <summary>
/// Clear all traces (main window)
/// </summary>

void TTrace::ClearAll()
{
    m_winTrace->ClearAll() ;
}

//------------------------------------------------------------------------------

/// <summary>
///  Show or hide the trace program
/// </summary>
/// <param name="IsVisible">When false, the viewer is minimized in systray</param>

void TTrace::Show(bool IsVisible)
{
    CommandList * Commands = new CommandList() ;

    if (IsVisible == true)
        Commands->Add(CST_SHOW , 1);
    else
        Commands->Add(CST_SHOW , 0);

    TTrace::SendToClient(Commands);
}

//------------------------------------------------------------------------------

/// Set the global search criteria. You must call TTrace.Wintrace.FindNext to position to the next or previous matching node
/// <param name="Text">Text to search</param>
/// <param name="Sensitive">Search is case sensitive</param>
/// <param name="WholeWord">match only whole word</param>
/// <param name="Highlight">Highlight results</param>
/// <param name="SearchInAllPages">call to FindNext will search also in other traces windows if true</param>
void TTrace::Find(const char *Text , const bool Sensitive, const bool WholeWord , const bool Highlight, const bool SearchInAllPages)
{
    CommandList * Commands = new CommandList() ;
    int flags = 0 ;

    // Sensitive<<3+WholeWord<<2+highlight<<1+SearchInAllPages
    if (Sensitive)
        flags += 8;

    if (WholeWord)
        flags += 4;

    if (Highlight)
        flags += 2;

    if (SearchInAllPages)
        flags += 1;

    Commands->Add(CST_FIND_TEXT , flags, Text);
    TTrace::SendToClient(Commands);
}

//------------------------------------------------------------------------------

/// Set the global search criteria. You must call TTrace.Wintrace.FindNext to position to the next or previous matching node
/// <param name="Text">Text to search</param>
/// <param name="Sensitive">Search is case sensitive</param>
/// <param name="WholeWord">match only whole word</param>
/// <param name="Highlight">Highlight results</param>
/// <param name="SearchInAllPages">call to FindNext will search also in other traces windows if true</param>
void TTrace::Find(const wchar_t *Text , const bool Sensitive, const bool WholeWord , const bool Highlight, const bool SearchInAllPages)
{
    CommandList * Commands = new CommandList() ;
    int flags = 0 ;

    // Sensitive<<3+WholeWord<<2+highlight<<1+SearchInAllPages
    if (Sensitive)
        flags += 8;

    if (WholeWord)
        flags += 4;

    if (Highlight)
        flags += 2;

    if (SearchInAllPages)
        flags += 1;

    char * temp = TTrace::WideToMbs(Text) ;
    Commands->Add(CST_FIND_TEXT , flags, temp);
    free(temp) ;
    TTrace::SendToClient(Commands);
}

//------------------------------------------------------------------------------

/// <summary>
///  Shutdown viewer
/// </summary>

void TTrace::CloseViewer()
{
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_CLOSE_VIEWER);
    TTrace::SendToClient(Commands);
}


//------------------------------------------------------------------------------
//==========================================================================
// TraceNodeBase
//==========================================================================

/// <summary>
/// Construct a new trace node, derived from a parent node
/// </summary>
/// <param name="leftMsg" >Optional left message</param>
/// <param name="rightMsg">Optional right message</param>
/// <returns>a new ItraceNodeEx</returns>

TraceNodeEx * TraceNodeBase::CreateChildEx(const char *leftMsg /* = NULL */, const char *rightMsg /* = NULL */)
{
    TraceNodeEx *Node;
    Node = new TraceNodeEx(this);   // Node->id is generated (GUID)

    if (leftMsg != NULL && leftMsg[0] != 0)
        Node->leftMsg = leftMsg ;

    if (rightMsg != NULL && rightMsg[0] != 0)
        Node->rightMsg = rightMsg ;

    return Node ;
}

//-------------------------------------------------------------------------
/// <summary>
/// Construct a new trace node, derived from a parent node
/// </summary>
/// <param name="leftMsg" >Optional left message</param>
/// <param name="rightMsg">Optional right message</param>
/// <returns>a new ItraceNodeEx</returns>

TraceNodeEx * TraceNodeBase::CreateChildEx(const wchar_t *leftMsg /* = NULL */, const wchar_t *rightMsg /* = NULL */)
{
    TraceNodeEx *Node;
    Node = new TraceNodeEx(this);   // Node->id is generated (GUID)

    if (leftMsg != NULL && leftMsg[0] != 0) {
        char * temp = TTrace::WideToMbs(leftMsg) ;
        Node->leftMsg = temp ; // copy
        free(temp);
    }

    if (rightMsg != NULL && rightMsg[0] != 0) {
        char * temp = TTrace::WideToMbs(rightMsg) ;
        Node->rightMsg = temp ; // copy
        free(temp);
    }

    return Node ;
}

//==========================================================================
// InternalWinTrace
//==========================================================================

/// <summary>
/// InternalWinTrace constructor.<p/>
/// </summary>

InternalWinTrace::InternalWinTrace(void)
{
    ID = "";
    IsMultiColTree = false;
    MainCol = 0;
    TitleList = "";
    LogFileName = "";
    LastLocalLogFileName = "";
    LogFileType = 3;
    MaxLines = -1;         // Max number of lines before starting a new file
    CurrentFileNumber = 0; // Current file number, when MaxLines is set
    LinesWritten = 0;      // Number of lines written , when MaxLines is set
}


//==========================================================================
// TMemberNode
//==========================================================================

/// <summary>
/// Create a Member node (or a sub member)
/// </summary>
/// <param name="strCol1">optional column 1</param>
/// <param name="strCol2">optional column 2</param>
/// <param name="strCol3">optional column 3</param>

TMemberNode::TMemberNode(const char * strCol1 /* = NULL */, const char * strCol2 /* = NULL */, const char * strCol3 /* = NULL */)
{
    if (strCol1 != NULL)
        col1 = strCol1;

    if (strCol2 != NULL)
        col2 = strCol2;

    if (strCol3 != NULL)
        col3 = strCol3;

    // create sub members only if needed
    m_Members = NULL ; // new deque <TMemberNode *> ;
    m_FontDetails = NULL ;
}

//-------------------------------------------------------------------------

/// <summary>
/// Create a Member node (or a sub member)
/// </summary>
/// <param name="strCol1">optional column 1</param>
/// <param name="strCol2">optional column 2</param>
/// <param name="strCol3">optional column 3</param>

TMemberNode::TMemberNode(const wchar_t * strCol1 , const wchar_t * strCol2 /* = NULL */, const wchar_t * strCol3 /* = NULL */)
{
    if (strCol1 != NULL) {
        char * col = TTrace::WideToMbs(strCol1) ;
        col1 = col ; // copy
        free(col) ;
    }

    if (strCol2 != NULL) {
        char * col = TTrace::WideToMbs(strCol2) ;
        col2 = col ; // copy
        free(col) ;
    }

    if (strCol3 != NULL) {
        char * col = TTrace::WideToMbs(strCol3) ;
        col3 = col ; // copy
        free(col) ;
    }

    // create sub members only if needed
    m_Members = NULL ; // new deque <TMemberNode *> ;
    m_FontDetails = NULL ;
}

//-------------------------------------------------------------------------

/// <summary>
// When a TraceNode is send, the "Members" field is converted to commands.<p>
// The convertion automatically delete all sub members and the "Members" field.<p>
// Destructor is then needed only if you don't send the node.
/// </summary>

TMemberNode::~TMemberNode()
{
    if (m_Members != NULL) {
        deque <TMemberNode *>::const_iterator MemberBegin;
        deque <TMemberNode *>::const_iterator MemberEnd;
        MemberEnd  = m_Members->end() ;

        for (MemberBegin = m_Members->begin(); MemberBegin < MemberEnd ; MemberBegin++) {
            TMemberNode * member = * MemberBegin ;
            delete member ;
        }

        delete m_Members ;
        m_Members = NULL ;
    }

    if (m_FontDetails != NULL) {
        deque <FontDetail *>::const_iterator FontDetailBegin;
        deque <FontDetail *>::const_iterator FontDetailEnd;
        FontDetailEnd  = m_FontDetails->end() ;

        for (FontDetailBegin = m_FontDetails->begin(); FontDetailBegin < FontDetailEnd ; FontDetailBegin++) {
            FontDetail * fontDetail = * FontDetailBegin ;
            delete fontDetail ;
        }

        delete m_FontDetails ;
        m_FontDetails = NULL ;
    }
}

//-------------------------------------------------------------------------

/// <summary>
// Sub members
/// </summary>
/// <returns>array of TMember</returns>

deque <TMemberNode *> * TMemberNode::Members()
{
    if (m_Members == NULL)
        m_Members = new deque <TMemberNode *> ;

    return m_Members ;
}

//-------------------------------------------------------------------------

/// <summary>
/// Recursively add members to the node commandList
/// </summary>
/// <param name="commands">Where to store members </param>

void TMemberNode::AddToStringList(CommandList * commands)
{
    if (m_Members == NULL)
        return ;

    // the root node node itself is not send
    deque <TMemberNode *>::const_iterator MemberBegin;
    deque <TMemberNode *>::const_iterator MemberEnd;
    MemberEnd  = m_Members->end() ;

    for (MemberBegin = m_Members->begin(); MemberBegin < MemberEnd ; MemberBegin++) {
        TMemberNode * member = * MemberBegin ;
        member->_AddToStringList(commands);
        delete member ;  // delete sub members
    }

    delete m_Members ;  // delete member array
    m_Members = NULL ;
}

//-------------------------------------------------------------------------

/// internal recursive
void TMemberNode::_AddToStringList(CommandList * commands)
{
    commands->Add(CST_CREATE_MEMBER, col1.c_str()) ;      // first column can be NULL

    if (! col2.empty())
        commands->Add(CST_MEMBER_COL2, col2.c_str()) ;

    if (! col3.empty())
        commands->Add(CST_MEMBER_COL3, col3.c_str()) ;

    // add viewer kind
    if (ViewerKind != 0)
        commands->Add(CST_MEMBER_VIEWER_KIND, ViewerKind);

    if (m_FontDetails != NULL) {
        deque <FontDetail *>::const_iterator FontBegin;
        deque <FontDetail *>::const_iterator FontEnd;
        FontEnd  = m_FontDetails->end() ;

        for (FontBegin = m_FontDetails->begin(); FontBegin < FontEnd ; FontBegin++) {
            FontDetail * font = * FontBegin ;
            char * message ;
            int bold   = (font->Bold) ? 1 : 0 ;
            int italic = (font->Italic) ? 1 : 0;
            size_t MsgLen ;

            if (font->FontName.empty()) {
                message = (char*)malloc(32 + 1) ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
                sprintf_s(message, 33, "%5d%3d%1d%1d%11d%11d", CST_MEMBER_FONT_DETAIL, font->ColId, bold, italic, font->Color, font->Size);
#else
                sprintf(message, "%5d%3d%1d%1d%11d%11d", CST_MEMBER_FONT_DETAIL, font->ColId, bold, italic, font->Color, font->Size);
#endif
            } else {
                MsgLen = font->FontName.length() + 32 + 1 ;
                message = (char*)malloc(MsgLen) ;
                const char * fontName = font->FontName.c_str() ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
                sprintf_s(message, MsgLen, "%5d%3d%1d%1d%11d%11d%s", CST_MEMBER_FONT_DETAIL, font->ColId, bold, italic, font->Color, font->Size, fontName);
#else
                sprintf(message, "%5d%3d%1d%1d%11d%11d%s", CST_MEMBER_FONT_DETAIL, font->ColId, bold, italic, font->Color, font->Size, fontName);
#endif
            }

            commands->push_back(message) ;
            free(message) ;
            delete font ;
        }

        delete m_FontDetails ;
        m_FontDetails = NULL ;
    }

    if (m_Members != NULL) {
        deque <TMemberNode *>::const_iterator MemberBegin;
        deque <TMemberNode *>::const_iterator MemberEnd;
        MemberEnd  = m_Members->end() ;

        for (MemberBegin = m_Members->begin(); MemberBegin < MemberEnd ; MemberBegin++) {
            TMemberNode * member = * MemberBegin ;
            member->_AddToStringList(commands);
            delete member ;
        }

        delete m_Members ;
        m_Members = NULL ;
    }

    commands->Add(CST_ADD_MEMBER) ;      // close the member group
}

//-------------------------------------------------------------------------

/// <summary>
/// Add a sub member. NOTE : the member in argument will be destroyed when the container node is destroyed
/// </summary>
/// <param name="member">an already constructed sub member</param>
/// <returns>the TMember node to add</returns>
TMemberNode * TMemberNode::Add(TMemberNode * member)
{
    Members()->push_back(member);
    return member;
}

//-------------------------------------------------------------------------

/// <summary>
/// create and add a sub member
/// </summary>
/// <param name="strCol1">Column 1 of the new sub member</param>
/// <param name="strCol2">Optional column 2 of the new sub member</param>
/// <param name="strCol3">Optional column 3 of the new sub member</param>
/// <returns>the TMember node to add</returns>
TMemberNode * TMemberNode::Add(const char * strCol1 , const char * strCol2 /* = NULL */, const char * strCol3 /* = NULL */)
{
    TMemberNode * member;
    member = new TMemberNode(strCol1, strCol2, strCol3);
    Add(member);
    return member;
}

//-------------------------------------------------------------------------

/// <summary>
/// create and add a sub member
/// </summary>
/// <param name="strCol1">Column 1 of the new sub member</param>
/// <param name="strCol1">Optional column 2 of the new sub member</param>
/// <param name="strCol1">Optional column 3 of the new sub member</param>
/// <returns>the TMember node to add</returns>
TMemberNode * TMemberNode::Add(const wchar_t * strCol1 , const wchar_t * strCol2 /* = NULL */, const wchar_t * strCol3 /* = NULL */)
{
    TMemberNode * member;
    member = new TMemberNode(strCol1, strCol2, strCol3);
    Add(member);
    return member;
}

//-------------------------------------------------------------------------

/// <summary>
/// Set member font
/// </summary>
/// <param name="ColId">Column number (-1,0,1,2)</param>
/// <param name="Bold">Change font to bold</param>
/// <param name="Italic">Change font to Italic</param>
/// <param name="Color">Change Color. Use -1 to keep default color</param>
/// <param name="Size">Change font size, use zero to keep normal size</param>
/// <param name="FontName">Change font name</param>
/// <returns>The TMemberNode </returns>
TMemberNode * TMemberNode::SetFontDetail(const int ColId,  const bool Bold ,  const bool Italic /*= false*/ ,  const int Color /*= -1*/ , const int Size /*= 0*/ ,  const char * FontName /*= NULL*/)
{
    if (m_FontDetails == NULL)
        m_FontDetails = new deque <FontDetail *> ;

    FontDetail * font = new FontDetail() ;
    font->ColId    = ColId ;
    font->Bold     = Bold ;
    font->Italic   = Italic ;
    font->Color    = Color ;
    font->Size     = Size ;

    if (FontName != NULL)
        font->FontName = FontName ;

    m_FontDetails->push_back(font);
    return this ;
}

//==========================================================================
// TraceOptions
//==========================================================================

/// <summary>
/// TraceTool Options constructor
/// </summary>

TraceOptions::TraceOptions(void)
{
    SendProcessName = false ;
    SendDate = false ;
    SendThreadId = true ;
    m_processFileName = NULL ;
    socketHost = NULL ;
    socketPort = 8090 ;
#ifdef _WIN32_WCE
    // for pocket pc : default is socket and host on 192.168.55.100
    sendMode = Socket ;
    SetSocketHost("192.168.55.100");
#else
    // For windows desktop : default is windows messages. In case of you switch to socket, the default host is localhost
    sendMode = WinMsg ;
    SetSocketHost("127.0.0.1");
#endif
}

//-------------------------------------------------------------------------

/// <summary>
/// TraceTool Options destructor
/// </summary>

TraceOptions::~TraceOptions(void)
{
    if (socketHost != NULL)
        free(socketHost) ;

    if (m_processFileName != NULL)
        free(m_processFileName) ;
}

//-------------------------------------------------------------------------

/// <summary>
/// Set the socket host
/// </summary>
/// <param name="Host">socket host</param>

void TraceOptions::SetSocketHost(const char * Host)
{
    if (socketHost != NULL)
        free(socketHost) ;

    socketHost = NULL ;

    if (Host == NULL)
        return ;

    size_t lenhost = strlen(Host) + 1 ;
    socketHost = (char *) malloc(lenhost) ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
    strcpy_s(socketHost, lenhost, Host) ;
#else
    strcpy(socketHost, Host) ;
#endif
}

//-------------------------------------------------------------------------

/// <summary>
/// Set the socket host
/// </summary>
/// <param name="Host">socket host</param>

void TraceOptions::SetSocketHost(const wchar_t * Host)
{
    if (socketHost != NULL)
        free(socketHost) ;

    socketHost = TTrace::WideToMbs(Host) ;
}

//-------------------------------------------------------------------------

/// <summary>
/// Helper function : get the process name from the module
/// </summary>
/// <returns>the process name without path</returns>

const char * TraceOptions::CheckProcessName(void)
{
    if (m_processFileName == NULL) {
        WCHAR wFileName [MAX_PATH + 1] ;
        size_t nbChar ;
        // use wide GetModuleFileNameW in place of GetModuleFileNameA for winCE compatibility
        wFileName[GetModuleFileNameW(0 /* hInstance */ , wFileName, MAX_PATH)] = 0;
        // Convert widestring (wcs) to multibyte string (mbs)
        char *pmbFilename = (char *)malloc(MAX_PATH + 1);
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
        //size_t i ;
        //nbChar =
        wcstombs_s(&nbChar, pmbFilename, MAX_PATH + 1, wFileName, MAX_PATH);
#else
        nbChar = wcstombs(pmbFilename, wFileName, MAX_PATH);
#endif
        // bypass any path before the module name
        const char * ptr ;
        ptr = pmbFilename + nbChar - 1;

        while (ptr > pmbFilename) {
            if ((*ptr == '/') ||
                (*ptr == '\\') ||
                (*ptr == ':')) {
                ptr ++ ;
                break ;
            } else {
                ptr-- ;
            }
        }

        if ((*ptr == '/') ||
            (*ptr == '\\') ||
            (*ptr == ':'))
            ptr ++ ;

        size_t fileLength = strlen(ptr) + 1 ;
        m_processFileName = (char *) malloc(fileLength) ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)   // visual studio 2005 : deprecated function
        strcpy_s(m_processFileName, fileLength, ptr) ;
#else
        strcpy(m_processFileName, ptr) ;
#endif
        free(pmbFilename) ;
    }

    return m_processFileName ;
}

//==========================================================================
// WinWatch
//==========================================================================

/// <summary>
/// WinWatch constructor
/// </summary>
WinWatch::WinWatch(void)
{
    id = "" ;                                                          // Wintrace id (empty for the main window)
    enabled = true ;                                                  // enable or disable watches
}

//-------------------------------------------------------------------------

/// <summary>
/// WinWatch constructor
/// </summary>
/// <param name="WinWatchID">Winwatch id</param>
/// <param name="WinWatchText">caption for the winwatch </param>

WinWatch::WinWatch(const char * WinWatchID , const char * WinWatchText)
{
    id = "" ;                                                          // Wintrace id (empty for the main window)
    enabled = true ;                                                   // enable or disable watches

    if (WinWatchID == NULL || WinWatchID == "") {
        char *c;
        c = TTrace::CreateTraceID() ;
        id = c; //copy
        free(c);
    } else
        id = WinWatchID ;

    CommandList * Commands = new CommandList() ;

    if (WinWatchText == NULL || WinWatchText == "")
        Commands->Add(CST_WINWATCH_NAME, id.c_str()) ; // id.c_str()
    else
        Commands->Add(CST_WINWATCH_NAME, WinWatchText) ;

    TTrace::SendToWinWatchClient(Commands, id.c_str());
}

//-------------------------------------------------------------------------

/// <summary>
/// WinWatch constructor
/// </summary>
/// <param name="WinWatchID">Winwatch id</param>
/// <param name="WinWatchText">caption for the winwatch </param>

WinWatch::WinWatch(const wchar_t * WinWatchID , const wchar_t * WinWatchText)
{
    id = "" ;                                                          // Wintrace id (empty for the main window)
    enabled = true ;                                                   // enable or disable watches
    char * strWinWatchID   = TTrace::WideToMbs(WinWatchID) ;
    char * strWinWatchText = TTrace::WideToMbs(WinWatchText) ;

    if (strWinWatchID == NULL || strWinWatchID == "") {
        char *c;
        c = TTrace::CreateTraceID() ;
        id = c; //copy
        free(c);
    } else {
        id = strWinWatchID ;  // copy
    }

    CommandList * Commands = new CommandList() ;

    if (strWinWatchText == NULL || strWinWatchText == "")
        Commands->Add(CST_WINWATCH_NAME, id.c_str()) ;
    else
        Commands->Add(CST_WINWATCH_NAME, strWinWatchText) ;

    if (strWinWatchID != NULL)
        free(strWinWatchID) ;

    if (strWinWatchText != NULL)
        free(strWinWatchText) ;

    TTrace::SendToWinWatchClient(Commands, id.c_str());
}
//-------------------------------------------------------------------------

/// <summary>
/// Display the window
/// </summary>
void WinWatch::DisplayWin(void)
{
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_DISPLAY_TREE) ;
    TTrace::SendToWinWatchClient(Commands, id.c_str());
}

//-------------------------------------------------------------------------

/// <summary>
/// clear all watches
/// </summary>
void WinWatch::ClearAll(void)
{
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_CLEAR_ALL);
    TTrace::SendToWinWatchClient(Commands, id.c_str());
}

//-------------------------------------------------------------------------

/// <summary>
/// Close the winWatch
/// </summary>
void WinWatch::Close(void)
{
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_CLOSE_WIN);
    TTrace::SendToWinWatchClient(Commands, id.c_str());
}

//-------------------------------------------------------------------------

/// <summary>
/// Add or change a watch
/// </summary>
/// <param name="WatchName">watch name</param>
/// <param name="WatchValue">watch value</param>
void WinWatch::Send(const char * WatchName , const char * WatchValue)
{
    if (! enabled)
        return ;

    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_WATCH_NAME, WatchName) ;
    // create the member and set col1
    Commands->Add(CST_CREATE_MEMBER);
    // col2 is the value
    Commands->Add(CST_MEMBER_COL2, WatchValue);
    // close the member group
    Commands->Add(CST_ADD_MEMBER);
    TTrace::SendToWinWatchClient(Commands, id.c_str());
}

//-------------------------------------------------------------------------

/// <summary>
/// Add or change a watch
/// </summary>
/// <param name="WatchName">watch name</param>
/// <param name="WatchValue">watch value</param>
void WinWatch::Send(const wchar_t * WatchName , const wchar_t * WatchValue)
{
    if (! enabled)
        return ;

    char * strWatchName  = TTrace::WideToMbs(WatchName) ;
    char * strWatchValue = TTrace::WideToMbs(WatchValue) ;
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_WATCH_NAME, strWatchName) ;
    // create the member and set col1
    Commands->Add(CST_CREATE_MEMBER);
    // col2 is the value
    Commands->Add(CST_MEMBER_COL2, strWatchValue);
    // close the member group
    Commands->Add(CST_ADD_MEMBER);

    if (strWatchName != NULL)
        free(strWatchName) ;

    if (strWatchValue != NULL)
        free(strWatchValue) ;

    TTrace::SendToWinWatchClient(Commands, id.c_str());
}

//==========================================================================
// WinTrace
//==========================================================================

/// <summary>
/// WinTrace constructor.<p/>
/// You can map a WinTrace to an existing window.<p/>
/// Nothing Is send to the viewer
/// </summary>

WinTrace::WinTrace(void)
{
    CreateNodes() ;
}

//-------------------------------------------------------------------------

/// <summary>
/// WinTrace constructor. The Window Trace is create on the viewer (if not already done)
/// </summary>
/// <param name="WinTraceID">Required window trace Id. If empty, a guid will be generated</param>
/// <param name="WinTraceTitle">The Window Title on the viewer.If empty, a default name will be used</param>

WinTrace::WinTrace(const char * WinTraceID , const char * WinTraceTitle)
{
    if (WinTraceID == NULL || WinTraceID == "") {
        char *c;
        c = TTrace::CreateTraceID() ;
        id = c; //copy
        free(c);
    } else
        id = WinTraceID ;

    CreateNodes() ;

    if (WinTraceID != NULL && WinTraceID == "_")
        return ;  // don't create new window on the viewer

    // create the trace window
    CommandList * Commands = new CommandList() ;

    if (WinTraceTitle == NULL || WinTraceTitle == "")
        Commands->Add(CST_TREE_NAME, id.c_str());
    else
        Commands->Add(CST_TREE_NAME, WinTraceTitle);

    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//-------------------------------------------------------------------------

/// <summary>
/// WinTrace constructor. The Window Trace is create on the viewer (if not already done)
/// </summary>
/// <param name="WinTraceID">Required window trace Id. If empty, a guid will be generated</param>
/// <param name="WinTraceTitle">The Window Title on the viewer.If empty, a default name will be used</param>

WinTrace::WinTrace(const wchar_t * WinTraceID , const wchar_t * WinTraceTitle)
{
    char * strWinTraceID    = TTrace::WideToMbs(WinTraceID) ;
    char * strWinTraceTitle = TTrace::WideToMbs(WinTraceTitle) ;

    if (strWinTraceID == NULL || strWinTraceID == "") {
        char *c;
        c = TTrace::CreateTraceID() ;
        id = c; //copy
        free(c);
    } else
        id = strWinTraceID ; // copy

    CreateNodes() ;

    if (strWinTraceID != NULL && strWinTraceID == "_") {
        // don't create new window on the viewer
    } else {
        // create the trace window
        CommandList * Commands = new CommandList() ;

        if (strWinTraceTitle == NULL || strWinTraceTitle == "")
            Commands->Add(CST_TREE_NAME, id.c_str());
        else
            Commands->Add(CST_TREE_NAME, strWinTraceTitle);

        TTrace::SendToWinTraceClient(Commands, id.c_str());
    }

    if (strWinTraceID != NULL)
        free(strWinTraceID) ;

    if (strWinTraceTitle != NULL)
        free(strWinTraceTitle) ;
}

//-------------------------------------------------------------------------
/// private initialize (called by the 2 constructors)
void WinTrace::CreateNodes()
{
    iconIndex = CST_ICO_DEFAULT;    // TWinTrace don't have icon (for now)
    enabled = true;
    winTraceId = id ;    // winTraceId need to be the same as 'id' if we want to call sendXxx() directly on WinTrace object
    contextList = new deque <NodeContext *> ;
    //winTraceContext = NULL ;
    InitializeCriticalSection(&criticalSection) ;   // TraceToSend::criticalSection
    debug   = new TraceNode(NULL, false);     // no parent node, don't create a GUID
    error   = new TraceNode(NULL, false);
    warning = new TraceNode(NULL, false);
    debug->iconIndex   = CST_ICO_INFO;        // store the iconIndex. don't add CST_ICO_INDEX command
    error->iconIndex   = CST_ICO_ERROR;
    warning->iconIndex = CST_ICO_WARNING;
    debug->winTraceId   = id ;                // link the 3 node to the window
    error->winTraceId   = id ;
    warning->winTraceId = id ;
    debug->winTraceContext = contextList;
    error->winTraceContext = contextList;
    warning->winTraceContext = contextList;
    debug->enabled = true;
    error->enabled = true;
    warning->enabled = true;
}

//-------------------------------------------------------------------------

/// <summary>
/// Destructor
/// </summary>
WinTrace::~WinTrace(void)
{
    delete debug;
    delete error;
    delete warning;
    DeleteCriticalSection(&criticalSection) ;   // TraceToSend::criticalSection
    delete contextList ;
    contextList = NULL ;
}

//-------------------------------------------------------------------------

/// <summary>
/// Save the window tree traces to a text file
/// </summary>
/// <param name="FileName">file to save</param>
void WinTrace::SaveToTextfile(const char * FileName)
{
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_SAVETOTEXT, FileName);
    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//-------------------------------------------------------------------------

/// <summary>
/// Save the window tree traces to a text file
/// </summary>
/// <param name="FileName">file to save</param>
void WinTrace::SaveToTextfile(const wchar_t * FileName)
{
    CommandList * Commands = new CommandList() ;
    char * temp = TTrace::WideToMbs(FileName) ;
    Commands->Add(CST_SAVETOTEXT, temp);

    if (temp != NULL)
        free(temp) ;

    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//-------------------------------------------------------------------------

/// <summary>
/// Save the window tree traces to an XML file
/// </summary>
/// <param name="FileName">file to save</param>
void WinTrace::SaveToXml(const char * FileName)
{
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_SAVETOXML , FileName);
    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//-------------------------------------------------------------------------

/// <summary>
/// Save the window tree traces to an XML file and specifying a styleSheet
/// </summary>
/// <param name="FileName">file to save</param>
/// <param name="styleSheet">optional StyleSheet file name added in xml</param>
void WinTrace::SaveToXml(const char * FileName, const char * styleSheet)
{
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_SAVETOXML , FileName, styleSheet) ;
    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//-------------------------------------------------------------------------

/// <summary>
/// Save the window tree traces to an XML file
/// </summary>
/// <param name="FileName">file to save</param>
void WinTrace::SaveToXml(const wchar_t * FileName)
{
    CommandList * Commands = new CommandList() ;
    char * temp   = TTrace::WideToMbs(FileName) ;
    Commands->Add(CST_SAVETOXML , temp);

    if (temp != NULL)
        free(temp) ;

    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//-------------------------------------------------------------------------

/// <summary>
/// Save the window tree traces to an XML file and XML file specifying a styleSheet
/// </summary>
/// <param name="FileName">file to save</param>
/// <param name="styleSheet">optional StyleSheet file name added in xml</param>
void WinTrace::SaveToXml(const wchar_t * FileName, const wchar_t * styleSheet)
{
    CommandList * Commands = new CommandList() ;
    char * temp1   = TTrace::WideToMbs(FileName) ;
    char * temp2   = TTrace::WideToMbs(styleSheet) ;
    Commands->Add(CST_SAVETOXML , temp1, temp2);

    if (temp1 != NULL)
        free(temp1) ;

    if (temp2 != NULL)
        free(temp2) ;

    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//-------------------------------------------------------------------------

/// <summary>
/// Load an XML file to the window tree traces
/// </summary>
/// <param name="FileName">file to open</param>
void WinTrace::LoadXml(const char * FileName)
{
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_LOADXML , FileName);
    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//-------------------------------------------------------------------------

/// <summary>
/// Load an XML file to the window tree traces
/// </summary>
/// <param name="FileName">file to open</param>
void WinTrace::LoadXml(const wchar_t * FileName)
{
    CommandList * Commands = new CommandList() ;
    char * temp   = TTrace::WideToMbs(FileName) ;
    Commands->Add(CST_LOADXML , temp);

    if (temp != NULL)
        free(temp) ;

    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//-------------------------------------------------------------------------

/// <summary>
/// Show the window tree
/// </summary>
void WinTrace::DisplayWin()
{
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_DISPLAY_TREE);
    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//-------------------------------------------------------------------------

/// <summary>
/// Change the tree to display user defined multiple columns.<p/>
/// Must be called before setting column titles
/// </summary>
/// <param name="MainColIndex">The Main column index (default is 0)</param>
void WinTrace::SetMultiColumn(const int MainColIndex)
{
    InternalWinTrace * TraceForm = TTrace::getInternalTraceForm(id.c_str(), true);
    TraceForm->IsMultiColTree = true;
    TraceForm->MainCol = MainColIndex;
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_TREE_MULTI_COLUMN, MainColIndex);
    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//-------------------------------------------------------------------------

/// <summary>
/// Set the log file.(Path is relative to the viewer)
/// To enabled log on local AND on the viewer call this funtion twice.
/// To don't use the viewer, set the TTrace.options.SendMode to None
/// <code>
/// The Mode can be one of the folowing :
/// 0, Viewer Log is disabled.
/// 1, Viewer log enabled.
/// 2, Viewer log enabled. A new file is create each day (CCYYMMDD is appended to the filename)
/// 3, Local log is disabled
/// 4, Local log enabled.
/// 5, Local log enabled. A new file is create each day (CCYYMMDD is appended to the filename)
/// </code>
/// </summary>
/// <param name="FileName">file to open</param>
/// <param name="Mode">Local and viewer site log mode.</param>
/// <param name="MaxLines">Number of lines before starting a new file (default : -1 = unlimited).</param>
void WinTrace::SetLogFile(const char * FileName, const int Mode, const int MaxLines)
{
    // 3, Local log is disabled
    // 4, Local log enabled.
    // 5, Local log enabled. A new file is create each day (CCYYMMDD is appended to the filename)
    if (Mode >= 3) {
        InternalWinTrace * TraceForm = TTrace::getInternalTraceForm(id.c_str(), true);
        TraceForm->LogFileName = FileName;
        TraceForm->LogFileType = Mode;
        TraceForm->MaxLines = MaxLines;
        // don't send anything to the viewer.
    } else {
        CommandList * Commands = new CommandList() ;
        Commands->Add(CST_LOGFILE, Mode , MaxLines, FileName);
        TTrace::SendToWinTraceClient(Commands, id.c_str());
    }
}

//-------------------------------------------------------------------------
/// <summary>
///   Return the last local log file. (when mode 4 or 5 is used). Note : Call TTrace::Flush() to ensure traces are saved
/// </summary>

string WinTrace::GetLocalLogFile()
{
    InternalWinTrace * TraceForm = TTrace::getInternalTraceForm(id.c_str(), true);

    if (TraceForm == NULL)
        return "";

    return TraceForm->LastLocalLogFileName;
}

//-------------------------------------------------------------------------

/// <summary>
/// Set the log file.(Path is relative to the viewer)
/// To enabled log on local AND on the viewer call this funtion twice.
/// To don't use the viewer, set the TTrace.options.SendMode to None
/// <code>
/// The Mode can be one of the folowing :
/// 0, Viewer Log is disabled.
/// 1, Viewer log enabled.
/// 2, Viewer log enabled. A new file is create each day (CCYYMMDD is appended to the filename)
/// 3, Local log is disabled
/// 4, Local log enabled.
/// 5, Local log enabled. A new file is create each day (CCYYMMDD is appended to the filename)
/// </code>
/// </summary>
/// <param name="FileName">file to open</param>
/// <param name="Mode">Local and viewer site log mode.
/// <param name="MaxLines">Number of lines before starting a new file (default : -1 = unlimited).</param>
/// </param>
void WinTrace::SetLogFile(const wchar_t * FileName, const int Mode, const int MaxLines)
{
    // 3, Local log is disabled
    // 4, Local log enabled. No size limit.
    // 5, Local log enabled. A new file is create each day (CCYYMMDD is appended to the filename)
    if (Mode >= 3) {
        InternalWinTrace * TraceForm = TTrace::getInternalTraceForm(id.c_str(), true);
        char * temp = TTrace::WideToMbs(FileName) ;
        TraceForm->LogFileName = temp ; // copy
        free(temp) ;
        TraceForm->LogFileType = Mode;
        TraceForm->MaxLines = MaxLines;
        // don't send anything to the viewer.
    } else {
        CommandList * Commands = new CommandList() ;
        char * temp   = TTrace::WideToMbs(FileName) ;
        Commands->Add(CST_LOGFILE, Mode , MaxLines, temp);

        if (temp != NULL)
            free(temp) ;

        TTrace::SendToWinTraceClient(Commands, id.c_str());
    }
}
//-------------------------------------------------------------------------

/// <summary>
/// set columns title
/// </summary>
/// <param name="Titles">Tab separated columns titles</param>
void WinTrace::SetColumnsTitle(const char * Titles)
{
    InternalWinTrace * TraceForm = TTrace::getInternalTraceForm(id.c_str(), true);
    TraceForm->IsMultiColTree = true;
    TraceForm->TitleList = Titles ;
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_TREE_COLUMNTITLE , Titles);
    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//-------------------------------------------------------------------------

/// <summary>
/// set columns title
/// </summary>
/// <param name="Titles">Tab separated columns titles</param>
void WinTrace::SetColumnsTitle(const wchar_t * Titles)
{
    InternalWinTrace * TraceForm = TTrace::getInternalTraceForm(id.c_str(), true);
    TraceForm->IsMultiColTree = true;
    char * temp = TTrace::WideToMbs(Titles) ;
    TraceForm->TitleList = temp ; // copy
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_TREE_COLUMNTITLE , temp);

    if (temp != NULL)
        free(temp) ;

    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//-------------------------------------------------------------------------

/// <summary>
/// set columns widths
/// </summary>
/// <param name="Widths">Tab separated columns width.
///    The format for each column is width[:Min[:Max]]
///    where Min and Max are optional minimum and maximum column width for resizing purpose.
///    Example : 100:20:80 tab 200:50 tab 100
/// </param>
void WinTrace::SetColumnsWidth(const char * Widths)
{
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_TREE_COLUMNWIDTH , Widths);
    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//-------------------------------------------------------------------------

/// <summary>
/// set columns widths
/// </summary>
/// <param name="Widths">Tab separated columns width.
///    The format for each column is width[:Min[:Max]]
///    where Min and Max are optional minimum and maximum column width for resizing purpose.
///    Example : 100:20:80 tab 200:50 tab 100
/// </param>
void WinTrace::SetColumnsWidth(const wchar_t * Widths)
{
    CommandList * Commands = new CommandList() ;
    char * temp   = TTrace::WideToMbs(Widths) ;
    Commands->Add(CST_TREE_COLUMNWIDTH , temp);

    if (temp != NULL)
        free(temp) ;

    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//-------------------------------------------------------------------------

/// <summary>
/// Clear all trace for the window tree
/// </summary>
void WinTrace::ClearAll()
{
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_CLEAR_ALL);
    TTrace::SendToWinTraceClient(Commands, id.c_str());
}
//-------------------------------------------------------------------------

/// <summary>
/// Close the WinTrace
/// </summary>
void WinTrace::Close()
{
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_CLOSE_WIN);
    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//------------------------------------------------------------------------------

/// <summary>
/// Set the focus to the first trace node
/// </summary>
void WinTrace::GotoFirstNode()
{
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_GOTO_FIRST_NODE);
    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//------------------------------------------------------------------------------
/// <summary>
/// Set the focus to the last trace node
/// </summary>
void WinTrace::GotoLastNode()
{
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_GOTO_LAST_NODE);
    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//------------------------------------------------------------------------------
/// <summary>
/// Set the focus to the next matching node
/// </summary>
/// <param name="SearForward">If true search down, else search up  </param>
void WinTrace::FindNext(const bool SearForward)
{
    CommandList * Commands = new CommandList() ;

    if (SearForward)
        Commands->Add(CST_FIND_NEXT, 1);
    else
        Commands->Add(CST_FIND_NEXT, 0);

    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//------------------------------------------------------------------------------

/// <summary>
/// Set the focus to a bookmarked node identified by his position. Bookmarks are cheched by the user or with the node.SetBookmark() function
/// </summary>
/// <param name="Pos">Indice of the bookmark </param>
void WinTrace::GotoBookmark(int Pos)
{
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_GOTO_BOOKMARK, Pos);
    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//------------------------------------------------------------------------------
/// <summary>
/// Clear all bookmarks
/// </summary>
void WinTrace::ClearBookmark()
{
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_CLEAR_BOOKMARK);
    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//------------------------------------------------------------------------------
/// <summary>
/// Clear all filters
/// </summary>
void WinTrace::ClearFilter()
{
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_CLEAR_FILTER);
    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//------------------------------------------------------------------------------

/// <summary>
/// Add a filter to node. Multiple calls to this function can be done. Call ApplyFilter() to apply filtering
/// </summary>
/// <param name="Column">Column to apply filter.<p/>
///   In multicolumn mode the first column start at 0 <p/>
///   In normal mode : <p/>
///   col icone   = 999    <p/>
///   col time    = 1      <p/>
///   col thread  = 2      <p/>
///   col traces  = 3      <p/>
///   col Comment = 4      <p/>
///   col members = 998
/// </param>
/// <param name="Compare">There is 5 kinds of filters : <p/>
///    Equal           = 0  <p/>
///    Not equal       = 1  <p/>
///    contains       = 2  <p/>
///    Don't contains  = 3  <p/>
///    (Ignore this filter) = 4 or -1
///</param>
/// <param name="Text">The text to search (insensitive) </param>
void WinTrace::AddFilter(int Column , int Compare , const char * Text)
{
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_ADD_FILTER , Column, Compare , Text);
    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//------------------------------------------------------------------------------

/// <summary>
/// Add a filter to node. Multiple calls to this function can be done. Call ApplyFilter() to apply filtering
/// </summary>
/// <param name="Column">Column to apply filter.<p/>
///   In multicolumn mode the first column start at 0 <p/>
///   In normal mode : <p/>
///   col icone   = 999    <p/>
///   col time    = 1      <p/>
///   col thread  = 2      <p/>
///   col traces  = 3      <p/>
///   col Comment = 4      <p/>
///   col members = 998
/// </param>
/// <param name="Compare">There is 5 kinds of filters : <p/>
///    Equal           = 0  <p/>
///    Not equal       = 1  <p/>
///    contains       = 2  <p/>
///    Don't contains  = 3  <p/>
///    (Ignore this filter) = 4 or -1
///</param>
/// <param name="Text">The text to search (insensitive) </param>
void WinTrace::AddFilter(int Column , int Compare , const wchar_t * Text)
{
    char * temp   = TTrace::WideToMbs(Text) ;
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_ADD_FILTER , Column, Compare , temp);

    if (temp != NULL)
        free(temp) ;

    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//------------------------------------------------------------------------------

/// <summary>
/// Apply filters after calls to AddFilter().
/// </summary>
/// <param name="ConditionAnd">If true, use an 'AND' condition for each filters, else use a "OR" </param>
/// <param name="ShowMatch">If true, show node that match filter and hide others. If false hide matching node and show others</param>
/// <param name="IncludeChildren">If true, search in subnodes</param>
void WinTrace::ApplyFilter(const bool ConditionAnd, const bool ShowMatch, const bool IncludeChildren)
{
    int flags = 0;

    // ConditionAnd<<2+ShowMatch<<1+IncludeChildren
    if (ConditionAnd)
        flags += 4;

    if (ShowMatch)
        flags += 2;

    if (IncludeChildren)
        flags += 1;

    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_APPLY_FILTER, flags);
    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//------------------------------------------------------------------------------
// PLUGIN API
//------------------------------------------------------------------------------
#ifndef _WIN32_WCE

/// <summary>
/// Plugin API : Create a resource.
/// </summary>
/// <param name="ResId">The resource Id (must be >= 100)</param>
/// <param name="ResType">Resource type. See TraceConst
/// <code>
/// CST_RES_BUT_RIGHT    : Button on right
/// CST_RES_BUT_LEFT     : Button on left
/// CST_RES_LABEL_RIGHT  : Label on right
/// CST_RES_LABELH_RIGHT : Label on right HyperLink
/// CST_RES_LABEL_LEFT   : Label on left
/// CST_RES_LABELH_LEFT  : Label on left hyperlink
/// CST_RES_MENU_ACTION  : Item menu in the Actions Menu
/// CST_RES_MENU_WINDOW  : Item menu in the Windows Menu.
///                        Call CreateResource on the main win trace to create this menu item
/// </code>
///</param>
/// <param name="ResWidth">Width of the resource. Applicable only to button and labels</param>
/// <param name="ResText">Resource text</param>

void WinTrace::CreateResource(const int ResId , const int ResType , const int ResWidth , const char * ResText /* = NULL */)
{
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_CREATE_RESOURCE, ResId , ResType, ResWidth , ResText);
    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//------------------------------------------------------------------------------

/// <summary>
/// Plugin API : Create a resource.
/// </summary>
/// <param name="ResId">The resource Id (must be >= 100)</param>
/// <param name="ResType">Resource type. See TraceConst
/// <code>
/// CST_RES_BUT_RIGHT    : Button on right
/// CST_RES_BUT_LEFT     : Button on left
/// CST_RES_LABEL_RIGHT  : Label on right
/// CST_RES_LABELH_RIGHT : Label on right HyperLink
/// CST_RES_LABEL_LEFT   : Label on left
/// CST_RES_LABELH_LEFT  : Label on left hyperlink
/// CST_RES_MENU_ACTION  : Item menu in the Actions Menu
/// CST_RES_MENU_WINDOW  : Item menu in the Windows Menu.
///                        Call CreateResource on the main win trace to create this menu item
/// </code>
///</param>
/// <param name="ResWidth">Width of the resource. Applicable only to button and labels</param>
/// <param name="ResText">Resource text</param>
void WinTrace::CreateResource(const int ResId , const int ResType , const int ResWidth , const wchar_t * ResText /* = NULL */)
{
    CommandList * Commands = new CommandList() ;
    char * temp   = TTrace::WideToMbs(ResText) ;
    Commands->Add(CST_CREATE_RESOURCE, ResId , ResType, ResWidth , temp);

    if (temp != NULL)
        free(temp) ;

    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//------------------------------------------------------------------------------

/// <summary>
/// Plugin API : Disable tracetool or user created resources
/// </summary>
/// <param name="ResId">The resource Id
/// ResId: resource id to disable. Tracetool resources :
/// <code>
/// CST_ACTION_CUT            : Cut. Same as copy then delete
/// CST_ACTION_COPY           : Copy
/// CST_ACTION_DELETE         : Delete selected
/// CST_ACTION_SELECT_ALL     : Select all
/// CST_ACTION_RESIZE_COLS    : Resize columns
/// CST_ACTION_VIEW_INFO      : View trace info
/// CST_ACTION_VIEW_PROP      : View properties
/// CST_ACTION_PAUSE          : Pause
/// CST_ACTION_SAVE           : SaveToFile
/// CST_ACTION_CLEAR_ALL      : Clear all
/// CST_ACTION_CLOSE_WIN      : Close win
/// CST_ACTION_LABEL_INFO     : TracesInfo label
/// CST_ACTION_LABEL_LOGFILE  : LabelLogFile label
/// CST_ACTION_VIEW_MAIN      : View Main trace
/// CST_ACTION_VIEW_ODS       : ODS
/// CST_ACTION_OPEN_XML       : XML trace -> Tracetool XML traces
/// CST_ACTION_EVENTLOG       : Event log
/// CST_ACTION_TAIL           : Tail
/// </code>
/// </param>

void WinTrace::DisableResource(const int ResId)
{
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_DISABLE_RESOURCE , ResId);
    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//------------------------------------------------------------------------------

/// <summary>
/// Plugin API : Set the resource text (tracetool or user created resources), specified by his Id
/// </summary>
/// <param name="ResId">The resource Id </param>
/// <param name="ResText">Resource text</param>

void WinTrace::SetTextResource(const int ResId, const char * ResText)
{
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_SET_TEXT_RESOURCE , ResId, ResText);
    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//------------------------------------------------------------------------------
/// <summary>
/// Plugin API : Set the resource text (tracetool or user created resources), specified by his Id
/// </summary>
/// <param name="ResId">The resource Id </param>
/// <param name="ResText">Resource text</param>

void WinTrace::SetTextResource(const int ResId, const wchar_t * ResText)
{
    CommandList * Commands = new CommandList() ;
    char * temp   = TTrace::WideToMbs(ResText) ;
    Commands->Add(CST_SET_TEXT_RESOURCE , ResId, temp);

    if (temp != NULL)
        free(temp) ;

    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//------------------------------------------------------------------------------

/// <summary>
/// Plugin API : Attach a winTrace to a plugin. Many winTrace can be attached to a plugin.<p/>
/// Note that a plugin don't need to be attached to a WinTrace.<p/>
/// The plugin is identified by his internal name (not dll name).<p/>
/// When linked, the plugin can receive event (see ITracePLugin).
/// </summary>
/// <param name="PluginName">name of the plugin</param>
/// <param name="flags">combinaison of CST_PLUG_ONACTION , CST_PLUG_ONBEFOREDELETE , CST_PLUG_ONTIMER</param>

void WinTrace::LinkToPlugin(const char * PluginName, const int flags)
{
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_LINKTOPLUGIN , flags, PluginName);
    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

//------------------------------------------------------------------------------

/// <summary>
/// Plugin API : Attach a winTrace to a plugin. Many winTrace can be attached to a plugin.<p/>
/// Note that a plugin don't need to be attached to a WinTrace.<p/>
/// The plugin is identified by his internal name (not dll name).<p/>
/// When linked, the plugin can receive event (see ITracePLugin).
/// </summary>
/// <param name="PluginName">name of the plugin</param>
/// <param name="flags">combinaison of CST_PLUG_ONACTION , CST_PLUG_ONBEFOREDELETE , CST_PLUG_ONTIMER</param>

void WinTrace::LinkToPlugin(const wchar_t * PluginName, int flags)
{
    CommandList * Commands = new CommandList() ;
    char * temp   = TTrace::WideToMbs(PluginName) ;
    Commands->Add(CST_LINKTOPLUGIN , flags, temp);

    if (temp != NULL)
        free(temp) ;

    TTrace::SendToWinTraceClient(Commands, id.c_str());
}

#endif

//==========================================================================
// TraceToSend
//==========================================================================

//-------------------------------------------------------------------------
/// Protected : Prepare the commandList. Common to all SendXXX function
CommandList * TraceToSend::prepareNewNode(const char * leftMsg, const char * newId)
{
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_NEW_NODE, GetLastContextId().c_str());            // param : parent Node id
    Commands->Add(CST_TRACE_ID, newId);          // param : guid
    Commands->Add(CST_LEFT_MSG, leftMsg);        // param : left string

    if (iconIndex != -1)
        Commands->Add(CST_ICO_INDEX, iconIndex);

    return Commands;
}

//-------------------------------------------------------------------------

/// Protected : Prepare the commandList. Common to all SendXXX function
CommandList * TraceToSend::prepareNewNode(const wchar_t *leftMsg, const char * newId)
{
    CommandList * Commands = new CommandList() ;
    Commands->Add(CST_NEW_NODE, GetLastContextId().c_str());            // param : parent Node id
    Commands->Add(CST_TRACE_ID, newId);          // param : guid
    char * strLeftMsg  = TTrace::WideToMbs(leftMsg) ;
    Commands->Add(CST_LEFT_MSG, strLeftMsg);              // param : left string
    free(strLeftMsg) ;

    if (iconIndex != -1)
        Commands->Add(CST_ICO_INDEX, iconIndex);

    return Commands;
}

//-------------------------------------------------------------------------
// Protected : return the last context
NodeContext * TraceToSend::GetLastContext()
{
    deque <NodeContext *> * cList ;

    if (winTraceContext != NULL)
        cList = winTraceContext;
    else if (contextList != NULL)
        cList = contextList;
    else
        return NULL ;

    // if empty, no need to enter critical section
    if (cList->empty())
        return NULL ;

    deque<NodeContext *>::const_iterator stack_end ;
    deque<NodeContext *>::const_iterator stack_ptr ;
    DWORD thid = GetCurrentThreadId();
    // enter the node context critical section
    EnterCriticalSection(&criticalSection) ;           // TraceToSend::criticalSection
    // loop context for the current thread
    stack_end = cList->end() ;

    for (stack_ptr = cList->begin() ; stack_ptr < stack_end ; stack_ptr++) {
        NodeContext * context = * stack_ptr ;

        if (context->threadId == thid) {
            LeaveCriticalSection(&criticalSection) ;     // TraceToSend::criticalSection
            return context ;
        }
    }

    LeaveCriticalSection(&criticalSection) ;           // TraceToSend::criticalSection
    return NULL ;
}

//-------------------------------------------------------------------------


// Protected : Retun the last context id for the current thread
string TraceToSend::GetLastContextId()
{
    NodeContext * aContext = GetLastContext();

    if (aContext == NULL)
        return id;
    else
        return aContext->nodeId;
} ;

//-------------------------------------------------------------------------

// Protected : create a context based on the node id and the current thread

void TraceToSend::PushContextId(const char * contextId)
{
    NodeContext * context = new NodeContext() ;
    context->nodeId = contextId ; // string copy (nodeId is a string not a char *)
    context->threadId = GetCurrentThreadId() ;
    deque <NodeContext *> * cList ;

    if (winTraceContext != NULL)
        cList = winTraceContext;   // TraceToSend::winTraceContext
    else if (contextList != NULL)
        cList = contextList;       // TraceToSend::contextList
    else {
        contextList = new deque <NodeContext *> ;
        cList = contextList;
    }

    // enter the node context critical section
    EnterCriticalSection(&criticalSection) ;    // TraceToSend::criticalSection
    cList->push_front(context) ;
    LeaveCriticalSection(&criticalSection) ;    // TraceToSend::criticalSection
}

//-------------------------------------------------------------------------

// Protected : create a context based on the node id and the current thread

void TraceToSend::PushContext(NodeContext * context)
{
    deque <NodeContext *> * cList ;

    if (winTraceContext != NULL)
        cList = winTraceContext;      // TraceToSend::winTraceContext
    else if (contextList != NULL)
        cList = contextList;          // TraceToSend::contextList
    else {
        contextList = new deque <NodeContext *> ;
        cList = contextList;
    }

    // enter the node context critical section
    EnterCriticalSection(&criticalSection) ;    // TraceToSend::criticalSection
    cList->push_front(context) ;
    LeaveCriticalSection(&criticalSection) ;    // TraceToSend::criticalSection
}

//-------------------------------------------------------------------------

// protected : delete the last context for the current thread

void TraceToSend::deleteLastContext(void)
{
    deque <NodeContext *> * cList ;

    if (winTraceContext != NULL)
        cList = winTraceContext;      // TraceToSend::winTraceContext
    else if (contextList != NULL)
        cList = contextList;          // TraceToSend::contextList
    else
        return ;

    // if empty, no need to enter critical section
    if (cList->empty())  // should not happens
        return ;

    deque <NodeContext *>::iterator stack_end ;
    deque <NodeContext *>::iterator stack_ptr ;
    NodeContext * context ;
    context = NULL ;
    DWORD thid = GetCurrentThreadId();
    // enter the node context critical section
    EnterCriticalSection(&criticalSection) ;    // TraceToSend::criticalSection
    // loop context for the current thread
    stack_end = cList->end() ;

    for (stack_ptr = cList->begin() ; stack_ptr < stack_end ; stack_ptr++) {
        NodeContext * context = * stack_ptr ;

        if (context->threadId == thid) {
            delete context ;
            cList->erase(stack_ptr) ;
            LeaveCriticalSection(&criticalSection) ;     // TraceToSend::criticalSection
            return ;
        }
    }

    LeaveCriticalSection(&criticalSection) ;           // TraceToSend::criticalSection
    return ;
}

//-------------------------------------------------------------------------

/// <summary>
/// Same as Indent, but display an 'enter' icon on the viewer
/// </summary>
/// <param name="leftMsg">left message </param>
/// <param name="rightMsg">right message </param>
/// <param name="BackGroundColor">Background Color </param>

void TraceToSend::EnterMethod(const char *leftMsg , const char *rightMsg , int BackGroundColor)
{
    string msg = "Enter " ;
    msg = msg + leftMsg ;
    Indent(msg.c_str(), rightMsg, BackGroundColor, true);
}

//-------------------------------------------------------------------------

/// <summary>
/// Same as Indent, but display an 'enter' icon on the viewer
/// </summary>
/// <param name="leftMsg">left message </param>
/// <param name="rightMsg">right message </param>
/// <param name="BackGroundColor">Background Color </param>

void TraceToSend::EnterMethod(const wchar_t *leftMsg , const wchar_t *rightMsg , int BackGroundColor)
{
    char * strLeftMsg  = TTrace::WideToMbs(leftMsg) ;
    char * strRightMsg = TTrace::WideToMbs(rightMsg) ;
    string msg = "Enter " ;
    msg = msg + strLeftMsg ;
    Indent(msg.c_str(), strRightMsg, BackGroundColor, true);
    free(strLeftMsg) ;
    free(strRightMsg) ;
}

//-------------------------------------------------------------------------

/// <summary>
/// Same as UnIndent, but display an 'exit' icon on the viewer
/// </summary>
/// <param name="leftMsg">left message </param>
/// <param name="rightMsg">right message </param>
/// <param name="BackGroundColor">Background Color </param>

void TraceToSend::ExitMethod(const char *leftMsg, const char *rightMsg , int BackGroundColor)
{
    string msg = "Exit " ;
    msg = msg + leftMsg ;
    UnIndent(msg.c_str(), rightMsg, BackGroundColor, true);
}

//-------------------------------------------------------------------------

/// <summary>
/// Same as UnIndent, but display an 'exit' icon on the viewer
/// </summary>
/// <param name="leftMsg">left message </param>
/// <param name="rightMsg">right message </param>
/// <param name="BackGroundColor">Background Color </param>

void TraceToSend::ExitMethod(const wchar_t *leftMsg, const wchar_t *rightMsg , int BackGroundColor)
{
    char * strLeftMsg  = TTrace::WideToMbs(leftMsg) ;
    char * strRightMsg = TTrace::WideToMbs(rightMsg) ;
    string msg = "Exit " ;
    msg = msg + strLeftMsg ;
    UnIndent(msg.c_str(), strRightMsg, BackGroundColor, true);
    free(strLeftMsg) ;
    free(strRightMsg) ;
}

//-------------------------------------------------------------------------
/// <summary>
/// Send a message. further trace to the same node are indented under this one.
/// </summary>
/// <param name="leftMsg">Left message to send</param>
/// <param name="rightMsg">Right message to send</param>
/// <param name="BackGroundColor">BackGround Color</param>
/// <param name="IsEnter">if true , a special "enter" icon is added on the node</param>
void TraceToSend::Indent(const char *leftMsg, const char *rightMsg, int BackGroundColor, bool IsEnter)
{
    if (enabled == false)
        return ;

    NodeContext * newContext = new NodeContext() ;
    newContext->threadId = GetCurrentThreadId() ;
    char *c;
    c = TTrace::CreateTraceID() ;
    newContext->nodeId = c; //copy
    free(c);
    CommandList * Commands = new CommandList() ;
    string last = GetLastContextId() ;
    Commands->Add(CST_NEW_NODE, last.c_str());              // param : parent Node id
    Commands->Add(CST_TRACE_ID, newContext->nodeId.c_str());   // param : Node Id
    Commands->Add(CST_LEFT_MSG, leftMsg);              // param : left string

    if (rightMsg != NULL && rightMsg != "")
        Commands->Add(CST_RIGHT_MSG, rightMsg);        // param : right string

    if (BackGroundColor != -1)
        Commands->Add(CST_BACKGROUND_COLOR, BackGroundColor, "-1");      // param : color, colId

    if (IsEnter) {
        TMemberNode * member = new TMemberNode();                     // create root member
        member->Add("")->ViewerKind = CST_VIEWER_ENTER;    // then add an empty member with special viewer
        member->AddToStringList(Commands);                        // convert all groups and nested items/group to strings
        delete member ;
    }

    Commands->Add(CST_ICO_INDEX, iconIndex);          // param : icon index
    TTrace::SendToWinTraceClient(Commands, winTraceId.c_str());
    PushContext(newContext) ;
}

//------------------------------------------------------------------------------
/// <summary>
/// Send a message. further trace to the same node are indented under this one.
/// </summary>
/// <param name="wLeftMsg">Left message to send</param>
/// <param name="wRightMsg">Right message to send</param>
/// <param name="BackGroundColor">BackGround Color</param>
/// <param name="IsEnter">if true , a special "enter" icon is added on the node</param>
void TraceToSend::Indent(const wchar_t *wLeftMsg, const wchar_t *wRightMsg, int BackGroundColor, bool IsEnter)
{
    if (enabled == false)
        return ;

    NodeContext * newContext = new NodeContext() ;
    newContext->threadId = GetCurrentThreadId() ;
    char *c;
    c = TTrace::CreateTraceID() ;
    newContext->nodeId = c; //copy
    free(c);
    CommandList * Commands = new CommandList() ;
    string last = GetLastContextId() ;
    Commands->Add(CST_NEW_NODE, last.c_str());              // param : parent Node id
    Commands->Add(CST_TRACE_ID, newContext->nodeId.c_str());   // param : Node Id
    char * LeftMsg  = TTrace::WideToMbs(wLeftMsg) ;
    Commands->Add(CST_LEFT_MSG, LeftMsg);              // param : left string
    free(LeftMsg) ;
    char * RightMsg = TTrace::WideToMbs(wRightMsg) ;

    if (RightMsg != NULL && RightMsg != "")
        Commands->Add(CST_RIGHT_MSG, RightMsg);        // param : right string

    if (RightMsg != NULL)
        free(RightMsg) ;

    if (BackGroundColor != -1)
        Commands->Add(CST_BACKGROUND_COLOR, BackGroundColor, "-1");      // param : color, colId

    if (IsEnter) {
        TMemberNode * member = new TMemberNode();                     // create root member
        member->Add("")->ViewerKind = CST_VIEWER_ENTER;    // then add an empty member with special viewer
        member->AddToStringList(Commands);                        // convert all groups and nested items/group to strings
        delete member ;
    }

    Commands->Add(CST_ICO_INDEX, iconIndex);          // param : icon index
    TTrace::SendToWinTraceClient(Commands, winTraceId.c_str());
    PushContext(newContext) ;
}

//------------------------------------------------------------------------------
/// <summary>
/// send a node then change the indentation
/// </summary>
/// <param name="leftMsg" >Left message</param>
/// <param name="rightMsg">Optional right message</param>

void TraceToSend::Indent(const char *leftMsg, const char *rightMsg)
{
    Indent(leftMsg, rightMsg, -1, false) ;
}

//-------------------------------------------------------------------------

/// <summary>
/// send a node then change the indentation
/// </summary>
/// <param name="leftMsg" >Left message</param>
/// <param name="rightMsg">Optional right message</param>

void TraceToSend::Indent(const wchar_t *leftMsg, const wchar_t *rightMsg)
{
    Indent(leftMsg, rightMsg, -1, false) ;
}

//-------------------------------------------------------------------------
/// <summary>
/// Delete indentation to the node added by indent()
/// </summary>
/// <param name="leftMsg">Message to send to close indentation (optional)</param>
/// <param name="rightMsg">Message to send to close indentation (optional)</param>
/// <param name="BackGroundColor">RGB background color (optional)(see Color.ToArgb function)</param>
/// <param name="isExit">if true, viewer type 'exit' is used</param>
void TraceToSend::UnIndent(const char *leftMsg, const char *rightMsg, int BackGroundColor, bool isExit)
{
    if (enabled == false)
        return;

    deleteLastContext();

    if (leftMsg != NULL || rightMsg != NULL) {
        char *c;
        c = TTrace::CreateTraceID() ;
        CommandList * Commands = prepareNewNode(leftMsg, c);
        free(c);

        if (rightMsg != NULL)
            Commands->Add(CST_RIGHT_MSG, rightMsg);    // param : right string

        if (BackGroundColor != -1)
            Commands->Add(CST_BACKGROUND_COLOR, BackGroundColor, "-1");      // param : color, colId

        if (isExit) {
            TMemberNode * member = new TMemberNode();                     // create root member
            member->Add("")->ViewerKind = CST_VIEWER_EXIT;     // then add an empty member with special viewer
            member->AddToStringList(Commands);                        // convert all groups and nested items/group to strings
            delete member ;
        }

        TTrace::SendToWinTraceClient(Commands, winTraceId.c_str());
    }
}
//------------------------------------------------------------------------------
/// <summary>
/// Delete indentation to the node added by indent()
/// </summary>
/// <param name="leftMsg">Message to send to close indentation (optional)</param>
/// <param name="rightMsg">Message to send to close indentation (optional)</param>
/// <param name="BackGroundColor">RGB background color (optional)(see Color.ToArgb function)</param>
/// <param name="isExit">if true, viewer type 'exit' is used</param>
void TraceToSend::UnIndent(const wchar_t *leftMsg, const wchar_t *rightMsg, int BackGroundColor, bool isExit)
{
    if (enabled == false)
        return;

    deleteLastContext();

    if (leftMsg != NULL || rightMsg != NULL) {
        // create a node with same properties as "self" with new ID
        TraceNode * node = new TraceNode(this);   // copy constructor
        char *c;
        c = TTrace::CreateTraceID() ; // then give new ID
        node->id = c; //copy
        free(c);
        CommandList * Commands = prepareNewNode(leftMsg, node->id.c_str());

        if (rightMsg != NULL) {
            char * strRightMsg  = TTrace::WideToMbs(rightMsg) ;
            Commands->Add(CST_RIGHT_MSG, strRightMsg);    // param : right string
            free(strRightMsg) ;
        }

        if (BackGroundColor != -1)
            Commands->Add(CST_BACKGROUND_COLOR, BackGroundColor, "-1");      // param : color, colId

        if (isExit) {
            TMemberNode * member = new TMemberNode();                     // create root member
            member->Add("")->ViewerKind = CST_VIEWER_EXIT;     // then add an empty member with special viewer
            member->AddToStringList(Commands);                        // convert all groups and nested items/group to strings
            delete member ;
        }

        TTrace::SendToWinTraceClient(Commands, winTraceId.c_str());
    }
}
//------------------------------------------------------------------------------
/// <summary>
/// Decrement indentation
/// </summary>
/// <param name="leftMsg" >Optional left message</param>
/// <param name="rightMsg">Optional right message</param>

void TraceToSend::UnIndent(const char *leftMsg, const char *rightMsg)
{
    UnIndent(leftMsg, rightMsg, -1, false);
}

//-------------------------------------------------------------------------

/// <summary>
/// Decrement indentation
/// </summary>
/// <param name="leftMsg" >Optional left message</param>
/// <param name="rightMsg">Optional right message</param>

void TraceToSend::UnIndent(const wchar_t *leftMsg, const wchar_t *rightMsg)
{
    UnIndent(leftMsg, rightMsg, -1, false);
}

//-------------------------------------------------------------------------

/// <summary>
/// Decrement indentation. Nothing is send
/// </summary>
void TraceToSend::UnIndent(void)
{
    if (enabled == false)
        return;

    deleteLastContext();
}

//-------------------------------------------------------------------------



void TraceToSend::SendFormat(const char* format, ...)
{
    va_list arglist;
    char _buffer[MAX_LEN];
    va_start(arglist, format);    
    vsnprintf_s(_buffer, MAX_LEN, MAX_LEN, format, arglist);
    va_end(arglist);
    Send(_buffer);
}


void TraceToSend::SendFormat(const wchar_t* format, ...)
{
    va_list arglist;
    wchar_t _buffer[MAX_LEN];
    va_start(arglist, format);
    _vsnwprintf_s(_buffer, MAX_LEN, MAX_LEN, format, arglist);   
    va_end(arglist);
    Send(_buffer);
}

/// <summary>
/// Create and send a trace node to the viewer
/// </summary>
/// <param name="leftMsg">the left message</param>
/// <param name="rightMsg">the optional right message</param>

void TraceToSend::Send(const char *leftMsg, const char *rightMsg /* = NULL */)
{
    if (enabled == false)
        return ;

    char * newId = TTrace::CreateTraceID() ;
    CommandList * Commands = prepareNewNode(leftMsg, newId);
    free(newId);

    if (rightMsg != NULL)
        Commands->Add(CST_RIGHT_MSG, rightMsg);   // param : right string

    // 添加 dbgview 支持
    OutputDebugStringA(leftMsg);
    OutputDebugStringA("\n");
    printf_s("%s\n", leftMsg);
    TTrace::SendToWinTraceClient(Commands, winTraceId.c_str());
}

//-------------------------------------------------------------------------
/// <summary>
/// Create and send a trace node to the viewer
/// </summary>
/// <param name="wLeftMsg">the left message</param>
/// <param name="wRightMsg">Optional right message</param>

void TraceToSend::Send(const wchar_t *wLeftMsg, const wchar_t *wRightMsg /* = NULL */)
{
    if (enabled == false)
        return ;

    char * strLeftMsg  = TTrace::WideToMbs(wLeftMsg) ;
    char * strRightMsg = TTrace::WideToMbs(wRightMsg) ;
    char * newId = TTrace::CreateTraceID() ;
    CommandList * Commands = prepareNewNode(strLeftMsg, newId);
    free(newId) ;
    free(strLeftMsg) ;

    if (strRightMsg != NULL) 
        Commands->Add(CST_RIGHT_MSG, strRightMsg);   // param : right string

    // 添加 dbgview 支持
    OutputDebugStringW(wLeftMsg);
    OutputDebugStringA("\n");
    wprintf_s(L"%s\n", wLeftMsg);
    TTrace::SendToWinTraceClient(Commands, winTraceId.c_str());

    if (strRightMsg != NULL)
        free(strRightMsg) ;
}

void TraceToSend::SendA(const char *leftMsg, const char *rightMsg /* = NULL */)
{
    Send(leftMsg, rightMsg);
}


void TraceToSend::SendW(const wchar_t *wLeftMsg, const wchar_t *wRightMsg /* = NULL */)
{
    Send(wLeftMsg, wRightMsg);
}

//-------------------------------------------------------------------------

/// <summary>
/// send an xml string to the viewer
/// </summary>
/// <param name="leftMsg">Left message</param>
/// <param name="xml">the xml to display</param>
void TraceToSend::SendXml(const char *leftMsg, const char * xml)
{
    if (enabled == false)
        return ;

    TraceNodeEx * Node;
    Node = new TraceNodeEx(this, true);
    Node->leftMsg = leftMsg ;
    Node->AddXml(xml) ;
    Node->Send() ;
    delete Node ;
}

//-------------------------------------------------------------------------

/// <summary>
/// send an xml string to the viewer
/// </summary>
/// <param name="leftMsg">Left message</param>
/// <param name="xml">the xml to display</param>
void TraceToSend::SendXml(const wchar_t *leftMsg, const wchar_t * xml)
{
    if (enabled == false)
        return ;

    char * strLeftMsg  = TTrace::WideToMbs(leftMsg) ;
    char * strXml      = TTrace::WideToMbs(xml) ;
    TraceNodeEx * Node;
    Node = new TraceNodeEx(this, true);
    Node->leftMsg = strLeftMsg ;
    Node->AddXml(strXml) ;
    Node->Send() ;
    delete Node ;
    free(strLeftMsg) ;
    free(strXml) ;
}

//-------------------------------------------------------------------------
/// <summary>
/// send a dump to the viewer
/// </summary>
/// <param name="leftMsg">Left message</param>
/// <param name="rightMsg">Optional right message</param>
/// <param name="title">The title that appears on top of the dump</param>
/// <param name="memory">The memory to dump</param>
/// <param name="byteCount"></param>

void TraceToSend::SendDump(const char *leftMsg, const char *rightMsg, const char * title, const char * memory, const unsigned byteCount)
{
    if (enabled == false)
        return ;

    TraceNodeEx * Node;
    Node = new TraceNodeEx(this, true);
    Node->leftMsg = leftMsg ;

    if (rightMsg != NULL)
        Node->rightMsg = rightMsg ;

    Node->AddDump(title, memory, 0 , byteCount) ;
    Node->Send() ;
    delete Node ;
}

//-------------------------------------------------------------------------
/// <summary>
/// send a dump to the viewer
/// </summary>
/// <param name="wLeftMsg">Left message</param>
/// <param name="wRightMsg">Optional right message</param>
/// <param name="title">The title that appears on top of the dump</param>
/// <param name="memory">The memory to dump</param>
/// <param name="byteCount"></param>

void TraceToSend::SendDump(const wchar_t *wLeftMsg, const wchar_t *wRightMsg, const wchar_t * title, const char * memory, const unsigned byteCount)
{
    if (enabled == false)
        return ;

    char * strLeftMsg  = TTrace::WideToMbs(wLeftMsg) ;
    char * strRightMsg = TTrace::WideToMbs(wRightMsg) ;
    TraceNodeEx * Node;
    Node = new TraceNodeEx(this, true);
    Node->leftMsg = strLeftMsg ;

    if (strRightMsg != NULL)
        Node->rightMsg = strRightMsg ;

    Node->AddDump(title, memory, 0 , byteCount) ;
    Node->Send() ;
    free(strLeftMsg) ;

    if (strRightMsg != NULL)
        free(strRightMsg) ;

    delete Node ;
}

//==========================================================================
// TraceNodeEx
//==========================================================================

/// <summary>
/// Construct a new trace node, derived from a parent node
/// </summary>
/// <param name="parentNode">Optional Parent node. if null : the root tree become the parent node, enabled is true and the default icon is used</param>
/// <param name="generateUniqueId">Optional. If true, the id is generated automatically, else set the empty string </param>
TraceNodeEx::TraceNodeEx(TraceToSend * parentNode , bool generateUniqueId)
{
    m_Members = NULL ;              // create members only when needed
    m_FontDetails = NULL ;
    traceNode = NULL ;              // represent the node on the viewer

    if (generateUniqueId) {
        char *c;
        c = TTrace::CreateTraceID() ;
        id = c; //copy
        free(c);
    }

    if (parentNode == NULL) {
        iconIndex = CST_ICO_DEFAULT;
        enabled = true;
        winTraceId = "";
        parentNodeId = "";
    } else {
        iconIndex    = parentNode->iconIndex;
        enabled      = parentNode->enabled;
        winTraceId   = parentNode->winTraceId;
        parentNodeId = parentNode->GetLastContextId();
    }
}

//-------------------------------------------------------------------------
/// <summary>
/// Construct a new trace node, derived from a parent node
/// </summary>
/// <param name="parentNode">Optional Parent node. if null : the root tree become the parent node, enabled is true and the default icon is used</param>
/// <param name="generateUniqueId">Optional. If true, the id is generated automatically, else set the empty string </param>
TraceNodeEx::TraceNodeEx(TraceNodeEx * parentNode , bool generateUniqueId)
{
    m_Members = NULL ;              // create members only when needed
    m_FontDetails = NULL ;
    traceNode = NULL ;              // represent the node on the viewer

    if (generateUniqueId) {
        char *c;
        c = TTrace::CreateTraceID() ;
        id = c; //copy
        free(c);
    }

    iconIndex    = parentNode->iconIndex;
    enabled      = parentNode->enabled;
    winTraceId   = parentNode->winTraceId;
    parentNodeId = parentNode->id;
}

//-------------------------------------------------------------------------

/// <summary>
/// Construct a new trace node, derived from a parent node
/// </summary>
/// <param name="parentNode">Parent node. </param>
/// <param name="generateUniqueId">Optional. If true, the id is generated automatically, else set the empty string </param>
TraceNodeEx::TraceNodeEx(TraceNodeBase * parentNode, const bool generateUniqueId)
{
    m_Members = NULL ;              // create members only when needed
    m_FontDetails = NULL ;
    traceNode = NULL ;              // represent the node on the viewer

    if (generateUniqueId) {
        char *c;
        c = TTrace::CreateTraceID() ;
        id = c; //copy
        free(c);
    }

    iconIndex    = parentNode->iconIndex;
    enabled      = parentNode->enabled;
    winTraceId   = parentNode->winTraceId;
    parentNodeId = parentNode->id;
}

//-------------------------------------------------------------------------
/// <summary>
/// Normally destructors are not needed : TTrace::Send clear the node content.<p/>
/// It's usefull only if you create a node without sending it.
/// </summary>

TraceNodeEx::~TraceNodeEx(void)
{
    if (m_Members != NULL)      // happens only if the nodeEx is not send
        delete m_Members ;

    if (traceNode != NULL)
        delete traceNode;        // can be null only if the nodeEx is not send

    if (m_FontDetails != NULL) { // happens only if the nodeEx is not send
        deque <FontDetail *>::const_iterator FontDetailBegin;
        deque <FontDetail *>::const_iterator FontDetailEnd;
        FontDetailEnd  = m_FontDetails->end() ;

        for (FontDetailBegin = m_FontDetails->begin(); FontDetailBegin < FontDetailEnd ; FontDetailBegin++) {
            FontDetail * fontDetail = * FontDetailBegin ;
            delete fontDetail ;
        }

        delete m_FontDetails ;
        m_FontDetails = NULL ;
    }
}

//-------------------------------------------------------------------------

/// <summary>
/// return the protected "members" (and create it if needed)
/// </summary>
/// <returns>The memberNode</returns>

TMemberNode * TraceNodeEx::Members()
{
    if (m_Members == NULL)
        m_Members = new TMemberNode() ;

    return m_Members ;
}

//-------------------------------------------------------------------------
/// <summary>
/// add an xml string to the node
/// </summary>
/// <param name="xml">the xml to display</param>
/// <returns>itself</returns>

TraceNodeEx * TraceNodeEx::AddXml(const char * xml)
{
    if (enabled == false)
        return this;

    Members() ;   // ensure root member is create
    TMemberNode * xmlGroup ;
    xmlGroup = m_Members->Add(xml) ;
    xmlGroup->ViewerKind = CST_VIEWER_XML ;
    return this ;
}

//-------------------------------------------------------------------------
/// <summary>
/// Add a dump to the "member"
/// </summary>
/// <param name="Title">The title that appears on top of the dump</param>
/// <param name="memory">The memory to dump</param>
/// <param name="index">offset of first byte to dump</param>
/// <param name="byteCount"></param>
/// <returns>itself</returns>

TraceNodeEx * TraceNodeEx::AddDump(const wchar_t * Title , const char * memory , const unsigned index , const unsigned byteCount)
{
    char * temp = TTrace::WideToMbs(Title) ;
    AddDump(temp , memory , index , byteCount) ;
    free(temp) ;
    return this ;
}

//-------------------------------------------------------------------------
/// <summary>
/// Add a dump to the "member"
/// </summary>
/// <param name="Title">The title that appears on top of the dump</param>
/// <param name="memory">The memory to dump</param>
/// <param name="index">offset of first byte to dump</param>
/// <param name="byteCount"></param>
/// <returns>itself</returns>

TraceNodeEx * TraceNodeEx::AddDump(const char * Title , const char * memory , const unsigned index , const unsigned byteCount)
{
    unsigned c, d, byteDumped, beginLine ;
    char hexa_representation [50] ;
    //char Str_representation [20] ;
    char adress [10] ;
    unsigned char * PtrBuf ;
    char * ptrHex ; //, * ptrString  ;
    unsigned int OneChar ;
    TMemberNode * DumpGroup ;

    if (enabled == false)
        return this;

    Members() ;   // ensure root member is create
    DumpGroup = new TMemberNode(Title) ;
    DumpGroup->SetFontDetail(0, true);
    DumpGroup->ViewerKind = CST_VIEWER_DUMP ;
    m_Members->Add(DumpGroup) ;
    PtrBuf = (unsigned char *) memory ;
    c = 0 ;
    byteDumped = 0 ;

    while (byteDumped < byteCount) {
        d = 0 ;
        beginLine = c ;
        ptrHex = hexa_representation ;

        //ptrString = Str_representation ;
        while ((byteDumped < byteCount) && (d < 16)) {
            OneChar = *PtrBuf ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
            sprintf_s(ptrHex, 4, "%02X ", OneChar) ;
#else
            sprintf(ptrHex, "%02X ", OneChar) ;
#endif
            ptrHex += 3 ;
            // since 12.4 : string representation is no more send to viewer. The viewer will calculate hitself the string
            //if (isprint(OneChar) == 0)
            //   OneChar = '.' ;
            //#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
            //sprintf_s (ptrString, 2 ,"%c",OneChar) ;
            //#else
            //sprintf (ptrString,"%c",OneChar) ;
            //#endif
            //ptrString++ ;
            d++ ;
            c++ ;
            PtrBuf++ ;
            byteDumped++;
        }

#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
        sprintf_s(adress, 8, "%06X", beginLine) ;
#else
        sprintf(adress, "%06X", beginLine) ;
#endif
        DumpGroup->Add(adress , hexa_representation) ;   // , Str_representation) ;
    }

    // reuse the hexa_representation to printf the number of byte dumped
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
    sprintf_s(hexa_representation, 50, "%d byte(s) dumped", byteDumped) ;
#else
    sprintf(hexa_representation, "%d byte(s) dumped", byteDumped) ;
#endif
    DumpGroup->col2 = hexa_representation ;
    return this ;
}

//------------------------------------------------------------------------------


/// <summary>
/// Change font detail for an item in the trace
/// </summary>
/// <param name="ColId">Column index : all columns=-1, Icon=0, Time=1, thread=2, left msg=3, right msg =4 or user defined column</param>
/// <param name="Bold">Change font to bold</param>
/// <param name="Italic">Change font to Italic</param>
/// <param name="Color">Change Color. Use -1 to keep default color</param>
/// <param name="Size">Change font size, use zero to keep normal size</param>
/// <param name="FontName">Change font name</param>
/// <returns>itself</returns>

TraceNodeEx * TraceNodeEx::AddFontDetail(const int ColId,  const bool Bold ,  const bool Italic /*= false*/ ,  const int Color /*= -1*/ , const int Size /*= 0*/ ,  const char * FontName /*= NULL*/)
{
    if (enabled == false)
        return this;

    if (m_FontDetails == NULL)
        m_FontDetails = new deque <FontDetail *> ;

    FontDetail * font = new FontDetail() ;
    font->ColId    = ColId ;
    font->Bold     = Bold ;
    font->Italic   = Italic ;
    font->Color    = Color ;
    font->Size     = Size ;

    if (FontName != NULL)
        font->FontName = FontName ;

    m_FontDetails->push_back(font);
    return this ;
}

//-------------------------------------------------------------------------

/// <summary>
/// Change Background color
/// </summary>
/// <param name="Color">Change Color. Use -1 to keep default color</param>
/// <param name="ColId">Column index : all columns=-1, Icon=0, Time=1, thread=2, left msg=3, right msg =4 or user defined column</param>
/// <returns>itself</returns>
TraceNodeEx * TraceNodeEx::AddBackgroundColor(const int Color, const int ColId)
{
    if (enabled == false)
        return this;

    FontDetail * font = new FontDetail() ;
    font->ColId    = ColId ;
    font->Color    = Color ;
    font->FontName = "BackgroundColor" ;  // special name.
    m_FontDetails->push_back(font);
    return this ;
}

//-------------------------------------------------------------------------

/// <summary>
/// Send the node to the viewer
/// </summary>
/// <Returns>
/// The traceNode. Deleted at the same time as the TraceNodeEx.<p/>
/// This traceNode can also be accessed from the TraceNodeEx::traceNode property
/// </Returns>
TraceNode * TraceNodeEx::Send(void)
{
    if (enabled == false)
        return NULL;

    CommandList * Commands ;                // Commands to send
    Commands = new CommandList() ;
    Commands->Add(CST_NEW_NODE  , parentNodeId.c_str()) ; // param : parent Node id
    Commands->Add(CST_TRACE_ID, id.c_str());               // param : guid

    if (leftMsg.length() != 0) {
        Commands->Add(CST_LEFT_MSG, leftMsg.c_str());       // param : left string
        leftMsg = "" ;    // reset after send
    }

    if (rightMsg.length() != 0) {
        Commands->Add(CST_RIGHT_MSG, rightMsg.c_str());     // param : right string
        rightMsg = "" ;   // reset after send
    }

    Commands->Add(CST_ICO_INDEX, iconIndex);       // param : the icon index

    if (m_FontDetails != NULL) {
        deque <FontDetail *>::const_iterator FontBegin;
        deque <FontDetail *>::const_iterator FontEnd;
        FontEnd  = m_FontDetails->end() ;

        for (FontBegin = m_FontDetails->begin(); FontBegin < FontEnd ; FontBegin++) {
            FontDetail * font = * FontBegin ;
            char * message ;
            int bold   = (font->Bold) ? 1 : 0 ;
            int italic = (font->Italic) ? 1 : 0;
            size_t MsgLen ;

            if (font->FontName.compare("BackgroundColor") == 0) {
                //special color : background color
                message = (char*)malloc(27 + 1) ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
                sprintf_s(message, 28, "%5d%11d%11d", CST_BACKGROUND_COLOR, font->Color, font->ColId); // param : color, colId
#else
                sprintf(message, "%5d%11d%11d", CST_BACKGROUND_COLOR, font->Color, font->ColId); // param : color, colId
#endif
            } else if (font->FontName.empty()) {
                message = (char*)malloc(32 + 1) ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
                sprintf_s(message, 33, "%5d%3d%1d%1d%11d%11d", CST_FONT_DETAIL, font->ColId, bold, italic, font->Color, font->Size);
#else
                sprintf(message, "%5d%3d%1d%1d%11d%11d", CST_FONT_DETAIL, font->ColId, bold, italic, font->Color, font->Size);
#endif
            } else {
                MsgLen = font->FontName.length() + 32 + 1 ;
                message = (char*)malloc(MsgLen) ;
                const char * fontName = font->FontName.c_str() ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
                sprintf_s(message, MsgLen, "%5d%3d%1d%1d%11d%11d%s", CST_FONT_DETAIL, font->ColId, bold, italic, font->Color, font->Size, fontName);
#else
                sprintf(message, "%5d%3d%1d%1d%11d%11d%s", CST_FONT_DETAIL, font->ColId, bold, italic, font->Color, font->Size, fontName);
#endif
            }

            Commands->push_back(message) ;
            free(message) ;
            delete font ;
        }

        delete m_FontDetails ;
        m_FontDetails = NULL ;
    }

    // convert Members to commands
    if (m_Members != NULL) {
        m_Members->AddToStringList(Commands) ;
        delete m_Members ;
        m_Members = NULL ;
    }

    TTrace::SendToWinTraceClient(Commands, winTraceId.c_str(), time.c_str(), threadName.c_str());
    // create the TraceNode property and return it.
    traceNode = new TraceNode(this);   // owned by the TraceNodeEx.
    return traceNode ;
}


//==========================================================================
// TraceNode
//==========================================================================

/// <summary>
/// construct a new trace node, derived from a parent node
/// </summary>
/// <param name="parentNode">Parent node (Optional) </param>
/// <param name="generateUniqueId">generate an unique Id if true (Optional, default is true)</param>

TraceNode::TraceNode(const TraceNode * parentNode  , const bool generateUniqueId /*  = true */)
{
    contextList = NULL ;
    winTraceContext = NULL ;

    if (generateUniqueId) {
        char *c;
        c = TTrace::CreateTraceID() ;
        id = c; //copy
        free(c);
    }

    if (parentNode == NULL) {
        iconIndex    = CST_ICO_DEFAULT;
        enabled      = true;
        winTraceId   = "" ;
        tag          = 0 ;
    } else {
        iconIndex    = parentNode->iconIndex ;
        enabled      = parentNode->enabled ;
        winTraceId   = parentNode->winTraceId ;
        tag          = parentNode->tag ;
    }

    InitializeCriticalSection(&criticalSection) ;   // TraceToSend::criticalSection
}

//-------------------------------------------------------------------------
/// <summary>
/// Copy constructor from a TraceToSend object
/// </summary>
/// <param name="source">source to convert </param>

TraceNode::TraceNode(const TraceToSend * source)
{
    contextList     = NULL ;
    winTraceContext = NULL ;
    iconIndex  = source->iconIndex;
    enabled    = source->enabled;
    winTraceId = source->winTraceId;
    tag        = source->tag ;
    id         = source->id;
    InitializeCriticalSection(&criticalSection) ;   // TraceToSend::criticalSection
}

//-------------------------------------------------------------------------

/// <summary>
/// Copy constructor from a TraceNodeEx object
/// </summary>
/// <param name="source">source to convert </param>
TraceNode::TraceNode(const TraceNodeEx * source)
{
    contextList     = NULL ;
    winTraceContext = NULL ;
    iconIndex  = source->iconIndex;
    enabled    = source->enabled;
    winTraceId = source->winTraceId;
    tag        = source->tag ;
    id         = source->id;
    InitializeCriticalSection(&criticalSection) ;   // TraceToSend::criticalSection
}

//-------------------------------------------------------------------------

/// <summary>
/// destructor
/// </summary>

TraceNode::~TraceNode(void)
{
    DeleteCriticalSection(&criticalSection) ;    // TraceToSend::criticalSection

    if (contextList != NULL)
        delete contextList ;

    contextList = NULL ;
}

//-------------------------------------------------------------------------

/// <summary>
/// Delete the node
/// </summary>
/// <returns>itself</returns>

TraceNode * TraceNode::Delete()
{
    if (enabled == false)
        return this;

    if (id == "")
        return this;

    CommandList * Commands ;                // Commands to send
    Commands = new CommandList() ;
    Commands->AddFront(CST_CLEAR_NODE, id.c_str());
    TTrace::SendToWinTraceClient(Commands, winTraceId.c_str());
    return this ;
}

//------------------------------------------------------------------------------
/// <summary>
/// Delete children node
/// </summary>
/// <returns>itself</returns>

TraceNode * TraceNode::DeleteChildren()
{
    if (enabled == false)
        return this;

    if (id == "")
        return this;

    CommandList * Commands ;                // Commands to send
    Commands = new CommandList() ;
    Commands->AddFront(CST_CLEAR_SUBNODES, id.c_str());
    TTrace::SendToWinTraceClient(Commands, winTraceId.c_str());
    return this ;
}

//----------------------------------------------------------------------
/// <summary>
/// Show the node in the tree (not means selected, just visible in the tree)
/// </summary>
/// <returns>itself</returns>

TraceNode * TraceNode::Show()
{
    if (enabled == false)
        return this;

    if (id == "")
        return this;

    CommandList * Commands ;                // Commands to send
    Commands = new CommandList() ;
    Commands->AddFront(CST_FOCUS_NODE, id.c_str());
    TTrace::SendToWinTraceClient(Commands, winTraceId.c_str());
    return this ;
}

//----------------------------------------------------------------------
/// <summary>
/// Select the node in the viewer
/// </summary>
/// <returns>itself</returns>

TraceNode * TraceNode::SetSelected()
{
    if (enabled == false)
        return this;

    if (id == "")
        return this;

    CommandList * Commands ;                // Commands to send
    Commands = new CommandList() ;
    Commands->AddFront(CST_SELECT_NODE, id.c_str());
    TTrace::SendToWinTraceClient(Commands, winTraceId.c_str());
    return this ;
}

//----------------------------------------------------------------------
/// <summary>
/// Append right and left texts to an existing node
/// </summary>
/// <param name="LeftMsg">Left message to append.Set To NULL to don't append left part</param>
/// <param name="RightMsg">Optional right message to append.Set To NULL to don't append right part</param>
/// <returns>itself</returns>

TraceNode * TraceNode::Append(const char * LeftMsg, const char * RightMsg /* = NULL */)
{
    if (enabled == false)
        return this;

    if (id == "")
        return this;

    CommandList * Commands ;                // Commands to send
    Commands = new CommandList() ;
    Commands->AddFront(CST_USE_NODE, id.c_str());                           // param : guid

    if (LeftMsg != NULL)
        Commands->Add(CST_APPEND_LEFT_MSG, LeftMsg);                 // param : left string

    if (RightMsg != NULL)
        Commands->Add(CST_APPEND_RIGHT_MSG, RightMsg);               // param : right string

    TTrace::SendToWinTraceClient(Commands, winTraceId.c_str());
    return this ;
}

//----------------------------------------------------------------------

/// <summary>
/// Append right and left texts to an existing node
/// </summary>
/// <param name="LeftMsg">Left message to append.Set To NULL to don't append left part</param>
/// <param name="RightMsg">Optional right message to append.Set To NULL to don't append right part</param>
/// <returns>itself</returns>

TraceNode * TraceNode::Append(const wchar_t * LeftMsg, const wchar_t * RightMsg /* = NULL */)
{
    char * left  = TTrace::WideToMbs(LeftMsg) ;
    char * right = TTrace::WideToMbs(RightMsg) ;
    Append(left, right) ;

    if (left != NULL)
        free(left) ;

    if (right != NULL)
        free(right) ;

    return this ;
}

//----------------------------------------------------------------------

/// <summary>
/// Resend the left and right trace message to the viewer
/// </summary>
/// <param name="LeftMsg">Left message to replace.Set To NULL to don't resend left part</param>
/// <param name="RightMsg">Optional right message to replace.Set To NULL to don't resend right part</param>
/// <returns>itself</returns>

TraceNode * TraceNode::Resend(const wchar_t *LeftMsg, const wchar_t *RightMsg /* = NULL */)
{
    char * left  = TTrace::WideToMbs(LeftMsg) ;
    char * right = TTrace::WideToMbs(RightMsg) ;
    Resend(left, right) ;

    if (left != NULL)
        free(left) ;

    if (right != NULL)
        free(right) ;

    return this ;
}

//----------------------------------------------------------------------

/// <summary>
/// Resend the left and right trace message to the viewer
/// </summary>
/// <param name="LeftMsg">Left message to replace.Set To NULL to don't resend left part</param>
/// <param name="RightMsg">Optional right message to replace.Set To NULL to don't resend right part</param>
/// <returns>itself</returns>

TraceNode * TraceNode::Resend(const char *LeftMsg, const char *RightMsg /* = NULL */)
{
    if (enabled == false)
        return this;

    if (id == "")
        return this;

    CommandList * Commands ;                // Commands to send
    Commands = new CommandList() ;
    Commands->AddFront(CST_USE_NODE, id.c_str());   // param : guid

    if (LeftMsg != NULL)
        Commands->Add(CST_LEFT_MSG, LeftMsg);

    if (RightMsg != NULL)
        Commands->Add(CST_RIGHT_MSG, RightMsg);

    TTrace::SendToWinTraceClient(Commands, winTraceId.c_str());
    return this ;
}

//----------------------------------------------------------------------

/// <summary>
/// Resend the icon
/// </summary>
/// <param name="index">index of the icon. see CST_ICO_WARNING const</param>
/// <returns>itself</returns>

TraceNode * TraceNode::ResendIconIndex(const int index)
{
    if (enabled == false)
        return this;

    if (id == "")
        return this;

    CommandList * Commands ;                // Commands to send
    Commands = new CommandList() ;
    Commands->AddFront(CST_USE_NODE, id.c_str());   // param : guid
    Commands->Add(CST_ICO_INDEX, index);       // param : icon index
    TTrace::SendToWinTraceClient(Commands, winTraceId.c_str());
    return this ;
}
//----------------------------------------------------------------------
/// <summary>
/// Change Background Color (specific column) of a node
/// </summary>
/// <param name="color">new background color of the node</param>
/// <param name="colId">Column index : All columns=-1, Icon=0, Time=1, thread=2, left msg=3, right msg =4 or user defined column</param>
/// <returns>itself</returns>

TraceNode * TraceNode::SetBackgroundColor(const int color, const int colId)
{
    if (enabled == false)
        return this;

    if (id == "")
        return this ;

    char * StrColId ;
    StrColId = (char*)malloc(12) ;  // 11 + 1
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
    sprintf_s(StrColId, 12, "%11d", colId);                      //  11 + 1
#else
    sprintf(StrColId, "%11d", colId);                            //  11 + 1
#endif
    CommandList * Commands ;                                     // Commands to send
    Commands = new CommandList() ;
    Commands->AddFront(CST_USE_NODE, id.c_str());                // param : guid
    Commands->Add(CST_BACKGROUND_COLOR, color, StrColId);        // param : color, colId
    TTrace::SendToWinTraceClient(Commands, winTraceId.c_str());
    free(StrColId) ;
    return this;
}
//----------------------------------------------------------------------

/// <summary>
/// Change font detail for an item in the trace
/// </summary>
/// <param name="colId">Column index : All columns=-1, Icon=0, Time=1, thread=2, left msg=3, right msg =4 or user defined column</param>
/// <param name="bold">Change font to bold</param>
/// <param name="italic">Change font to Italic</param>
/// <param name="color">Change Color. To reduce the number assembly reference, the Color structure is not used. Use YourColor.ToArgb() instead. Use -1 to keep default color</param>
/// <param name="size">Change font size, use zero to keep normal size</param>
/// <param name="fontName">Change font name</param>
/// <returns>itself</returns>
TraceNode * TraceNode::SetFontDetail(const int colId, const bool bold, const bool italic, const int color, const int size, const wchar_t * fontName)
{
    char * strFontName = TTrace::WideToMbs(fontName) ;
    TraceNode * ret = SetFontDetail(colId, bold, italic, color, size, strFontName) ;

    if (strFontName != NULL)
        free(strFontName) ;

    return ret ;
}

//----------------------------------------------------------------------

/// <summary>
/// Change font detail for an item in the trace
/// </summary>
/// <param name="colId">Column index : All columns=-1, Icon=0, Time=1, thread=2, left msg=3, right msg =4 or user defined column</param>
/// <param name="bold">Change font to bold</param>
/// <param name="italic">Change font to Italic</param>
/// <param name="color">Change Color. To reduce the number assembly reference, the Color structure is not used. Use YourColor.ToArgb() instead. Use -1 to keep default color</param>
/// <param name="size">Change font size, use zero to keep normal size</param>
/// <param name="fontName">Change font name</param>
/// <returns>itself</returns>
TraceNode * TraceNode::SetFontDetail(const int colId, const bool bold, const bool italic, const int color, const int size, const char * fontName)
{
    if (enabled == false)
        return this;

    if (id == "")
        return this ;

    CommandList * Commands ;                                     // Commands to send
    Commands = new CommandList() ;
    Commands->AddFront(CST_USE_NODE, id.c_str());                // param : guid
    char * message ;
    int int_bold   = (bold) ? 1 : 0 ;
    int int_italic = (italic) ? 1 : 0;
    size_t MsgLen ;

    if (fontName == NULL) {
        message = (char*)malloc(32 + 1) ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
        sprintf_s(message, 33, "%5d%3d%1d%1d%11d%11d", CST_FONT_DETAIL, colId, int_bold, int_italic, color, size);
#else
        sprintf(message, "%5d%3d%1d%1d%11d%11d", CST_FONT_DETAIL, colId, int_bold, int_italic, color, size);
#endif
    } else {
        MsgLen = strlen(fontName) + 32 + 1 ;
        message = (char*)malloc(MsgLen) ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
        sprintf_s(message, MsgLen, "%5d%3d%1d%1d%11d%11d%s", CST_FONT_DETAIL, colId, int_bold, int_italic, color, size, fontName);
#else
        sprintf(message, "%5d%3d%1d%1d%11d%11d%s", CST_FONT_DETAIL, colId, int_bold, int_italic, color, size, fontName);
#endif
    }

    Commands->push_back(message) ;
    free(message) ;
    TTrace::SendToWinTraceClient(Commands, winTraceId.c_str());
    return this;
}

// TODO

//----------------------------------------------------------------------

/// <summary>
/// Set or reset the bookmark for the node
/// </summary>
/// <param name="Bookmarked">true/false</param>
/// <returns>The trace node</returns>
TraceNode * TraceNode::SetBookmark(const bool Bookmarked)
{
    if (enabled == false)
        return this;

    if (id == "")
        return this ;

    CommandList * Commands ;                                     // Commands to send
    Commands = new CommandList() ;
    Commands->AddFront(CST_USE_NODE, id.c_str());                // param : guid

    if (Bookmarked)
        Commands->Add(CST_SET_BOOKMARK, 1);
    else
        Commands->Add(CST_SET_BOOKMARK, 0);

    TTrace::SendToWinTraceClient(Commands, winTraceId.c_str());
    return this;
}

//----------------------------------------------------------------------

/// <summary>
/// set a node visible or invisible
/// </summary>
/// <param name="Visible">true/false</param>
/// <returns>The trace node</returns>
TraceNode * TraceNode::SetVisible(const bool Visible)
{
    if (enabled == false)
        return this;

    if (id == "")
        return this ;

    CommandList * Commands ;                                     // Commands to send
    Commands = new CommandList() ;
    Commands->AddFront(CST_USE_NODE, id.c_str());                // param : guid

    if (Visible)
        Commands->Add(CST_VISIBLE_NODE, 1) ;                 // param : visible flag
    else
        Commands->Add(CST_VISIBLE_NODE, 0) ;

    TTrace::SendToWinTraceClient(Commands, winTraceId.c_str());
    return this;
}

//----------------------------------------------------------------------

/// <summary>
/// Set focus to next sibling
/// </summary>
/// <returns>The trace node</returns>
TraceNode * TraceNode::GotoNextSibling()
{
    if (enabled == false)
        return this;

    if (id == "")
        return this ;

    CommandList * Commands ;                                     // Commands to send
    Commands = new CommandList() ;
    Commands->Add(CST_GOTO_NEXTSIBLING, id.c_str());                // param : guid
    TTrace::SendToWinTraceClient(Commands, winTraceId.c_str());
    return this;
}

//----------------------------------------------------------------------

/// <summary>
/// Set focus to previous sibling
/// </summary>
/// <returns>The trace node</returns>
TraceNode * TraceNode::GotoPrevSibling()
{
    if (enabled == false)
        return this;

    if (id == "")
        return this ;

    CommandList * Commands ;                                     // Commands to send
    Commands = new CommandList() ;
    Commands->Add(CST_GOTO_PREVSIBLING, id.c_str());                // param : guid
    TTrace::SendToWinTraceClient(Commands, winTraceId.c_str());
    return this;
}

//----------------------------------------------------------------------

/// <summary>
/// Set focus to first child
/// </summary>
/// <returns>The trace node</returns>
TraceNode * TraceNode::GotoFirstChild()
{
    if (enabled == false)
        return this;

    if (id == "")
        return this ;

    CommandList * Commands ;                                     // Commands to send
    Commands = new CommandList() ;
    Commands->Add(CST_GOTO_FIRST_CHILD, id.c_str());                // param : guid
    TTrace::SendToWinTraceClient(Commands, winTraceId.c_str());
    return this;
}

//----------------------------------------------------------------------

/// <summary>
/// Set focus to last child
/// </summary>
/// <returns>The trace node</returns>
TraceNode * TraceNode::GotoLastChild()
{
    if (enabled == false)
        return this;

    if (id == "")
        return this ;

    CommandList * Commands ;                                     // Commands to send
    Commands = new CommandList() ;
    Commands->Add(CST_GOTO_LAST_CHILD, id.c_str());                // param : guid
    TTrace::SendToWinTraceClient(Commands, winTraceId.c_str());
    return this;
}

//==========================================================================

// Close TTrace properly on exit. No memory leak are reported
class _TraceTool_Cleaner
{
public:
    ~_TraceTool_Cleaner(void)
    {
        TTrace::Stop() ;
    } ;
}  _traceTool_Cleaner ;

//==========================================================================



namespace cactus
{
void InitConsole()
{
    BOOL rc = ::AllocConsole();

    if (rc) {
        HANDLE hin = ::GetStdHandle(STD_INPUT_HANDLE);
        HANDLE hout = ::GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hout, 0x0a);
        int hCrt = _open_osfhandle((intptr_t)hin , _O_TEXT);

        if (hCrt != -1) {
            FILE * fpin = _fdopen(hCrt , "r");

            if (fpin) *stdin = *fpin ;
        }

        hCrt = _open_osfhandle((intptr_t)hout , _O_TEXT);

        if (hCrt != -1) {
            FILE * fpout = _fdopen(hCrt , "wt");

            if (fpout) *stdout = *fpout ;
        }

        hCrt = _open_osfhandle((intptr_t)::GetStdHandle(STD_ERROR_HANDLE), _O_TEXT);

        if (hCrt != -1) {
            FILE *hf = ::_fdopen(hCrt, "w");

            if (hf) *stderr = *hf;
        }

        std ::ios_base ::sync_with_stdio();      // 将iostream 流同c runtime lib 的stdio 同步，标准是同步的
        locale::global(locale(""));
        //setlocale(LC_CTYPE, "");    // MinGW gcc.
        wcout.imbue(locale(""));
    }
}
} // namespace cactus







