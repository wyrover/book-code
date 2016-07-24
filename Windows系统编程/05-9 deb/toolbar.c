
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
// MODULE    : ToolBar.C
// PURPOSE   : A Win32 DLL containing a simple text ToolBar
// FUNCTIONS :
//   DllMain()         - Dll entry point (via _DllMainCRTStartup)
//   TextButtonBar()   - generate a text button ToolBar
//   TextButtonBarProc - processes messages for the TextButtonBarWClass
// COMMENTS  :
//
// ************************************************************************
#define   STRICT               // enable strict typing
#include <Windows.H>           // required for all Windows applications

#include "ToolBar.H"           // specific to this program

//-- internal data
HWND hWndToolBarOwner;         // window handle of Toolbar owner
                               // where all WM_COMMAND messages are sent

//-- internal prototypes
LRESULT CALLBACK TextButtonBarProc( HWND, UINT, WPARAM, LPARAM );


// ************************************************************************
// FUNCTION : DllMain( HINSTANCE, DWORD, LPVOID )
// PURPOSE  : DllMain is called by the C run-time library from the
//            _DllMainCRTStartup entry point.  The DLL entry point gets
//            called (entered) on the following events: "Process Attach",
//            "Thread Attach", "Thread Detach" or "Process Detach".
// COMMENTS : No initialization is needed here so this entry point simply
//            returns TRUE.
// ************************************************************************
BOOL WINAPI
DllMain( HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
  UNREFERENCED_PARAMETER( hInstDLL );
  UNREFERENCED_PARAMETER( fdwReason );
  UNREFERENCED_PARAMETER( lpvReserved );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : TextButtonBar( HWND, LPTEXTBUTTON, LPINT )
// PURPOSE  : Generate a simple text button ToolBar
// COMMENTS : lpTextButton points to an array of TEXTBUTTON
//            structures.  The last structure must set the
//            member lpButtonText to NULL.  lpHeight contains the
//            height of the ToolBar window.
// ************************************************************************
HWND
TextButtonBar( HWND hWndParent, LPTEXTBUTTON lpTextButton, LPINT lpHeight )
{
  #define BUTTON_BORDER    4
  #define BUTTONBAR_BORDER 4

  static HWND         hWndTextButtonBar;

  HDC          hDC;
  WNDCLASS     WndClass;
  LONG         ButtonWidth;
  LONG         ButtonHeight;
  INT          xPosButton;
  INT          ButtonSpacing;
  INT          ButtonSpace;
  LPTSTR       lpButtonString;
  LPTEXTBUTTON lpTempTextButton;
  TEXTMETRIC   tm;
  SIZE         ButtonTextSize;
  RECT         rect;

  //-- store owner of Toolbar
  hWndToolBarOwner = hWndParent;

  //-- register the TestButtonBar window class
  WndClass.style         = (UINT) NULL;
  WndClass.lpfnWndProc   = (WNDPROC) TextButtonBarProc;
  WndClass.cbClsExtra    = 0;
  WndClass.cbWndExtra    = 0;
  WndClass.hInstance     = NULL;
  WndClass.hIcon         = NULL;
  WndClass.hCursor       = NULL;
  WndClass.hbrBackground = (HBRUSH) GetStockObject(LTGRAY_BRUSH);
  WndClass.lpszMenuName  = NULL;
  WndClass.lpszClassName = TEXT( "TextButtonBarWClass" );

  if( !RegisterClass( &WndClass ) )
    return( FALSE );

  hDC = GetDC( hWndParent );
  GetTextMetrics( hDC, &tm );
  ButtonHeight = 2 * BUTTON_BORDER + tm.tmHeight;
  *lpHeight = 2 * BUTTONBAR_BORDER + ButtonHeight;

  GetWindowRect( GetDesktopWindow(), &rect );
  hWndTextButtonBar = CreateWindow(
                        TEXT( "TextButtonBarWClass" ), TEXT( "Toolbar" ),
                        WS_CHILD | WS_CLIPSIBLINGS,
                        0, 0, rect.right - rect.left, *lpHeight,
                        hWndToolBarOwner,
                        (HMENU) NULL, (HANDLE) NULL, NULL );

  SetWindowPos( hWndTextButtonBar, (HWND) 1, 0, 0, 0, 0,
    SWP_NOMOVE | SWP_NOSIZE );

  //-- button spacing information
  ButtonSpacing = BUTTON_BORDER / 2;
  ButtonSpace   = BUTTON_BORDER * 4;
  xPosButton    = BUTTON_BORDER * 2;

  //-- Create all the button windows
  for( lpTempTextButton = lpTextButton;
         !( (lpTempTextButton->lpButtonText) == '\0'
           && (lpTempTextButton->idButton) == (UINT) NULL);
         lpTempTextButton++ ) {

    //-- if TB_SPACE then adjust new xPos and continue
    if( lpTempTextButton->idButton == TB_SPACE ) {
      xPosButton += ButtonSpace;
      continue;
    }

    lpButtonString = lpTempTextButton->lpButtonText;
    GetTextExtentPoint( hDC, lpButtonString, lstrlen(lpButtonString), &ButtonTextSize );
    ButtonWidth = (4 * BUTTON_BORDER) + (LONG) ButtonTextSize.cx;

    (lpTempTextButton->hWndButton) = CreateWindow(
         TEXT( "BUTTON" ), lpButtonString,
         WS_VISIBLE | BS_PUSHBUTTON | WS_CHILD,
         xPosButton, BUTTONBAR_BORDER, ButtonWidth, ButtonHeight,
         hWndTextButtonBar,
         (HMENU) lpTempTextButton->idButton,
         (HANDLE) NULL, NULL );

    xPosButton += ButtonWidth + ButtonSpacing;
  }
  ReleaseDC( hWndParent, hDC );

  return( hWndTextButtonBar );
}


// ************************************************************************
// FUNCTION : TextButtonBarProc( HWND, UINT, WPARAM, LPARAM )
// PURPOSE  : Processes messages for the TextButtonBarWClass
// MESSAGES :
//  WM_COMMAND - forwarded to the owner window
//  WM_PAINT   - draws a 3D background
// COMMENTS :
// ************************************************************************
LRESULT CALLBACK
TextButtonBarProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  static HDC         hDC;
  static RECT        rect;
  static PAINTSTRUCT ps;

  switch( uMsg ) {

    case WM_COMMAND:
      SendMessage( hWndToolBarOwner, uMsg, wParam, lParam );
      return( FALSE );

    case WM_PAINT:
      hDC = BeginPaint( hWnd, &ps );
      GetClientRect( hWnd, &rect );

      SelectObject( hDC, GetStockObject(WHITE_PEN) );
      MoveToEx( hDC, rect.left, rect.top, NULL );
      LineTo( hDC, rect.right+1, rect.top );

      #define DKGRAY_PEN RGB(128, 128, 128)
      SelectObject( hDC, (HANDLE) CreatePen( PS_SOLID, 1, DKGRAY_PEN ) );
      MoveToEx( hDC, rect.left, rect.bottom-2, NULL );
      LineTo( hDC, rect.right+1, rect.bottom-2 );

      SelectObject( hDC, (HANDLE) CreatePen( PS_SOLID, 1, BLACK_PEN ) );
      MoveToEx( hDC, rect.left, rect.bottom-1, NULL );
      LineTo( hDC, rect.right+1, rect.bottom-1 );

      DeleteObject( SelectObject( hDC, GetStockObject(BLACK_PEN) ) );
      EndPaint( hWnd, &ps );
      return( FALSE );

    default:
      return( DefWindowProc(hWnd, uMsg, wParam, lParam) );
  }
}
