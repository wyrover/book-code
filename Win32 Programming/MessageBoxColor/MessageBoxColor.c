//*-------------------------------------------------------------------------
//| Title:
//|     MessageBoxColor.c
//| Purpose:
//|      Calls colorMessageBox to create a message box whose backgound and
//|      text color can be changed.
//|                                                         
//| Development :  VC, 5/27/93
//|
//| Notes:
//| 
//| This program is based on the code
//|    GENERIC.C and MSGCOLOR.C
//| Written by Microsoft Product Support Services, Windows Developer Support
//|
//| CopyRight (c) 1992 Microsoft Corporation. All rights reserved.
//|
//| Why?
//|   Why didn't we base this source code on SKELETON or TEMPLATE?  Because
//|   I did this program about a year before I started the book, and there
//|   was little motivation to change it, since I was going to incorporate
//|   only the snippets appropriate to illustrate the technique into the
//|   book.  When the decision was made to include all the running examples,
//|   I had forgotten about this being the old GENERIC/MSGCOLOR template.  
//|   The old MSGCOLOR.C example was a Win16-based example that did not 
//|   transfer readily to Win32.  The amount of rework required to make it
//|   work in Win32 seems to satisfy the Microsoft License Agreement, that
//|   says I should add "substantial value".  A version that works in Win32
//|   is in my judgment a substantial improvement over one that, as I recall,
//|   would not even compile in Win32.  The changes were more than cosmetic.
//|   This version also adds the notion of a bitmap brush background, which
//|   is a neat trick (and the reason I needed to write it in the first place).
//|   
//|   Reality has a nasty way of intruding...
//|
//|   I thought I had two more weeks to prepare the CDROM.  But I'm going
//|   into the hospital tomorrow (2-Oct-96) for surgery which cannot be put
//|   off and which will take me out for five days of those two weeks.  I
//|   want the CDROM content stable within the next 24 hours!!! I can't do
//|   all of that and rewrite this using SKELETON.  So this old code is
//|   on the CDROM.  I don't think this poses too much of a challenge to read.
//|   
//|            - joseph m. newcomer, 1-Oct-96.
//*-------------------------------------------------------------------------

#define STRICT
#include <windows.h>          
#include <tchar.h>
#include "MessageBoxColor.h"  
#include "msgcolor.h"        
#include "resource.h"
#include "is95.h"

// Globals
HINSTANCE g_hinst;               // Instance of application

char Title[STR_LEN];

int PASCAL WinMain (HINSTANCE, HINSTANCE, LPSTR, int);
static BOOL initInstance (HINSTANCE, int);
static LRESULT CALLBACK mainWndProc (HWND, UINT, WPARAM, LPARAM);

//***********************************************************************
// Function: WinMain
//
// Purpose: Called by Windows on app startup.  Initializes everything,
//          and enters a message loop.
//
// Parameters:
//    hInstance     == Handle to this instance.
//    hPrevInstance == Handle to last instance of app.
//    lpCmdLine     == Command Line passed into app.
//    nCmdShow      == How app should come up (i.e. minimized/normal)
//
// Returns: Return value from PostQuitMessage.
//
//****************************************************************************
int PASCAL WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
   MSG msg;

   LoadString(hInstance, IDS_PROGNAME, Title, STR_LEN);
   
   if (!initInstance(hInstance, nCmdShow))
      return (FALSE);

   while (GetMessage(&msg, NULL, 0, 0))
   {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }
   
   return (msg.wParam); // Returns the value from PostQuitMessage
}


//****************************************************************************
// Function: registerClasses
//
// Purpose: Called by WinMain on first instance of app.  Registers
//          the window class.
//
// Parameters:
//    hInstance == Handle to this instance.
//
// Returns: TRUE on success, FALSE otherwise.
//
//****************************************************************************

BOOL registerClasses (HINSTANCE hInstance)
{
   WNDCLASS wc;

   wc.style = CS_DBLCLKS;
   wc.lpfnWndProc = mainWndProc;
   wc.cbClsExtra = 0;
   wc.cbWndExtra = 0;
   wc.hInstance = hInstance;
   wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINFRAME));
   wc.hCursor = LoadCursor(NULL, IDC_ARROW);
   wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
   wc.lpszMenuName = _T("GenericMenu");
   wc.lpszClassName = _T("MainWndClass");
     
   return RegisterClass(&wc);
 
}

//****************************************************************************
// Function: InitInstance
//
// Purpose: Called by WinMain on instance startup.  Creates and
//            displays the main, overlapped window.
//
// Parameters:
//    hInstance     == Handle to this instance.
//    nCmdShow      == How app should come up (i.e. minimized/normal)
//
// Returns: TRUE on success, FALSE otherwise.
//
// Comments:
//
// History:  Date       Author           Reason
//           10/30/92   VC               Created
//****************************************************************************

static BOOL initInstance (HINSTANCE hInstance, int nCmdShow)
{
   HWND g_hwnd;  

   if(!registerClasses(hInstance))
      return FALSE;

   g_hinst = hInstance;
   // Create Main Window
   g_hwnd = CreateWindow(_T("MainWndClass"), Title,
                       WS_OVERLAPPEDWINDOW,
                       CW_USEDEFAULT, CW_USEDEFAULT,
                       600, 400,
                       NULL, NULL, hInstance, NULL);
   if (!g_hwnd)
      return FALSE;
   
   ShowWindow(g_hwnd, nCmdShow);               
   UpdateWindow(g_hwnd);           
   return TRUE;
}

//****************************************************************************
// Function: mainWndProc
//
// Purpose: Message handler for main overlapped window.
//
// Parameters & return value:
//    Standard. See documentaion for WindowProc
//
//****************************************************************************

static LRESULT CALLBACK 
mainWndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
   switch (msg)
   {     
      case WM_COMMAND:
         // Create message boxes of different colors
         switch (wParam)
         {
            case ID_EXIT:
                    PostMessage(hwnd, WM_CLOSE, 0, 0);
                    return 0;

            case IDM_GRAYMSGBOX:               
               colorMessageBox(hwnd,              // Owner
                              _T("Gray Background"),  // Text in message box
                              _T("Message Box"),      // Title of message box
                              MB_OK,              // Message box styles
                              GetStockObject(LTGRAY_BRUSH),  // Background brush
                              RGB(0, 0, 0),                  // Text color
                              g_hinst);                       // HINSTANCE of module
               return 0L;
               
            case IDM_REDMSGBOX: 
              {
               HBRUSH hbrush = CreateSolidBrush(RGB(255, 0, 0));              
               colorMessageBox(hwnd, _T("Red Background"), _T("Message Box"),  
                               MB_ICONQUESTION | MB_ABORTRETRYIGNORE, 
                               hbrush, RGB(0, 0, 255), g_hinst);
               DeleteObject(hbrush);
               return 0L;
              }
            case IDM_PLAIDBOX:
                    {
                     HBITMAP bmp;
                     HBRUSH  hbr;
                     
                     if(Is95())
                        bmp = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_PLAID8X8));
                     else
                     if(GetSystemMetrics(SM_CXSCREEN) >= 800)
                        bmp = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_PLAID800));
                     else
                        bmp = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_PLAID));

                     hbr = CreatePatternBrush(bmp);

                     colorMessageBox(hwnd, 
_T("Captain! I canna get more energy from the dilithium crystals!"), 
                                           _T("Plaid Message Box"),
                                           MB_ICONWARNING | MB_RETRYCANCEL,
                                           hbr, 
					   Is95() ? RGB(255, 255, 255)
					   	  : RGB(0, 0, 0),
                                           g_hinst);
                     DeleteObject(hbr);
                     DeleteObject(bmp);
                    }
                 return 0;
            default:
               return 0L;
         }
         break;
         
      case WM_DESTROY:                
         PostQuitMessage(0);
         break;

      default:                         
         return DefWindowProc(hwnd, msg, wParam, lParam);
   }
   
   return 0;
}

