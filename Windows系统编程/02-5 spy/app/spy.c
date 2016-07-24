
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
* Module: spy.c
*
*   Main module for the Windows debugging Spy SDK applet.
*
* Functions:
*
*   WinMain()
*   SpyWndProc()
*   SpyInit()
*   PutOptions()
*   InitMenu()
*   SpyCommand()
*
* Comments:
*
\*****************************************************************************/

#include "spy.h"
#include <stdlib.h>


#define WM_EXECINSTANCE     (WM_USER+100)


/*
 * Macros to simplify working with menus.
 */
#define MyEnableMenuItem(hMenu, wIDEnableItem, fEnable) \
    EnableMenuItem((hMenu),(wIDEnableItem),(fEnable)?MF_ENABLED:MF_GRAYED)

#define MyCheckMenuItem(hMenu, wIDCheckItem, fCheck) \
    CheckMenuItem((hMenu),(wIDCheckItem),(fCheck)?MF_CHECKED:MF_UNCHECKED)


HANDLE ghInst;
HWND ghwndSpyApp;
HWND ghwndPrintf = NULL;
HANDLE ghHookThread = NULL;
HWND ghwndSpyHook = NULL;
HWND ghwndSpyingOn = NULL;              // The window we are spying on.
HFONT ghfontPrintf;
INT gnLines;
BOOL gfSpyOn = FALSE;
BOOL gfSpyAll;
BOOL gfOutputWin;
BOOL gfOutputCom1;
BOOL gfOutputFile;
HFILE gfhFile;
HANDLE gfhCom1;
CHAR gszFile[MAXSTRING];
INT gcxBorder;
INT gcyBorder;
BOOL gfMsgsUser;                        // TRUE to spy on all WM_USER messages.
BOOL gfMsgsUnknown;                     // TRUE to spy on all unknown msgs.
CHAR gszAppName[] = SPYAPPNAME;
UCHAR gszWindowName[40];
WINDOWPLACEMENT gwndpl;


PRIVATE HANDLE ghaccelTbl;              // Accelerator table handle.
PRIVATE CHAR gszSpyClassName[] = SPYCLASSNAME;


PRIVATE BOOL SpyInit(HANDLE hInstance, INT nCmdShow);
PRIVATE VOID PutOptions(VOID);
PRIVATE VOID InitMenu(HMENU hmenu);
PRIVATE LRESULT SpyCommand(HWND hwnd, INT nCmd, INT nNotifyCode);



/*****************************************************************************\
* WinMain
*
* Main entry point for the Spy app.
*
\*****************************************************************************/

INT WINAPI
WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    INT nCmdShow
    )
{
    MSG msg;

    if (!SpyInit(hInstance, nCmdShow))
        return FALSE;

    if (!CreateHookThread())
        goto closespy;

    /*
     * Polling messages from event queue
     */
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(ghwndSpyApp, ghaccelTbl, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

closespy:
    if (IsWindow(ghwndSpyApp))
    {
        if (DestroyWindow(ghwndSpyApp))
        {
            ghwndSpyApp = NULL;
        }
    }

    if (IsWindow(ghwndPrintf))
    {
        if (DestroyWindow(ghwndPrintf))
        {
            ghwndPrintf = NULL;
        }
    }

    return (INT)msg.wParam;
}



/*****************************************************************************\
* SpyInit
*
* Initializes the Spy application.
*
* Arguments:
*   HANDLE hInstance - handle to the instance of SPY.
*   INT nCmdShow - show the window?
*
* Returns:
*   TRUE if successful, FALSE otherwise.
*
\*****************************************************************************/

PRIVATE BOOL
SpyInit(
    HANDLE hInstance,
    INT nCmdShow
    )
{
    WNDCLASS wc;
    HWND hwndT;
    CHAR szClassName[40];
    BOOL bFoundPrevSpy = FALSE;
    INT i;
    INT j;

    ghInst = hInstance;

    /*
     * Loop through windows to find one of the spy class.
     */
    for (hwndT = GetWindow(GetDesktopWindow(), GW_CHILD); hwndT;
        hwndT = GetWindow(hwndT, GW_HWNDNEXT))
    {
        if (GetClassName(hwndT, szClassName, 40))
        {
            if (!lstrcmpi(szClassName, gszSpyClassName))
            {
                bFoundPrevSpy = TRUE;
                break;
            }
        }
    }

    if (bFoundPrevSpy)
    {
        if (hwndT)
            SendMessage(hwndT, WM_EXECINSTANCE, 0, 0);

        return FALSE;
    }

    if (!(ghaccelTbl = LoadAccelerators(ghInst, "spy")))
        return FALSE;

    ReadRegistry();

    gcxBorder = GetSystemMetrics(SM_CXBORDER);
    gcyBorder = GetSystemMetrics(SM_CYBORDER);

    //
    // Calculate the counts in the message groups.  This is best
    // done at run time to be safe.
    //
    for (i = 0; i < gcMessages; i++)
    {
        //
        // If this message belongs to a message group,
        // increment the total for that group.
        //
        for (j = 0; j < gcMsgGroups; j++)
        {
            if (gaMsgGroup[j].flMask & gaMsgs[i].Flags)
                gaMsgGroup[j].cMsgs++;
        }
    }

    lstrcpy(gszWindowName,LoadResourceString(IDS_APPLICATION_NAME));

    wc.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon          = LoadIcon(hInstance, gszAppName);
    wc.lpszMenuName   = gszAppName;
    wc.lpszClassName  = gszSpyClassName;
    wc.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hInstance      = hInstance;
    wc.style          = CS_BYTEALIGNCLIENT;
    wc.lpfnWndProc    = SpyWndProc;
    wc.cbWndExtra     = 0;
    wc.cbClsExtra     = 0;

    if (!RegisterClass(&wc))
        return FALSE;

    ghwndSpyApp = CreateWindow(gszSpyClassName, gszWindowName,
        WS_OVERLAPPEDWINDOW, 0, 0, 0, 0,
        NULL, NULL, hInstance, NULL);

    if (!ghwndSpyApp)
        return FALSE;

    if (nCmdShow != SW_SHOWNORMAL)
        gwndpl.showCmd = nCmdShow;

    SetWindowPlacement(ghwndSpyApp, &gwndpl);

    return TRUE;
}



/*****************************************************************************\
* SpyWndProc
*
* Main window procedure for the spy app.
*
* Arguments:
*    HWND hwnd - handle to the spy window
*    UINT msg - message
*    WPARAM wParam - message parameter
*    LPARAM lParam - message parameter
*
* Returns:
*   The value that the window proc should return, based on the processing
*   of the specific WM_COMMAND message received.
\*****************************************************************************/

LRESULT CALLBACK
SpyWndProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
    )
{
    switch (msg)
    {
        case WM_CREATE:
            MyCreatePrintfWin(hwnd);
            return 0;

        case WM_INITMENU:
            if (GetMenu(ghwndSpyApp) == (HMENU)wParam)
                InitMenu((HMENU)wParam);

            break;

        case WM_COMMAND:
            return SpyCommand(hwnd, LOWORD(wParam), HIWORD(wParam));

        case WM_ACTIVATE:
            /*
             * Set the focus to the printf window if we are being activated.
             */
            if (LOWORD(wParam))
                SetFocus(ghwndPrintf);

            break;

        case WM_SIZE:
            /*
             * Size the printf window to fit into the new client area size.
             */
            MoveWindow(ghwndPrintf, -gcxBorder, -gcyBorder,
                LOWORD(lParam) + (2 * gcxBorder),
                HIWORD(lParam) + (2 * gcyBorder), TRUE);
            break;

        case WM_CLOSE:
            SetSpyHook(FALSE);

            if (gfhCom1 != INVALID_HANDLE_VALUE)
                CloseHandle(gfhCom1);
            if (gfhFile)
                _lclose(gfhFile);

            SendMessage(ghwndSpyHook, WM_CLOSE, 0, 0);
            WriteRegistry();
            WaitForSingleObject(ghHookThread, INFINITE);
            DestroyWindow(ghwndSpyApp);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);   /* Kill the main window */
            ghwndSpyApp = NULL;
            ghwndPrintf = NULL;
            break;

        case WM_EXECINSTANCE:
            /*
             * another instance of spy has been started.
             */
            if (IsIconic(hwnd))
                ShowWindow(hwnd,SW_SHOWNORMAL);

            SetForegroundWindow(hwnd);
            BringWindowToTop(hwnd);

            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}



/*****************************************************************************\
* InitMenu
*
* This function grays/enables and checks/unchecks the menu items
* appropriately for the given state.
*
* Arguments:
*   HMENU hmenu - The menu handle.
*
* Returns:
*   VOID
\*****************************************************************************/

PRIVATE VOID
InitMenu(
    HMENU hmenu
    )
{
    BOOL fEnable = !IsPrintfEmpty();

    MyEnableMenuItem(hmenu, MENU_EDIT_CUT, fEnable);
    MyEnableMenuItem(hmenu, MENU_EDIT_COPY, fEnable);
    MyEnableMenuItem(hmenu, MENU_EDIT_CLEAR, fEnable);
}



/*****************************************************************************\
* SpyCommand
*
* Handles thw WM_COMMAND messages for the Spy app.
*
* Arguments:
*   HWND hwnd       - Window handle of the main app window.
*   INT nCmd        - Command value.
*   INT nNotifyCode - The notify code.
*
* Returns:
*   The value that the window proc should return, based on the processing
*   of the specific WM_COMMAND message received.
\*****************************************************************************/

PRIVATE LRESULT
SpyCommand(
    HWND hwnd,
    INT nCmd,
    INT nNotifyCode
    )
{
    HMENU hmenu;

    switch (nCmd)
    {
        case MENU_SPY_SELECTWINDOW:
            MyDialogBox(DID_SELECTWINDOW, SelectWindowDlgProc);
            break;

        case MENU_SPY_ABOUT:
            MyDialogBox(DID_ABOUT, AboutDlgProc);
            break;

        case MENU_SPY_EXIT:
            PostMessage(hwnd, WM_CLOSE, 0, 0);
            break;

        case MENU_EDIT_CUT:
            if (CopyToClipboard())
                ClearPrintfWindow(ghwndPrintf);

            break;

        case MENU_EDIT_COPY:
            CopyToClipboard();
            break;

        case MENU_EDIT_CLEAR:
            ClearPrintfWindow(ghwndPrintf);
            break;

        case MENU_OPTIONS_MESSAGES:
            MyDialogBox(DID_MESSAGES, MessagesDlgProc);
            break;

        case MENU_OPTIONS_FONT:
            SelectFont();
            break;

        case MENU_OPTIONS_OUTPUT:
            MyDialogBox(DID_OUTPUT, OutputDlgProc);
            break;

        case MENU_START:
            if (SetSpyHook(TRUE))
            {
                hmenu = GetMenu(hwnd);
                ModifyMenu(hmenu, MENUPOS_STARTSTOP, MF_BYPOSITION | MF_STRING,
                    MENU_STOP, LoadResourceString(IDS_MENU_STOP));
                DrawMenuBar(hwnd);

                SetSpyCaption();
            }

            break;

        case MENU_STOP:
            if (SetSpyHook(FALSE))
            {
                hmenu = GetMenu(hwnd);
                ModifyMenu(hmenu, MENUPOS_STARTSTOP, MF_BYPOSITION | MF_STRING,
                    MENU_START, LoadResourceString(IDS_MENU_START));
                DrawMenuBar(hwnd);

                SetSpyCaption();
            }

            break;
    }

    return 0;
}


