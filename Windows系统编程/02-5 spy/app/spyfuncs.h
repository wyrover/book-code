
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993-1997 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/*****************************************************************************\
*
* Module: spyfuncs.h
*
*   External function declarations for the Windows debugging Spy SDK applet.
*
\*****************************************************************************/


//
// dialogs.c
//

BOOL MyDialogBox(INT idDlg, DLGPROC pfnDlgProc);
BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SelectWindowDlgProc(HWND hwnd, UINT msg, WPARAM wParam,
    LPARAM lParam);
BOOL CALLBACK OutputDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
VOID SelectFont(VOID);
BOOL CALLBACK MessagesDlgProc(HWND hwnd, UINT msg, WPARAM wParam,
    LPARAM lParam);


//
// hook.c
//

BOOL CreateHookThread(VOID);
DWORD HookMain(LPVOID lpv);
LRESULT CALLBACK HookWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL SetSpyHook(BOOL fSet);
VOID SetWindowToSpyOn(HWND hwndSpyingOn);

#ifdef DBG
VOID DbgPrintf(LPTSTR fmt, ...);
#endif


//
// misc.c
//

VOID ReadRegistry(VOID);
VOID WriteRegistry(VOID);
INT Message(UINT fuStyle, LPSTR pszFormat, ...);
VOID SetSpyCaption(VOID);
LPTSTR LoadResourceString(UINT wId);


//
// spy.c
//

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, INT nCmdShow);
LRESULT CALLBACK SpyWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


//
// wm.c
//

VOID CalculatePrintfTabs(HFONT hfont);
VOID PrintMsg(LPMSG lpMsg);


//
// wprintf.c
//

VOID MyCreatePrintfWin(HWND hwnd);
VOID SetPrintfFont(HWND hwnd, HFONT hfont);
VOID SetPrintfTabs(INT n, LPINT pTabs);
VOID ClearPrintfWindow(HWND hwnd);
BOOL CopyToClipboard(VOID);
BOOL IsPrintfEmpty(VOID);

