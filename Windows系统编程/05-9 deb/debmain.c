
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
// MODULE    : DEBMain.C
// PURPOSE   : A Win32 Application demonstrating the Debug APIs
// FUNCTIONS :
//   WinMain()                 - application entry point
//   MainWndProc()             - processes messages
//   ProcessCommandsWndProc()  - processes WM_COMMAND messages
//   PreferencesDlgProc()      - processes messages for "Preferences" dialog box
//   AttachDlgProc()           - processes messages for "Attach" dialog box
//   AboutDlgProc()            - processes messages for "About" dialog box
//   NewListBoxWndProc()       - subclass procedure to prevent listbox moving
//   TimerProc()               - timer procedure for animated icon
// COMMENTS  :
//
// ************************************************************************
#define   STRICT               // enable strict typing
#include <Windows.H>           // required for all Windows applications
#include "StdLib.H"            // __argc, __argv

#include "LinkList.H"          // double linked list package (OBJ)
#include "DEBDebug.H"          // debugging support functions
#include "DEBMisc.H"           // misc support functions
#include "DEBMain.H"           // specific to this module

// global data
// ------------------------------------------------------------------------
GLOBAL  Global;                // various global data
PROFILE Profile;               // various profile data

// internal data
// ------------------------------------------------------------------------
#define TIMEOUT_ANIMATED_ICON 150L // animated icon timeout
static TCHAR szSourceFileName[] = TEXT(__FILE__);

// location of various files
// ------------------------------------------------------------------------
TCHAR   szPath[MAX_PATH];         // path where the running application
                                  //  resides
TCHAR   szExePathName[MAX_PATH];  // full pathname of the application
TCHAR   szHelpPathName[MAX_PATH]; // full pathname of the application's
                                  //  help file
TCHAR   szIniPathName[MAX_PATH];  // full pathname of the application's
                                  //  ini file
// internal function prototypes
// ------------------------------------------------------------------------
LRESULT CALLBACK MainWndProc            ( HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK ProcessCommandsWndProc ( HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK NewListBoxWndProc      ( HWND, UINT, WPARAM, LPARAM );
BOOL    CALLBACK PreferencesDlgProc     ( HWND, UINT, WPARAM, LPARAM );
BOOL    CALLBACK AttachDlgProc          ( HWND, UINT, WPARAM, LPARAM );
BOOL    CALLBACK AboutDlgProc           ( HWND, UINT, WPARAM, LPARAM );
VOID    CALLBACK TimerProc              ( HWND, UINT, UINT, DWORD );


// ************************************************************************
// FUNCTION : WinMain( HINSTANCE, HINSTANCE, LPSTR, INT )
// PURPOSE  : initialize the window, process the message dispatch loop
// COMMENTS :
//
// ************************************************************************
INT WINAPI
WinMain( HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine,
  INT nCmdShow )
{
  MSG      Msg;
  WNDCLASS WndClass;
  HACCEL   hAccel;
  OSVERSIONINFO osvi;

  LPCTSTR lpszIconName    = TEXT( "DebugIcon"  );
  LPCTSTR lpszMenuName    = TEXT( "DebugMenu"  );
  LPCTSTR lpszClassName   = TEXT( "DebugClass" );
  LPCTSTR lpszAccelName   = TEXT( "DebugAccel" );
  LPCTSTR lpszIniFileExt  = TEXT( "INI"        );
  LPCTSTR lpszHelpFileExt = TEXT( "HLP"        );

  Global.hInstance = hInstance;
  Global.dwActiveDebuggees = 0;

  //-- Load the "A Windows API Failed" resource string
  {
    TCHAR szApiFailed[] = TEXT( "A Windows API Failed" );

    if( !LoadString( Global.hInstance, IDS_API_FAILED_MSG,
          Global.szApiFailed, sizeof(Global.szApiFailed) ) ) {
      ErrorMessageBox( TEXT( "First LoadString()" ),
        szApiFailed, szSourceFileName, __LINE__ );
      lstrcpy( Global.szApiFailed, szApiFailed );
    }
  }

  //-- Load all other resource strings
  if( !LoadString( Global.hInstance, IDS_APPNAME,  Global.szAppName,
         sizeof(Global.szAppName) ) )
    ErrorMessageBox( TEXT("LoadString()"),
      Global.szApiFailed, szSourceFileName, __LINE__ );
  if( !LoadString( Global.hInstance, IDS_SHORT_APPNAME,
         Global.szShortAppName, sizeof(Global.szShortAppName) ) )
    ErrorMessageBox( TEXT("LoadString()"),
      Global.szApiFailed, szSourceFileName, __LINE__ );

  //-- if compiled for Win32 (Unicode) and not Win32s then display
  //    notice and terminate


  //
  // Detect platform and exit gracefully if unsupported platform.
  //

  osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
  GetVersionEx (&osvi);
  if (osvi.dwPlatformId == VER_PLATFORM_WIN32s) {
    TCHAR szTitleBuffer[64];
    TCHAR szTextBuffer[256];

    if( !LoadString( Global.hInstance, IDS_WINDOWS_NT_REQUIRED_TITLE,
           szTitleBuffer, sizeof(szTitleBuffer) ) )
      ErrorMessageBox( TEXT("LoadString()"),
        Global.szApiFailed, szSourceFileName, __LINE__ );

    if( !LoadString( Global.hInstance, IDS_WINDOWS_NT_REQUIRED,
           szTextBuffer, sizeof(szTextBuffer) ) )
      ErrorMessageBox( TEXT("LoadString()"),
        Global.szApiFailed, szSourceFileName, __LINE__ );

    MessageBox( NULL, szTextBuffer, szTitleBuffer,
      MB_APPLMODAL | MB_ICONSTOP | MB_OK );
    return( -1 );
  }

  //-- register the debug event window class
  WndClass.style         = CS_DBLCLKS;
  WndClass.lpfnWndProc   = (WNDPROC) MainWndProc;
  WndClass.cbClsExtra    = (INT) NULL;
  WndClass.cbWndExtra    = (INT) NULL;
  WndClass.hInstance     = Global.hInstance;
  WndClass.hIcon         = LoadIcon( Global.hInstance, lpszIconName );
  WndClass.hCursor       = LoadCursor( NULL, (LPTSTR) IDC_ARROW );
  WndClass.hbrBackground = (HBRUSH) (COLOR_APPWORKSPACE+1);
  WndClass.lpszMenuName  = lpszMenuName;
  WndClass.lpszClassName = lpszClassName;

  if( !RegisterClass(&WndClass) ) {
    ErrorMessageBox( TEXT("RegisterClass()"),
      Global.szApiFailed, szSourceFileName, __LINE__ );
    return( FALSE );
  }

  //-- get application pathname and store the ini and help file pathname
  //   (which is located in the same directory as the application)
  GetModuleFileName( (HANDLE) NULL, szExePathName,
    sizeof(szExePathName)/sizeof(TCHAR) );
  GetPathFromFullPathName( szExePathName, szPath,
    sizeof(szPath)/sizeof(TCHAR) );
  wsprintf( szIniPathName,  TEXT( "%s\\%s.%s" ), szPath,
    Global.szShortAppName, lpszIniFileExt  );
  wsprintf( szHelpPathName, TEXT( "%s\\%s.%s" ), szPath,
    Global.szShortAppName, lpszHelpFileExt );

  //-- retrieve stored default location from private profile data
  GetPrivateProfileSettings( Global.szAppName, szIniPathName, &Profile );

  //-- Create a main window for this application instance
  Global.hWndMain = CreateWindow( lpszClassName, Global.szAppName,
                      WS_OVERLAPPEDWINDOW,
                      Profile.xPos, Profile.yPos,
                      Profile.nWidth, Profile.nHeight,
                      NULL, NULL, Global.hInstance, NULL );

  //-- If window could not be created, return "failure"
  if( !Global.hWndMain ) {
    ErrorMessageBox( TEXT("CreateWindow()"),
      Global.szApiFailed, szSourceFileName, __LINE__ );
    return( FALSE );
  }

  //-- Load main menu accelerators
  if( !(hAccel = LoadAccelerators( Global.hInstance, lpszAccelName) ) ) {
    ErrorMessageBox( TEXT("LoadAccelerators()"),
      Global.szApiFailed, szSourceFileName, __LINE__ );
    return( FALSE );
  }

  //-- modify the menu to reflect saved settings
  UpdateMenuSettings( Global.hWndMain );

  //-- Make the window visible; update its client area; and return "success"
  if( Profile.fMaximized )
    ShowWindow( Global.hWndMain, SW_SHOWMAXIMIZED );
  else if ( Profile.fMinimized )
    ShowWindow( Global.hWndMain, SW_SHOWMINIMIZED );
  else
    ShowWindow( Global.hWndMain, SW_SHOWDEFAULT );
  UpdateWindow( Global.hWndMain );

  //-- Acquire and dispatch messages until a WM_QUIT message is received.
  while( GetMessage( &Msg, NULL, 0, 0 ) ) {
    if( !TranslateAccelerator( Global.hWndMain, hAccel, &Msg ) ) {
      TranslateMessage( &Msg );
      DispatchMessage( &Msg );
    }
  }

  return( Msg.wParam );
  UNREFERENCED_PARAMETER( lpCmdLine );  // avoid warnings
  UNREFERENCED_PARAMETER( hPrevInst );  // always NULL under Windows NT
  UNREFERENCED_PARAMETER( nCmdShow  );  //
}


// ************************************************************************
// FUNCTION : MainWndProc( HWND, UINT, WPARAM, LPARAM )
// PURPOSE  : Processes uMsgs
// MESSAGES :
//   WM_COMMAND   - passed to ProcessCommandsWndProc()
//   WM_DESTROY   - destroy window
//    ...
// COMMENTS :
//
// ************************************************************************
LRESULT CALLBACK
MainWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  #define TOP_BORDER      4
  #define BOTTOM_BORDER   4
  #define SIDE_BORDER     4
  #define MIN_HEIGHT    128

  static HDC hDC;

  switch( uMsg ) {

    //-- forward all WM_COMMANDS to separate handler function
    case WM_COMMAND:
      return( ProcessCommandsWndProc( hWnd, uMsg, wParam, lParam) );

    //-- create debug event listbox
    case UM_CREATE_LISTBOX: {
      TCHAR  szWindowName[64];
      HFONT  hFont;

      LoadString( Global.hInstance, IDS_DEBUG_EVENTS, szWindowName,
        sizeof(szWindowName)/sizeof(TCHAR) );

      Global.hWndListBox = CreateWindow(
                           TEXT( "ListBox" ),
                           szWindowName,
                           WS_CHILD             | WS_VISIBLE      |
                           WS_CAPTION           | WS_VSCROLL      |
                           WS_HSCROLL           | LBS_NOTIFY      |
                           LBS_DISABLENOSCROLL  | LBS_USETABSTOPS |
                           LBS_NOINTEGRALHEIGHT,
                           (INT) SIDE_BORDER,
                           (INT) (Profile.fToolBar
                             ? (Global.ToolBarHeight + TOP_BORDER)
                             : TOP_BORDER),
                           (INT) Global.ListBoxSize.cx,
                           (INT) Global.ListBoxSize.cy,
                           hWnd, (HMENU) NULL, Global.hInstance, NULL );

      //-- Subclass the listbox so the user cannot move it
      Global.OldListBoxWndProc = SubclassWindow( Global.hWndListBox,
        (WNDPROC) NewListBoxWndProc );

      //-- set listbox font & background color
      hDC = GetDC( Global.hWndListBox );
      hFont = CreateFontIndirect( &(Profile.LogFont) );
      SelectObject( hDC, hFont );
      SendMessage( Global.hWndListBox, WM_CTLCOLORLISTBOX, (WPARAM) hDC,
        (LPARAM) Global.hWndListBox );
      SendMessage( Global.hWndListBox, WM_SETFONT, (WPARAM) hFont, TRUE );
      ReleaseDC( Global.hWndListBox, hDC );

      //-- if command line contains a debuggee name then
      //   start and detach the debug event processing thread
      if( __argc == 2 )
        StartDebuggee( (LPTSTR) __argv[1], Global.hWndListBox );

      return( FALSE );
    }

    //-- create ToolBar & send message to create the Debug Events listbox
    case WM_CREATE:
      Global.hWndToolBar = CreateTextButtonBar( hWnd, &Global.ToolBarHeight );
      if( Profile.fToolBar )
        ShowWindow( Global.hWndToolBar, SW_SHOW );
      PostMessage( hWnd, UM_CREATE_LISTBOX, 0, 0 );
      return( FALSE );

    //-- resize the debug event listbox when the window size changes
    case WM_SIZE:
      Global.ClientSize.cx = LOWORD( lParam );
      Global.ClientSize.cy = HIWORD( lParam );
      Global.ListBoxSize.cx = Global.ClientSize.cx - ( 2*SIDE_BORDER );
      Global.ListBoxSize.cy = max( Global.ClientSize.cy, MIN_HEIGHT
                                + TOP_BORDER )
        - (TOP_BORDER + BOTTOM_BORDER);

      if( Profile.fToolBar )
        Global.ListBoxSize.cy -= Global.ToolBarHeight;

      if( Global.hWndListBox != NULL)
        MoveWindow( Global.hWndListBox,
          (INT) SIDE_BORDER,
          (INT) (Profile.fToolBar ? Global.ToolBarHeight + TOP_BORDER : TOP_BORDER),
          (INT) Global.ListBoxSize.cx, (INT) Global.ListBoxSize.cy, TRUE );

      switch( wParam ) {

        case SIZE_RESTORED: {
          RECT rect;

          Profile.fMaximized = FALSE;
          Profile.fMinimized = FALSE;
          GetWindowRect( Global.hWndMain, &rect );
          Profile.nWidth  = (INT) (rect.right - rect.left);
          Profile.nHeight = (INT) (rect.bottom - rect.top);
          return( FALSE );
        }

        case SIZE_MAXIMIZED:
          Profile.fMaximized = TRUE;
          Profile.fMinimized = FALSE;
          Profile.xPos       = Global.xPosOld;
          Profile.yPos       = Global.yPosOld;
          return( FALSE );

        case SIZE_MINIMIZED:
          Profile.fMinimized = TRUE;
          Profile.fMaximized = FALSE;
          Profile.xPos       = Global.xPosOld;
          Profile.yPos       = Global.yPosOld;
          return( FALSE );

      }
      return( DefWindowProc(hWnd, uMsg, wParam, lParam) );

    //-- keep track of window position so it can be saved
    case WM_MOVE: {
      RECT rect;

      GetWindowRect( Global.hWndMain, &rect );
      Global.xPosOld      = Profile.xPos;
      Global.yPosOld      = Profile.yPos;
      Profile.xPos = (INT) rect.left;
      Profile.yPos = (INT) rect.top;
      return( FALSE );
    }

    //-- colorize the debug event listbox
    case WM_CTLCOLORLISTBOX: {
      LOGBRUSH LogBrush;

      LogBrush.lbStyle = BS_SOLID;
      LogBrush.lbColor = Profile.rgbBackColor;
      LogBrush.lbHatch = (LONG) NULL;

      SetTextColor( (HDC) wParam, Profile.rgbForeColor );
      SetBkColor( (HDC) wParam, Profile.rgbBackColor );

      return( (LPARAM) CreateBrushIndirect( &LogBrush ) );
    }

    //-- check if a debuggee is still active, save profile settings
    case WM_CLOSE:
      if( Global.dwActiveDebuggees ) {
        TCHAR  szExitBoxTitle[64];
        TCHAR  szExitBoxText[256];

        LoadString( Global.hInstance, IDS_EXIT_BOX_TITLE, szExitBoxTitle,
          sizeof(szExitBoxTitle)/sizeof(TCHAR) );
        LoadString( Global.hInstance, IDS_EXIT_BOX_TEXT, szExitBoxText,
          sizeof(szExitBoxText)/sizeof(TCHAR) );
        if ( MessageBox( hWnd, szExitBoxText, szExitBoxTitle,
               MB_YESNO | MB_ICONEXCLAMATION ) == IDNO )
          return( FALSE );
      }

      //-- store location information to private profile data
      WritePrivateProfileSettings( Global.szAppName, szIniPathName, &Profile );

      DestroyWindow( Global.hWndToolBar );
      DestroyWindow( Global.hWndListBox );
      DestroyWindow( Global.hWndMain );

      return( FALSE );

    case WM_DESTROY:
      if( Global.fHelpUsed )
        WinHelp( hWnd, szHelpPathName, (UINT) HELP_QUIT, (DWORD) NULL );
      PostQuitMessage( 0 );
      return( FALSE );

    default:               // Passes it on if unproccessed
      return( DefWindowProc(hWnd, uMsg, wParam, lParam) );
  }

  return( FALSE );
}


// ************************************************************************
// FUNCTION : ProcessCommandsWndProc( HWND, UINT, WPARAM, LPARAM )
// PURPOSE  : Processes WM_COMMAND messages for MainWndProc()
// MESSAGES :
//   WM_COMMAND         - application menu
//     IDM_FILE_EXIT    - exit the application
//     IDM_FILE_ABOUT   - About Dialog Box
//    ...
// COMMENTS :
//
// ************************************************************************
LRESULT CALLBACK
ProcessCommandsWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  static LPCTSTR  lpszAboutDlgBox       = TEXT( "AboutDlgBox"       );
  static LPCTSTR  lpszAttachDlgBox      = TEXT( "AttachDlgBox"      );
  static LPCTSTR  lpszPreferencesDlgBox = TEXT( "PreferencesDlgBox" );

  switch( LOWORD(wParam) ) {

    //-- user requests to open a new debuggee
    case IDM_FILE_OPEN: {
      static TCHAR szDebuggeeFileName[MAX_PATH];

      if( Global.dwActiveDebuggees ) {
        MaxDebuggeesMessageBox( Global.hWndMain );
        return( FALSE );
      }
      if( !GetDebuggeeFileName( szDebuggeeFileName, hWnd ) ) {
        return( FALSE );
      }
      else {
        if( Profile.fClearOnNew ) {
          SendMessage( Global.hWndListBox, LB_RESETCONTENT, 0, 0 );
          Global.MaxStrLen = 0;
        }
        StartDebuggee( szDebuggeeFileName, Global.hWndListBox );
      }
      return( FALSE );
    }

    //-- user requests to attach to an existing process
    case IDM_FILE_ATTACH:
      if( Global.dwActiveDebuggees ) {
        MaxDebuggeesMessageBox( Global.hWndMain );
        return( FALSE );
      }
      if( !DialogBox( Global.hInstance, lpszAttachDlgBox, hWnd,
            (DLGPROC) AttachDlgProc ) ) {
        // handle cancel condition...
      }
      else {
        if( Profile.fClearOnNew ) {
          SendMessage( Global.hWndListBox, LB_RESETCONTENT, 0, 0 );
          Global.MaxStrLen = 0;
        }
      }
      return( FALSE );

    //-- copy listbox contents to the clipboard and clear the listbox
    case IDM_EDIT_CUT:
      CopyListBoxToClipboard( Global.hWndListBox, Global.MaxStrLen );
      SendMessage( hWnd, WM_COMMAND, IDM_EDIT_DELETE, 0 );
      return( FALSE );

    //-- copy listbox contents to the clipboard
    case IDM_EDIT_COPY: {
      CopyListBoxToClipboard( Global.hWndListBox, Global.MaxStrLen );
      return( FALSE );
    }

    //-- clear the contents of the listbox
    case IDM_EDIT_DELETE:
      SendMessage( Global.hWndListBox, LB_RESETCONTENT, 0, 0 );
      Global.MaxStrLen = 0;
      return( FALSE );

    //-- user requests a new font for the listbox
    case IDM_OPTIONS_FONT:
      if( !ChooseNewFont( Global.hWndListBox ) ) {
        // handle cancel condition...
      }
      return( FALSE );

    //-- user requests a new background color for the listbox
    case IDM_OPTIONS_COLOR:
      ChooseNewBackColor( Global.hWndListBox );
      return( FALSE );

    //-- invoke the preferences dialog box
    case IDM_OPTIONS_PREFERENCES:
      DialogBox( Global.hInstance, lpszPreferencesDlgBox, hWnd,
        (DLGPROC) PreferencesDlgProc );
      return( FALSE );

    //-- toggle the toolbar on or off
    case IDM_OPTIONS_TOOLBAR:
      if( Profile.fToolBar ) {
        Profile.fToolBar = 0;
        CheckMenuItem( GetMenu(Global.hWndMain), IDM_OPTIONS_TOOLBAR,
          MF_UNCHECKED );
        ShowWindow( Global.hWndToolBar, SW_HIDE );
        SendWmSizeMessage( Global.hWndMain );
      }
      else {
        Profile.fToolBar = 1;
        CheckMenuItem( GetMenu(Global.hWndMain), IDM_OPTIONS_TOOLBAR,
          MF_CHECKED );
        ShowWindow( Global.hWndToolBar, SW_SHOW );
        SendWmSizeMessage( Global.hWndMain );
      }
      return( FALSE );

    //-- toggles whether the used directory is used for the 'open' command
    case IDM_OPTIONS_SAVEDDIR:
      if( Profile.fSavedDirectory ) {
        Profile.fSavedDirectory = 0;
        CheckMenuItem( GetMenu(Global.hWndMain), IDM_OPTIONS_SAVEDDIR,
          MF_UNCHECKED );
      }
      else {
        Profile.fSavedDirectory = 1;
        CheckMenuItem( GetMenu(Global.hWndMain), IDM_OPTIONS_SAVEDDIR,
          MF_CHECKED );
      }
      return( FALSE );

    //-- toggles the 'save on exit' feature
    case IDM_OPTIONS_SAVEONEXIT:
      if( Profile.fSaveOnExit ) {
        Profile.fSaveOnExit = 0;
        CheckMenuItem( GetMenu(Global.hWndMain), IDM_OPTIONS_SAVEONEXIT,
          MF_UNCHECKED );
      }
      else {
        Profile.fSaveOnExit = 1;
        CheckMenuItem( GetMenu(Global.hWndMain), IDM_OPTIONS_SAVEONEXIT,
          MF_CHECKED );
      }
      return( FALSE );

    //-- store location information to private profile data now
    case IDM_OPTIONS_SAVENOW: {
      BOOL fOldSaveOnExit = Profile.fSaveOnExit;
      BOOL fOldSavePreferences = Profile.fSavePreferences;

      Profile.fSaveOnExit = TRUE;
      Profile.fSavePreferences = TRUE;
      WritePrivateProfileSettings( Global.szAppName, szIniPathName, &Profile );
      Profile.fSaveOnExit = fOldSaveOnExit;
      Profile.fSavePreferences = fOldSavePreferences;
      return( FALSE );
    }

    //-- invoke help and display the contents panel
    case IDM_HELP_CONTENTS:
      Global.fHelpUsed = TRUE;
      WinHelp( hWnd, (LPCTSTR) szHelpPathName, HELP_CONTENTS, (DWORD) NULL );
      return( FALSE );

    //-- search the help indexes
    case IDM_HELP_SEARCH:
      Global.fHelpUsed = TRUE;
      WinHelp( hWnd, (LPCTSTR) szHelpPathName, HELP_PARTIALKEY,
        (DWORD) TEXT( "" ) );
      return( FALSE );

    //-- invoke the main 'how to use' help panel
    case IDM_HELP_HOWTOUSE:
      Global.fHelpUsed = TRUE;
      WinHelp( hWnd, (LPTSTR) NULL, HELP_HELPONHELP, (DWORD) NULL );
      return( FALSE );

    //-- display the product information dialog box
    case IDM_HELP_ABOUT:
      DialogBox( Global.hInstance, lpszAboutDlgBox, hWnd,
        (DLGPROC) AboutDlgProc );
      return( FALSE );

    //-- the usr requests to terminate the app
    case IDM_FILE_EXIT:
      SendMessage( Global.hWndMain, WM_CLOSE, 0, 0 );
      return( FALSE );

    default:
      return( DefWindowProc(hWnd, uMsg, wParam, lParam) );
  }

  return( FALSE );
}


// ************************************************************************
// FUNCTION : PreferencesDlgProc( HWND, UINT, WPARAM, LPARAM )
// PURPOSE  : Processes message for "Preferences" dialog box
// MESSAGES :
//   WM_INITDIALOG - initialize dialog box
//   WM_COMMAND    - Input received
// COMMENTS :
//   Wait for user to click on "Ok" button, then close the dialog box.
// ************************************************************************
BOOL CALLBACK
PreferencesDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  switch( uMsg ) {

    case WM_COMMAND:
      switch( LOWORD(wParam) ) {

        case IDOK:

          //-- Debugger Setting Group
          if( SendMessage( GetDlgItem( hDlg, IDC_DEBUG_PROCESS),
                BM_GETCHECK, 0, 0 ) )
            Profile.DebugMode = DEBUG_PROCESS;
          if( SendMessage( GetDlgItem( hDlg, IDC_DEBUG_ONLY_THIS_PROCESS),
                BM_GETCHECK, 0, 0 ) )
            Profile.DebugMode = DEBUG_PROCESS | DEBUG_ONLY_THIS_PROCESS;

          //-- Debuggee Priority Group
          if( SendMessage( GetDlgItem( hDlg, IDC_IDLE_PRIORITY_CLASS),
                BM_GETCHECK, 0, 0 ) )
            Profile.DebuggeePriority = IDLE_PRIORITY_CLASS;
          if( SendMessage( GetDlgItem( hDlg, IDC_NORMAL_PRIORITY_CLASS),
                BM_GETCHECK, 0, 0 ) )
            Profile.DebuggeePriority = NORMAL_PRIORITY_CLASS;
          if( SendMessage( GetDlgItem( hDlg, IDC_HIGH_PRIORITY_CLASS),
                BM_GETCHECK, 0, 0 ) )
            Profile.DebuggeePriority = HIGH_PRIORITY_CLASS;
          if( SendMessage( GetDlgItem( hDlg, IDC_REALTIME_PRIORITY_CLASS),
                BM_GETCHECK, 0, 0 ) )
            Profile.DebuggeePriority = REALTIME_PRIORITY_CLASS;

          //-- Miscellaneous Options Group
          Profile.fClearOnNew  = (BOOL) SendMessage(
                                          GetDlgItem( hDlg, IDC_CLEAR_ON_NEW),
                                          BM_GETCHECK, 0 , 0 );
          Profile.fVerbose     = (BOOL) SendMessage(
                                          GetDlgItem( hDlg, IDC_VERBOSE),
                                          BM_GETCHECK, 0 , 0 );

#ifdef SHOW_SYMBOLS
          Profile.fShowSymbols = (BOOL) SendMessage(
                                          GetDlgItem( hDlg, IDC_SHOW_SYMBOLS),
                                          BM_GETCHECK, 0 , 0 );
#endif


          //- Debug Error Level Group
          if( SendMessage( GetDlgItem( hDlg, IDC_DEBUG_MODE_NONE),
                BM_GETCHECK, 0, 0 ) )
            Profile.DebugErrorLevel = 0;
          if( SendMessage( GetDlgItem( hDlg, IDC_DEBUG_MODE_ERROR),
                BM_GETCHECK, 0, 0 ) )
            Profile.DebugErrorLevel = SLE_ERROR;
          if( SendMessage( GetDlgItem( hDlg, IDC_DEBUG_MODE_MINORERROR),
                BM_GETCHECK, 0, 0 ) )
            Profile.DebugErrorLevel = SLE_MINORERROR;
          if( SendMessage( GetDlgItem( hDlg, IDC_DEBUG_MODE_WARNING),
                BM_GETCHECK, 0, 0 ) )
            Profile.DebugErrorLevel = SLE_WARNING;

          Profile.fSavePreferences = (BOOL) SendMessage(
                                              GetDlgItem( hDlg,
                                                IDC_SAVE_PREFERENCES),
                                              BM_GETCHECK, 0 , 0 );
          EndDialog( hDlg, TRUE );
          return( TRUE );

        case IDCANCEL:
          EndDialog( hDlg, FALSE );
          return( TRUE );

        case IDHELP:
          return( TRUE );

      }
      break;

    case WM_INITDIALOG:

      //-- Debugger Setting Group
      switch( Profile.DebugMode ) {

        case DEBUG_PROCESS:
          SendMessage( GetDlgItem( hDlg, IDC_DEBUG_PROCESS),
            BM_SETCHECK, 1, 0);
          break;

        case ( DEBUG_PROCESS | DEBUG_ONLY_THIS_PROCESS ):
          SendMessage( GetDlgItem( hDlg, IDC_DEBUG_ONLY_THIS_PROCESS),
            BM_SETCHECK, 1, 0);
          break;
      }

      //-- Debuggee Priority Group
      switch( Profile.DebuggeePriority ) {

        case IDLE_PRIORITY_CLASS:
          SendMessage( GetDlgItem( hDlg, IDC_IDLE_PRIORITY_CLASS),
            BM_SETCHECK, 1, 0);
          break;

        case NORMAL_PRIORITY_CLASS:
          SendMessage( GetDlgItem( hDlg, IDC_NORMAL_PRIORITY_CLASS),
            BM_SETCHECK, 1, 0);
          break;

        case HIGH_PRIORITY_CLASS:
          SendMessage( GetDlgItem( hDlg, IDC_HIGH_PRIORITY_CLASS),
            BM_SETCHECK, 1, 0);
          break;

        case REALTIME_PRIORITY_CLASS:
          SendMessage( GetDlgItem( hDlg, IDC_REALTIME_PRIORITY_CLASS),
            BM_SETCHECK, 1, 0);
          break;

      }

      //-- Miscellaneous Options Group
      SendMessage( GetDlgItem( hDlg, IDC_CLEAR_ON_NEW),     BM_SETCHECK,
        Profile.fClearOnNew, 0 );
      SendMessage( GetDlgItem( hDlg, IDC_VERBOSE),          BM_SETCHECK,
        Profile.fVerbose, 0 );
      SendMessage( GetDlgItem( hDlg, IDC_SAVE_PREFERENCES), BM_SETCHECK,
        Profile.fSavePreferences, 0 );

#ifdef SHOW_SYMBOLS
      SendMessage( GetDlgItem( hDlg, IDC_SHOW_SYMBOLS),     BM_SETCHECK,
        Profile.fShowSymbols, 0 );
#endif

      //- Debug Error Level Group
      switch( Profile.DebugErrorLevel ) {

        case 0:
          SendMessage( GetDlgItem( hDlg, IDC_DEBUG_MODE_NONE),
            BM_SETCHECK, 1, 0);
          break;

        case SLE_ERROR:
          SendMessage( GetDlgItem( hDlg, IDC_DEBUG_MODE_ERROR),
            BM_SETCHECK, 1, 0);
          break;

        case SLE_MINORERROR:
          SendMessage( GetDlgItem( hDlg, IDC_DEBUG_MODE_MINORERROR),
            BM_SETCHECK, 1, 0);
          break;

        case SLE_WARNING:
          SendMessage( GetDlgItem( hDlg, IDC_DEBUG_MODE_WARNING),
            BM_SETCHECK, 1, 0);
          break;

      }

      return( TRUE );
  }

  return( FALSE );
  UNREFERENCED_PARAMETER( lParam );
}


// ************************************************************************
// FUNCTION : AttachDlgProc( HWND, UINT, WPARAM, LPARAM )
// PURPOSE  : Processes messages for "Attach" dialog box
// MESSAGES :
//   WM_COMMAND    - Input received
//   WM_INITDIALOG - initialize dialog box
// COMMENTS :
//   Wait for user to click on "Ok" button, then close the dialog box.
// ************************************************************************
BOOL CALLBACK
AttachDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  static HWND hWndProcessList;

  switch( uMsg ) {

    case WM_COMMAND:
      switch( LOWORD(wParam) ) {

        case IDOK: {
          LONG  Index;
          DWORD dwProcessId;

          Index = (UINT) SendMessage( hWndProcessList, LB_GETCURSEL,
                         (WPARAM) NULL, (LPARAM) NULL );
          dwProcessId = (DWORD) SendMessage( hWndProcessList,
                                  LB_GETITEMDATA,
                                  (WPARAM) Index, (LPARAM) NULL );
          AttachToDebuggee( dwProcessId, Global.hWndListBox );
          EndDialog( hDlg, TRUE );
          return( TRUE );
        }

        case IDCANCEL:
          EndDialog( hDlg, FALSE );
          return( TRUE );

        case IDHELP:
          return( TRUE );

      }
      switch( HIWORD( wParam ) ) {
        case LBN_DBLCLK:
          SendMessage( hDlg, WM_COMMAND, (WPARAM) IDOK, (LPARAM) 0L );
          return( TRUE );
      }
      break;

    case WM_INITDIALOG:
      hWndProcessList = GetDlgItem( hDlg, IDC_PROCESSLIST );
      SendMessage( hWndProcessList, LB_RESETCONTENT, 0 , 0 );
      for(; !EnumWindows( (WNDENUMPROC) EnumProcessListFunc,
               (LPARAM) hWndProcessList ); )
        ; // continue looping until done
      return( TRUE );
  }

  return( FALSE );
  UNREFERENCED_PARAMETER( lParam );
}


// **************************************************************************
// FUNCTION : AboutDlgProc( HWND, UINT, WPARAM, LPARAM )
// PURPOSE  : Processes messages for "About" dialog box
// MESSAGES :
//   WM_COMMAND    - Input received
//     IDOK        - OK button selected
//     IDCANCEL    - Cancel button selected
//     ...
//   WM_INITDIALOG - initialize dialog box
//   WM_CLOSE      - close the dialog box
//   ...
// COMMENTS:
//   No initialization is needed for this particular dialog box.
//   In this case, TRUE must be returned to Windows.
//   Wait for user to click on "Ok" button, then close the dialog box.
// **************************************************************************
BOOL CALLBACK
AboutDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  switch( uMsg ) {

    case WM_COMMAND:
      switch( LOWORD(wParam) ) {

        case IDOK:
          EndDialog( hDlg, TRUE );
          return( TRUE );

        case IDCANCEL:
          EndDialog( hDlg, FALSE );
          return( FALSE );

      }
      break;

    case WM_INITDIALOG:
      SetTimer( hDlg, 1, TIMEOUT_ANIMATED_ICON, TimerProc );
      return( TRUE );

    case WM_CLOSE:
      EndDialog( hDlg, TRUE );
      return( TRUE );

    case WM_DESTROY:
      KillTimer( hDlg, 1 );
      return( TRUE );
  }

  return( FALSE );
  UNREFERENCED_PARAMETER( lParam );
}


// ************************************************************************
// FUNCTION : NewListBoxWndProc( HWND, UINT, WPARAM, LPARAM )
// PURPOSE  : Processes messages for "LISTBOX" class.
// COMMENTS : Prevents the user from moving the window
//            by dragging the titlebar.
// ************************************************************************
LRESULT CALLBACK
NewListBoxWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  switch( uMsg ) {

    case WM_NCLBUTTONDOWN:
      if( wParam == HTCAPTION ) {
        SetFocus( hWnd );
        return( FALSE );
      }
      else
        break;
  }

  return( CallWindowProc( Global.OldListBoxWndProc, hWnd, uMsg, wParam, lParam) );
}


// ************************************************************************
// FUNCTION : TimerProc( HWND, UINT, UINT, DWORD )
// PURPOSE  : Timer callback fuction that changes the dialog's icon (thus
//            a animated icon) for each timer message received.
// COMMENTS :
// ************************************************************************
VOID CALLBACK
TimerProc( HWND hDlg, UINT uMsg, UINT idEvent, DWORD dwTime )
{
 #define NUM_ICONS 8

 static UINT    uIconNumber = 0;
 static HWND    hWndIcon    = NULL;
 static HWND    hWndIconOld = NULL;
 static LPCTSTR lpszIconNames[NUM_ICONS] = {
                  TEXT( "DebugIcon1" ), TEXT( "DebugIcon2" ),
                  TEXT( "DebugIcon3" ), TEXT( "DebugIcon4" ),
                  TEXT( "DebugIcon5" ), TEXT( "DebugIcon6" ),
                  TEXT( "DebugIcon7" ), TEXT( "DebugIcon8" ) };

 if( (++uIconNumber) >= NUM_ICONS )
   uIconNumber = 0;

 hWndIconOld = hWndIcon;
 hWndIcon = CreateIconWindow( hDlg, lpszIconNames[uIconNumber] );
 if( hWndIcon != NULL )
   DestroyWindow( hWndIconOld );

 return;
 UNREFERENCED_PARAMETER( uMsg );
 UNREFERENCED_PARAMETER( idEvent );
 UNREFERENCED_PARAMETER( dwTime );
}
