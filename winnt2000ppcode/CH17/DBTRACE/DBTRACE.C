/*----------------------------------------------------
    DBTRACE.C

    Written by Brian G. Myers
  ----------------------------------------------------*/
#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>     /* _MAX_FNAME */
#pragma hdrstop
#include "dbtrace.h"

#define MAX_LINES 500   /* size cap for edit buffer */

/*----------------------------------------------------
    GLOBAL VARIABLES
  ----------------------------------------------------*/
HWND ghMainWnd;
HWND ghEditBox;
char gszWndClass[] = "DebugTrace";
char gszAppTitle[] = "Debug Trace";
BOOL gbContinueDebug = FALSE;

/*----------------------------------------------------
    WIN MAIN
  ----------------------------------------------------*/
int WINAPI WinMain(HINSTANCE hinstThis,
  HINSTANCE hinstPrev, LPSTR lpszCmdLine, int iCmdShow)
{
  MSG msg;

  if (!Init(hinstThis, iCmdShow)) {
    MessageBox(NULL, "Cannot initialize program",
      gszAppTitle, MB_OK | MB_ICONEXCLAMATION);
      return(FALSE);
  }
    
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  
  return(msg.wParam);
}

/*----------------------------------------------------
    INIT
  ----------------------------------------------------*/
BOOL Init( HINSTANCE hInst, int iCmdShow )
{
  WNDCLASS wc;
  HFONT hFont;
  
  /* register a class for the main window */  
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_APPICON));
  wc.lpszMenuName = MAKEINTRESOURCE(IDM_APPMENU);
  wc.lpszClassName = gszWndClass;
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wc.hInstance = hInst;
  wc.style = CS_BYTEALIGNCLIENT;
  wc.lpfnWndProc = DBTrace_WndProc;
  wc.cbWndExtra = 0;
  wc.cbClsExtra = 0;
  if (!RegisterClass(&wc))
    return(FALSE);

  /* create the main window */
  ghMainWnd = CreateWindow(gszWndClass, gszAppTitle,
    WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
    CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL,
    hInst, NULL);     
  if (!ghMainWnd) {
    ShowErrorMsg(NULL, __LINE__);
    return(FALSE);
  }

  /* create an edit window to fill the client area */
  ghEditBox = CreateWindow( "EDIT", NULL, WS_CHILD
    | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_AUTOHSCROLL
    | ES_AUTOVSCROLL | ES_MULTILINE | ES_WANTRETURN,
    0, 0, 0, 0, ghMainWnd, NULL, hInst, NULL );
  if (!ghEditBox) {
    ShowErrorMsg(ghMainWnd,  __LINE__ );
    return( FALSE );
  }
  hFont = GetStockFont(SYSTEM_FONT);
  SetWindowFont(ghEditBox, hFont, FALSE);
  ShowWindow(ghMainWnd, iCmdShow);
  UpdateWindow(ghMainWnd);
  return(TRUE);
}

/*----------------------------------------------------
    WINDOW PROCEDURE
  ----------------------------------------------------*/
LRESULT WINAPI DBTrace_WndProc( HWND hWnd, UINT uMessage,
    WPARAM wParam, LPARAM lParam )
{
  HMENU hMenu;
  
  switch (uMessage)
  {
    case WM_COMMAND:
      DoCommand( hWnd, (WORD)wParam );
      break;

    case WM_ACTIVATE:
      if (wParam)
        SetFocus(ghEditBox);
      break;
          
    case WM_INITMENU:
      hMenu = GetMenu(hWnd);
      if (gbContinueDebug) {
        EnableMenuItem(hMenu, CMD_FILELAUNCH, MF_GRAYED);
        EnableMenuItem(hMenu, CMD_FILEDETACH, MF_ENABLED);
      } else {
        EnableMenuItem(hMenu, CMD_FILELAUNCH, MF_ENABLED);
        EnableMenuItem(hMenu, CMD_FILEDETACH, MF_GRAYED);
      }
      break;
      
    case WM_SIZE:
      if (ghEditBox) {
        RECT r;
        GetClientRect(hWnd, &r);
        InflateRect(&r, 1, 1);
        MoveWindow(ghEditBox, r.top, r.left,
          r.right - r.left, r.bottom - r.top, TRUE);
      }
      break;
        
    case WM_DESTROY:
      PostQuitMessage(0);  
      break;

    default:
      return(DefWindowProc(hWnd, uMessage, wParam, lParam));
  }
    return(0); 
}
    
/*----------------------------------------------------
    DO COMMAND
  ----------------------------------------------------*/
void DoCommand( HWND hWnd, WORD wCmd )
{
  switch (wCmd) {
    
    case CMD_FILELAUNCH:
      DoLaunch(hWnd);
      break;

    case CMD_FILEDETACH:
      gbContinueDebug = FALSE;
      break;
      
    case CMD_ABOUT:
      DoAbout(hWnd);
      break;
        
    case CMD_FILEEXIT:
      PostQuitMessage(0);
      break;

    case CMD_EDITCUT:
      PostMessage(ghEditBox, WM_CUT, 0, 0L);
      break;
          
    case CMD_EDITCOPY:
      PostMessage(ghEditBox, WM_COPY, 0, 0L);
      break;
          
    case CMD_EDITPASTE:
      PostMessage(ghEditBox, WM_PASTE, 0, 0L);
      break;
          
    case CMD_EDITCLEAR:
      PostMessage(ghEditBox, WM_CLEAR, 0, 0L);
      break;

    case CMD_EDITCLEARALL:
      Edit_SetSel(ghEditBox, 0, 32767);
      Edit_ReplaceSel(ghEditBox, "");
      break;

    case CMD_EDITSELECTALL:
      Edit_SetSel(ghEditBox, 0, 32767);
      break;
  }
}

/*----------------------------------------------------
    DO LAUNCH
  ----------------------------------------------------*/
void DoLaunch( HWND hWnd )
{
  /* This must be static so that the debugging thread */
  /* can read it even if DoLaunch ends before the     */
  /* thread calls CreateProcess.                      */
  static TCHAR szPath[_MAX_PATH*sizeof(TCHAR)] = TEXT("");

  TCHAR szName[_MAX_FNAME*sizeof(TCHAR)] = TEXT("");
  BOOL bSuccess;                      
  HANDLE hThread;           /* debugging thread */
  DWORD dwThreadID;

  /* ask user for name of file to open */
  bSuccess = GetFileName(hWnd, szPath, szName);
  if (!bSuccess)
    return;
  
  /* create a thread to wait for debugging events */
  gbContinueDebug = TRUE;
  hThread = CreateThread(NULL, 0, 
    (LPTHREAD_START_ROUTINE)DebuggingThread,    
    (LPTSTR)szPath, 0, &dwThreadID);
  if (!hThread) {
    ShowErrorMsg(ghMainWnd, __LINE__);
    gbContinueDebug = FALSE;
    return;
  }
  CloseHandle(hThread);
}

/*----------------------------------------------------
    DEBUGGING THREAD
  ----------------------------------------------------*/
LRESULT DebuggingThread( LPTSTR lpszFilePath )
{
  STARTUPINFO sui;    
  PROCESS_INFORMATION pi; 
  BOOL bSuccess;
  DEBUG_EVENT DebugEvent;

  /* fill in the process's startup information */
  ZeroMemory( &sui, sizeof(STARTUPINFO) );
  sui.cb = sizeof(STARTUPINFO);
  sui.wShowWindow = SW_SHOWDEFAULT;

  pi.hProcess = NULL;

  /* create the debuggee process */
  bSuccess = CreateProcess(NULL, lpszFilePath, NULL,
    NULL, TRUE, DEBUG_PROCESS | DEBUG_ONLY_THIS_PROCESS,
    NULL, NULL, &sui, &pi);
  if (!bSuccess) {
    ShowErrorMsg(ghMainWnd, __LINE__);
    gbContinueDebug = FALSE;
    ExitThread( 1 );
  }
    
  while (gbContinueDebug) {
    DWORD dwBytesRead;            /* return from ReadProcessMem */
    TCHAR szDebugString[256];     /* buffer to hold string */
    UINT length;                  /* num chars in string */   
      
    /* block until a debugging event occurs */
    WaitForDebugEvent(&DebugEvent, INFINITE);

    switch (DebugEvent.dwDebugEventCode) {
      
      case OUTPUT_DEBUG_STRING_EVENT:
        length = DebugEvent.u.DebugString.nDebugStringLength;
        
        bSuccess = ReadProcessMemory(pi.hProcess,
                      DebugEvent.u.DebugString.lpDebugStringData,
                      szDebugString, length, &dwBytesRead);
        if (!bSuccess) 
          ShowErrorMsg(ghMainWnd, __LINE__);

        szDebugString[length] = '\0';
        
        Edit_Append(ghEditBox, szDebugString);
        break;

      case EXIT_PROCESS_DEBUG_EVENT:
        Edit_Append(ghEditBox, "DETACHING" );
        gbContinueDebug = FALSE;
        break;
    }   
    /* allow debuggee to resume execution */
    ContinueDebugEvent(DebugEvent.dwProcessId,
      DebugEvent.dwThreadId, DBG_CONTINUE);
  }

  CloseHandle( pi.hThread );
  CloseHandle( pi.hProcess );
  return(0L);
}

/*----------------------------------------------------
    GET FILE NAME
    Invoke the File Open common dialog box to let the
    user select a file. The lpszFile and lpszFileTitle
    parameters should point to buffers of size
    _MAX_PATH and _MAX_FNAME, respectively.

    Returns TRUE if the dialog box closes without error.
    If the dialog box returns TRUE, then lpszFile and
    lpszFileTitle point to the new file path and name,
    respectively.
  ----------------------------------------------------*/
BOOL GetFileName(
  HWND hWnd, LPTSTR lpszFile, LPTSTR lpszFileTitle )
{
  OPENFILENAME ofn;
  BOOL bSuccess;
  CONST char *szFilter[] = {
    TEXT("programs (*.exe)\0 *.exe\0 All Files (*.*)\0 *.* \0\0")
  };

  /* initialize structure for the common dialog box */
  ZeroMemory(&ofn, sizeof(OPENFILENAME));
  ofn.lStructSize    = sizeof(OPENFILENAME);
  ofn.hwndOwner      = hWnd;
  ofn.hInstance      = GetWindowInstance(hWnd);
  ofn.lpstrFilter    = szFilter[0];
  ofn.nFilterIndex   = 1;
  ofn.lpstrFile      = lpszFile;
  ofn.nMaxFile       = MAX_PATH;
  ofn.lpstrFileTitle = lpszFileTitle;
  ofn.nMaxFileTitle  = _MAX_FNAME;
  ofn.lpstrDefExt    = TEXT("exe");
  ofn.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST |
    OFN_FILEMUSTEXIST;

  /* let the user choose a file */
  bSuccess = GetOpenFileName(&ofn);
  return(bSuccess);
}

/*----------------------------------------------------
    EDIT_APPEND
  ----------------------------------------------------*/
void Edit_Append( HWND hEditBox, LPCTSTR lpszString )
{
  Edit_SetSel(hEditBox, 32767, 32767);
  Edit_ReplaceSel(hEditBox, lpszString);
  Edit_SetSel(hEditBox, 32767, 32767);
}

/*----------------------------------------------------
    DO ABOUT
    ShellAbout is an undocumented function, but all
    you have to do is declare its prototype to call it.
  ----------------------------------------------------*/
void DoAbout( HWND hWnd )
{
  HICON hIcon;
  HINSTANCE hInst = GetWindowInstance(hWnd);
  hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_APPICON));
  ShellAbout(hWnd, gszAppTitle, "by Brian Myers", hIcon);
}

/*----------------------------------------------------
    SHOW ERROR MESSAGE
  ----------------------------------------------------*/
void ShowErrorMsg( HWND hParent, int iLine )
{
  LPTSTR pBuffer;                         
  LPTSTR pMessage;

  /* retrieve a message from the system message table */
  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
    FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(),                     
    MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), 
    (LPTSTR)&pBuffer, 0, NULL);                            

  /* allocate a larger buffer and append */
  /* the line number to the end */
  pMessage = LocalAlloc( LHND,
    (lstrlen(pBuffer) * sizeof(TCHAR))
    + (15 * sizeof(TCHAR)));
  wsprintf(pMessage, TEXT("%s (line %u)"),
    (LPTSTR)pBuffer, iLine);

  /* display the message in a message box */
  MessageBox(hParent, pMessage, TEXT("Debug Monitor"),
    MB_ICONEXCLAMATION | MB_OK);
    
  LocalFree(pBuffer);
  LocalFree(pMessage);
}


