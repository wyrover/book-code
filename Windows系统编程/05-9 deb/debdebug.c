
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993-1997 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

// ************************************************************************
// MODULE    : DEBDebug.C
// PURPOSE   : Debug support functions for the Debug Event Browser
// FUNCTIONS :
//   DebugEventThread()         - debug event processing thread
// COMMENTS  :
//
// ************************************************************************
#define   STRICT               // enable strict typing
#include <Windows.H>           // required for all Windows applications
#include <StdDef.H>            // offsetof()

#include "LinkList.H"          // include the linked list functions
#include "DEBMisc.H"           // include the misc. support functions
#include "DEBDebug.H"          // include the DEB debugging functions

// internal global data
// ------------------------------------------------------------------------
HANDLE hHeap;                  // local heap
PLIST  pProcessList;           // pointer to the process list
BOOL   fFinished = FALSE;      // set to TRUE if the DebugEventThread
                               //  is no longer needed (such as the
                               //  debuggee failed to load or the number
                               //  of debuggee threads goes to zero
                               //  indicating debug session termination

static TCHAR szSourceFileName[] = TEXT(__FILE__);

// internal function prototypes
// ------------------------------------------------------------------------

//-- debug event handling functions
BOOL HandleExceptionEvent( LPDEBUG_EVENT );
BOOL HandleBreakPointException( LPDEBUG_EVENT );
BOOL HandleCreateThreadEvent( LPDEBUG_EVENT );
BOOL HandleCreateProcessEvent( LPDEBUG_EVENT );
BOOL HandleExitThreadEvent( LPDEBUG_EVENT );
BOOL HandleExitProcessEvent( LPDEBUG_EVENT );
BOOL HandleLoadDllEvent( LPDEBUG_EVENT );
BOOL HandleUnloadDllEvent( LPDEBUG_EVENT );
BOOL HandleOutputDebugStringEvent( LPDEBUG_EVENT );
BOOL HandleRipEvent( LPDEBUG_EVENT );
BOOL HandleUnknownEvent( LPDEBUG_EVENT );

//-- misc debug event helper functions
BOOL  DebugNewProcess( LPTSTR, LPTSTR );
BOOL  GetDllFileName( LPDEBUG_EVENT, LPTSTR, DWORD );
BOOL  GetDllFileNameFromList( LPDEBUG_EVENT, LPTSTR, DWORD );
BOOL  GetOutputDebugString( LPDEBUG_EVENT, LPTSTR, DWORD );
DWORD GetModuleFileNameFromHeader( HANDLE, HANDLE, DWORD, LPTSTR, DWORD );
#if defined(_MIPS_) || defined(_ALPHA_) || defined(_PPC_)
 BOOL  SkipBreakPoint( HANDLE );
#endif

//-- linked list wrapper functions

//-- process list and node specific linked list wrapper functions
int  ProcessOrderFunction( PNODE, PNODE );
BOOL CreateProcessList( PLIST* );
BOOL DestroyProcessList( PLIST );
BOOL AllocProcessNode( PNODE*, PDEB_PROCESS_NODE_INFO* );
BOOL InitProcessNodeInfo( PDEB_PROCESS_NODE_INFO*, LPDEBUG_EVENT );
BOOL InsertProcessNode( PLIST, PNODE );
BOOL SetCurrentProcessNode( PLIST, PNODE );
BOOL DeleteProcessNode( PLIST, PNODE );
BOOL FreeProcessNodeInfo( PNODE );
BOOL DestroyProcessNode( PNODE );
BOOL DeleteCurrentProcessNode( PLIST );

//-- thread list and node specific linked list wrapper functions
int  ThreadOrderFunction( PNODE, PNODE );
BOOL CreateThreadList( PLIST* );
BOOL DestroyThreadList( PLIST );
BOOL AllocThreadNode( PNODE*, PDEB_THREAD_NODE_INFO* );
BOOL InitThreadNodeInfo( PDEB_THREAD_NODE_INFO*, LPDEBUG_EVENT );
BOOL InsertThreadNode( PLIST, PNODE );
BOOL SetCurrentThreadNode( PLIST, PNODE );
BOOL DeleteThreadNode( PLIST, PNODE );
BOOL FreeThreadNodeInfo( PNODE );
BOOL DestroyThreadNode( PNODE );
BOOL DeleteCurrentThreadNode( PLIST );

//-- dll list and node specific linked list wrapper functions
int  DllOrderFunction( PNODE, PNODE );
BOOL CreateDllList( PLIST* );
BOOL DestroyDllList( PLIST );
BOOL AllocDllNode( PNODE*, PDEB_DLL_NODE_INFO* );
BOOL InitDllNodeInfo( PDEB_DLL_NODE_INFO*, LPDEBUG_EVENT );
BOOL InsertDllNode( PLIST, PNODE );
BOOL SetCurrentDllNode( PLIST, PNODE );
BOOL DeleteDllNode( PLIST, PNODE );
BOOL FreeDllNodeInfo( PNODE );
BOOL DestroyDllNode( PNODE );
BOOL DeleteCurrentDllNode( PLIST );


// ************************************************************************
// FUNCTION : DebugEventThread( PDEB_STARTUP_INFO )
// PURPOSE  : Main debug event processing loop
// COMMENTS :
//   A new debug event thread is created for each Debuggee process.
//   Return TRUE (non 0) if success, else FALSE (0)
// ************************************************************************
DWORD WINAPI
DebugEventThread( PDEB_STARTUP_INFO pDebStartupInfo )
{
  #define BUFFER_SIZE 256

  static BOOL  fFirstTime = TRUE;
  static TCHAR szDebuggeeTitle[128];

  DEBUG_EVENT  DebugEvent;

  LPTSTR lpszDebugEventBuffer;
  LPTSTR lpszTempBuffer;

  //-- set the minimum error level for debugging events
  SetDebugErrorLevel( Profile.DebugErrorLevel );

  if( fFirstTime ) {
    if( !LoadString( Global.hInstance, IDS_OFN_DEBUGGEE_TITLE, szDebuggeeTitle,
           sizeof(szDebuggeeTitle)/sizeof(TCHAR) ) )
      ErrorMessageBox( TEXT("LoadString()"),
        Global.szApiFailed, szSourceFileName, __LINE__ );
  }

  //-- determine if 'attach to' or 'open new' debuggee
  if( pDebStartupInfo->fActive ) {
    if( !DebugActiveProcess( pDebStartupInfo->dwProcessId ) )
      ErrorMessageBox( TEXT("DebugActiveProcess()"),
        Global.szApiFailed, szSourceFileName, __LINE__ );
  }
  else {
    if( !DebugNewProcess( pDebStartupInfo->lpstrPathName, szDebuggeeTitle ) )
    ExitThread( FALSE );
  }

  //-- increment active process count
  Global.dwActiveDebuggees++;

  //-- create a local heap
  {
    SYSTEM_INFO SysInfo;

    GetSystemInfo( &SysInfo );  // get the system memory page size
    hHeap = HeapCreate( (DWORD) NULL, SysInfo.dwPageSize, 1000*SysInfo.dwPageSize );
  }

  //-- create and initialize the process list
  CreateProcessList( &pProcessList );

  //-- alloc temporary (life of thread) string buffers
  lpszDebugEventBuffer = (LPTSTR) HeapAlloc( hHeap, (DWORD) NULL, BUFFER_SIZE );
  lpszTempBuffer = (LPTSTR) HeapAlloc( hHeap, (DWORD) NULL, BUFFER_SIZE );

  // ----------------------------------------------------------------------
  // begin debug event processing loop
  // ----------------------------------------------------------------------
  for(;;) {

    //-- wait for debug events
    if( !WaitForDebugEvent( &DebugEvent, INFINITE ) ) {
      ListBoxPrintF( pDebStartupInfo->hWndListBox, TEXT( "%s" ), TEXT( "Failed to attach to Debuggee..." ) );
      fFinished = TRUE;
      break;
    }

    // --------------------------------------------------------------------
    // display each debug event as it occurs and handle minimal debug
    // event processing
    // --------------------------------------------------------------------
    MakeCommonDebugEventString( lpszDebugEventBuffer, &DebugEvent );

    switch( DebugEvent.dwDebugEventCode ) {

      // ------------------------------------------------------------------
      // exception occured
      // ------------------------------------------------------------------
      case EXCEPTION_DEBUG_EVENT:

        //-- figure out which type of exception
        switch( DebugEvent.u.Exception.ExceptionRecord.ExceptionCode ) {

          //--standard exceptions
          case EXCEPTION_ACCESS_VIOLATION:
            StringAppendF( lpszDebugEventBuffer, TEXT( "%s%s" ),
              TEXT( "Exception: " ), TEXT( "Access Violation" ) );
            break;

          case EXCEPTION_DATATYPE_MISALIGNMENT:
            StringAppendF( lpszDebugEventBuffer, TEXT( "%s%s" ),
              TEXT( "Exception: " ), TEXT( "Datatype Misalignment" ) );
            break;

          case EXCEPTION_BREAKPOINT:
            StringAppendF( lpszDebugEventBuffer, TEXT( "%s%s" ),
              TEXT( "Exception: " ), TEXT( "Breakpoint" ) );
            break;

          case EXCEPTION_SINGLE_STEP:
            StringAppendF( lpszDebugEventBuffer, TEXT( "%s%s" ),
              TEXT( "Exception: " ), TEXT( "Single Step" ) );
            break;

          case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
            StringAppendF( lpszDebugEventBuffer, TEXT( "%s%s" ),
              TEXT( "Exception: " ), TEXT( "Array Bound Exceeded" ) );
            break;

          case EXCEPTION_FLT_DENORMAL_OPERAND:
            StringAppendF( lpszDebugEventBuffer, TEXT( "%s%s %s" ),
              TEXT( "Exception: " ), TEXT( "Floating Point" ),
              TEXT( "Denormal Operand" ) );
            break;

          case EXCEPTION_FLT_DIVIDE_BY_ZERO:
            StringAppendF( lpszDebugEventBuffer, TEXT( "%s%s %s" ),
              TEXT( "Exception: " ), TEXT( "Floating Point" ),
              TEXT( "Divide By Zero" ) );
            break;

          case EXCEPTION_FLT_INEXACT_RESULT:
            StringAppendF( lpszDebugEventBuffer, TEXT( "%s%s %s" ),
              TEXT( "Exception: " ), TEXT( "Floating Point" ),
              TEXT( "Inexact Result" ) );
            break;

          case EXCEPTION_FLT_INVALID_OPERATION:
            StringAppendF( lpszDebugEventBuffer, TEXT( "%s%s %s" ),
              TEXT( "Exception: " ), TEXT( "Floating Point" ),
              TEXT( "Invalid Operation" ) );
            break;

          case EXCEPTION_FLT_OVERFLOW:
            StringAppendF( lpszDebugEventBuffer, TEXT( "%s%s %s" ),
              TEXT( "Exception: " ), TEXT( "Floating Point" ),
              TEXT( "Overflow" ) );
            break;

          case EXCEPTION_FLT_STACK_CHECK:
            StringAppendF( lpszDebugEventBuffer, TEXT( "%s%s %s" ),
              TEXT( "Exception: " ), TEXT( "Floating Point" ),
              TEXT( "Stack Check" ) );
            break;

          case EXCEPTION_FLT_UNDERFLOW:
            StringAppendF( lpszDebugEventBuffer, TEXT( "%s%s %s" ),
              TEXT( "Exception: " ), TEXT( "Floating Point" ),
              TEXT( "Underflow" ) );
            break;

          case EXCEPTION_INT_DIVIDE_BY_ZERO:
            StringAppendF( lpszDebugEventBuffer, TEXT( "%s%s %s" ),
              TEXT( "Exception: " ), TEXT( "Integer" ),
              TEXT( "Divide By Zero" ) );
            break;

          case EXCEPTION_INT_OVERFLOW:
            StringAppendF( lpszDebugEventBuffer, TEXT( "%s%s %s" ),
              TEXT( "Exception: " ), TEXT( "Integer" ),
              TEXT( "Overflow" ) );
            break;

          case EXCEPTION_PRIV_INSTRUCTION:
            StringAppendF( lpszDebugEventBuffer, TEXT( "%s%s" ),
              TEXT( "Exception: " ), TEXT( "Privileged Instruction" ) );
            break;

          case EXCEPTION_IN_PAGE_ERROR:
            StringAppendF( lpszDebugEventBuffer, TEXT( "%s%s" ),
              TEXT( "Exception: " ), TEXT( "In Page Error" ) );
            break;

          //-- Debug exceptions
          case DBG_TERMINATE_THREAD:
            StringAppendF( lpszDebugEventBuffer, TEXT( "%s%s" ),
              TEXT( "Debug Exception: " ), TEXT( "Terminate Thread" ) );
            break;

          case DBG_TERMINATE_PROCESS:
            StringAppendF( lpszDebugEventBuffer, TEXT( "%s%s" ),
              TEXT( "Debug Exception: " ), TEXT( "Terminate Process" ) );
            break;

          case DBG_CONTROL_C:
            StringAppendF( lpszDebugEventBuffer, TEXT( "%s%s" ),
              TEXT( "Debug Exception: " ), TEXT( "Control+C" ) );
            break;

          case DBG_CONTROL_BREAK:
            StringAppendF( lpszDebugEventBuffer, TEXT( "%s%s" ),
              TEXT( "Debug Exception: " ), TEXT( "Control+Break" ) );
            break;

          //-- RPC exceptions (some)
          case RPC_S_UNKNOWN_IF:
            StringAppendF( lpszDebugEventBuffer, TEXT( "%s%s" ),
              TEXT( "RPC Exception: " ), TEXT( "Unknown Interface" ) );
            break;

          case RPC_S_SERVER_UNAVAILABLE:
            StringAppendF( lpszDebugEventBuffer, TEXT( "%s%s" ),
              TEXT( "RPC Exception: " ), TEXT( "Server Unavailable" ) );
            break;

          //-- VDM exceptions (minimal information)
          case EXCEPTION_VDM_EVENT:  // see DEBDebug.H for definition
            StringAppendF( lpszDebugEventBuffer, TEXT( "%s" ),
              TEXT( "VDM Exception: " ) );

          default:
            StringAppendF( lpszDebugEventBuffer, TEXT( "%s%s%X%s" ),
              TEXT( "Exception: " ), TEXT( "Unknown [0x" ),
               DebugEvent.u.Exception.ExceptionRecord.ExceptionCode,
               TEXT( "]" ) );
            break;

        }
        if( Profile.fVerbose ) {
          StringAppendF( lpszDebugEventBuffer, TEXT( "\n + %s%d" ),
            TEXT( "dwFirstChance: " ), DebugEvent.u.Exception.dwFirstChance );
        }
        else {
          if( DebugEvent.u.Exception.dwFirstChance != 0 )
            StringAppendF( lpszDebugEventBuffer, TEXT( "%s" ),
              TEXT( " - First Chance" ) );
          else
            StringAppendF( lpszDebugEventBuffer, TEXT( "%s" ),
              TEXT( " - Second Chance" ) );
        }
        HandleExceptionEvent( &DebugEvent );
        break;

      // ------------------------------------------------------------------
      // new thread started
      // ------------------------------------------------------------------
      case CREATE_THREAD_DEBUG_EVENT:
        StringAppendF( lpszDebugEventBuffer, TEXT( "%s" ),
          TEXT( "Create Thread: " ) );
        if( Profile.fVerbose ) {
          StringAppendF( lpszDebugEventBuffer, TEXT( "\n + %s%d\n + %s%X\n + %s%d" ),
            TEXT( "hThread:0x" ), DebugEvent.u.CreateThread.hThread,
            TEXT( "lpThreadLocalBase:0x" ), DebugEvent.u.CreateThread.lpThreadLocalBase,
            TEXT( "lpStartAddress:0x" ), DebugEvent.u.CreateThread.lpStartAddress );
        }
        HandleCreateThreadEvent( &DebugEvent );
        break;

      // ------------------------------------------------------------------
      // new process started
      // ------------------------------------------------------------------
      case CREATE_PROCESS_DEBUG_EVENT:
        StringAppendF( lpszDebugEventBuffer, TEXT( "%s" ),
          TEXT( "Create Process: " ) );
        if( Profile.fVerbose ) {
          StringAppendF( lpszDebugEventBuffer,
            TEXT( "\n + %s%X\n + %s%X\n + %s%X\n + %s%X\n + %s%d" )
            TEXT( "\n + %s%d\n + %s%X\n + %s%X\n + %s%X\n + %s%d" ),
            TEXT( "hFile:0x" ), DebugEvent.u.CreateProcessInfo.hFile,
            TEXT( "hProcess:0x" ), DebugEvent.u.CreateProcessInfo.hProcess,
            TEXT( "hThread:0x" ), DebugEvent.u.CreateProcessInfo.hThread,
            TEXT( "lpBaseOfImage:0x" ), DebugEvent.u.CreateProcessInfo.lpBaseOfImage,
            TEXT( "dwDebugInfoFileOffset: " ), DebugEvent.u.CreateProcessInfo.dwDebugInfoFileOffset,
            TEXT( "nDebugInfoSize: " ), DebugEvent.u.CreateProcessInfo.nDebugInfoSize,
            TEXT( "lpThreadLocalBase:0x" ), DebugEvent.u.CreateProcessInfo.lpThreadLocalBase,
            TEXT( "lpStartAddress:0x" ), DebugEvent.u.CreateProcessInfo.lpStartAddress,
            TEXT( "lpImageName:0x" ), DebugEvent.u.CreateProcessInfo.lpImageName,
            TEXT( "fUnicode: " ), DebugEvent.u.CreateProcessInfo.fUnicode );
        }
        HandleCreateProcessEvent( &DebugEvent );
        break;

      // ------------------------------------------------------------------
      // existing thread terminated
      // ------------------------------------------------------------------
      case EXIT_THREAD_DEBUG_EVENT:
        StringAppendF( lpszDebugEventBuffer, TEXT( "%s" ),
          TEXT( "Exit Thread: " ) );
        if( Profile.fVerbose ) {
          StringAppendF( lpszDebugEventBuffer, TEXT( "\n + %s%d" ),
            TEXT( "dwExitCode: " ), DebugEvent.u.ExitThread.dwExitCode );
        }
        else {
          StringAppendF( lpszDebugEventBuffer, TEXT( "%s%d" ),
            TEXT( "Returned " ), DebugEvent.u.ExitThread.dwExitCode );
        }
        HandleExitThreadEvent( &DebugEvent );
        break;

      // ------------------------------------------------------------------
      // existing process terminated
      // ------------------------------------------------------------------
      case EXIT_PROCESS_DEBUG_EVENT:
        StringAppendF( lpszDebugEventBuffer, TEXT( "%s" ),
          TEXT( "Exit Process: " ) );
        if( Profile.fVerbose ) {
          StringAppendF( lpszDebugEventBuffer, TEXT( "\n + %s%d" ),
            TEXT( "dwExitCode: " ), DebugEvent.u.ExitProcess.dwExitCode );
        }
        else {
          StringAppendF( lpszDebugEventBuffer, TEXT( "%s%d" ),
            TEXT( "Returned " ), DebugEvent.u.ExitProcess.dwExitCode );
        }
        HandleExitProcessEvent( &DebugEvent );
        break;

      // ------------------------------------------------------------------
      // new DLL loaded
      // ------------------------------------------------------------------
      case LOAD_DLL_DEBUG_EVENT:
        StringAppendF( lpszDebugEventBuffer, TEXT( "%s" ),
          TEXT( "Load DLL: " ) );
        lstrcpy( lpszTempBuffer, TEXT("Empty!") );
        GetDllFileName( &DebugEvent, lpszTempBuffer, BUFFER_SIZE );
        StringAppendF( lpszDebugEventBuffer, TEXT( "%s" ), lpszTempBuffer );
        if( Profile.fVerbose ) {
          StringAppendF( lpszDebugEventBuffer,
            TEXT( "\n + %s%X\n + %s%X\n + %s%d\n + %s%d\n + %s%X\n + %s%d" ),
            TEXT( "hFile:0x" ), DebugEvent.u.LoadDll.hFile,
            TEXT( "lpBaseOfDll:0x" ), DebugEvent.u.LoadDll.lpBaseOfDll,
            TEXT( "dwDebugInfoFileOffset: " ), DebugEvent.u.LoadDll.dwDebugInfoFileOffset,
            TEXT( "nDebugInfoSize: " ), DebugEvent.u.LoadDll.nDebugInfoSize,
            TEXT( "lpImageName:0x" ), DebugEvent.u.LoadDll.lpImageName,
            TEXT( "fUnicode: " ), DebugEvent.u.LoadDll.fUnicode );
        }
        HandleLoadDllEvent( &DebugEvent );
        break;

      // ------------------------------------------------------------------
      // existing DLL explicitly unloaded
      // ------------------------------------------------------------------
      case UNLOAD_DLL_DEBUG_EVENT:
        StringAppendF( lpszDebugEventBuffer, TEXT( "%s" ), TEXT( "Unload DLL: " ) );
        GetDllFileNameFromList( &DebugEvent, lpszTempBuffer, BUFFER_SIZE );
        StringAppendF( lpszDebugEventBuffer, TEXT( "%s" ), lpszTempBuffer );
        if( Profile.fVerbose ) {
          StringAppendF( lpszDebugEventBuffer, TEXT( "\n + %s%X" ),
            TEXT( "lpBaseOfDLL:0x" ), DebugEvent.u.UnloadDll.lpBaseOfDll );
        }
        HandleUnloadDllEvent( &DebugEvent );
        break;

      // ------------------------------------------------------------------
      // OutputDebugString() occured
      // ------------------------------------------------------------------
      case OUTPUT_DEBUG_STRING_EVENT:
        StringAppendF( lpszDebugEventBuffer, TEXT( "%s" ),
          TEXT( "Output Debug String: " ) );
        GetOutputDebugString( &DebugEvent, lpszTempBuffer, BUFFER_SIZE );
        StringAppendF( lpszDebugEventBuffer, TEXT( "%s" ), lpszTempBuffer );
        if( Profile.fVerbose ) {
          StringAppendF( lpszDebugEventBuffer, TEXT( "\n + %s%X\n + %s%d\n + %s%d" ),
            TEXT( "lpDebugStringData:0x" ), DebugEvent.u.DebugString.lpDebugStringData,
            TEXT( "fUnicode: " ), DebugEvent.u.DebugString.fUnicode,
            TEXT( "nDebugStringLength: " ), DebugEvent.u.DebugString.nDebugStringLength );
        }
        HandleOutputDebugStringEvent( &DebugEvent );
        break;

      // ------------------------------------------------------------------
      // RIP occured
      // ------------------------------------------------------------------
      case RIP_EVENT:
        StringAppendF( lpszDebugEventBuffer, TEXT( "%s" ),
          TEXT( "RIP" ) );
        if( Profile.fVerbose ) {
          StringAppendF( lpszDebugEventBuffer, TEXT( "\n + %s%d\n + %s%d" ),
            TEXT( "dwError: " ), DebugEvent.u.RipInfo.dwError,
            TEXT( "dwType: " ), DebugEvent.u.RipInfo.dwType );
        }
        HandleRipEvent( &DebugEvent );
        break;

      // ------------------------------------------------------------------
      // unknown debug event occured
      // ------------------------------------------------------------------
      default:
        StringAppendF( lpszDebugEventBuffer, TEXT( "%s%X%s" ),
          TEXT( "Debug Event:Unknown [0x" ),
          DebugEvent.dwDebugEventCode, lpszTempBuffer,
          TEXT( "]" ) );
        HandleUnknownEvent( &DebugEvent );
        break;
    }

    //-- insert the debug event string into the listbox
    ListBoxPrintF( pDebStartupInfo->hWndListBox, TEXT( "%s" ), lpszDebugEventBuffer );

    //-- default action, just continue
    if( fFinished ) {
      fFinished = FALSE;   // reset the value
      break;
    }
    else
      ContinueDebugEvent( DebugEvent.dwProcessId, DebugEvent.dwThreadId,
        DBG_CONTINUE );
  }

  //-- free temporary (life of thread) string buffers
  HeapFree( hHeap, (DWORD) NULL, (PVOID) lpszDebugEventBuffer );
  HeapFree( hHeap, (DWORD) NULL, (PVOID) lpszTempBuffer );

  //-- free list
  DestroyProcessList( pProcessList );

  //-- free the heap
  HeapDestroy( hHeap );

  //-- decrement active process count
  Global.dwActiveDebuggees--;
  ExitThread( TRUE );

  return( TRUE );  // avoid the "no return value" warning
}


// ========================================================================
// debug event handling functions
// ========================================================================


// ************************************************************************
// FUNCTION : HandleExceptionEvent( LPDEBUG_EVENT lpDebugEvent )
// PURPOSE  : handle EXCEPTION_DEBUG_EVENT
// COMMENTS : except for the BreakPoint event, continue and let the
//            application or system exception handlers to the work
// ************************************************************************
BOOL
HandleExceptionEvent( LPDEBUG_EVENT lpDebugEvent )
{
  switch( lpDebugEvent->u.Exception.ExceptionRecord.ExceptionCode ) {

    case EXCEPTION_BREAKPOINT:
      HandleBreakPointException( lpDebugEvent );
      break;

    default:
      ContinueDebugEvent( lpDebugEvent->dwProcessId, lpDebugEvent->dwThreadId,
        DBG_EXCEPTION_NOT_HANDLED );

  }

  return( TRUE );
}


// ************************************************************************
// FUNCTION : HandleBreakPointException( LPDEBUG_EVENT lpDebugEvent )
// PURPOSE  : handle the BREAKPOINT exception
// COMMENTS : search process list, search thread list, skip over breakpoint
// ************************************************************************
BOOL
HandleBreakPointException( LPDEBUG_EVENT lpDebugEvent )
{
#if defined(_MIPS_) || defined(_ALPHA_) || defined(_PPC_)
  PNODE                  pProcessNode, pSearchProcessNode;
  PNODE                  pThreadNode, pSearchThreadNode;
  PDEB_PROCESS_NODE_INFO pProcessNodeInfo, pSearchProcessNodeInfo;
  PDEB_THREAD_NODE_INFO  pThreadNodeInfo, pSearchThreadNodeInfo;
  PDEB_THREAD_LIST_INFO  pThreadListInfo;

  AllocProcessNode( &pSearchProcessNode, &pSearchProcessNodeInfo );
  pSearchProcessNodeInfo->dwProcessId = lpDebugEvent->dwProcessId;
  SetCurrentProcessNode( pProcessList, pSearchProcessNode );
  GetCurrentNode( pProcessList, &pProcessNode );
  pProcessNodeInfo = (PDEB_PROCESS_NODE_INFO) pProcessNode->pNodeData;
  pThreadListInfo = (PDEB_THREAD_LIST_INFO) pProcessNodeInfo->pThreadList->pListData;
  //-- if no thread nodes then hThread is stored in the process node
  if( !pThreadListInfo->dwActiveThreads ) {
    SkipBreakPoint( (pProcessNodeInfo->ProcessDebugInfo).hThread );
  }
  else {
    AllocThreadNode( &pSearchThreadNode, &pSearchThreadNodeInfo );
    pSearchThreadNodeInfo->dwThreadId = lpDebugEvent->dwThreadId;
    SetCurrentThreadNode( pProcessNodeInfo->pThreadList, pSearchThreadNode );
    GetCurrentNode( pProcessNodeInfo->pThreadList, &pThreadNode );
    pThreadNodeInfo = (PDEB_THREAD_NODE_INFO) pThreadNode->pNodeData;
    SkipBreakPoint( (pThreadNodeInfo->ThreadDebugInfo).hThread );
    DestroyThreadNode( pSearchThreadNode );
  }
  DestroyProcessNode( pSearchProcessNode );
 #else
  ContinueDebugEvent( lpDebugEvent->dwProcessId, lpDebugEvent->dwThreadId,
    DBG_CONTINUE );
 #endif

  return( TRUE );
}


// ************************************************************************
// FUNCTION : HandleCreateThreadEvent( LPDEBUG_EVENT )
// PURPOSE  : handle CREATE_THREAD_DEBUG_EVENT
// COMMENTS : search process list, insert new thread node
// ************************************************************************
BOOL
HandleCreateThreadEvent( LPDEBUG_EVENT lpDebugEvent )
{
  PNODE                  pProcessNode, pSearchProcessNode;
  PNODE                  pThreadNode;
  PDEB_PROCESS_NODE_INFO pProcessNodeInfo, pSearchProcessNodeInfo;
  PDEB_THREAD_NODE_INFO  pThreadNodeInfo;

  AllocProcessNode( &pSearchProcessNode, &pSearchProcessNodeInfo );
  pSearchProcessNodeInfo->dwProcessId = lpDebugEvent->dwProcessId;
  SetCurrentProcessNode( pProcessList, pSearchProcessNode );
  GetCurrentNode( pProcessList, &pProcessNode );
  pProcessNodeInfo = (PDEB_PROCESS_NODE_INFO) pProcessNode->pNodeData;
  AllocThreadNode( &pThreadNode, &pThreadNodeInfo );
  InitThreadNodeInfo( &pThreadNodeInfo, lpDebugEvent );
  InsertThreadNode( pProcessNodeInfo->pThreadList, pThreadNode );
  DestroyProcessNode( pSearchProcessNode );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : HandleCreateProcessEvent( LPDEBUG_EVENT )
// PURPOSE  : handle CREATE_PROCESS_DEBUG_EVENT
// COMMENTS : insert new process node
// ************************************************************************
BOOL
HandleCreateProcessEvent( LPDEBUG_EVENT lpDebugEvent )
{
  PNODE                  pProcessNode;
  PDEB_PROCESS_NODE_INFO pProcessNodeInfo;

  AllocProcessNode( &pProcessNode, &pProcessNodeInfo );
  InitProcessNodeInfo( &pProcessNodeInfo, lpDebugEvent );
  InsertProcessNode( pProcessList, pProcessNode );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : HandleExitThreadEvent( LPDEBUG_EVENT )
// PURPOSE  : handle EXIT_THREAD_DEBUG_EVENT
// COMMENTS : search process list, search thread list, delete existing
//            thread node
// ************************************************************************
BOOL
HandleExitThreadEvent( LPDEBUG_EVENT lpDebugEvent )
{
  PNODE                  pProcessNode, pSearchProcessNode;
  PNODE                  pSearchThreadNode;
  PDEB_PROCESS_NODE_INFO pProcessNodeInfo, pSearchProcessNodeInfo;
  PDEB_THREAD_NODE_INFO  pSearchThreadNodeInfo;

  AllocProcessNode( &pSearchProcessNode, &pSearchProcessNodeInfo );
  pSearchProcessNodeInfo->dwProcessId = lpDebugEvent->dwProcessId;
  SetCurrentProcessNode( pProcessList, pSearchProcessNode );
  GetCurrentNode( pProcessList, &pProcessNode );
  pProcessNodeInfo = (PDEB_PROCESS_NODE_INFO) pProcessNode->pNodeData;
  AllocThreadNode( &pSearchThreadNode, &pSearchThreadNodeInfo );
  pSearchThreadNodeInfo->dwThreadId = lpDebugEvent->dwThreadId;
  DeleteThreadNode( pProcessNodeInfo->pThreadList, pSearchThreadNode );
  DestroyThreadNode( pSearchThreadNode );
  DestroyProcessNode( pSearchProcessNode );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : HandleExitProcessEvent( LPDEBUG_EVENT )
// PURPOSE  : handle EXIT_PROCESS_DEBUG_EVENT
// COMMENTS : search process list, delete existing process node,
// ************************************************************************
BOOL
HandleExitProcessEvent( LPDEBUG_EVENT lpDebugEvent )
{
  PNODE                  pSearchProcessNode;
  PDEB_PROCESS_NODE_INFO pSearchProcessNodeInfo;
  PDEB_PROCESS_LIST_INFO pProcessListInfo;

  AllocProcessNode( &pSearchProcessNode, &pSearchProcessNodeInfo );
  pSearchProcessNodeInfo->dwProcessId = lpDebugEvent->dwProcessId;
  DeleteProcessNode( pProcessList, pSearchProcessNode );
  //-- if last process? free all temporary memory, exit thread
  pProcessListInfo = (PDEB_PROCESS_LIST_INFO) pProcessList->pListData;
  if( !pProcessListInfo->dwActiveProcesses )
    fFinished = TRUE;
  DestroyProcessNode( pSearchProcessNode );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : HandleLoadDllEvent( LPDEBUG_EVENT )
// PURPOSE  : handle LOAD_DLL_DEBUG_EVENT
// COMMENTS : search process list, insert new DLL node
// ************************************************************************
BOOL
HandleLoadDllEvent( LPDEBUG_EVENT lpDebugEvent )
{
  PNODE                  pProcessNode, pSearchProcessNode;
  PNODE                  pDllNode;
  PDEB_PROCESS_NODE_INFO pProcessNodeInfo, pSearchProcessNodeInfo;
  PDEB_DLL_NODE_INFO     pDllNodeInfo;

  AllocProcessNode( &pSearchProcessNode, &pSearchProcessNodeInfo );
  pSearchProcessNodeInfo->dwProcessId = lpDebugEvent->dwProcessId;
  SetCurrentProcessNode( pProcessList, pSearchProcessNode );
  GetCurrentNode( pProcessList, &pProcessNode );
  pProcessNodeInfo = (PDEB_PROCESS_NODE_INFO) pProcessNode->pNodeData;
  AllocDllNode( &pDllNode, &pDllNodeInfo );
  InitDllNodeInfo( &pDllNodeInfo, lpDebugEvent );
  InsertDllNode( pProcessNodeInfo->pDllList, pDllNode );
  DestroyProcessNode( pSearchProcessNode );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : HandleUnloadDllEvent( LPDEBUG_EVENT )
// PURPOSE  : handle UNLOAD_DLL_DEBUG_EVENT
// COMMENTS : search process list, search DLL list, delete existing DLL
//            node
// ************************************************************************
BOOL
HandleUnloadDllEvent( LPDEBUG_EVENT lpDebugEvent )
{
  PNODE                  pProcessNode, pSearchProcessNode;
  PNODE                  pSearchDllNode;
  PDEB_PROCESS_NODE_INFO pProcessNodeInfo, pSearchProcessNodeInfo;
  PDEB_DLL_NODE_INFO     pSearchDllNodeInfo;

  AllocProcessNode( &pSearchProcessNode, &pSearchProcessNodeInfo );
  pSearchProcessNodeInfo->dwProcessId = lpDebugEvent->dwProcessId;
  SetCurrentProcessNode( pProcessList, pSearchProcessNode );
  GetCurrentNode( pProcessList, &pProcessNode );
  pProcessNodeInfo = (PDEB_PROCESS_NODE_INFO) pProcessNode->pNodeData;
  AllocDllNode( &pSearchDllNode, &pSearchDllNodeInfo );
  pSearchDllNodeInfo->DllDebugInfo.lpBaseOfDll = lpDebugEvent->u.UnloadDll.lpBaseOfDll;
  DeleteDllNode( pProcessNodeInfo->pDllList, pSearchDllNode );
  DestroyDllNode( pSearchDllNode );
  DestroyProcessNode( pSearchProcessNode );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : HandleOutputDebugStringEvent( LPDEBUG_EVENT )
// PURPOSE  : handle OUTPUT_DEBUG_STRING_EVENT
// COMMENTS : do nothing
// ************************************************************************
BOOL
HandleOutputDebugStringEvent( LPDEBUG_EVENT lpDebugEvent )
{
  return( TRUE );
}


// ************************************************************************
// FUNCTION : HandleRipEvent( LPDEBUG_EVENT )
// PURPOSE  : handle RIP_EVENT
// COMMENTS : do nothing
// ************************************************************************
BOOL
HandleRipEvent( LPDEBUG_EVENT lpDebugEvent )
{
  return( TRUE );
}


// ************************************************************************
// FUNCTION : HandleUnknownEvent( LPDEBUG_EVENT )
// PURPOSE  : handle all unknown debug events
// COMMENTS : do nothing
// ************************************************************************
BOOL
HandleUnknownEvent( LPDEBUG_EVENT lpDebugEvent )
{
  return( TRUE );
}


// ========================================================================
// misc debug event helper functions
// ========================================================================


// ************************************************************************
// FUNCTION : DebugNewProcess( LPTSTR, LPTSTR )
// PURPOSE  : starts a new process as a debuggee
// COMMENTS :
// ************************************************************************
BOOL
DebugNewProcess( LPTSTR lpszFileName, LPTSTR lpszTitle )
{
  static STARTUPINFO           StartupInfo;
  static LPSTARTUPINFO         lpStartupInfo = &StartupInfo;
  static PROCESS_INFORMATION   ProcessInfo;
  static LPPROCESS_INFORMATION lpProcessInfo = &ProcessInfo;

  lpStartupInfo->cb          = sizeof( STARTUPINFO );
  lpStartupInfo->lpDesktop   = NULL;
  lpStartupInfo->lpTitle     = lpszTitle;
  lpStartupInfo->dwX         = 0;
  lpStartupInfo->dwY         = 0;
  lpStartupInfo->dwXSize     = 0;
  lpStartupInfo->dwYSize     = 0;
  lpStartupInfo->dwFlags     = (DWORD) NULL;
  lpStartupInfo->wShowWindow = SW_SHOWDEFAULT;

  lpProcessInfo->hProcess = NULL;

  //-- create the Debuggee process instead
  if( !CreateProcess(
         NULL,
         lpszFileName,
         (LPSECURITY_ATTRIBUTES) NULL,
         (LPSECURITY_ATTRIBUTES) NULL,
         TRUE,
         Profile.DebugMode | Profile.DebuggeePriority | CREATE_NEW_CONSOLE,
         (LPVOID) NULL,
         (LPTSTR) NULL,
         lpStartupInfo, lpProcessInfo ) ) {

    switch( GetLastError() ) {

      case ERROR_FILE_NOT_FOUND:
        MessageBox( GetDesktopWindow(), TEXT( "This file does not exist." ),
          TEXT( "Open File Error" ), MB_OK | MB_APPLMODAL | MB_SETFOREGROUND );
        break;
      case ERROR_ACCESS_DENIED:
        MessageBox( GetDesktopWindow(), TEXT( "Access denied." ),
          TEXT( "Open File Error" ), MB_OK | MB_APPLMODAL | MB_SETFOREGROUND );
        break;
      case ERROR_FILE_INVALID:
        MessageBox( GetDesktopWindow(), TEXT( "Invalid file." ),
          TEXT( "Open File Error" ), MB_OK | MB_APPLMODAL | MB_SETFOREGROUND );
        break;
      case ERROR_FILE_CORRUPT:
        MessageBox( GetDesktopWindow(), TEXT( "The file is corrupt." ),
          TEXT( "Open File Error" ), MB_OK | MB_APPLMODAL | MB_SETFOREGROUND );
        break;
      case ERROR_BAD_EXE_FORMAT:
        MessageBox( GetDesktopWindow(), TEXT( "The file has a bad format." ),
          TEXT( "Open File Error" ), MB_OK | MB_APPLMODAL | MB_SETFOREGROUND );
        break;
      default:
        ErrorMessageBox( TEXT( "CreateProcess()" ),
          Global.szApiFailed, szSourceFileName, __LINE__ );
        break;

    }
    return( FALSE );

  }
  else {
    CloseHandle( ProcessInfo.hProcess );
    CloseHandle( ProcessInfo.hThread );
  }

  return( TRUE
 );
}


// ************************************************************************
// FUNCTION : GetDllFileName( LPDEBUG_EVENT, LPTSTR, DWORD )
// PURPOSE  : get DLL filename when LOAD_DLL_DEBUG_EVENT occurs
// COMMENTS : search process list, get DLL name from header
// ************************************************************************
BOOL
GetDllFileName( LPDEBUG_EVENT lpDebugEvent, LPTSTR lpszBuffer,
  DWORD cchBuffer )
{
  PNODE                  pProcessNode, pSearchProcessNode;
  PDEB_PROCESS_NODE_INFO pProcessNodeInfo, pSearchProcessNodeInfo;

  AllocProcessNode( &pSearchProcessNode, &pSearchProcessNodeInfo );
  pSearchProcessNodeInfo->dwProcessId = lpDebugEvent->dwProcessId;
  SetCurrentProcessNode( pProcessList, pSearchProcessNode );
  GetCurrentNode( pProcessList, &pProcessNode );
  pProcessNodeInfo = (PDEB_PROCESS_NODE_INFO) pProcessNode->pNodeData;
  GetModuleFileNameFromHeader(
    pProcessNodeInfo->ProcessDebugInfo.hProcess,
    lpDebugEvent->u.LoadDll.hFile,
    (DWORD) lpDebugEvent->u.LoadDll.lpBaseOfDll,
    lpszBuffer, cchBuffer);
  DestroyProcessNode( pSearchProcessNode );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : GetDllFileNameFromList( LPDEBUG_EVENT, LPTSTR, DWORD )
// PURPOSE  : get DLL filename when UNLOAD_DLL_DEBUG_EVENT occurs
// COMMENTS : search process list, search DLL list, get DLL name
// ************************************************************************
BOOL
GetDllFileNameFromList( LPDEBUG_EVENT lpDebugEvent, LPTSTR lpszBuffer,
  DWORD cchBuffer )
{
  PNODE                  pProcessNode, pSearchProcessNode;
  PNODE                  pDllNode, pSearchDllNode;
  PDEB_PROCESS_NODE_INFO pProcessNodeInfo, pSearchProcessNodeInfo;
  PDEB_DLL_NODE_INFO     pDllNodeInfo, pSearchDllNodeInfo;

  UNREFERENCED_PARAMETER( cchBuffer );

  AllocProcessNode( &pSearchProcessNode, &pSearchProcessNodeInfo );
  pSearchProcessNodeInfo->dwProcessId = lpDebugEvent->dwProcessId;
  SetCurrentProcessNode( pProcessList, pSearchProcessNode );
  GetCurrentNode( pProcessList, &pProcessNode );
  pProcessNodeInfo = (PDEB_PROCESS_NODE_INFO) pProcessNode->pNodeData;
  AllocDllNode( &pSearchDllNode, &pSearchDllNodeInfo );
  pSearchDllNodeInfo->DllDebugInfo.lpBaseOfDll = lpDebugEvent->u.UnloadDll.lpBaseOfDll;
  SetCurrentDllNode( pProcessNodeInfo->pDllList, pSearchDllNode );
  GetCurrentNode( pProcessNodeInfo->pDllList, &pDllNode );
  pDllNodeInfo = (PDEB_DLL_NODE_INFO) pDllNode->pNodeData;
  lstrcpy( lpszBuffer, pDllNodeInfo->lpstrFileName );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : GetOutputDebugString( LPDEBUG_EVENT, LPTSTR, DWORD )
// PURPOSE  : get the output debug string from the debuggee when
//            OUTPUT_DEBUG_STRING_EVENT occurs
// COMMENTS : search process list, read the string from the debuggee
// ************************************************************************
BOOL
GetOutputDebugString( LPDEBUG_EVENT lpDebugEvent, LPTSTR lpszBuffer,
  DWORD cchBuffer )
{
  PNODE                  pProcessNode, pSearchProcessNode;
  PDEB_PROCESS_NODE_INFO pProcessNodeInfo, pSearchProcessNodeInfo;
  DWORD dwNumberOfBytesRead;

  UNREFERENCED_PARAMETER( cchBuffer );

  AllocProcessNode( &pSearchProcessNode, &pSearchProcessNodeInfo );
  pSearchProcessNodeInfo->dwProcessId = lpDebugEvent->dwProcessId;
  SetCurrentProcessNode( pProcessList, pSearchProcessNode );
  GetCurrentNode( pProcessList, &pProcessNode );
  pProcessNode = (PNODE) pProcessList->pCurrentNode;
  pProcessNodeInfo = (PDEB_PROCESS_NODE_INFO) pProcessNode->pNodeData;
  ReadProcessMemory(
    pProcessNodeInfo->ProcessDebugInfo.hProcess,
    lpDebugEvent->u.DebugString.lpDebugStringData,
    lpszBuffer, lpDebugEvent->u.DebugString.nDebugStringLength,
    &dwNumberOfBytesRead );
  DestroyProcessNode( pSearchProcessNode );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : GetModuleFileNameFromHeader( HANDLE, HANDLE, DWORD, LPTSTR, DWORD )
// PURPOSE  : Retrieves the DLL module name for a given file handle of a
//            the module.  Reads the module name from the EXE header.
// COMMENTS :
//   Retrieves only the module name and not the pathname.  Returns the
//   number of characters copies to the buffer, else returns 0.
// ************************************************************************
DWORD
GetModuleFileNameFromHeader( HANDLE hProcess, HANDLE hFile, DWORD BaseOfDll,
  LPTSTR lpszPath, DWORD cchPath )
{
  #define IMAGE_SECOND_HEADER_OFFSET    (15 * sizeof(ULONG)) // relative to file beginning
  #define IMAGE_BASE_OFFSET             (13 * sizeof(DWORD)) // relative to PE header base
  #define IMAGE_EXPORT_TABLE_RVA_OFFSET (30 * sizeof(DWORD)) // relative to PE header base
  #define IMAGE_NAME_RVA_OFFSET         offsetof(IMAGE_EXPORT_DIRECTORY, Name)

  WORD   DosSignature;
  DWORD  NtSignature;
  DWORD  dwNumberOfBytesRead = 0;
  DWORD  PeHeader, ImageBase, ExportTableRVA, NameRVA;

  //-- verify that the handle is not NULL
  if( !hFile ) {
    lstrcpy( lpszPath, TEXT("Invalid File Handle") );
    return( 0 );
  }

  //-- verify that the handle is for a disk file
  if( GetFileType(hFile) != FILE_TYPE_DISK ) {
    lstrcpy( lpszPath, TEXT("Invalid File Type") );
    return( 0 );
  }

  //-- Extract the filename from the EXE header
  SetFilePointer( hFile, 0L, NULL, FILE_BEGIN );
  ReadFile( hFile, &DosSignature, sizeof(DosSignature), &dwNumberOfBytesRead,
    (LPOVERLAPPED) NULL);

  //-- verify DOS signature found
  if( DosSignature != IMAGE_DOS_SIGNATURE ) {
    wsprintf( lpszPath, TEXT( "Bad MZ Signature: 0x%x" ), DosSignature );
    return( 0 );
  }

  SetFilePointer( hFile, IMAGE_SECOND_HEADER_OFFSET, (LPLONG) NULL,
    FILE_BEGIN );
  ReadFile( hFile, &PeHeader, sizeof(PeHeader), &dwNumberOfBytesRead,
    (LPOVERLAPPED) NULL );
  SetFilePointer( hFile, PeHeader, (LPLONG) NULL, FILE_BEGIN );
  ReadFile( hFile, &NtSignature, sizeof(NtSignature), &dwNumberOfBytesRead,
    (LPOVERLAPPED) NULL);

  //-- verify Windows NT (PE) signature found
  if( NtSignature != IMAGE_NT_SIGNATURE ) {
    wsprintf( lpszPath, TEXT( "Bad PE Signature: 0x%x" ), DosSignature );
    return( 0 );
  }

  SetFilePointer( hFile, PeHeader + IMAGE_BASE_OFFSET, (LPLONG) NULL,
    FILE_BEGIN );
  ReadFile( hFile, &ImageBase, sizeof(ImageBase), &dwNumberOfBytesRead,
    (LPOVERLAPPED) NULL);
  SetFilePointer( hFile, PeHeader + IMAGE_EXPORT_TABLE_RVA_OFFSET,
    (LPLONG) NULL, FILE_BEGIN );
  ReadFile( hFile, &ExportTableRVA, sizeof(ExportTableRVA),
    &dwNumberOfBytesRead, (LPOVERLAPPED) NULL);

  //-- now read from the virtual address space in the process
  ReadProcessMemory( hProcess,
     (LPVOID) (BaseOfDll + ExportTableRVA + IMAGE_NAME_RVA_OFFSET),
     &NameRVA, sizeof(NameRVA), &dwNumberOfBytesRead );
  lstrcpy( lpszPath, TEXT("Empty!") );
  if( !ReadProcessMemory( hProcess,
         (LPVOID) (BaseOfDll + NameRVA),
         lpszPath, cchPath, &dwNumberOfBytesRead ) )
     lstrcpy( lpszPath, TEXT("Access Denied!") );

  return( dwNumberOfBytesRead );
}


#if defined(_MIPS_) || defined(_ALPHA_) || defined(_PPC_)
// ************************************************************************
// FUNCTION : SkipThreadBreakPoint( HANDLE );
// PURPOSE  : Skip over the break point instruction belonging to
//            hThread.
// COMMENTS :
//   Only the MIPS R4x00 and DEC Alpha AXP require this.
// ************************************************************************
BOOL
SkipBreakPoint( HANDLE hThread )
{
  static CONTEXT Context;

  Context.ContextFlags = CONTEXT_CONTROL;
  if( !GetThreadContext( hThread, &Context ) )
    return( FALSE );
#if defined(_PPC_)
  Context.Iar += 4L;  // Iar is the PC (program counter)
#else
  Context.Fir += 4L;  // Fir is the PC (program counter)
                      // BREAK (breakpoint instruction) occupies 4 bytes
#endif

  // -----------------------------------------------------------------------
  //  Below would be equivalent for the Intel 80x86 (if it were necessary)
  //  however the Intel x86 automatically increments ip past the 'int 3':
  //  Context.Eip += 1L;  // Eip is the PC (program counter)
  //			  // int 3 (breakpoint instruction) occupies 1 byte
  // -----------------------------------------------------------------------

  if( !SetThreadContext( hThread, &Context ) )
    return( FALSE );

  return( TRUE );
}
#endif



// ========================================================================
// wrapper functions to the linked list services
// ========================================================================

// ========================================================================
// Debug Event Browser Data Structure Overview
// -------------------------------------------
//
// The Debug Event Browser (DEB) maintains a rather involved data structure
// to store various debug event and debuggee process information.  It
// attempts to encapsulate the intricacies of what makes a process based on
// the occuring events.  Much of this stored information is never utilized
// by the Debug Event Browser but it is included to demonstrate what types
// of debug event may be useful to a full blown debugger application.
//
// This data structure uses the generalized, sorted, double-linked list
// package provided with the sample.  Each list can store list-specific
// instance data, list-specific node data, and maintain various pointers
// to these nodes.  The list is sorted via the insertion sort method where
// the programmer defines the list-specific sort function whose purpose is
// to compare two given nodes and return their relative sort location.  The
// list package in generalized in the sense that the list and node-specific
// data type is not known to this package at compile time or at runtime.
// The application programmer is merely responsible for defining the list
// and node-specific data structures and the sorting and optional searching
// functions and the list package keeps track of these nodes and provides
// easy access to them.
//
// DEB uses this list package to create three unique list types: process,
// thread and DLL lists.
//
// The backbone of the data structure is the process list.  The nodes of
// the process list are the individual debuggee processes.  DEB allows
// debugging (or should I say debug event browsing) of other processes that
// are spawned by the initial debuggee.  Thus each debug session may have
// multiple debuggees and thus the process becomes the logical node unit.
//
// A visual diagram of the process list is as follows:
//
//                 (ProcessList)
//                      |
//                      |
//                      v
//                 +-----------------+           +----------------------+
//                 | -ProcessList-   |           | -ListData-           |
//                 |                 |           |                      |
//                 | ListData--------+---------->| ActiveProcessCount=N |
//           +-----+-FirstNode       |           +----------------------+
//           |   +-+ CurrentNode     |
//           |   | | LastNode--------+-----------------------+
//           |   | | OrderFunction=& |                       |
//           |   | | ListError=0     |                       |
//           |   | +-----------------+                       |
//           |   |                                           |
//           |   +-------------------+                       |
//           |                       |                       |
//           v                       v                       v
//         +------------+     +------------+             +------------+
// NULL <- |ProcessNode1| <=> |ProcessNode2| <=> ... <=> |ProcessNodeN| -> NULL
//         +------------+     +------------+             +------------+
//
// Each process node also contains two lists: the thread list and the DLL list.
// This node also stores some of the relevent debug event information
// particular to the create process event.  Visually it is as follows:
//
//    +------------------+         +--------------+
//    | -ProcessNode-    | +------>| -ThreadList- |
//    |                  | |       +--------------+
//    | ProcessID=0      | |
//    | ThreadID=0       | |       +-----------+
//    | FileName=""      | |   +-->| -DllList- |
//    | PathName=""      | |   |   +-----------+
//    | ThreadList-------+-+   |
//    | DllList----------+-----+   +-------------------+
//    | ProcessDebugInfo-+-------->|-ProcessDebugInfo- |
//    +------------------+         +-------------------+
//
// Much like the process list, the visual diagram of the thread list is as
// follows:
//
//                 +-----------------+           +---------------------+
//                 | -ThreadList-    |           | -ListData-          |
//                 |                 |           |                     |
//                 | ListData--------+---------->| ActiveThreadCount=N |
//           +-----+-FirstNode       |           +---------------------+
//           |   +-+ CurrentNode     |
//           |   | | LastNode--------+-----------------------+
//           |   | | OrderFunction=& |                       |
//           |   | | ListError=0     |                       |
//           |   | +-----------------+                       |
//           |   |                                           |
//           |   +-------------------+                       |
//           |                       |                       |
//           v                       v                       v
//          +-----------+     +-----------+             +-----------+
//  NULL <- |ThreadNode1| <=> |ThreadNode2| <=> ... <=> |ThreadNodeN| -> NULL
//          +-----------+     +-----------+             +-----------+
//
// The thread nodes store some of the relevent debug event information
// particular to the create thread event.  Visually it is as follows:
//
//    +--------------------+
//    | -ThreadNode-       |
//    |                    |
//    | ProcessID=0        |
//    | ThreadID=0         |
//    | ThreadDebugInfo={} |
//    +--------------------+
//
// Much like the process and thread lists, the visual diagram of the Dll list
// is as follows:
//
//                 +-----------------+           +------------------+
//                 | -DllList-       |           | -ListData-       |
//                 |                 |           |                  |
//                 | ListData--------+---------->| ActiveDllCount=N |
//           +-----+-FirstNode       |           +------------------+
//           |   +-+ CurrentNode     |
//           |   | | LastNode--------+-----------------+
//           |   | | OrderFunction=& |                 |
//           |   | | ListError=0     |                 |
//           |   | +-----------------+                 |
//           |   |                                     |
//           |   +-------------+                       |
//           |                 |                       |
//           v                 v                       v
//          +--------+     +--------+             +--------+
//  NULL <- |DllNode1| <=> |DllNode2| <=> ... <=> |DllNodeN| -> NULL
//          +--------+     +--------+             +--------+
//
// The Dll nodes store some of the relevent debug event information particular
// to the Dll load event.  Visually it is as follows:
//
//    +----------------+
//    | -DllNode-      |
//    |                |
//    | FileName=""    |
//    | PathName=""    |
//    | DllDebugInfo={}|
//    +----------------+
//
// ========================================================================

// ------------------------------------------------------------------------
// Process list and node specific linked list wrapper functions
// ------------------------------------------------------------------------


// ************************************************************************
// FUNCTION : ProcessOrderFunction( PNODE, PNODE );
// PURPOSE  : Provides the sorting/search logic for the double linked
//            list package.
// COMMENTS :
//   Sorted by process ID value
// ************************************************************************
int
ProcessOrderFunction( PNODE pNode1, PNODE pNode2 )
{
  PDEB_PROCESS_NODE_INFO pProcessNodeInfo1 = pNode1->pNodeData;
  PDEB_PROCESS_NODE_INFO pProcessNodeInfo2 = pNode2->pNodeData;

  if( pProcessNodeInfo1->dwProcessId < pProcessNodeInfo2->dwProcessId )
    return( LIST_LEFT_OF );

  if( pProcessNodeInfo1->dwProcessId > pProcessNodeInfo2->dwProcessId )
    return( LIST_RIGHT_OF );

  return( LIST_MATCH );
}


// ************************************************************************
// FUNCTION : CreateProcessList( PLIST* )
// PURPOSE  :
// COMMENTS :
//
// ************************************************************************
BOOL
CreateProcessList( PLIST* ppProcessList )
{
  PDEB_PROCESS_LIST_INFO pProcessListInfo;

  //-- create list
  CreateList( ppProcessList, ProcessOrderFunction );

  //-- alloc info data
  pProcessListInfo = (PDEB_PROCESS_LIST_INFO) HeapAlloc( hHeap, (DWORD) NULL,
                                                sizeof( DEB_PROCESS_LIST_INFO ) );
  (*ppProcessList)->pListData = pProcessListInfo;

  //-- init info data
  pProcessListInfo->dwActiveProcesses = 0;

  return( TRUE );
}


// ************************************************************************
// FUNCTION : DestroyProcessList( PLIST )
// PURPOSE  :
// COMMENTS :
//
// ************************************************************************
BOOL
DestroyProcessList( PLIST pProcessList )
{
  PDEB_PROCESS_LIST_INFO pProcessListInfo = pProcessList->pListData;
  PNODE                  pDeleteNode;

  //-- make sure all nodes are removed first
  while( pProcessListInfo->dwActiveProcesses ) {
    GetCurrentNode( pProcessList, &pDeleteNode );
    DeleteCurrentProcessNode( pProcessList );
  }

  //-- free info data
  HeapFree( hHeap, (DWORD) NULL, (PVOID) pProcessListInfo );

  //-- destroy list
  DestroyList( pProcessList );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : AllocProcessNode( PNODE*, PDEB_PROCESS_NODE_INFO* )
// PURPOSE  :
// COMMENTS :
//
// ************************************************************************
BOOL
AllocProcessNode( PNODE* ppProcessNode, PDEB_PROCESS_NODE_INFO* ppProcessNodeInfo )
{
  //-- create node
  CreateNode( ppProcessNode );

  //-- alloc info data
  *ppProcessNodeInfo = (PDEB_PROCESS_NODE_INFO) HeapAlloc( hHeap, (DWORD) NULL,
                                              sizeof( DEB_PROCESS_NODE_INFO ) );
  (*ppProcessNode)->pNodeData = *(ppProcessNodeInfo);

  (*ppProcessNodeInfo)->lpstrFileName = (LPTSTR) HeapAlloc( hHeap, (DWORD) NULL,
                                               (DWORD) MAX_PATH );
  (*ppProcessNodeInfo)->lpstrPathName = (LPTSTR) HeapAlloc( hHeap, (DWORD) NULL,
                                               (DWORD) MAX_PATH );
  CreateThreadList( &((*ppProcessNodeInfo)->pThreadList) );
  CreateDllList( &((*ppProcessNodeInfo)->pDllList) );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : InitProcessNodeInfo( PDEB_PROCESS_NODE_INFO*, LPDEBUG_EVENT )
// PURPOSE  :
// COMMENTS :
//
// ************************************************************************
BOOL
InitProcessNodeInfo( PDEB_PROCESS_NODE_INFO* ppProcessNodeInfo, LPDEBUG_EVENT lpDebugEvent )
{
  //-- init info data
  (*ppProcessNodeInfo)->dwProcessId = lpDebugEvent->dwProcessId;
  (*ppProcessNodeInfo)->dwThreadId  = lpDebugEvent->dwThreadId;
  // Note:pThreadList initialized via previous CreateThreadList() call
  // Note:pDllList initialized via previous CreateDllList() call
  (*ppProcessNodeInfo)->ProcessDebugInfo = lpDebugEvent->u.CreateProcessInfo;

  return( TRUE );
}


// ************************************************************************
// FUNCTION : InsertProcessNode( PLIST, PNODE )
// PURPOSE  :
// COMMENTS :
//
// ************************************************************************
BOOL
InsertProcessNode( PLIST pProcessList, PNODE pProcessNode )
{
  PDEB_PROCESS_LIST_INFO pProcessListInfo = pProcessList->pListData;

  //-- insert the node
  InsertNode( pProcessList, pProcessNode );

  //-- increment dwActiveProcesss
  pProcessListInfo->dwActiveProcesses++;

  return( TRUE );
}


// ************************************************************************
// FUNCTION : SetCurrentProcessNode( PLIST, PNODE )
// PURPOSE  :
// COMMENTS :
//
// ************************************************************************
BOOL
SetCurrentProcessNode( PLIST pProcessList, PNODE pProcessNode )
{
  SetCurrentNode( pProcessList, pProcessNode, pProcessList->OrderFunction );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : DeleteProcessNode( PLIST, PNODE )
// PURPOSE  :
// COMMENTS :
//
// ************************************************************************
BOOL
DeleteProcessNode( PLIST pProcessList, PNODE pProcessNode )
{
  PNODE pDeleteNode;

  SetCurrentNode( pProcessList, pProcessNode, pProcessList->OrderFunction );
  GetCurrentNode( pProcessList, &pDeleteNode );
  DeleteCurrentProcessNode( pProcessList );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : FreeProcessNodeInfo( PNODE )
// PURPOSE  :
// COMMENTS :
//
// ************************************************************************
BOOL
FreeProcessNodeInfo( PNODE pProcessNode )
{
  PDEB_PROCESS_NODE_INFO pProcessNodeInfo = (PDEB_PROCESS_NODE_INFO) pProcessNode->pNodeData;

  //-- free info data
  DestroyDllList( pProcessNodeInfo->pDllList );
  DestroyThreadList( pProcessNodeInfo->pThreadList );
  HeapFree( hHeap, (DWORD) NULL, (PVOID) pProcessNodeInfo->lpstrPathName );
  HeapFree( hHeap, (DWORD) NULL, (PVOID) pProcessNodeInfo->lpstrFileName );
  HeapFree( hHeap, (DWORD) NULL, (PVOID) pProcessNodeInfo );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : DestroyProcessNode( PNODE )
// PURPOSE  :
// COMMENTS : Frees all memory associated with the node.
// ************************************************************************
BOOL
DestroyProcessNode( PNODE pProcessNode )
{
  //-- free info data
  FreeProcessNodeInfo( pProcessNode );

  //-- destroy node
  DestroyNode( pProcessNode );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : DeleteCurrentProcessNode( PLIST )
// PURPOSE  :
// COMMENTS :
//
// ************************************************************************
BOOL
DeleteCurrentProcessNode( PLIST pProcessList )
{
  PDEB_PROCESS_LIST_INFO pProcessListInfo = pProcessList->pListData;
  PNODE                  pProcessNode = (PNODE) pProcessList->pCurrentNode;

  //-- free info data
  FreeProcessNodeInfo( pProcessNode );

  //-- delete and destroy node
  DeleteCurrentNode( pProcessList );

  //-- decrement dwActiveProcesss
  pProcessListInfo->dwActiveProcesses--;

  return( TRUE );
}


// ------------------------------------------------------------------------
// Thread list and node specific linked list wrapper functions
// ------------------------------------------------------------------------


// ************************************************************************
// FUNCTION : ThreadOrderFunction( PNODE, PNODE );
// PURPOSE  : Provides the sorting/search logic for the double linked
//            list package.
// COMMENTS :
//   Sorted by thread ID value
// ************************************************************************
int
ThreadOrderFunction( PNODE pNode1, PNODE pNode2 )
{
  PDEB_THREAD_NODE_INFO pThreadNodeInfo1 = pNode1->pNodeData;
  PDEB_THREAD_NODE_INFO pThreadNodeInfo2 = pNode2->pNodeData;

  if( pThreadNodeInfo1->dwThreadId < pThreadNodeInfo2->dwThreadId )
    return( LIST_LEFT_OF );

  if( pThreadNodeInfo1->dwThreadId > pThreadNodeInfo2->dwThreadId )
    return( LIST_RIGHT_OF );

  return( LIST_MATCH );
}


// ************************************************************************
// FUNCTION : CreateThreadList( PLIST* )
// PURPOSE  :
// COMMENTS :
//
// ************************************************************************
BOOL
CreateThreadList( PLIST* ppThreadList )
{
  PDEB_THREAD_LIST_INFO pThreadListInfo;

  //-- create list
  CreateList( ppThreadList, ThreadOrderFunction );

  //-- alloc info data
  pThreadListInfo = (PDEB_THREAD_LIST_INFO) HeapAlloc( hHeap, (DWORD) NULL,
                                                sizeof( DEB_THREAD_LIST_INFO ) );
  (*ppThreadList)->pListData = pThreadListInfo;

  //-- init info data
  pThreadListInfo->dwActiveThreads = 0;

  return( TRUE );
}


// ************************************************************************
// FUNCTION : DestroyThreadList( PLIST )
// PURPOSE  :
// COMMENTS :
//
// ************************************************************************
BOOL
DestroyThreadList( PLIST pThreadList )
{
  PDEB_THREAD_LIST_INFO pThreadListInfo = pThreadList->pListData;
  PNODE                 pDeleteNode;

  //-- make sure all nodes are removed first
  while( pThreadListInfo->dwActiveThreads ) {
    GetCurrentNode( pThreadList, &pDeleteNode );
    DeleteCurrentThreadNode( pThreadList );
  }

  //-- free info data
  HeapFree( hHeap, (DWORD) NULL, (PVOID) pThreadListInfo );

  //-- destroy list
  DestroyList( pThreadList );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : AllocThreadNode( PNODE*, PDEB_THREAD_NODE_INFO* )
// PURPOSE  :
// COMMENTS :
//
// ************************************************************************
BOOL
AllocThreadNode( PNODE* ppThreadNode, PDEB_THREAD_NODE_INFO* ppThreadNodeInfo )
{
  //-- create node
  CreateNode( ppThreadNode );

  //-- alloc info data
  *ppThreadNodeInfo = (PDEB_THREAD_NODE_INFO) HeapAlloc( hHeap, (DWORD) NULL,
                                            (DWORD) sizeof( DEB_THREAD_NODE_INFO ) );
  (*ppThreadNode)->pNodeData = *(ppThreadNodeInfo);

  return( TRUE );
}


// ************************************************************************
// FUNCTION : InitThreadNodeInfo( PDEB_THREAD_NODE_INFO*, LPDEBUG_EVENT )
// PURPOSE  :
// COMMENTS :
//
// ************************************************************************
BOOL
InitThreadNodeInfo( PDEB_THREAD_NODE_INFO* ppThreadNodeInfo,
  LPDEBUG_EVENT lpDebugEvent )
{
  //-- init info data
  (*ppThreadNodeInfo)->dwProcessId     = lpDebugEvent->dwProcessId;
  (*ppThreadNodeInfo)->dwThreadId      = lpDebugEvent->dwThreadId;
  (*ppThreadNodeInfo)->ThreadDebugInfo = lpDebugEvent->u.CreateThread;

  return( TRUE );
}


// ************************************************************************
// FUNCTION : InsertThreadNode( PLIST, PNODE )
// PURPOSE  :
// COMMENTS :
//
// ************************************************************************
BOOL
InsertThreadNode( PLIST pThreadList, PNODE pThreadNode )
{
  PDEB_THREAD_LIST_INFO pThreadListInfo = (PDEB_THREAD_LIST_INFO) pThreadList->pListData;

  //-- insert the thread node
  InsertNode( pThreadList, pThreadNode );

  //-- increment dwActiveThreads
  pThreadListInfo->dwActiveThreads++;

  return( TRUE );
}


// ************************************************************************
// FUNCTION : SetCurrentThreadNode( PLIST, PNODE )
// PURPOSE  :
// COMMENTS :
//
// ************************************************************************
BOOL
SetCurrentThreadNode( PLIST pThreadList, PNODE pThreadNode )
{
  SetCurrentNode( pThreadList, pThreadNode, pThreadList->OrderFunction );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : DeleteThreadNode( PLIST, PNODE )
// PURPOSE  :
// COMMENTS :
//
// ************************************************************************
BOOL
DeleteThreadNode( PLIST pThreadList, PNODE pThreadNode )
{
  PNODE pDeleteNode;

  SetCurrentNode( pThreadList, pThreadNode, pThreadList->OrderFunction );
  GetCurrentNode( pThreadList, &pDeleteNode );
  DeleteCurrentThreadNode( pThreadList );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : FreeThreadNodeInfo( PNODE )
// PURPOSE  :
// COMMENTS :
//
// ************************************************************************
BOOL
FreeThreadNodeInfo( PNODE pThreadNode )
{
  PDEB_THREAD_NODE_INFO pThreadNodeInfo = (PDEB_THREAD_NODE_INFO) pThreadNode->pNodeData;

  //-- free info data
  HeapFree( hHeap, (DWORD) NULL, (PVOID) pThreadNodeInfo );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : DestroyThreadNode( PNODE )
// PURPOSE  :
// COMMENTS : Frees all memory associated with the node.
// ************************************************************************
BOOL
DestroyThreadNode( PNODE pThreadNode )
{
  //-- free info data
  FreeThreadNodeInfo( pThreadNode );

  //-- destroy node
  DestroyNode( pThreadNode );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : DeleteCurrentThreadNode( PLIST )
// PURPOSE  :
// COMMENTS :
//
// ************************************************************************
BOOL
DeleteCurrentThreadNode( PLIST pThreadList )
{
  PDEB_THREAD_LIST_INFO pThreadListInfo = pThreadList->pListData;
  PNODE                 pThreadNode = (PNODE) pThreadList->pCurrentNode;

  //-- free info data
  FreeThreadNodeInfo( pThreadNode );

  //-- delete and destroy node
  DeleteCurrentNode( pThreadList );

  //-- decrement dwActiveThreads
  pThreadListInfo->dwActiveThreads--;

  return( TRUE );
}


// ------------------------------------------------------------------------
// DLL list and node specific linked list wrapper functions
// ------------------------------------------------------------------------


// ************************************************************************
// FUNCTION : DllOrderFunction( PNODE, PNODE );
// PURPOSE  : Provides the sorting/search logic for the double linked
//            list package.
// COMMENTS :
//   Sorted by base address of the DLL
// ************************************************************************
int
DllOrderFunction( PNODE pNode1, PNODE pNode2 )
{
  PDEB_DLL_NODE_INFO pDllNodeInfo1 = pNode1->pNodeData;
  PDEB_DLL_NODE_INFO pDllNodeInfo2 = pNode2->pNodeData;

  if( pDllNodeInfo1->DllDebugInfo.lpBaseOfDll < pDllNodeInfo2->DllDebugInfo.lpBaseOfDll )
    return( LIST_LEFT_OF );

  if( pDllNodeInfo1->DllDebugInfo.lpBaseOfDll > pDllNodeInfo2->DllDebugInfo.lpBaseOfDll )
    return( LIST_RIGHT_OF );

  return( LIST_MATCH );
}


// ************************************************************************
// FUNCTION : CreateDllList( PLIST* )
// PURPOSE  :
// COMMENTS :
//
// ************************************************************************
BOOL
CreateDllList( PLIST* ppDllList )
{
  PDEB_DLL_LIST_INFO pDllListInfo;

  //-- create list
  CreateList( ppDllList, DllOrderFunction );

  //-- alloc info data
  pDllListInfo = (PDEB_DLL_LIST_INFO) HeapAlloc( hHeap, (DWORD) NULL,
                                                sizeof( DEB_DLL_LIST_INFO ) );
  (*ppDllList)->pListData = pDllListInfo;

  //-- init info data
  pDllListInfo->dwActiveDlls = 0;

  return( TRUE );
}


// ************************************************************************
// FUNCTION : DestroyDllList( PLIST )
// PURPOSE  :
// COMMENTS :
//
// ************************************************************************
BOOL
DestroyDllList( PLIST pDllList )
{
  PDEB_DLL_LIST_INFO pDllListInfo = pDllList->pListData;
  PNODE              pDeleteNode;

  //-- make sure all nodes are removed first
  while( pDllListInfo->dwActiveDlls ) {
    GetCurrentNode( pDllList, &pDeleteNode );
    DeleteCurrentDllNode( pDllList );
  }

  //-- free list data and destroy the list
  HeapFree( hHeap, (DWORD) NULL, (PVOID) pDllListInfo );
  DestroyList( pDllList );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : AllocDllNode( PNODE*, PDEB_DLL_NODE_INFO* )
// PURPOSE  :
// COMMENTS :
//
// ************************************************************************
BOOL
AllocDllNode( PNODE* ppDllNode, PDEB_DLL_NODE_INFO* ppDllNodeInfo )
{
  //-- create node
  CreateNode( ppDllNode );

  //-- alloc info data
  *ppDllNodeInfo = (PDEB_DLL_NODE_INFO) HeapAlloc( hHeap, (DWORD) NULL,
                                      sizeof( DEB_DLL_NODE_INFO ) );
  (*ppDllNode)->pNodeData = *(ppDllNodeInfo);
  (*ppDllNodeInfo)->lpstrFileName = (LPTSTR) HeapAlloc( hHeap, (DWORD) NULL, (DWORD) MAX_PATH );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : InitDllNodeInfo( PDEB_DLL_NODE_INFO*, LPDEBUG_EVENT )
// PURPOSE  :
// COMMENTS :
//
// ************************************************************************
BOOL
InitDllNodeInfo( PDEB_DLL_NODE_INFO* ppDllNodeInfo,
  LPDEBUG_EVENT lpDebugEvent )
{
  //-- init info data
  GetDllFileName( lpDebugEvent, (*ppDllNodeInfo)->lpstrFileName, MAX_PATH );
  (*ppDllNodeInfo)->DllDebugInfo = lpDebugEvent->u.LoadDll;

  return( TRUE );
}


// ************************************************************************
// FUNCTION : InsertDllNode( PLIST, PNODE )
// PURPOSE  :
// COMMENTS :
//
// ************************************************************************
BOOL
InsertDllNode( PLIST pDllList, PNODE pDllNode )
{
  PDEB_DLL_LIST_INFO pDllListInfo = pDllList->pListData;

  // insert the node
  InsertNode( pDllList, pDllNode );

  //-- increment dwActiveDlls
  pDllListInfo->dwActiveDlls++;

  return( TRUE );
}


// ************************************************************************
// FUNCTION : SetCurrentDllNode( PLIST, PNODE )
// PURPOSE  :
// COMMENTS :
//
// ************************************************************************
BOOL
SetCurrentDllNode( PLIST pDllList, PNODE pDllNode )
{
  SetCurrentNode( pDllList, pDllNode, pDllList->OrderFunction );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : DeleteDllNode( PLIST, PNODE )
// PURPOSE  :
// COMMENTS :
//
// ************************************************************************
BOOL
DeleteDllNode( PLIST pDllList, PNODE pDllNode )
{
  PNODE pDeleteNode;

  SetCurrentNode( pDllList, pDllNode, pDllList->OrderFunction );
  GetCurrentNode( pDllList, &pDeleteNode );
  DeleteCurrentDllNode( pDllList );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : FreeDllNodeInfo( PNODE )
// PURPOSE  :
// COMMENTS :
//
// ************************************************************************
BOOL
FreeDllNodeInfo( PNODE pDllNode )
{
  PDEB_DLL_NODE_INFO pDllNodeInfo = (PDEB_DLL_NODE_INFO) pDllNode->pNodeData;

  //-- free info data
  HeapFree( hHeap, (DWORD) NULL, (PVOID) pDllNodeInfo->lpstrFileName );
  HeapFree( hHeap, (DWORD) NULL, (PVOID) pDllNodeInfo );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : DestroyDllNode( PNODE )
// PURPOSE  :
// COMMENTS : Frees all memory associated with the node.
// ************************************************************************
BOOL
DestroyDllNode( PNODE pDllNode )
{
  //-- free info data
  FreeDllNodeInfo( pDllNode );

  //-- destroy node
  DestroyNode( pDllNode );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : DeleteCurrentDllNode( PLIST )
// PURPOSE  :
// COMMENTS : Deletes the current DLL node from the list and frees all
//            memory associated with it.
// ************************************************************************
BOOL
DeleteCurrentDllNode( PLIST pDllList )
{
  PDEB_DLL_LIST_INFO pDllListInfo = pDllList->pListData;
  PNODE              pDllNode = (PNODE) pDllList->pCurrentNode;

  //-- free info data
  FreeDllNodeInfo( pDllNode );

  //-- delete and destroy node
  DeleteCurrentNode( pDllList );

  //-- decrement dwActiveDlls
  pDllListInfo->dwActiveDlls--;

  return( TRUE );
}
