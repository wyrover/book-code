
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
// MODULE    : DEBMisc.C
// PURPOSE   : Miscellaneous support functions for the Debug Event Browser
// FUNCTIONS :
//   StartDebuggee()               - starts a new debuggee process
//   AttachToDebuggee()            - attaches to an existing process
//   EnumProcessListFunc()         - enumeration func for active processes
//   GetDebuggeeFileName()         - get the name of a debugee file to open
//   ChooseNewFont()               - choose a new font
//   ChooseNewBackColor()          - choose a new background color
//   MakeCommonDebugEventString()  - create common debug event info string
//   CreateTextButtonBar()         - create a text button bar
//   CreateIconWindow()            - creates the icon window in the dialog
//   GetPrivateProfileSettings()   - gets stored profile settings
//   WritePrivateProfileSettings() - stores default settings in profile
//   WritePrivateProfileInt()      - opposite of GetPrivateProfileInt()
//   UpdateMenuSettings()          - update menu check marks
//   OutOfMemoryMessageBox()       - displays an out of memory message box
//   MaxDebuggeesMessageBox()      - displays max debuggees exceeded
//   ErrorMessageBox()             - displays an error message box
//   SubclassWindow()              - generic window subclass func
//   SendWmSizeMessage()           - sends WM_SIZE with current size
//   GetPathFromFullPathName()     - extracts path from a pathname
//   CopyListBoxToClipboard()      - copies a listbox into the clipboard
//   ListBoxInsert()               - insert a string into a listbox
//   ListBoxPrintF()               - printf for listboxes
//   StringPrintF()                - formats a string buffer
//   StringAppendF()               - appends a formated string
// COMMENTS  :
//
// ************************************************************************
#define   STRICT               // enable strict typing
#include <Windows.H>           // required for all Windows applications
#include <CommDlg.H>           // GetOpenFileName(), ChooseFont(), etc.
#include <Dlgs.H>              // templates and defines for the common dialogs
#include <StdArg.H>            // va_list, va_start()

#include "LinkList.H"          //
#include "ToolBar.H"           //
#include "DEBMisc.H"           //


// ************************************************************************
// FUNCTION : StartDebuggee( LPTSTR, HWND )
// PURPOSE  : starts a new debuggee process given a filename
// COMMENTS :
//   Return TRUE on success else FALSE.
// ************************************************************************
BOOL
StartDebuggee( LPTSTR lpszDebuggeeFileName, HWND hWndListBox )
{
  static PDEB_STARTUP_INFO pDebStartupInfo;
  static BOOL              fFirstTime = TRUE;

  static TCHAR szDebuggeeTitle[128];

  HANDLE hDebugEventThread;
  DWORD  idDebugEventThread;

  //-- Load resource strings and init the OpenFileName struct
  //   (do this only one time)
  if( fFirstTime ) {
    fFirstTime = FALSE;
    LoadString( NULL, IDS_OFN_DEBUGGEE_TITLE, szDebuggeeTitle,
      sizeof(szDebuggeeTitle)/sizeof(TCHAR) );
  }

  //-- allocate the Debuggee Information structure
  pDebStartupInfo = NULL;
  pDebStartupInfo = (PDEB_STARTUP_INFO) VirtualAlloc(
                                    pDebStartupInfo,
                                    sizeof( DEB_STARTUP_INFO ),
                                    MEM_RESERVE | MEM_COMMIT,
                                    PAGE_READWRITE );

  //-- init the StartupInfo struct
  (pDebStartupInfo->StartupInfo).cb          = sizeof( STARTUPINFO );
  (pDebStartupInfo->StartupInfo).lpDesktop   = NULL;
  (pDebStartupInfo->StartupInfo).lpTitle     = szDebuggeeTitle;
  (pDebStartupInfo->StartupInfo).dwX         = 0;
  (pDebStartupInfo->StartupInfo).dwY         = 0;
  (pDebStartupInfo->StartupInfo).dwXSize     = 0;
  (pDebStartupInfo->StartupInfo).dwYSize     = 0;
  (pDebStartupInfo->StartupInfo).dwFlags     = (DWORD) NULL;
  (pDebStartupInfo->StartupInfo).wShowWindow = SW_SHOWDEFAULT;

  (pDebStartupInfo->ProcessInfo).hProcess = NULL;

  //-- init other debuggee info
  pDebStartupInfo->fActive       = FALSE;
  pDebStartupInfo->dwProcessId   = (DWORD) NULL;
  pDebStartupInfo->lpstrFileName = NULL;
  pDebStartupInfo->lpstrPathName = lpszDebuggeeFileName;
  pDebStartupInfo->hWndListBox   = hWndListBox;

  //-- now start and detach the debug event processing thread
  if( !( hDebugEventThread = CreateThread(
                               (LPSECURITY_ATTRIBUTES) NULL,
                               (DWORD) 0,
                               (LPTHREAD_START_ROUTINE) DebugEventThread,
                               (LPVOID) pDebStartupInfo,
                               (DWORD) NULL,
                               (LPDWORD) &idDebugEventThread) ) ) {

    VirtualFree( pDebStartupInfo, 0, MEM_RELEASE );
    return( FALSE );
  }
  else{
    CloseHandle( hDebugEventThread );
  }

  return( TRUE );
}


// ************************************************************************
// FUNCTION : AttachToDebuggee( DWORD, HWND )
// PURPOSE  : attaches to a currently running process
// COMMENTS :
//   Return TRUE on success else FALSE.
// ************************************************************************
BOOL
AttachToDebuggee( DWORD dwProcessId, HWND hWndListBox )
{
  static PDEB_STARTUP_INFO pDebStartupInfo;
  static BOOL              fFirstTime = TRUE;

  static TCHAR szDebuggeeTitle[128];

  HANDLE hDebugEventThread;
  DWORD  idDebugEventThread;

  //-- Load resource strings and init the OpenFileName struct
  //   (do this only one time)
  if( fFirstTime ) {
    fFirstTime = FALSE;
    LoadString( NULL, IDS_OFN_DEBUGGEE_TITLE, szDebuggeeTitle,
      sizeof(szDebuggeeTitle)/sizeof(TCHAR) );
  }

  //-- allocate the Debuggee Information structure
  pDebStartupInfo = NULL;
  pDebStartupInfo = (PDEB_STARTUP_INFO) VirtualAlloc(
                                    pDebStartupInfo,
                                    sizeof( DEB_STARTUP_INFO ),
                                    MEM_RESERVE | MEM_COMMIT,
                                    PAGE_READWRITE );

  //-- init the StartupInfo struct
  (pDebStartupInfo->StartupInfo).cb          = sizeof( STARTUPINFO );
  (pDebStartupInfo->StartupInfo).lpDesktop   = NULL;
  (pDebStartupInfo->StartupInfo).lpTitle     = szDebuggeeTitle;
  (pDebStartupInfo->StartupInfo).dwX         = 0;
  (pDebStartupInfo->StartupInfo).dwY         = 0;
  (pDebStartupInfo->StartupInfo).dwXSize     = 0;
  (pDebStartupInfo->StartupInfo).dwYSize     = 0;
  (pDebStartupInfo->StartupInfo).dwFlags     = (DWORD) NULL;
  (pDebStartupInfo->StartupInfo).wShowWindow = SW_SHOWDEFAULT;

  (pDebStartupInfo->ProcessInfo).hProcess = NULL;

  //-- init other debuggee info
  pDebStartupInfo->fActive       = TRUE;
  pDebStartupInfo->dwProcessId   = dwProcessId;
  pDebStartupInfo->lpstrFileName = NULL;
  pDebStartupInfo->lpstrPathName = NULL;
  pDebStartupInfo->hWndListBox   = hWndListBox;

  //-- now start and detach the debug event processing thread
  if( !( hDebugEventThread = CreateThread(
                               (LPSECURITY_ATTRIBUTES) NULL,
                               (DWORD) 0,
                               (LPTHREAD_START_ROUTINE) DebugEventThread,
                               (LPVOID) pDebStartupInfo,
                               (DWORD) NULL,
                               (LPDWORD) &idDebugEventThread) ) ) {

    VirtualFree( pDebStartupInfo, 0, MEM_RELEASE );
    return( FALSE );
  }
  else{
    CloseHandle( hDebugEventThread );
  }

  return( TRUE );
}


// ************************************************************************
// FUNCTION : EnumProcessListFunc( HWND, LPARAM )
// PURPOSE  : Callback function for EnumWindows
// COMMENTS : Inserts found window title strings into the listbox.
//            Return !NULL to continue enumeration.
// ************************************************************************
BOOL CALLBACK
EnumProcessListFunc( HWND hWnd, LPARAM lParam )
{
  static DWORD dwCurrentProcessId;
  static BOOL  fFirstTime = TRUE;
  static LONG  MaxStrLen  = 0;
  static TCHAR TextBuffer[256];

  if( fFirstTime ) {
    fFirstTime = FALSE;
    dwCurrentProcessId = GetCurrentProcessId();
  }

  if( hWnd ) {
    GetWindowText( hWnd, (LPTSTR) &TextBuffer, sizeof(TextBuffer) );
    if( *TextBuffer ) {
      DWORD  dwProcessId;

      GetWindowThreadProcessId( hWnd, &dwProcessId );
      if( dwProcessId != dwCurrentProcessId ) {
        LONG Index;
        HWND hWndListBox = (HWND) lParam;

        Index = ListBoxInsert( hWndListBox, &MaxStrLen, TextBuffer );
        SendMessage( hWndListBox, LB_SETITEMDATA, (WPARAM) Index,
          (LPARAM) dwProcessId );
      }
    }
  }

  return( TRUE );
}


// ************************************************************************
// FUNCTION : GetDebuggeeFileName( LPTSTR, HWND )
// PURPOSE  : Get the name of a debugee file to open.
// COMMENTS :
//   Return TRUE on success else FALSE.
// ************************************************************************
BOOL
GetDebuggeeFileName( LPTSTR lpszDebuggeeFileName, HWND hWnd )
{
  static BOOL  fFirstTime = TRUE;
  static TCHAR szFilter[128]         = TEXT("");
  static TCHAR szTitle[64]           = TEXT("");
  static TCHAR szFileTitle[MAX_PATH] = TEXT("");
  static TCHAR szFile[MAX_PATH]      = TEXT("");
  static TCHAR szDefExt[8]           = TEXT("");
  static OPENFILENAME OpenFileName;

  //-- Load resource strings and init the OpenFileName struct
  //   (do this only one time)
  if( fFirstTime ) {
    static UINT FilterNameLen;

    fFirstTime = FALSE;
    FilterNameLen = (UINT) LoadString( NULL, IDS_OFN_FILTERNAME,
                             szFilter, sizeof(szFilter) );
    FilterNameLen++;
    LoadString( NULL, IDS_OFN_FILTER, &szFilter[FilterNameLen],
      sizeof(szFilter)-FilterNameLen );
    LoadString( NULL, IDS_OFN_TITLE, szTitle, sizeof(szTitle) );

    OpenFileName.lStructSize       = sizeof(OPENFILENAME);
    OpenFileName.hwndOwner         = hWnd;
    OpenFileName.hInstance         = (HANDLE) NULL;
    OpenFileName.lpstrFilter       = szFilter;
    OpenFileName.lpstrCustomFilter = NULL;
    OpenFileName.nMaxCustFilter    = (DWORD) NULL;
    OpenFileName.nFilterIndex      = 1L;
    OpenFileName.lpstrFile         = szFile;
    OpenFileName.nMaxFile          = sizeof(szFile);
    OpenFileName.lpstrFileTitle    = szFileTitle;
    OpenFileName.nMaxFileTitle     = sizeof(szFileTitle);
    OpenFileName.lpstrInitialDir   = (Profile.fSavedDirectory ? Profile.szInitialDir : NULL);
    OpenFileName.lpstrTitle        = (LPTSTR) szTitle;
    OpenFileName.Flags             = OFN_HIDEREADONLY;
    OpenFileName.nFileOffset       = (WORD) NULL;
    OpenFileName.nFileExtension    = (WORD) NULL;
    OpenFileName.lpstrDefExt       = szDefExt;
    OpenFileName.lCustData         = (DWORD) NULL;
    OpenFileName.lpfnHook          = (LPOFNHOOKPROC) NULL;
    OpenFileName.lpTemplateName    = (LPTSTR) NULL;
  }

  if( !GetOpenFileName( &OpenFileName ) ) {
    return( FALSE );
  }

  //-- store recent directory by stripping off the EXE name from the full path
  GetPathFromFullPathName( (LPTSTR) OpenFileName.lpstrFile, Profile.szInitialDir,
    sizeof(Profile.szInitialDir) );

  //-- copy name to return buffer
  lstrcpy( lpszDebuggeeFileName, OpenFileName.lpstrFile );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : ChooseFontHookProc(  )
// PURPOSE  : Disable the Effects group and its contents
// COMMENTS :
// ************************************************************************
LRESULT CALLBACK
ChooseFontHookProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  UNREFERENCED_PARAMETER( lParam );
  UNREFERENCED_PARAMETER( wParam );

  switch( uMsg ) {

    case WM_INITDIALOG:
      ShowWindow( GetDlgItem(hDlg, grp1), SW_HIDE );
      ShowWindow( GetDlgItem(hDlg, chx1), SW_HIDE );
      ShowWindow( GetDlgItem(hDlg, chx2), SW_HIDE );
      break;

  }

  return( FALSE );
}


// ************************************************************************
// FUNCTION : ChooseNewFont( HWND )
// PURPOSE  : Choose a new font for a listbox
// COMMENTS : Return TRUE on success else FALSE
// ************************************************************************
BOOL
ChooseNewFont( HWND hWndListBox )
{
  static CHOOSEFONT ChooseFontStruct;
  static BOOL       fFirstTime = TRUE;
  HFONT             hFont;

  if( fFirstTime ) {
    fFirstTime = FALSE;

    ChooseFontStruct.lStructSize    = sizeof( CHOOSEFONT );
    ChooseFontStruct.hwndOwner      = hWndListBox;
    ChooseFontStruct.hDC            = (HDC) NULL;
    ChooseFontStruct.lpLogFont      = &(Profile.LogFont);
    ChooseFontStruct.iPointSize     = (INT) NULL;
    ChooseFontStruct.Flags          = CF_INITTOLOGFONTSTRUCT |
                                      CF_SCREENFONTS | CF_EFFECTS |
                                      CF_ENABLEHOOK;
    ChooseFontStruct.rgbColors      = Profile.rgbForeColor;
    ChooseFontStruct.lCustData      = (DWORD) NULL;
    ChooseFontStruct.lpfnHook       = (LPCFHOOKPROC) ChooseFontHookProc;
    ChooseFontStruct.lpTemplateName = (LPTSTR) NULL;
    ChooseFontStruct.hInstance      = (HANDLE) NULL;
    ChooseFontStruct.lpszStyle      = (LPTSTR) NULL;
    ChooseFontStruct.nFontType      = SCREEN_FONTTYPE;
    ChooseFontStruct.nSizeMin       = (INT) NULL;
    ChooseFontStruct.nSizeMax       = (INT) NULL;
  }

  if( ChooseFont( &ChooseFontStruct ) ) {
    HDC    hDC;

    hFont = CreateFontIndirect( &(Profile.LogFont) );
    hDC = GetDC( hWndListBox );
    SelectObject( hDC, hFont );
    Profile.rgbForeColor = ChooseFontStruct.rgbColors;
    InvalidateRect( hWndListBox, NULL, TRUE );
    SendMessage( hWndListBox, WM_CTLCOLORLISTBOX, (DWORD) hDC,
      (LONG) hWndListBox );
    SendMessage( hWndListBox, WM_SETFONT, (DWORD) hFont, TRUE );
    ReleaseDC( hWndListBox, hDC );
  }

  return( TRUE );
}


// ************************************************************************
// FUNCTION : ChooseNewBackColor( HWND )
// PURPOSE  : Choose the background color for a listbox
// COMMENTS : Return TRUE on success else FALSE
// ************************************************************************
BOOL
ChooseNewBackColor( HWND hWndListBox )
{
  static BOOL        fFirstTime = TRUE;
  static CHOOSECOLOR ChooseColorStruct;
  static DWORD       dwCustColors[16];

  if( fFirstTime ) {
    int i;

    fFirstTime = FALSE;

    //-- set the custom colors to white
    for( i = 0; i < 16; i++ )
      dwCustColors[i] = RGB(255,255,255);

    ChooseColorStruct.lStructSize    = sizeof( CHOOSECOLOR );
    ChooseColorStruct.hwndOwner      = hWndListBox;
    ChooseColorStruct.hInstance      = (HANDLE) NULL;
    ChooseColorStruct.rgbResult      = Profile.rgbBackColor;
    ChooseColorStruct.lpCustColors   = (LPDWORD) dwCustColors;
    ChooseColorStruct.Flags          = CC_RGBINIT;
    ChooseColorStruct.lCustData      = 0L;
    ChooseColorStruct.lpfnHook       = (LPCCHOOKPROC) NULL;
    ChooseColorStruct.lpTemplateName = (LPTSTR) NULL;
  }

  if( ChooseColor( &ChooseColorStruct ) ) {
    HDC    hDC;

    Profile.rgbBackColor = (COLORREF) ChooseColorStruct.rgbResult;
    hDC = GetDC( hWndListBox );
    SendMessage( hWndListBox, WM_CTLCOLORLISTBOX, (DWORD) hDC,
      (LONG) hWndListBox );
    ReleaseDC( hWndListBox, hDC );
    InvalidateRect( hWndListBox, NULL, TRUE );
  }

  return( TRUE );
}


// ************************************************************************
// FUNCTION : MakeCommonDebugEventString( LPTSTR, LPDEBUG_EVENT )
// PURPOSE  : Fill in the Debug Event information into a string buffer
// COMMENTS : The string buffer contains the information common to all
//            debug events ( PID, TID ).
// ************************************************************************
BOOL
MakeCommonDebugEventString( LPTSTR lpszBuffer, LPDEBUG_EVENT lpDebugEvent )
{
  StringPrintF( lpszBuffer, TEXT( "PID:0x%X \t TID:0x%X \t " ),
    lpDebugEvent->dwProcessId, lpDebugEvent->dwThreadId );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : CreateTextButtonBar( HWND, LPINT )
// PURPOSE  : Creates a Text Button Bar from the resource strings.
// COMMENTS : Returns window handle to the created text button bar
// ************************************************************************
HWND
CreateTextButtonBar( HWND hWndParent, LPINT lpTextButtonBarHeight )
{
  TEXTBUTTON   TextButton[9];
  static TCHAR szButtonOpen[32];
  static TCHAR szButtonAttach[32];
  static TCHAR szButtonCut[32];
  static TCHAR szButtonCopy[32];
  static TCHAR szButtonDelete[32];
  static TCHAR szButtonHelp[32];

  LoadString( NULL, IDS_BUTTON_OPEN,   szButtonOpen,   sizeof(szButtonOpen)/sizeof(TCHAR)   );
  LoadString( NULL, IDS_BUTTON_ATTACH, szButtonAttach, sizeof(szButtonAttach)/sizeof(TCHAR) );
  LoadString( NULL, IDS_BUTTON_CUT,    szButtonCut,    sizeof(szButtonCut)/sizeof(TCHAR)    );
  LoadString( NULL, IDS_BUTTON_COPY,   szButtonCopy,   sizeof(szButtonCopy)/sizeof(TCHAR)   );
  LoadString( NULL, IDS_BUTTON_DELETE, szButtonDelete, sizeof(szButtonDelete)/sizeof(TCHAR) );
  LoadString( NULL, IDS_BUTTON_HELP,   szButtonHelp,   sizeof(szButtonHelp)/sizeof(TCHAR)   );

  TextButton[0].lpButtonText = szButtonOpen;
  TextButton[0].idButton     = IDM_FILE_OPEN;
  TextButton[0].hWndButton   = NULL;

  TextButton[1].lpButtonText = szButtonAttach;
  TextButton[1].idButton     = IDM_FILE_ATTACH;
  TextButton[1].hWndButton   = NULL;

  TextButton[2].lpButtonText = NULL;
  TextButton[2].idButton     = TB_SPACE;
  TextButton[2].hWndButton   = NULL;

  TextButton[3].lpButtonText = szButtonCut;
  TextButton[3].idButton     = IDM_EDIT_CUT;
  TextButton[3].hWndButton   = NULL;

  TextButton[4].lpButtonText = szButtonCopy;
  TextButton[4].idButton     = IDM_EDIT_COPY;
  TextButton[4].hWndButton   = NULL;

  TextButton[5].lpButtonText = szButtonDelete;
  TextButton[5].idButton     = IDM_EDIT_DELETE;
  TextButton[5].hWndButton   = NULL;

  TextButton[6].lpButtonText = NULL;
  TextButton[6].idButton     = TB_SPACE;
  TextButton[6].hWndButton   = NULL;

  TextButton[7].lpButtonText = szButtonHelp;
  TextButton[7].idButton     = IDM_HELP_CONTENTS;
  TextButton[7].hWndButton   = NULL;

  TextButton[8].lpButtonText = (LPTSTR) NULL;
  TextButton[8].idButton     = (INT)   NULL;
  TextButton[8].hWndButton   = (HWND)  NULL;

  return( TextButtonBar( hWndParent, TextButton, lpTextButtonBarHeight ) );
}


// ************************************************************************
// FUNCTION : CreateIconWindow( HWND, LPCTSTR )
// PURPOSE  :
// COMMENTS :
// ************************************************************************
HWND
CreateIconWindow( HWND hDlg, LPCTSTR lpstrIcon )
{
  return ( CreateWindow( TEXT( "static" ), lpstrIcon,
             WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | SS_ICON,
             7, 5, 0, 0,
             hDlg, NULL, GetModuleHandle( NULL ), NULL ) );
}


// ************************************************************************
// FUNCTION : GetPrivateProfileSettings( LPCTSTR, LPCTSTR, PPROFILE )
// PURPOSE  : Retrieves the stored preferences and options from the
//            profile (registry)
// COMMENTS :
// ************************************************************************
BOOL
GetPrivateProfileSettings( LPCTSTR lpszAppTitle, LPCTSTR lpszIniPathName,
  PPROFILE pProfile )
{
  pProfile->xPos       = (INT) GetPrivateProfileInt(  lpszAppTitle, TEXT( "xPos" ),       CW_USEDEFAULT, lpszIniPathName );
  pProfile->yPos       = (INT) GetPrivateProfileInt(  lpszAppTitle, TEXT( "yPos" ),       CW_USEDEFAULT, lpszIniPathName );
  pProfile->nWidth     = (INT) GetPrivateProfileInt(  lpszAppTitle, TEXT( "nWidth" ),     CW_USEDEFAULT, lpszIniPathName );
  pProfile->nHeight    = (INT) GetPrivateProfileInt(  lpszAppTitle, TEXT( "nHeight" ),    CW_USEDEFAULT, lpszIniPathName );
  pProfile->fMaximized = (BOOL) GetPrivateProfileInt( lpszAppTitle, TEXT( "fMaximized" ), (INT) FALSE,   lpszIniPathName );
  pProfile->fMinimized = (BOOL) GetPrivateProfileInt( lpszAppTitle, TEXT( "fMinimized" ), (INT) FALSE,   lpszIniPathName );

  pProfile->fToolBar         = (BOOL) GetPrivateProfileInt( lpszAppTitle, TEXT( "fToolBar" ),         (INT) TRUE,  lpszIniPathName );
  pProfile->fSavedDirectory  = (BOOL) GetPrivateProfileInt( lpszAppTitle, TEXT( "fSavedDirectory" ),  (INT) TRUE,  lpszIniPathName );
  GetPrivateProfileString( lpszAppTitle, TEXT( "szInitialDir" ), NULL, pProfile->szInitialDir,
    sizeof( pProfile->szInitialDir ), lpszIniPathName );

  pProfile->LogFont.lfHeight         = (LONG) GetPrivateProfileInt( lpszAppTitle, TEXT( "lfHeight" ),         (INT) 13   ,       lpszIniPathName );
  pProfile->LogFont.lfWidth          = (LONG) GetPrivateProfileInt( lpszAppTitle, TEXT( "lfWidth" ),          (INT) 0    ,       lpszIniPathName );
  pProfile->LogFont.lfWeight         = (LONG) GetPrivateProfileInt( lpszAppTitle, TEXT( "lfWeight" ),         (INT) 400  ,       lpszIniPathName );
  pProfile->LogFont.lfItalic         = (BYTE) GetPrivateProfileInt( lpszAppTitle, TEXT( "lfItalic" ),         (INT) FALSE,       lpszIniPathName );
  pProfile->LogFont.lfUnderline      = (BYTE) GetPrivateProfileInt( lpszAppTitle, TEXT( "lfUnderline" ),      (INT) FALSE,       lpszIniPathName );
  pProfile->LogFont.lfStrikeOut      = (BYTE) GetPrivateProfileInt( lpszAppTitle, TEXT( "lfStrikeOut" ),      (INT) FALSE,       lpszIniPathName );
  pProfile->LogFont.lfPitchAndFamily = (BYTE) GetPrivateProfileInt( lpszAppTitle, TEXT( "lfPitchAndFamily" ), (INT) FF_DONTCARE, lpszIniPathName );
  GetPrivateProfileString( lpszAppTitle, TEXT( "lfFaceName" ), TEXT( "System" ), pProfile->LogFont.lfFaceName,
    LF_FACESIZE, lpszIniPathName );
  pProfile->rgbForeColor = (COLORREF) GetPrivateProfileInt( lpszAppTitle, TEXT( "rgbForeColor" ),
                            (INT) GetSysColor(COLOR_WINDOWTEXT), lpszIniPathName );
  pProfile->rgbBackColor = (COLORREF) GetPrivateProfileInt( lpszAppTitle, TEXT( "rgbBackColor" ),
                            (INT) GetSysColor(COLOR_WINDOW), lpszIniPathName );

  pProfile->fClearOnNew      = (BOOL) GetPrivateProfileInt( lpszAppTitle, TEXT( "fClearOnNew" ),      (INT) TRUE,  lpszIniPathName );
  pProfile->fVerbose         = (BOOL) GetPrivateProfileInt( lpszAppTitle, TEXT( "fVerbose" ),         (INT) FALSE, lpszIniPathName );
  pProfile->fShowSymbols     = (BOOL) GetPrivateProfileInt( lpszAppTitle, TEXT( "fShowSymbols" ),     (INT) FALSE, lpszIniPathName );
  pProfile->DebugMode        = (LONG) GetPrivateProfileInt( lpszAppTitle, TEXT( "DebugMode" ),        (INT) DEBUG_PROCESS        , lpszIniPathName );
  pProfile->DebuggeePriority = (LONG) GetPrivateProfileInt( lpszAppTitle, TEXT( "DebuggeePriority" ), (INT) NORMAL_PRIORITY_CLASS, lpszIniPathName );
  pProfile->DebugErrorLevel  = (DWORD) GetPrivateProfileInt( lpszAppTitle, TEXT( "DebugErrorLevel" ), (INT) 0, lpszIniPathName );
  pProfile->fSavePreferences = (BOOL) GetPrivateProfileInt( lpszAppTitle, TEXT( "fSavePreferences" ), (INT) FALSE, lpszIniPathName );

  pProfile->fSaveOnExit      = (BOOL) GetPrivateProfileInt( lpszAppTitle, TEXT( "fSaveOnExit" ),      (INT) TRUE,  lpszIniPathName );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : WritePrivateProfileSettings( LPCTSTR, LPCTSTR, PPROFILE )
// PURPOSE  : Saves the preferences and options to the profile (registry)
// COMMENTS :
// ************************************************************************
BOOL
WritePrivateProfileSettings( LPCTSTR lpszAppTitle, LPCTSTR lpszIniPathName,
  PPROFILE pProfile )
{
  if( pProfile->fSaveOnExit ) {
    WritePrivateProfileInt( lpszAppTitle, TEXT( "xPos" ),       (INT) pProfile->xPos,       lpszIniPathName );
    WritePrivateProfileInt( lpszAppTitle, TEXT( "yPos" ),       (INT) pProfile->yPos,       lpszIniPathName );
    WritePrivateProfileInt( lpszAppTitle, TEXT( "nWidth" ),     (INT) pProfile->nWidth,     lpszIniPathName );
    WritePrivateProfileInt( lpszAppTitle, TEXT( "nHeight" ),    (INT) pProfile->nHeight,    lpszIniPathName );
    WritePrivateProfileInt( lpszAppTitle, TEXT( "fMaximized" ), (INT) pProfile->fMaximized, lpszIniPathName );
    WritePrivateProfileInt( lpszAppTitle, TEXT( "fMinimized" ), (INT) pProfile->fMinimized, lpszIniPathName );

    WritePrivateProfileInt( lpszAppTitle, TEXT( "fToolBar" ),         (INT) pProfile->fToolBar,         lpszIniPathName );
    WritePrivateProfileInt( lpszAppTitle, TEXT( "fSavedDirectory" ),  (INT) pProfile->fSavedDirectory,  lpszIniPathName );
    WritePrivateProfileString( lpszAppTitle, TEXT( "szInitialDir" ), pProfile->szInitialDir, lpszIniPathName );

    WritePrivateProfileInt( lpszAppTitle, TEXT( "lfHeight" ),         (INT) pProfile->LogFont.lfHeight,         lpszIniPathName );
    WritePrivateProfileInt( lpszAppTitle, TEXT( "lfWidth" ),          (INT) pProfile->LogFont.lfWidth,          lpszIniPathName );
    WritePrivateProfileInt( lpszAppTitle, TEXT( "lfWeight" ),         (INT) pProfile->LogFont.lfWeight,         lpszIniPathName );
    WritePrivateProfileInt( lpszAppTitle, TEXT( "lfItalic" ),         (INT) pProfile->LogFont.lfItalic,         lpszIniPathName );
    WritePrivateProfileInt( lpszAppTitle, TEXT( "lfUnderline" ),      (INT) pProfile->LogFont.lfUnderline,      lpszIniPathName );
    WritePrivateProfileInt( lpszAppTitle, TEXT( "lfStrikeOut" ),      (INT) pProfile->LogFont.lfStrikeOut,      lpszIniPathName );
    WritePrivateProfileInt( lpszAppTitle, TEXT( "lfPitchAndFamily" ), (INT) pProfile->LogFont.lfPitchAndFamily, lpszIniPathName );
    WritePrivateProfileString( lpszAppTitle, TEXT( "lfFaceName" ), pProfile->LogFont.lfFaceName, lpszIniPathName );
    WritePrivateProfileInt( lpszAppTitle, TEXT( "rgbForeColor" ), (INT) pProfile->rgbForeColor, lpszIniPathName );
    WritePrivateProfileInt( lpszAppTitle, TEXT( "rgbBackColor" ), (INT) pProfile->rgbBackColor, lpszIniPathName );

    if( pProfile->fSavePreferences ) {
      WritePrivateProfileInt( lpszAppTitle, TEXT( "fClearOnNew" ),  (INT) pProfile->fClearOnNew,  lpszIniPathName );
      WritePrivateProfileInt( lpszAppTitle, TEXT( "fVerbose" ),     (INT) pProfile->fVerbose,     lpszIniPathName );
      WritePrivateProfileInt( lpszAppTitle, TEXT( "fShowSymbols" ), (INT) pProfile->fShowSymbols, lpszIniPathName );
      WritePrivateProfileInt( lpszAppTitle, TEXT( "DebugMode" ),        (INT) pProfile->DebugMode,        lpszIniPathName );
      WritePrivateProfileInt( lpszAppTitle, TEXT( "DebuggeePriority" ), (INT) pProfile->DebuggeePriority, lpszIniPathName );
      WritePrivateProfileInt( lpszAppTitle, TEXT( "DebugErrorLevel" ), (INT) pProfile->DebugErrorLevel, lpszIniPathName );
    }
    WritePrivateProfileInt( lpszAppTitle, TEXT( "fSavePreferences" ), (INT) pProfile->fSavePreferences, lpszIniPathName );
  }
  WritePrivateProfileInt( lpszAppTitle, TEXT( "fSaveOnExit" ), (INT) pProfile->fSaveOnExit, lpszIniPathName );

  return( TRUE );
}


// ************************************************************************
// FUNCTION: WritePrivateProfileInt( LPCTSTR, LPCTSTR, INT, LPCTSTR )
// PURPOSE : Writes the value of an integer from a specified keyname within
//           a specified section of the Private Profile
// COMMENTS: Matched pair to GetPrivateProfileInt
// ************************************************************************
BOOL
WritePrivateProfileInt( LPCTSTR lpAppName, LPCTSTR lpKeyName, INT Value, LPCTSTR lpFileName )
{
  TCHAR ValBuf[16];

  wsprintf( ValBuf, TEXT( "%i" ), Value);

  return( WritePrivateProfileString( lpAppName, lpKeyName, ValBuf, lpFileName ) );
}


// ************************************************************************
// FUNCTION : UpdateMenuSettings( HWND )
// PURPOSE  : Adds check marks to active menu items
// COMMENTS :
// ************************************************************************
BOOL
UpdateMenuSettings( HWND hWnd )
{
  if( Profile.fToolBar )
    CheckMenuItem( GetMenu(hWnd), IDM_OPTIONS_TOOLBAR, MF_CHECKED );
  if( Profile.fSavedDirectory )
    CheckMenuItem( GetMenu(hWnd), IDM_OPTIONS_SAVEDDIR, MF_CHECKED );
  if( Profile.fSaveOnExit )
    CheckMenuItem( GetMenu(hWnd), IDM_OPTIONS_SAVEONEXIT, MF_CHECKED );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : OutOfMemoryMessageBox( HWND )
// PURPOSE  : If called, displays an "Out of Memory" message box
// COMMENTS :
// ************************************************************************
BOOL
OutOfMemoryMessageBox( HWND hWndOwner )
{
  MessageBox(
    hWndOwner,
    TEXT( "Out of Memory" ),
    TEXT( "Memory Error" ),
    MB_ICONSTOP | MB_APPLMODAL );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : MaxDebuggeesMessageBox( HWND )
// PURPOSE  : If called, displays an "Max Debuggees" message box
// COMMENTS :
// ************************************************************************
BOOL
MaxDebuggeesMessageBox( HWND hWndOwner )
{
  MessageBox( hWndOwner,
    TEXT( "This build of the Debug Event Browser\n" )
    TEXT( "is limited to debugging only one debuggee\n" )
    TEXT( "at a time." ),
    TEXT( "Cannot Open Debuggee" ),
    MB_OK | MB_ICONEXCLAMATION );

  return( TRUE );
}


// ========================================================================
// helper functions
// ========================================================================


// ************************************************************************
// FUNCTION : ErrorMessageBox( LPCTSTR, LPCTSTR, LPCTSTR, INT )
// PURPOSE  : Displays an error message box with various error information
//            and allows the user to terminate or continue the process.
//            For a Win32 Application, GetLastError and FormatMessage are
//            used to retrieve the last API error code and error message.
// COMMENTS :
// ************************************************************************
BOOL
ErrorMessageBox( LPCTSTR lpszText, LPCTSTR lpszTitle, LPCTSTR lpszFile,
  INT Line )
{
  #define ERROR_BUFFER_SIZE 512

  static TCHAR Format[] =
    TEXT( "%s\n\n"                                  )
    TEXT( "-- Error Information --\n"               )
    TEXT( "File : %s\n"                             )
    TEXT( "Line : %d\n"                             )
    TEXT( "Error Number : %d\n"                     )
    TEXT( "Error Message : %s\n"                    )
    TEXT( "\n"                                      )
    TEXT( "Press OK to terminate this application." );

  LPTSTR lpFormatMessageBuffer;
  DWORD  dwFormatMessage;
  DWORD  dwGetLastError;
  HLOCAL hMessageBoxBuffer;
  LPVOID lpMessageBoxBuffer;

  //-- perform a simple check on the needed buffer size
  if( lstrlen(lpszText) > (ERROR_BUFFER_SIZE - lstrlen(Format)) )
    return( FALSE );

  //-- allocate the message box buffer
  hMessageBoxBuffer  = LocalAlloc( LMEM_MOVEABLE, ERROR_BUFFER_SIZE );
  lpMessageBoxBuffer = LocalLock( hMessageBoxBuffer );

  //-- get the system error and system error message
  dwGetLastError = GetLastError();
  dwFormatMessage = FormatMessage(
                      FORMAT_MESSAGE_ALLOCATE_BUFFER
                      | FORMAT_MESSAGE_FROM_SYSTEM,
                      NULL, dwGetLastError, LANG_NEUTRAL,
                      (LPTSTR) &lpFormatMessageBuffer, 0, NULL );
  if( !dwFormatMessage )
    lpFormatMessageBuffer = TEXT("FormatMessage() failed!");

  //-- format the error messge box string
  wsprintf( lpMessageBoxBuffer, Format, lpszText, lpszFile, Line,
    dwGetLastError, lpFormatMessageBuffer );

  // -- display the error and allow the user to terminate or continue
  if( MessageBox( NULL, lpMessageBoxBuffer, lpszTitle,
        MB_APPLMODAL | MB_ICONSTOP | MB_OKCANCEL )
    == IDOK )
    ExitProcess( 0 );

  //-- free all buffers
  if( dwFormatMessage )
    LocalFree( (HLOCAL) lpFormatMessageBuffer );
  LocalFree( (HLOCAL) hMessageBoxBuffer );

  return( TRUE );
}



// ************************************************************************
// FUNCTION : SubclassWindow( HWND, WNDPROC )
// PURPOSE  : Subclasses a window procedure
// COMMENTS : Returns the old window procedure
// ************************************************************************
WNDPROC
SubclassWindow( HWND hWnd, WNDPROC NewWndProc)
{
  WNDPROC OldWndProc;

  OldWndProc = (WNDPROC) GetWindowLong( hWnd, GWL_WNDPROC );
  SetWindowLong( hWnd, GWL_WNDPROC, (LONG) NewWndProc );

  return OldWndProc;
}


// ************************************************************************
// FUNCTION : SendWmSizeMessage( HWND )
// PURPOSE  : Sends a WM_SIZE message containing the current size
// COMMENTS : Forces a WM_SIZE message without actually changing the size
// ************************************************************************
BOOL
SendWmSizeMessage( HWND hWnd )
{
  RECT Rect;

  if( !GetClientRect( hWnd, &Rect ) )
    return( FALSE );

  return( SendMessage( hWnd, WM_SIZE, SIZENORMAL,
            MAKELONG( Rect.right - Rect.left, Rect.bottom - Rect.top) ) );
}


// ************************************************************************
// FUNCTION : GetPathFromFullPathName( LPCTSTR, LPTSTR, UINT )
// PURPOSE  : Extracts the path given a full pathname
// COMMENTS :
// ************************************************************************
UINT
GetPathFromFullPathName( LPCTSTR lpFullPathName, LPTSTR lpPathBuffer,
  UINT nPathBufferLength )
{
  UINT nLength;
  int i, j;

  if( (nLength = (UINT) lstrlen( lpFullPathName ) ) > nPathBufferLength )
    return( nLength );

  lstrcpy( lpPathBuffer, lpFullPathName );

  for( j = 0, i = nLength; (UINT)j < nLength;
                  j += IsDBCSLeadByte(lpPathBuffer[j]) ? 2 : 1){
      if( lpPathBuffer[j] == '\\' || lpPathBuffer[j] == ':' ){
          i = j;
      }
  }
  if( lpPathBuffer[i] == ':' )
    lpPathBuffer[i+1] = '\0';
  else
    lpPathBuffer[i] = '\0';

  return( (UINT) i );
}


// ************************************************************************
// FUNCTION : CopyListBoxToClipboard( HWND, LONG )
// PURPOSE  : Copies the entire contents of a listbox into the clipboard.
// COMMENTS : Returns TRUE on success, FALSE otherwise
// ************************************************************************
BOOL
CopyListBoxToClipboard( HWND hWndListBox, LONG MaxStrLen )
{
  LPTSTR  lpDataBuffer;
  HGLOBAL hDataBuffer;

  TCHAR  TempBuffer[256];
  DWORD  dwItemCount;
  DWORD  Count;
  LONG   StrLen;
  DWORD  dwMemSize;

  dwItemCount = (DWORD) SendMessage( hWndListBox, LB_GETCOUNT, 0 , 0 );
  dwMemSize = dwItemCount * (DWORD) MaxStrLen;

  //-- limit the size copied to the clipboard
  if( dwMemSize > 0xFFFFF )
    dwMemSize = 0xFFFFF;

  if( !(hDataBuffer = GlobalAlloc( GMEM_DDESHARE, dwMemSize ) ) ) {
    OutOfMemoryMessageBox( GetFocus() );
    return( FALSE );
  }
  if( !(lpDataBuffer = (LPTSTR) GlobalLock( hDataBuffer ) ) ) {
    GlobalFree( hDataBuffer );
    OutOfMemoryMessageBox( GetFocus() );
    return( FALSE );
  }

  *lpDataBuffer = '\0';

  for( Count = 0; Count < dwItemCount; Count++ ) {
    StrLen = SendMessage( hWndListBox, LB_GETTEXTLEN, Count, 0L );
    if( StrLen > (sizeof(TempBuffer)-3) )
      continue;
    StrLen = SendMessage( hWndListBox, LB_GETTEXT, Count, (LPARAM) TempBuffer );
    TempBuffer[StrLen]   = '\r';
    TempBuffer[StrLen+1] = '\n';
    TempBuffer[StrLen+2] = '\0';
    lstrcat( lpDataBuffer, TempBuffer );
  }

  GlobalUnlock( hDataBuffer );

  if( !OpenClipboard( hWndListBox ) ) {
    Sleep( 250 );  // wait a quarter second and try again.
    if( !OpenClipboard( hWndListBox ) ) {
      MessageBox( GetFocus(),
        TEXT( "Could not open the Clipboard!" ),
        TEXT( "Cannot Open Clipboard" ),
        MB_ICONSTOP | MB_APPLMODAL );
      GlobalFree( hDataBuffer );
      return( FALSE );
    }
  }
  if( !EmptyClipboard() ) {
    MessageBox( GetFocus(),
      TEXT( "Could not empty the Clipboard!" ),
      TEXT( "Cannot Empty Clipboard" ),
      MB_ICONSTOP | MB_APPLMODAL );
    GlobalFree( hDataBuffer );
    return( FALSE );
  }
  if( !SetClipboardData( CF_TEXT, hDataBuffer ) ) {
    MessageBox( GetFocus(),
      TEXT( "Could not copy data to the Clipboard!" ),
      TEXT( "Cannot Set Clipboard Data" ),
      MB_ICONSTOP | MB_APPLMODAL );
    GlobalFree( hDataBuffer );
    return( FALSE );
  }
  CloseClipboard();

  return( TRUE );
}


// ************************************************************************
// FUNCTION : ListBoxInsert( HWND, LPLONG, LPCTSTR )
// PURPOSE  : Inserts the string into the listbox.
// COMMENTS : Returns the index of the string inserted
// ************************************************************************
LONG
ListBoxInsert( HWND hWndListBox, LPLONG lpMaxStrLen, LPCTSTR lpszString )
{
  static LONG MaxTextExtent = 0;

  LONG        Index;

  if( lpszString == NULL ) {
    MaxTextExtent = 0;
    SendMessage( hWndListBox, LB_SETHORIZONTALEXTENT, 0, 0 );
    return( 0 );
  }

  if( hWndListBox != NULL ) {
    HDC  hDC;
    SIZE Size;
    LONG StrLen;

    if( (StrLen = lstrlen( lpszString)) > *lpMaxStrLen )
      *lpMaxStrLen = StrLen;
    hDC = GetDC( hWndListBox );
    GetTextExtentPoint( hDC, lpszString, StrLen, &Size );
    ReleaseDC( hWndListBox, hDC );
    if( Size.cx > MaxTextExtent ) {
      MaxTextExtent = Size.cx;
      SendMessage( hWndListBox, LB_SETHORIZONTALEXTENT, (WPARAM) (MaxTextExtent*1.1), 0 );
    }
    Index = SendMessage( hWndListBox, LB_ADDSTRING, 0, (LPARAM) lpszString );
    SendMessage( hWndListBox, LB_SETCURSEL, Index, 0 );
  }

  return( Index );
}


// ************************************************************************
// FUNCTION : ListBoxPrintF( HWND, LPCTSTR, ... )
// PURPOSE  : Inserts the string format into the listbox.
// COMMENTS : Returns the index of the last string inserted
// ************************************************************************
LONG
ListBoxPrintF( HWND hWndListBox, LPCTSTR szFormat, ... )
{
  static TCHAR szBuffer[ 1024 ];

  va_list valist;
  LONG    Index;
  LPTSTR  lpCurrentString, lpCurrentChar;

  va_start( valist, szFormat );
  wvsprintf( szBuffer, szFormat, valist );
  va_end( valist );

  //-- insert strings line-by-line
  for( lpCurrentString = lpCurrentChar = szBuffer; *lpCurrentChar != '\0'; ) {
    if( *lpCurrentChar == '\n' ) {
      LPTSTR lpNextChar = CharNext( lpCurrentChar );
      *lpCurrentChar = '\0';
      Index = ListBoxInsert( hWndListBox, &(Global.MaxStrLen), lpCurrentString );
      lpCurrentString = lpCurrentChar = lpNextChar;
    }
    else
      lpCurrentChar = CharNext( lpCurrentChar );
  }
  Index = ListBoxInsert( hWndListBox, &(Global.MaxStrLen), lpCurrentString );

  return( Index );
}


// ************************************************************************
// FUNCTION : StringPrintF( LPTSTR, LPCTSTR, ... )
// PURPOSE  : Formats a string buffer according to the format-control
//            string.  This function is wsprintf but the return
//            values is a pointer to the string instead of the count.
// COMMENTS :
// ************************************************************************
LPTSTR
StringPrintF( LPTSTR lpString, LPCTSTR szFormat, ...  )
{
  va_list valist;

  va_start( valist, szFormat );
  wvsprintf( lpString, szFormat, valist );
  va_end( valist );

  return( lpString );
}


// ************************************************************************
// FUNCTION : StringAppendF( LPTSTR, LPCTSTR, ... )
// PURPOSE  : Append a variable number of characters and values to the end
//            of an existing string buffer according to the format string
//            (which uses standard printf() formating notation).
// COMMENTS :
// ************************************************************************
BOOL
StringAppendF( LPTSTR lpszBuffer, LPCTSTR szFormat, ... )
{
  static TCHAR szLgBuffer[1024];

  va_list valist;
  int n;

  //-- add event specific information
  va_start( valist, szFormat );
  n = wvsprintf( szLgBuffer, szFormat, valist );
  va_end( valist );

  //-- append information to the string buffer
  lstrcat( lpszBuffer, szLgBuffer );

  return( TRUE );
}
