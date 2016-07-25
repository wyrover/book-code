/*----------------------------------------------------
    STEPPER.C
    The main module of a simple debugger for Windows
    NT.  STEPPER launches a child called FOO and
    lets the user set breakpoints in the source code
    from FOO.C.

    copyright (c) 1994, 1996 by Brian G. Myers
  ----------------------------------------------------*/
#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <assert.h>   
#include "stepper.h"

/* symbols for marking lines in the source code */
#define BREAK_LINE_CHAR     TEXT("-")
#define BREAKPOINT_CHAR     TEXT("+")
#define CURRENT_LINE_CHAR   TEXT("»")
#define BLANK_CHAR          TEXT(" ")

/*----------------------------------------------------
    PROTOTYPES (for this module)
  ----------------------------------------------------*/
int WINAPI WinMain(HINSTANCE hinstThis,
  HINSTANCE hinstPrev, LPSTR lpszCmdLine, int iCmdShow);
BOOL Init(HINSTANCE hInst, int iCmdShow);
BOOL CreateCommandEvents(void);
BOOL LoadSource(LPCTSTR pszFilePath);
LRESULT WINAPI Stepper_WndProc(HWND hWnd, UINT uMessage,
  WPARAM wParam, LPARAM lParam);
void DoCommand(HWND hWnd, WORD wCmd);
void DoInitMenu(HWND hWnd);
void DoAbout(HWND hWnd);
UINT GetCursorLine(void);

/*----------------------------------------------------
    GLOBAL VARIABLES (for all modules)
  ----------------------------------------------------*/
HWND ghMainWnd;
HWND ghEditBox;
TCHAR gszAppTitle[] = TEXT("Stepper");
BOOL gbAttached = FALSE;
BOOL gbPaused = FALSE;
HANDLE ghCommandEvent[NUM_COMMAND_EVENTS];
HANDLE ghDebuggingThread;

/*----------------------------------------------------
    STATIC VARIABLES (for this module)
  ----------------------------------------------------*/
HACCEL ghAccTable;
TCHAR gszWndClass[] = TEXT("stepper");
TCHAR gszSourceFile[] = TEXT("FOO.C");
TCHAR gszExeFile[] = TEXT("FOO.EXE");

/*----------------------------------------------------
    WIN MAIN
  ----------------------------------------------------*/
int WINAPI WinMain( HINSTANCE hinstThis,
  HINSTANCE hinstPrev, PSTR pszCmdLine, int iCmdShow )
{
  MSG msg;

  if (!Init(hinstThis, iCmdShow)) {
    MessageBox(NULL, TEXT("Cannot initialize program"),
      gszAppTitle, MB_OK | MB_ICONEXCLAMATION);
      return(FALSE);
  }
    
  while (GetMessage(&msg, NULL, 0, 0)) {
    if (!TranslateAccelerator(ghMainWnd,
        ghAccTable, &msg)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
  return( msg.wParam );
}

/*----------------------------------------------------
    INIT
    Create main frame window with an edit control
    covering its client area.
  ----------------------------------------------------*/
BOOL Init( HINSTANCE hInst, int iCmdShow )
{
  WNDCLASS wc;

  /* create events for talking to debugger thread */
  if (!CreateCommandEvents()) {
    ERROR_MSG(ghMainWnd);
    return(FALSE);
  }

  ghAccTable = LoadAccelerators(hInst,
    MAKEINTRESOURCE(IDA_ACCELERATORS));
    
  /* register a class for the main window */  
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_APPICON));
  wc.lpszMenuName = MAKEINTRESOURCE(IDM_APPMENU);
  wc.lpszClassName = gszWndClass;
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wc.hInstance = hInst;
  wc.style = CS_BYTEALIGNCLIENT;
  wc.lpfnWndProc = Stepper_WndProc;
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
    ERROR_MSG(ghMainWnd);
    return(FALSE);
  }

  /* create an edit window to fill the client area */
  ghEditBox = CreateWindow( TEXT("EDIT"), NULL, WS_CHILD
    | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_AUTOHSCROLL
    | ES_AUTOVSCROLL | ES_MULTILINE | DS_LOCALEDIT,
    0, 0, 0, 0, ghMainWnd, NULL, hInst, NULL );
  if (!ghEditBox) {
    ERROR_MSG(ghMainWnd);
    return( FALSE );
  }
        
  ShowWindow(ghMainWnd, iCmdShow);
  UpdateWindow(ghMainWnd);
  StartDebugging(ghMainWnd);
  return(TRUE);
}

/*----------------------------------------------------
    CREATE COMMAND EVENTS
    Initialize array of  event objects used to
    synchronize actions in the application.
  ----------------------------------------------------*/
BOOL CreateCommandEvents( void )
{
  enum EVENTS ev;
  BOOL bSuccess = TRUE;

  /* only EVENT_RUNNING uses manual reset */
  ghCommandEvent[EVENT_RUNNING] =
    CreateEvent(NULL, TRUE, FALSE, TEXT("event_running"));
  ghCommandEvent[EVENT_RESUME] =
    CreateEvent(NULL, FALSE, FALSE, TEXT("event_resume"));
  ghCommandEvent[EVENT_KILL] =
    CreateEvent(NULL, FALSE, FALSE, TEXT("event_kill"));

  /* were any events not created? */
  for (ev=EVENT_RUNNING; ev<=EVENT_KILL; ev++)
    if (!ghCommandEvent[ev])
      bSuccess = FALSE;

  /* if any were missed, destroy all of them */
  if (!bSuccess)
    for (ev=EVENT_RUNNING; ev<=EVENT_KILL; ev++) 
      CloseHandle(ghCommandEvent[ev]);
      
  return(bSuccess);
}

/*----------------------------------------------------
    START DEBUGGING
    Loads a source file into the edit window and
    launches a debugger thread.
  ----------------------------------------------------*/
void StartDebugging( HWND hWnd )
{
  DWORD dwThreadId;
  BOOL bSuccess;
  
  /* load source file into edit box */
  bSuccess = LoadSource(gszSourceFile);
  if (!bSuccess) {
    ERROR_MSG(hWnd);
    return;
  }
    
  /* create a thread to wait for debugging events */
  ghDebuggingThread = CreateThread(NULL, 0, 
    (PTHREAD_START_ROUTINE)DebuggingThread,    
    (PTSTR)gszExeFile, 0, &dwThreadId);
  if (!ghDebuggingThread) 
    ERROR_MSG(hWnd);

  return;
}

/*----------------------------------------------------
    LOAD SOURCE
    Copies the contents of the given file into the
    application's edit control client window.
  ----------------------------------------------------*/
BOOL LoadSource ( LPCTSTR pszFilePath )
{
  HANDLE hFile = NULL;
  PBYTE pBuffer = NULL;
  BOOL bSuccess;
  DWORD dwSize, dwRead;
  int iTabStops = 16;
  int iLine, iLineCount, iLineStart;
  HFONT hFont;
    
  __try {
    __try {
      hFile = CreateFile( pszFilePath, GENERIC_READ,
        FILE_SHARE_READ, NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL |
        FILE_FLAG_SEQUENTIAL_SCAN, NULL );
      if (!hFile) 
        return(FALSE);

      dwSize = GetFileSize(hFile, NULL);
      pBuffer = HeapAlloc(GetProcessHeap(), 0L, dwSize+50);
      if (!pBuffer) 
        return(FALSE);

      bSuccess = ReadFile(hFile, pBuffer, dwSize,
        &dwRead, NULL);
      if (!bSuccess) 
        return(FALSE);
      
      pBuffer[dwSize] = 0;  /* terminating \0 */

      hFont = GetStockFont(ANSI_FIXED_FONT);
      SetWindowFont(ghEditBox, hFont, FALSE);
      Edit_SetReadOnly(ghEditBox, TRUE);
      Edit_SetTabStops(ghEditBox, 1, &iTabStops);
      Edit_SetText(ghEditBox, pBuffer);      

    } __finally {
      if (hFile)
        CloseHandle(hFile);
      if (AbnormalTermination() && pBuffer)
        HeapFree(GetProcessHeap(), 0L, pBuffer);
    }
  } __except (EXCEPTION_EXECUTE_HANDLER) {
    return (FALSE);
  }

  /* prefix each line with two spaces */
  iLineCount = Edit_GetLineCount(ghEditBox);
  for (iLine=0; iLine<iLineCount; iLine++) {
    iLineStart = Edit_LineIndex(ghEditBox, iLine);
    Edit_SetSel(ghEditBox, iLineStart, iLineStart);
    Edit_ReplaceSel(ghEditBox, TEXT("  "));
  }

  /* write the glyphs that show where breaks can be */
  MarkAllBreakLines();
  return(TRUE);
}
  
/*----------------------------------------------------
    WINDOW PROCEDURE
  ----------------------------------------------------*/
LRESULT WINAPI Stepper_WndProc( HWND hWnd, UINT uMessage,
    WPARAM wParam, LPARAM lParam )
{ 
  switch (uMessage)
  {
    case WM_COMMAND:
      DoCommand(hWnd, (WORD)wParam);
      break;

    case WM_ACTIVATE:
      if (wParam)
        SetFocus(ghEditBox);
      break;
          
    case WM_INITMENU:
      DoInitMenu(hWnd);
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
      DestroyList();
      PostQuitMessage(0);  
      break;

    default:
      return(DefWindowProc(hWnd, uMessage, wParam, lParam));
  }
  return(0); 
}
    
/*----------------------------------------------------
    DO COMMAND
    Execute menu commands, including debugger
    instructions such as "Step" and "Set Breakpoint."
  ----------------------------------------------------*/
void DoCommand( HWND hWnd, WORD wCmd )
{
  UINT uLine;
  
  switch (wCmd)
  {
    case CMD_ABOUT:
      DoAbout(hWnd);
      break;
        
    case CMD_QUIT:
      if (gbAttached) {
        /* give debugging thread 20 seconds to quit */
        ResetEvent(ghCommandEvent[EVENT_RUNNING]);
        SetEvent(ghCommandEvent[EVENT_KILL]);
        WaitForSingleObject(ghDebuggingThread, 20000);
      }
      CloseHandle(ghDebuggingThread);
      DestroyWindow(ghMainWnd);
      break;
                
    case CMD_KILL:
      ResetEvent(ghCommandEvent[EVENT_RUNNING]);
      SetEvent(ghCommandEvent[EVENT_KILL]);
      
      /* clear the edit box */
      Edit_SetSel(ghEditBox, 0, 32767);
      Edit_ReplaceSel(ghEditBox, TEXT(""));
      break;
      
    case CMD_RESTART:
      StartDebugging(hWnd);
      break;

    case CMD_SETBREAK: {
      uLine = GetCursorLine();      
      if (!ToggleBreakpoint(uLine))
        MessageBox(ghMainWnd, TEXT("Cannot set a breakpoint there"),
          gszAppTitle, MB_OK | MB_ICONEXCLAMATION);       
      } break;

    case CMD_SETALLBREAKS:
      if (!SetAllBreakpoints())
        MessageBox(ghMainWnd, TEXT("Cannot set all breakpoints"),
          gszAppTitle, MB_OK | MB_ICONEXCLAMATION);         
      break;
      
    case CMD_CLEARALLBREAKS: 
      if (!ClearAllBreakpoints())
        MessageBox(ghMainWnd, TEXT("Cannot delete all breakpoints"),
          gszAppTitle, MB_OK | MB_ICONEXCLAMATION);         
      break;
      
    case CMD_RUNTOCURSOR: {
      uLine = GetCursorLine();
      if (!RunToLine(uLine))
        MessageBox(ghMainWnd, TEXT("Cannot set a breakpoint there"),
          gszAppTitle, MB_OK | MB_ICONEXCLAMATION);       
      } break;

    case CMD_RUN:
      SetEvent(ghCommandEvent[EVENT_RESUME]);
      SetLinePrefix(0, PREFIX_CURRENT_LINE);
      break;            
  }
}

/*----------------------------------------------------
    DO INITMENU
  ----------------------------------------------------*/
void DoInitMenu( HWND hWnd )
{
  HMENU hMenu = GetMenu(hWnd);
  
  if (gbAttached) {
    EnableMenuItem(hMenu, CMD_KILL, MF_ENABLED);
    EnableMenuItem(hMenu, CMD_RESTART, MF_GRAYED);    
  } else {
    EnableMenuItem(hMenu, CMD_KILL, MF_GRAYED);
    EnableMenuItem(hMenu, CMD_RESTART, MF_ENABLED);
  }
                
  if (gbAttached && gbPaused) {
    EnableMenuItem(hMenu, CMD_SETBREAK, MF_ENABLED);
    EnableMenuItem(hMenu, CMD_SETALLBREAKS, MF_ENABLED);
    EnableMenuItem(hMenu, CMD_CLEARALLBREAKS, MF_ENABLED);
    EnableMenuItem(hMenu, CMD_RUNTOCURSOR, MF_ENABLED);
    EnableMenuItem(hMenu, CMD_RUN, MF_ENABLED);
  } else {
    EnableMenuItem(hMenu, CMD_SETBREAK, MF_GRAYED);
    EnableMenuItem(hMenu, CMD_SETALLBREAKS, MF_GRAYED);
    EnableMenuItem(hMenu, CMD_CLEARALLBREAKS, MF_GRAYED);
    EnableMenuItem(hMenu, CMD_RUNTOCURSOR, MF_GRAYED);
    EnableMenuItem(hMenu, CMD_RUN, MF_GRAYED);
  }
}

/*----------------------------------------------------
    DO ABOUT
  ----------------------------------------------------*/
void DoAbout( HWND hWnd )
{
  HINSTANCE hInst = GetWindowInstance(hWnd);  
  HICON hIcon =
    LoadIcon(hInst, MAKEINTRESOURCE(IDI_APPICON));  
  ShellAbout(hWnd, gszAppTitle,
    TEXT("by Brian Myers"), hIcon);
}

/*----------------------------------------------------
    GET CURSOR LINE
    Find out where the cursor is in the edit box.
    The first line is line 1.
  ----------------------------------------------------*/
UINT GetCursorLine( void )
{
  DWORD dwSelection;
  UINT uFirstChar, uLine;

  dwSelection = Edit_GetSel(ghEditBox);
  uFirstChar = LOWORD(dwSelection);
  uLine = Edit_LineFromChar(ghEditBox, uFirstChar);
  return(++uLine);  /* convert to 1-based numbering */
}

/*----------------------------------------------------
    SET WINDOW TITLE
  ----------------------------------------------------*/
void SetWindowTitle( void )
{
  TCHAR szTitle[255];
  if (!gbAttached)
    wsprintf(szTitle, "%s", (LPTSTR)gszAppTitle);
  else 
    wsprintf(szTitle, "%s - %s - %s", gszAppTitle,
      (LPTSTR)gszExeFile,
      (LPTSTR)(gbPaused ? TEXT("[paused]") : TEXT("[running]")));
  
  SetWindowText(ghMainWnd, szTitle);
}

/*----------------------------------------------------
    SET LINE PREFIX
    Inserts glyphs in the right margin of the
    source code to indicate which lines may
    have breakpoints, which do have breakpoints,
    and which is the current line.  The iLine
    parameter is 1-based (1 is the first line).
  ----------------------------------------------------*/
void SetLinePrefix( int iLine, int iPrefix )
{
  static int iCurrentLine = -1; /* no current line */
  int iLineIndex;

  iLine--;  /* edit box line index is 0-based */
  switch (iPrefix)
  {
    case PREFIX_BREAK_LINE:
      iLineIndex = Edit_LineIndex(ghEditBox, iLine);
      Edit_SetSel(ghEditBox, iLineIndex, iLineIndex+1);
      Edit_ReplaceSel(ghEditBox, BREAK_LINE_CHAR);
      break;
      
    case PREFIX_BREAKPOINT:
      iLineIndex = Edit_LineIndex(ghEditBox, iLine);
      Edit_SetSel(ghEditBox, iLineIndex, iLineIndex+1);
      Edit_ReplaceSel(ghEditBox, BREAKPOINT_CHAR);
      break;
      
    case PREFIX_CURRENT_LINE:     
      /* erase marker for previous line */
      if (iCurrentLine != -1) {
        iLineIndex = Edit_LineIndex(ghEditBox, iCurrentLine);
        Edit_SetSel(ghEditBox, iLineIndex+1, iLineIndex+2);
        Edit_ReplaceSel(ghEditBox, TEXT(" "));
      }

      /* insert marker for new current line */
      if (iLine >= 0) {
        iLineIndex = Edit_LineIndex(ghEditBox, iLine);
        Edit_SetSel(ghEditBox, iLineIndex+1, iLineIndex+2);
        Edit_ReplaceSel(ghEditBox, CURRENT_LINE_CHAR);
        iCurrentLine = iLine;
      }
      break;
  }
}

/*----------------------------------------------------
    SHOW ERROR MESSAGE
    Queries system for thread's most recent error
    value, retrieves the corresponding descriptive
    message, and displays it in a message box.  The
    message includes the source file name and line
    number where the error occurred.
  ----------------------------------------------------*/
void ShowErrorMsg( HWND hwndParent, PTSTR szFile, int iLine )
{
  PTSTR pBuffer;                         
  PTSTR pMessage;

  /* retrieve a message from the system message table */
  FormatMessage(
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
    NULL, GetLastError(),                     
    MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), 
    (PTSTR)&pBuffer, 0, NULL);

  /* allocate a larger buffer and append the line number to the end */
  pMessage = HeapAlloc(GetProcessHeap(), 0L, 
    (lstrlen(pBuffer) * sizeof(TCHAR))
    + (lstrlen(szFile) * sizeof(TCHAR))
    + (15 * sizeof(TCHAR)));
  wsprintf(pMessage, TEXT("%s\n\r(file %s line %u)"),
    (PTSTR)pBuffer, (PTSTR)szFile, iLine);

  /* display the message in a message box */
  MessageBox(hwndParent, pMessage, gszAppTitle,
    MB_ICONEXCLAMATION | MB_OK);

  /* release the buffers allocated by FormatMessage and by us */
  LocalFree(pBuffer);
  HeapFree(GetProcessHeap(), 0L, pMessage);
}


