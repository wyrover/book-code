// MThread.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <stdio.h> 
#include <stdlib.h> 
 
#define MM_NEWWIN       8001 

typedef struct _PTHREADLIST 
{ 
   HANDLE hThread; 
   LPVOID lpvNext; 
} THREADLIST, *PTHREADLIST; 
 
HINSTANCE g_hInst;
HANDLE hModule;            // handle to .EXE file for this process 
HWND   hwndMain = NULL;    // handle to main window 
BOOL fKillAll = FALSE;     // sets TRUE to terminate all threads 
PTHREADLIST pHead = NULL;  // head of thread information linked list 
 
BOOL InitializeApp(VOID); 
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM); 
LRESULT CALLBACK ChildWndProc(HWND, UINT, WPARAM, LPARAM); 
DWORD ThreadFunc(HWND); 
VOID AddThreadToList(HANDLE); 
VOID ErrorExit(LPSTR); 
 
// Primary thread:  Initialize the application and dispatch messages. 
 
int WINAPI WinMain( HINSTANCE hInst, 
   HINSTANCE hPrevInst, 
   LPSTR lpszCmdLn, 
   int nShowCmd) 
{
   MSG msg;

   g_hInst = hInst;
   hModule = GetModuleHandle(NULL); 
   if (! InitializeApp()) 
      ErrorExit("InitializeApp failure!"); 
   while (GetMessage(&msg, NULL, 0, 0) != 0 && GetMessage(&msg, NULL, 0, 0) != -1) 
   { 
      TranslateMessage(&msg); 
      DispatchMessage(&msg); 
   } 
   return 1; 
   UNREFERENCED_PARAMETER(hInst); 
   UNREFERENCED_PARAMETER(hPrevInst); 
   UNREFERENCED_PARAMETER(lpszCmdLn); 
   UNREFERENCED_PARAMETER(nShowCmd); 
} 
 
// Register window classes and create the main window. 
 
BOOL InitializeApp(VOID) 
{ 
   HMENU hmenuMain, hmenuPopup; 
   WNDCLASS wc; 
 
// Register a window class for the main window. 
 
   wc.style            = CS_OWNDC; 
   wc.lpfnWndProc      = MainWndProc; 
   wc.cbClsExtra       = 0; 
   wc.cbWndExtra       = 0; 
   wc.hInstance        = g_hInst; 
   wc.hIcon            = LoadIcon(NULL,IDI_APPLICATION); 
   wc.hCursor          = LoadCursor(NULL, IDC_ARROW); 
   wc.hbrBackground    = (HBRUSH)(COLOR_BACKGROUND+1); 
   wc.lpszMenuName     = NULL; 
   wc.lpszClassName    = "MainWindowClass"; 
   if (! RegisterClass(&wc)) 
      return FALSE; 
 
// Register a window class for child windows. 
 
   wc.lpfnWndProc      = ChildWndProc; 
   wc.lpszClassName    = "ThreadWindowClass"; 
   if (! RegisterClass(&wc)) 
      return FALSE; 

// Create a menu for the main window. 
 
   hmenuMain = CreateMenu(); 
   hmenuPopup = CreateMenu(); 
   if (!AppendMenu(hmenuPopup, MF_STRING, MM_NEWWIN, "&New Window")) 
      return FALSE; 
   if (!AppendMenu(hmenuMain, MF_POPUP, (UINT)hmenuPopup, "&Threads")) 
      return FALSE; 
 
// Create the main window. 
 
   hwndMain = CreateWindow("MainWindowClass", "Primary Window", 
      WS_OVERLAPPED | WS_CAPTION | WS_BORDER | WS_THICKFRAME | 
      WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_CLIPCHILDREN | 
      WS_VISIBLE | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 
      CW_USEDEFAULT, CW_USEDEFAULT, NULL, hmenuMain, g_hInst, 
      NULL); 
   if (hwndMain == NULL) 
      return FALSE; 
 
// Set the initial focus. 
 
   SetFocus(hwndMain); 
   return TRUE; 
} 
 
// Main window procedure: Handle messages for the main window. 
 
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uiMessage, 
   WPARAM wParam, LPARAM lParam) 
{ 
   static HWND hwndClient; 
   static DWORD dwCount = 1; 
   CLIENTCREATESTRUCT ccsClientCreate; 
   HWND hwndChildWnd; 
   DWORD IDThread; 
   PTHREADLIST pNode; 
 
   switch (uiMessage) 
   { 
   // Create a client window to receive child window messages. 

      case WM_CREATE: 
         ccsClientCreate.hWindowMenu = NULL; 
         ccsClientCreate.idFirstChild = 1; 
         hwndClient = CreateWindow("MDICLIENT", NULL, 
            WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE, 0, 0, 0, 0, 
            hwnd, NULL, g_hInst, (LPVOID)&ccsClientCreate); 
         return 0L; 
 
   // Close the main window. First set fKillAll to TRUE to 
   // terminate all threads. Then wait for the threads to exit 
   // before passing a close message to a default handler.  If you 
   // don't wait for threads to terminate, the process terminates 
   // with no chance for thread cleanup. 
 
      case WM_CLOSE: 
         fKillAll = TRUE; 
         pNode = pHead; 
         while (pNode) 
         { 
            DWORD dwRes; 
            SetThreadPriority(pNode->hThread, 
               THREAD_PRIORITY_HIGHEST); 
            dwRes = WaitForSingleObject(pNode->hThread, 
               INFINITE); 
            pNode = (PTHREADLIST) pNode->lpvNext; 
         } 
         return DefFrameProc(hwnd, hwndClient, uiMessage, 
            wParam, lParam); 
 
   // Terminate the process. 
 
      case WM_DESTROY: 
         PostQuitMessage(0); 
         return 0L; 
 
   // Handle the menu commands. 
 
      case WM_COMMAND: 
         switch (LOWORD(wParam)) 
         { 
         // Create a child window and start a thread for it. 
            case MM_NEWWIN: 
            {
               HANDLE hThrd; 
               MDICREATESTRUCT mdicCreate; 
               TCHAR tchTitleBarText[32]; 
               LONG lPrev; 
 
               sprintf(tchTitleBarText, "Thread Window %d", dwCount); 
               mdicCreate.szClass = "ThreadWindowClass"; 
               mdicCreate.szTitle = tchTitleBarText; 
               mdicCreate.hOwner  = hModule; 
               mdicCreate.x = mdicCreate.y = 
               mdicCreate.cx = mdicCreate.cy = CW_USEDEFAULT; 
               mdicCreate.style = mdicCreate.lParam = 0L; 
 
            // Send a "create child window" message to the 
            // client window. 
 
               hwndChildWnd = (HWND) SendMessage(hwndClient, 
                  WM_MDICREATE, 0L, (LONG)&mdicCreate); 
               if (hwndChildWnd == NULL) 
                  ErrorExit("Failed in Creating Thread Window!"); 
 
            // Window structure used to pass a quit message to 
            // the thread. 
 
               lPrev = SetWindowLong(hwndChildWnd, GWL_USERDATA, 0); 
 
            // Create a suspended thread; alter its priority before 
            // calling ResumeThread. 
 
               hThrd = CreateThread(NULL,  // no security attributes 
                         0,                // use default stack size 
                         (LPTHREAD_START_ROUTINE) ThreadFunc, 
                         (LPVOID)hwndChildWnd, // param to thread func 
                         CREATE_SUSPENDED, // creation flag 
                         &IDThread);       // thread identifier 
               if (hThrd == NULL) 
                  ErrorExit("CreateThread Failed!"); 
               AddThreadToList(hThrd); 
               dwCount++; 
 
            // Set the priority lower than the primary (input) 
            // thread, so the process is responsive to user 
            // input.  Then resume the thread. 
 
               if (!SetThreadPriority(hThrd, 
                  THREAD_PRIORITY_BELOW_NORMAL)) 
                  ErrorExit("SetThreadPriority failed!"); 
               if ((ResumeThread(hThrd)) == -1) 
                  ErrorExit("ResumeThread failed!"); 
               return 0L; 
            }
 
            default: 
               return DefFrameProc(hwnd, hwndClient, uiMessage, 
                  wParam, lParam); 
         } 
 
      default: 
         return DefFrameProc(hwnd, hwndClient, uiMessage, 
            wParam, lParam); 
   } 
} 
 
// Process messages for the child windows. 
 
LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT uiMessage, WPARAM 
   wParam, LPARAM lParam) 
{ 
   LONG lPrevLong; 
   switch (uiMessage) 
   { 
   // Use a window structure to pass "close" message to thread. 
 
      case WM_CLOSE: 
         lPrevLong = SetWindowLong(hwnd, GWL_USERDATA, 1); 
         return DefMDIChildProc(hwnd, uiMessage, wParam, lParam); 
 
      case WM_DESTROY: 
         return 0L; 
 
      default: 
         return DefMDIChildProc(hwnd, uiMessage, wParam, lParam); 
   } 
} 
 
// Each child window has a thread that can be used to perform tasks 
// associated with that window--for example, drawing its contents. 
 
DWORD ThreadFunc(HWND hwnd) 
{ 
   LONG lKillMe = 0L; 
   while (TRUE) 
   { 
      lKillMe = GetWindowLong(hwnd, GWL_USERDATA); 
      if (fKillAll || lKillMe) break; 
 
   // Perform tasks. 
 
   } 
 
   // Perform actions needed before thread termination. 
 
   return 0; 
} 
 
VOID AddThreadToList(HANDLE hThread) 
{ 
   PTHREADLIST pNode; 
   pNode = (PTHREADLIST) LocalAlloc(LPTR, sizeof(PTHREADLIST)); 
   if (pNode == NULL) 
   {
      ErrorExit("malloc Failed!"); 
   }
   pNode->hThread = hThread; 
   pNode->lpvNext = (LPVOID) pHead; 
   pHead = pNode; 
} 
 
VOID ErrorExit(LPSTR lpszMessage) 
{ 
   MessageBox(hwndMain, lpszMessage, "Error", MB_OK); 
   ExitProcess(0); 
} 

