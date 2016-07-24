
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
* Module: dialogs.c
*
*   Contains dialog procs for the Windows debugging Spy SDK applet.
*
* Functions:
*
*   MyDialogBox()
*   AboutDlgProc()
*   SelectWindowDlgProc()
*   OutputDlgProc()
*   SelectFont()
*   MessagesDlgProc()
*   SelectWindowCommand()
*   FillListBox()
*   AddOneWindow()
*   MakeWindowName()
*   FindHwndInListBox()
*   HighlightWindow()
*   SelectWindowUpdateInfo()
*   SelectWindowEnableFields()
*   OutputCommand()
*   MessagesInit()
*   MessagesCommand()
*   MessagesUpdateCheckBoxes()
*
* Comments:
*
\*****************************************************************************/

#include "spy.h"

#include <commdlg.h>


#define DINV                3


PRIVATE HWND ghwndSpyingOnTemp;     // Temp when selecting hwnd to spy on.
PRIVATE HWND ghwndDlgBeingFilled = NULL;
PRIVATE BOOL bBorderOn = FALSE;
PRIVATE INT gcItemsSave;


PRIVATE BOOL SelectWindowCommand(HWND hwnd, INT nCmd, INT nNotifyCode);
PRIVATE VOID FillListBox(HWND hDlg, HWND hwndList, HWND hwnd);
BOOL CALLBACK AddOneWindow(HWND hwnd, LPARAM hwndListLP);
PRIVATE VOID MakeWindowName(HWND hwnd, LPSTR lpString, INT nStringLen);
PRIVATE INT FindHwndInListBox(HWND hwndList, HWND hSpyWnd);
PRIVATE VOID HighlightWindow(HWND hwnd, BOOL fDraw);
PRIVATE VOID SelectWindowUpdateInfo(HWND hDlg, HWND hwnd);
PRIVATE VOID SelectWindowEnableFields(HWND hwnd, BOOL fEnable);
PRIVATE BOOL OutputCommand(HWND hwnd, INT nCmd, INT nNotifyCode);
PRIVATE VOID MessagesInit(HWND hwnd);
PRIVATE BOOL MessagesCommand(HWND hwnd, INT nCmd, INT nNotifyCode);
PRIVATE VOID MessagesUpdateCheckBoxes(HWND hwnd);


CHAR szConsoleWindowClass[] = "ConsoleWindowClass";

/*****************************************************************************\
* MyDialogBox
*
* Puts up the specified dialog.
*
* Arguments:
*   INT idDlg          - The resource id of the dialog to display.
*   DLGPROC pfnDlgProc - The dialog proc to use.
*
* Returns:
*   The return value from DialogBox (whatever the dialog proc passes
*   to EndDialog).
\*****************************************************************************/

BOOL
MyDialogBox(
    INT idDlg,
    DLGPROC pfnDlgProc
    )
{
    return DialogBox(ghInst, MAKEINTRESOURCE(idDlg), ghwndSpyApp, pfnDlgProc);
}



/*****************************************************************************\
* AboutDlgProc
*
*   Dialog proc for the About box.
*
\*****************************************************************************/

BOOL CALLBACK
AboutDlgProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
    )
{
    switch (msg) {
        case WM_INITDIALOG:
            return TRUE;

        case WM_COMMAND:
            EndDialog(hwnd, IDOK);
            break;
    }

    return FALSE;
}



/*****************************************************************************\
* SelectWindowDlgProc
*
* Dialog proc for the Select Window dialog.  This dialog allows the user
* to select which window they want to spy on.
*
* Arguments:
*   HWND hwnd       - Window handle of the dialog.
*   UINT msg        - Message sent to window.
*   WPARAM wParam   - Message parameter.
*   LPARAM lParam   - Message parameter.
*
* Returns:
*   The value that the dialog proc should return, based on the processing
*   of the specific WM_COMMAND message received.
\*****************************************************************************/

BOOL CALLBACK
SelectWindowDlgProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
    )
{
    HWND hwndList;
    INT nIndex;

    UNREFERENCED_PARAMETER(lParam);

    switch (msg)
    {
        case WM_INITDIALOG:
            hwndList = GetDlgItem(hwnd, DID_SELWINLIST);
            ghwndSpyingOnTemp = ghwndSpyingOn == HWND_ALL ? NULL : (ghwndSpyingOn == NULL ? NULL
                : GetParent(ghwndSpyingOn));
            FillListBox(hwnd, hwndList, ghwndSpyingOnTemp);
            nIndex = FindHwndInListBox(hwndList, ghwndSpyingOn == HWND_ALL ? NULL
                : ghwndSpyingOn);
            SendMessage(hwndList, LB_SETCURSEL, nIndex, 0);
            ghwndSpyingOnTemp = (HWND)SendMessage(hwndList, LB_GETITEMDATA, nIndex, 0);
            SelectWindowUpdateInfo(hwnd, ghwndSpyingOnTemp);
            CheckDlgButton(hwnd, DID_SELWINALLWINDOWS, gfSpyAll);
            SelectWindowEnableFields(hwnd, !gfSpyAll);
            SetFocus(hwnd);
            HighlightWindow(ghwndSpyingOnTemp, TRUE);

            return TRUE;

        case WM_NCLBUTTONDOWN:
            if (wParam == HTCAPTION)
            {
                //
                // The mouse is down for a move of the dialog, so clean up the
                // border stuff.
                //
                if (bBorderOn)
                    HighlightWindow(ghwndSpyingOnTemp, FALSE);
            }

            return FALSE;

        case WM_KEYDOWN:
        case WM_LBUTTONUP:
        case WM_NCLBUTTONUP:
            //
            // The mouse is up from a move of the dialog, so put up the
            // border stuff again.
            //
            if (!bBorderOn)
                HighlightWindow(ghwndSpyingOnTemp, TRUE);

            return FALSE;

        case WM_CANCELMODE:
            return FALSE;

        case WM_COMMAND:
            return SelectWindowCommand(hwnd, LOWORD(wParam), HIWORD(wParam));
    }

    return FALSE;
}



/*****************************************************************************\
* SelectWindowCommand
*
* Handles thw WM_COMMAND messages for the Select Window dialog.
*
* Arguments:
*   HWND hwnd       - Window handle of the dialog.
*   INT nCmd        - Command value.
*   INT nNotifyCode - The notify code.
*
* Returns:
*   The value that the dialog proc should return, based on the processing
*   of the specific WM_COMMAND message received.
\*****************************************************************************/

PRIVATE BOOL
SelectWindowCommand(
    HWND hwnd,
    INT nCmd,
    INT nNotifyCode
    )
{
    INT nIndex;
    HWND hwndList;
    CHAR rgString[32];

    switch (nCmd)
    {
        case IDOK:
            SetWindowToSpyOn(IsDlgButtonChecked(hwnd, DID_SELWINALLWINDOWS) ?
                HWND_ALL : ghwndSpyingOnTemp);

            if (bBorderOn)
                HighlightWindow(ghwndSpyingOnTemp, FALSE);

            EndDialog(hwnd, IDOK);
            return TRUE;

        case IDCANCEL:
            if (bBorderOn)
                HighlightWindow(ghwndSpyingOnTemp, FALSE);

            EndDialog(hwnd, IDCANCEL);
            return TRUE;

        case DID_SELWINLIST:
            //
            // User single clicked or doubled clicked in listbox -
            //   Single click means select a window to spy on
            //   Double click means enumerate all the children of that window.
            //
            hwndList = GetDlgItem(hwnd, DID_SELWINLIST);
            switch (nNotifyCode)
            {
                case LBN_SELCHANGE:
                    //
                    // Single click case. Select a window to spy upon.
                    //
                    // Get the window handle, set it as the window to spy on.
                    //

                    if (bBorderOn)
                        HighlightWindow(ghwndSpyingOnTemp, FALSE);

                    nIndex = (INT)SendMessage(hwndList, LB_GETCURSEL, 0, 0);
                    ghwndSpyingOnTemp = (HWND)SendMessage(hwndList, LB_GETITEMDATA,
                        nIndex, 0);
                    SelectWindowUpdateInfo(hwnd, ghwndSpyingOnTemp);

                    HighlightWindow(ghwndSpyingOnTemp, TRUE);

                    break;

                case LBN_DBLCLK:
                    //
                    // Double click case - first click has already been
                    // processed as single click. In this case, the user has
                    // requested to look at all the children of a given
                    // selection.
                    //
                    // Get the current selection, and check to see if it is the
                    // " [ parent.. ]" entry. If so, go up one level first.
                    //

                    SetCursor(LoadCursor(NULL, IDC_WAIT));

                    if (bBorderOn)
                        HighlightWindow(ghwndSpyingOnTemp, FALSE);

                    nIndex = (INT)SendMessage(hwndList, LB_GETCURSEL, 0, 0);
                    ghwndSpyingOnTemp = (HWND)SendMessage(hwndList, LB_GETITEMDATA,
                        nIndex, 0);
                    SendMessage(hwndList, LB_GETTEXT, nIndex, (LPARAM)rgString);

                    if (rgString[0] == ' ')
                    {
                        // At top?  If so, we are done.
                        if (ghwndSpyingOnTemp == NULL)
                        {
                            SetCursor(LoadCursor(NULL, IDC_ARROW));
                            break;
                        }

                        ghwndSpyingOnTemp = GetParent(ghwndSpyingOnTemp);
                    }

                    SendMessage(hwndList, LB_RESETCONTENT, 0, 0);
                    FillListBox(hwnd, hwndList, ghwndSpyingOnTemp);

                    nIndex = FindHwndInListBox(hwndList, ghwndSpyingOnTemp);
                    SendMessage(hwndList, LB_SETCURSEL, nIndex, 0);
                    ghwndSpyingOnTemp = (HWND)SendMessage(hwndList, LB_GETITEMDATA,
                        nIndex, 0);
                    HighlightWindow(ghwndSpyingOnTemp,TRUE);
                    SelectWindowUpdateInfo(hwnd, ghwndSpyingOnTemp);
                    SetCursor(LoadCursor(NULL, IDC_ARROW));
                    break;
            }

            break;

        case DID_SELWINALLWINDOWS:
            SelectWindowEnableFields(hwnd,
                !IsDlgButtonChecked(hwnd, DID_SELWINALLWINDOWS));

            break;
    }

    return FALSE;
}



/*****************************************************************************\
* FillListBox
*
* Fills the listbox in the Select Window dialog with the names of
* the child windows of the given window.
*
* Arguments:
*   hDlg     - Window handle of the dialog window
*   hwndList - Handle to the listbox within the dialog.
*   hwnd     - Parent whose children to enumerate.
*
* Returns:
*   VOID
\*****************************************************************************/

PRIVATE VOID
FillListBox(
    HWND hDlg,
    HWND hwndList,
    HWND hwnd
    )
{
    INT nIndex;

    //
    // First fill the list box with child windows
    //
    // Make sure we display the list box after things are added.
    //

    SendMessage(hwndList, WM_SETREDRAW, 0, 0);

    //
    // remember which dialog we are processing
    //

    ghwndDlgBeingFilled = hDlg;

    if (hwnd == NULL)
    {
        //
        // Enumerate the top level separately... gross unsymmetry, but
        // hey.
        //
        EnumWindows(AddOneWindow, (LPARAM)hwndList);
    }
    else
    {
        EnumChildWindows(hwnd, AddOneWindow, (LPARAM)hwndList);
    }

    //
    // Now give the user a method of getting back to the parent. The space at
    // the beginning of the " [parent]" string identifies the entry as the
    // parent entry and makes it different from all the other entries since
    // the others start with a handle number of some sort.
    //

    nIndex = SendMessage(hwndList, LB_ADDSTRING, 0,
                        (LPARAM)(LPCTSTR)LoadResourceString(IDS_PARENT));
    SendMessage(hwndList, LB_SETITEMDATA, nIndex, (LONG)hwnd);

    //
    // Now do the redraw...
    //

    SendMessage(hwndList, WM_SETREDRAW, 1, 0);
}



/*****************************************************************************\
* AddOneWindow
*
* Gets the windows to add to the list of windows to spy on.
*
* Arguments:
*     HWND hwnd - handle of the window to add.
*     HWND hwndList - handle to the listbox.
*
* Returns:
*     TRUE - if a window was created.
\*****************************************************************************/
#define CCH_RGBUF   32

BOOL CALLBACK
AddOneWindow(
    HWND hwnd,
    LPARAM hwndListLP
    )
{
    CHAR rgBuf[CCH_RGBUF];
    INT nIndex;
    HWND htemp;
    HWND hwndList = (HWND)hwndListLP;
    //
    // Make sure we don't add any window that has anything to do with
    // the dialog or any other spy window
    //

    htemp = GetParent(hwnd);

    // Don't put console windows in the list since they can not be hooked
    if (GetClassName(hwnd, rgBuf, CCH_RGBUF) != 0 &&
        strcmp(rgBuf, szConsoleWindowClass) == 0 )
    {
        return 1;
    }

    // Don't put windows that belong to spy in the list
    if (hwnd == ghwndDlgBeingFilled || htemp == ghwndDlgBeingFilled
        || hwnd == ghwndSpyApp
        || htemp == ghwndSpyApp || hwnd == ghwndPrintf || htemp == ghwndPrintf
        || hwnd == ghwndSpyHook || htemp == ghwndSpyHook)
    {
        return 1;
    }

    MakeWindowName(hwnd, rgBuf, CCH_RGBUF);

    nIndex = SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)rgBuf);

    if (nIndex == LB_ERR || nIndex == LB_ERRSPACE)
        return 0;

    if (SendMessage(hwndList, LB_SETITEMDATA, nIndex, (LPARAM)hwnd) == LB_ERR)
        return 0;

    return 1;
}



/*****************************************************************************\
* MakeWindowName
*
* Builds the window name from the window handle passed in.
*
* Arguments:
*     HWND hwnd - handle to the window.
*     LPSTR lpString - String to put window name into.
*     INT nSTringLen - Length of window string.
*
* Returns:
*     VOID
\*****************************************************************************/

PRIVATE VOID
MakeWindowName(
    HWND hwnd,
    LPSTR lpString,
    INT nStringLen
    )
{
    wsprintf(lpString, "%8.8lX:", hwnd);

    if (hwnd == NULL || !IsWindow(hwnd))
    {
        lstrcat(lpString, "!!!");
    }
    else
    {
        GetWindowText(hwnd, lpString + 9, nStringLen - 9);
    }
}



/*****************************************************************************\
* FindHwndInListBox
*
* Gets the window from the list of windows in the listbox.
*
* Arguments:
*     HWND hwndList - handle to the listbox.
*     HWND hSpyWnd - handle to the spy window.
*
* Returns:
*     INT - Index to the window in the listbox. 
\*****************************************************************************/

PRIVATE INT
FindHwndInListBox(
    HWND hwndList,
    HWND hSpyWnd
    )
{
    CHAR rgBuf[9];
    INT nIndex;

    wsprintf(rgBuf, "%08lX", (LONG)hSpyWnd);

    nIndex = SendMessage(hwndList, LB_FINDSTRING, (WPARAM)-1, (LPARAM)rgBuf);

    if (nIndex == LB_ERR)
        nIndex = 0;

    return nIndex;
}



/*****************************************************************************\
* HighlightWindow
*
* Used to temporarily highlight the window that the user has selected from
* the Select Window dialog.  It does this by inverting a border around the
* window.
*
* Arguments:
*     HWND hwnd - handle to the selected window.
*     BOOL fdraw - whether to draw the window inverted on non-inverted.
*
* Returns:
*     VOID
\*****************************************************************************/

PRIVATE VOID
HighlightWindow(
    HWND hwnd,
    BOOL fDraw
    )
{
    HDC hdc;
    RECT rc;

    bBorderOn = fDraw;

    if (hwnd == NULL || !IsWindow(hwnd))
        return;

    hdc = GetWindowDC(hwnd);
    GetWindowRect(hwnd, &rc);
    OffsetRect(&rc, -rc.left, -rc.top);

    if (!IsRectEmpty(&rc))
    {
        PatBlt(hdc, rc.left, rc.top, rc.right - rc.left, DINV,  DSTINVERT);
        PatBlt(hdc, rc.left, rc.bottom - DINV, DINV,
            -(rc.bottom - rc.top - 2 * DINV), DSTINVERT);
        PatBlt(hdc, rc.right - DINV, rc.top + DINV, DINV,
            rc.bottom - rc.top - 2 * DINV, DSTINVERT);
        PatBlt(hdc, rc.right, rc.bottom - DINV, -(rc.right - rc.left),
            DINV, DSTINVERT);
    }

    ReleaseDC(hwnd, hdc);
}



/*****************************************************************************\
* SelectWindowUpdateInfo
*
* Updates the informational fields in the Select Window dialog when
* a new window is selected from the hwnd listbox.
*
* Arguments:
*     HWND hDlg - handle to the select window dialog box.
*     HWND hwnd - handle to the new window selected.
*
* Returns:
*     VOID
\*****************************************************************************/

PRIVATE VOID
SelectWindowUpdateInfo(
    HWND hDlg,
    HWND hwnd
    )
{
    HWND hParent;
    DWORD dwStyle;
    RECT rc;
    CHAR szTemp[MAXSTRING];

    if (hwnd)
    {
        hParent = GetParent(hwnd);
        dwStyle = GetWindowLong(hwnd, GWL_STYLE);

        MakeWindowName(hwnd, szTemp, MAXSTRING);
        SetDlgItemText(hDlg, DID_SELWINWINDOW, szTemp);

        GetClassName(hwnd, szTemp, MAXSTRING);
        SetDlgItemText(hDlg, DID_SELWINCLASS, szTemp);

        if (hParent)
        {
            MakeWindowName(hParent, szTemp, MAXSTRING);
            SetDlgItemText(hDlg, DID_SELWINPARENT, szTemp);
        }
        else
        {
            SetDlgItemText(hDlg, DID_SELWINPARENT,
                           LoadResourceString(IDS_NOPARENT));
        }

        GetWindowRect(hwnd, &rc);
        wsprintf(szTemp, "(%d,%d)-(%d,%d) %dx%d", rc,
            rc.right-rc.left, rc.bottom-rc.top);
        SetDlgItemText(hDlg, DID_SELWINRECT, szTemp);

        if (dwStyle & WS_POPUP)
            wsprintf (szTemp, "%08lX: WS_POPUP", dwStyle);
        else if (dwStyle & WS_CHILD)
            wsprintf (szTemp, "%08lX: WS_CHILD, ID: %lX", dwStyle,
            GetWindowLong(hwnd, GWL_ID));
        else if (dwStyle & WS_ICONIC)
            wsprintf (szTemp, "%08lX: WS_ICONIC", dwStyle);
        else
            wsprintf (szTemp, "%08lX: WS_OVERLAPPED", dwStyle);

        SetDlgItemText(hDlg, DID_SELWINSTYLE, szTemp);
    }
    else
    {
        TCHAR lpBuf[256];
        LoadString(GetModuleHandle(NULL), IDS_UNDEFINED, lpBuf, sizeof(lpBuf));
        SetDlgItemText(hDlg, DID_SELWINWINDOW, lpBuf);
        SetDlgItemText(hDlg, DID_SELWINCLASS,  lpBuf);
        SetDlgItemText(hDlg, DID_SELWINPARENT, lpBuf);
        SetDlgItemText(hDlg, DID_SELWINRECT,   lpBuf);
        SetDlgItemText(hDlg, DID_SELWINSTYLE,  lpBuf);
    }
}



/*****************************************************************************\
* SelectWindowEnableFields
*
* Enables/disables the different fields in the Select Window dialog
* based on whether the user wants to spy on all windows or individually
* select one.
*
* Arguments:
*   HWND hwnd    - Dialog window handle.
*   BOOL fEnable - TRUE to enable the fields, FALSE to disable them.
*
* Returns:
*   VOID
\*****************************************************************************/

PRIVATE VOID
SelectWindowEnableFields(
    HWND hwnd,
    BOOL fEnable
    )
{
    EnableWindow(GetDlgItem(hwnd, DID_SELWINLIST), fEnable);
    EnableWindow(GetDlgItem(hwnd, DID_SELWINWINDOW), fEnable);
    EnableWindow(GetDlgItem(hwnd, DID_SELWINCLASS), fEnable);
    EnableWindow(GetDlgItem(hwnd, DID_SELWINPARENT), fEnable);
    EnableWindow(GetDlgItem(hwnd, DID_SELWINRECT), fEnable);
    EnableWindow(GetDlgItem(hwnd, DID_SELWINSTYLE), fEnable);
}



/*****************************************************************************\
* OutputDlgProc
*
* Dialog proc for the Output dialog.
*
* Arguments:
*   HWND hwnd - handle to the output dialog
*   UINT msg - message sent to output dialog
*   WPARAM wParam - message parameter.
*   LPARAM lParam - message parameter.
*
* Returns:
*   The value that the dialog proc should return, based on the processing
*   of the specific WM_COMMAND message received.
*
\*****************************************************************************/

BOOL CALLBACK
OutputDlgProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
    )
{
    switch (msg)
    {
        case WM_INITDIALOG:
            CheckDlgButton(hwnd, DID_OUTPUTWINDOW, gfOutputWin);
            CheckDlgButton(hwnd, DID_OUTPUTCOM1, gfOutputCom1);
            CheckDlgButton(hwnd, DID_OUTPUTFILE, gfOutputFile);

            SetDlgItemText(hwnd, DID_OUTPUTFILENAME, gszFile);
            SetDlgItemInt(hwnd, DID_OUTPUTLINES, gnLines, FALSE);

            return TRUE;

        case WM_COMMAND:
            return OutputCommand(hwnd, LOWORD(wParam), HIWORD(wParam));
    }

    return FALSE;
}



/*****************************************************************************\
* OutputCommand
*
* Handles the WM_COMMAND messages for the Output dialog.
*
* Arguments:
*   HWND hwnd       - Window handle of the dialog.
*   INT nCmd        - Command value.
*   INT nNotifyCode - The notify code.
*
* Returns:
*   The value that the dialog proc should return, based on the processing
*   of the specific WM_COMMAND message received.
*
\*****************************************************************************/

PRIVATE BOOL
OutputCommand(
    HWND hwnd,
    INT nCmd,
    INT nNotifyCode
    )
{
    HFILE fh;
    INT i;
    CHAR szTemp[MAXSTRING];

    switch (nCmd)
    {
        case IDOK:
            i = GetDlgItemInt(hwnd, DID_OUTPUTLINES, &i, FALSE);
            if (i != gnLines)
            {
                if ( i > 0 && i <= LINES_MAX)
                {
                    gnLines = i;
                    MyCreatePrintfWin(ghwndSpyApp);
                }
                else
                {
                    Message(MB_OK | MB_ICONEXCLAMATION,
                        LoadResourceString(IDS_ERROR_WND_LINE), LINES_MAX);
                    SetFocus(GetDlgItem(hwnd, DID_OUTPUTLINES));
                    break;
                }
            }

            gfOutputWin = IsDlgButtonChecked(hwnd, DID_OUTPUTWINDOW);
            gfOutputCom1 = IsDlgButtonChecked(hwnd, DID_OUTPUTCOM1);
            gfOutputFile = IsDlgButtonChecked(hwnd, DID_OUTPUTFILE);

            if (gfOutputFile)
            {
                GetDlgItemText(hwnd, DID_OUTPUTFILENAME, szTemp, MAXSTRING);
                /*
                 * If they changed the filename, or the file is not open
                 * then open it.
                 */
                if (lstrcmp(gszFile, szTemp) != 0 || gfhFile == 0)
                {
                    fh = _lcreat(szTemp, 0);
                    if (fh == (HFILE)(-1))
                    {
                        if (Message(MB_OKCANCEL | MB_ICONEXCLAMATION,
                            LoadResourceString(IDS_ERROR_CANT_OPEN_FILE), szTemp) == IDCANCEL)
                        {
                            EndDialog(hwnd, FALSE);
                        }

                        return TRUE;
                    }

                    lstrcpy(gszFile, szTemp);
                    if (gfhFile)
                        _lclose(gfhFile);
                    gfhFile = fh;
                }
            }
            else
            {
                if (gfhFile)
                {
                    _lclose(gfhFile);
                    gfhFile = 0;
                }
            }

            if (gfOutputCom1)
            {
                if (gfhCom1 != INVALID_HANDLE_VALUE)
                    CloseHandle(gfhCom1);

                gfhCom1 = CreateFile(
                        "com1",
                        GENERIC_WRITE,
                        0,                    // exclusive access
                        NULL,                 // no security attrs
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL);
                if (gfhCom1 == INVALID_HANDLE_VALUE)
                {
                    if (Message(MB_OKCANCEL | MB_ICONEXCLAMATION,
                        LoadResourceString(IDS_ERROR_CANT_OPEN_COM1)) == IDCANCEL)
                    {
                        EndDialog(hwnd, FALSE);
                    }

                    return TRUE;
                }
            }
            else
            {
                if (gfhCom1 != INVALID_HANDLE_VALUE)
                {
                    CloseHandle(gfhCom1);
                }
            }

            EndDialog(hwnd, TRUE);
            return TRUE;

        case IDCANCEL:
            EndDialog(hwnd, FALSE);
            return TRUE;
    }

    return FALSE;
}



/*****************************************************************************\
* SelectFont
*
* Allows the user to select a new font for the display.
*
* Arguments:
*    none
*
* Returns:
*    VOID
\*****************************************************************************/

VOID
SelectFont(
    VOID
    )
{
    CHOOSEFONT cf;
    LOGFONT lf;
    HFONT hfontNew;

    GetObject(ghfontPrintf, sizeof(LOGFONT), (LPVOID)&lf);

    cf.lStructSize = sizeof(cf);
    cf.hwndOwner = ghwndSpyApp;
    cf.hDC = NULL;
    cf.lpLogFont = &lf;
    cf.iPointSize = 0;
    cf.Flags = CF_ANSIONLY | CF_FORCEFONTEXIST | CF_INITTOLOGFONTSTRUCT
        | CF_SCREENFONTS;
    cf.rgbColors = 0;
    cf.lCustData = 0;
    cf.lpfnHook = NULL;
    cf.lpTemplateName = NULL;
    cf.hInstance = NULL;
    cf.lpszStyle = NULL;
    cf.nFontType = 0;
    cf.nSizeMin = 0;
    cf.nSizeMax = 0;

    if (ChooseFont(&cf))
    {
        if (hfontNew = CreateFontIndirect(&lf))
        {
            SetPrintfFont(ghwndPrintf, hfontNew);
            ghfontPrintf = hfontNew;
        }
    }
}



/*****************************************************************************\
* MessagesDlgProc
*
* Dialog proc for the Messages dialog.  This dialog allows the user
* to select which messages they want to spy on.
*
* Arguments:
*    HWND hwnd - handle to the dialog window.
*    UINT msg - message to the window
*    WPARAM wParam - message parameter
*    LPARAM lParam - message parameter
*
* Returns:
*   The value that the dialog proc should return, based on the processing
*   of the specific WM_COMMAND message received.
\*****************************************************************************/

BOOL CALLBACK
MessagesDlgProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
    )
{
    switch (msg)
    {
        case WM_INITDIALOG:
            MessagesInit(hwnd);
            return TRUE;

        case WM_COMMAND:
            return MessagesCommand(hwnd, LOWORD(wParam), HIWORD(wParam));
    }

    return FALSE;
}



/*****************************************************************************\
* MessagesInit
*
* Initializes the messages dialog.
*
* Arguments:
*   HWND hwnd - Dialog window handle.
*
* Returns:
*   VOID
\*****************************************************************************/

PRIVATE VOID
MessagesInit(
    HWND hwnd
    )
{
    HWND hwndList;
    INT i;
    INT j;
    INT iSel;
    PMSGDESC pmd;

    for (j = 0; j < gcMsgGroups; j++)
        gaMsgGroup[j].cUseCount = 0;

    hwndList = GetDlgItem(hwnd, DID_MSGSLIST);

    for (i = 0, pmd = gaMsgs; i < gcMessages; i++, pmd++)
    {
        iSel = (INT)SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)pmd->pszMsg);
        SendMessage(hwndList, LB_SETITEMDATA, iSel, (LPARAM)pmd);

        if (pmd->Flags & MTF_SELECTED)
        {
            SendMessage(hwndList, LB_SETSEL, TRUE, iSel);

            for (j = 0; j < gcMsgGroups; j++)
            {
                if (gaMsgGroup[j].flMask & pmd->Flags)
                    gaMsgGroup[j].cUseCount++;
            }
        }
    }

    //
    // Set the selection rectangle to the first item in the listbox.
    //
    SendMessage(hwndList, LB_SETCARETINDEX, 0, FALSE);

    //
    // Loop through all the message groups.
    //
    for (j = 0; j < gcMsgGroups; j++)
    {
        //
        // Is at least one message in the group selected?
        //
        if (gaMsgGroup[j].cUseCount)
        {
            //
            // Check the corresponding checkbox.  If all messages
            // in the group are selected, the checkbox is checked.
            // If only some are selected, the checkbox is set to
            // grayed (3-state).
            //
            CheckDlgButton(hwnd, gaMsgGroup[j].idCheckBox,
                (gaMsgGroup[j].cUseCount == gaMsgGroup[j].cMsgs) ? 1 : 2);
        }
    }

    if (gfMsgsUser)
        CheckDlgButton(hwnd, DID_MSGSUSER, 1);

    if (gfMsgsUnknown)
        CheckDlgButton(hwnd, DID_MSGSUNKNOWN, 1);

    gcItemsSave = SendMessage(hwndList, LB_GETSELITEMS,
        gcMessages, (LPARAM)gaiSelected);
}



/*****************************************************************************\
* MessagesCommand
*
* Handles the WM_COMMAND messages for the Messages dialog.
*
* Arguments:
*   HWND hwnd       - Window handle of the dialog.
*   INT nCmd        - Command value.
*   INT nNotifyCode - The notify code.
*
* Returns:
*   The value that the dialog proc should return, based on the processing
*   of the specific WM_COMMAND message received.
*
\*****************************************************************************/

PRIVATE BOOL
MessagesCommand(
    HWND hwnd,
    INT nCmd,
    INT nNotifyCode
    )
{
    INT i;
    INT j;
    PMSGGROUP pmg;
    PMSGDESC pmd;
    BOOL fChecked;
    HWND hwndList;
    INT cItems;
    BOOL fSel;
    INT iSel;
    INT cSelItemsMax;
    INT iTopIndex;

    switch (nCmd)
    {
        case DID_MSGSLIST:
            if (nNotifyCode == LBN_SELCHANGE)
            {
                hwndList = GetDlgItem(hwnd, DID_MSGSLIST);
                cItems = SendMessage(hwndList, LB_GETSELITEMS,
                    gcMessages, (LPARAM)gaiSelected2);
                if (cItems == gcItemsSave)
                {
                    //
                    // Nothing changed except for the selection
                    // rectangle moving.  We are done.
                    //
                    break;
                }

                if (cItems > gcItemsSave)
                {
                    //
                    // A message was selected.  Look for it.
                    //
                    for (i = 0; i < gcItemsSave &&
                        gaiSelected[i] == gaiSelected2[i]; i++)
                        ;

                    iSel = gaiSelected2[i];
                    fSel = TRUE;
                }
                else
                {
                    //
                    // A message was unselected.  Look for it.
                    //
                    for (i = 0; i < cItems &&
                        gaiSelected[i] == gaiSelected2[i]; i++)
                        ;

                    iSel = gaiSelected[i];
                    fSel = FALSE;
                }

                //
                // Get the currently selected item.  It was either
                // just turned on or off.
                //
                pmd = (PMSGDESC)SendMessage(hwndList, LB_GETITEMDATA, iSel, 0);

                //
                // Loop through the message groups and update the use
                // counts for all groups that contain this message.
                //
                for (i = 0; i < gcMsgGroups; i++)
                {
                    if (pmd->Flags & gaMsgGroup[i].flMask)
                    {
                        gaMsgGroup[i].cUseCount += fSel ? 1 : -1;
                    }
                }

                //
                // Be sure that the checkboxes reflect the updated
                // status of the message group use counts.
                //
                MessagesUpdateCheckBoxes(hwnd);

                //
                // Save away the new selected item array.
                //
                cSelItemsMax = max(cItems, gcItemsSave);
                for (i = 0; i < cSelItemsMax; i++)
                {
                    gaiSelected[i] = gaiSelected2[i];
                }

                gcItemsSave = cItems;
            }

            break;

        case DID_MSGSALL:
            hwndList = GetDlgItem(hwnd, DID_MSGSLIST);
            SendMessage(hwndList, LB_SETSEL, TRUE, (LPARAM)-1);

            for (i = 0; i < gcMsgGroups; i++)
            {
                gaMsgGroup[i].cUseCount = gaMsgGroup[i].cMsgs;
                CheckDlgButton(hwnd, gaMsgGroup[i].idCheckBox, 1);
            }

            gcItemsSave = SendMessage(hwndList, LB_GETSELITEMS, gcMessages,
                (LPARAM)gaiSelected);

            CheckDlgButton(hwnd, DID_MSGSUSER, 1);
            CheckDlgButton(hwnd, DID_MSGSUNKNOWN, 1);

            break;

        case DID_MSGSNONE:
            hwndList = GetDlgItem(hwnd, DID_MSGSLIST);
            SendMessage(hwndList, LB_SETSEL, FALSE, (LPARAM)-1);

            for (i = 0; i < gcMsgGroups; i++)
            {
                gaMsgGroup[i].cUseCount = 0;
                CheckDlgButton(hwnd, gaMsgGroup[i].idCheckBox, 0);
            }

            gcItemsSave = 0;

            CheckDlgButton(hwnd, DID_MSGSUSER, 0);
            CheckDlgButton(hwnd, DID_MSGSUNKNOWN, 0);

            break;

        case DID_MSGSDDE:
        case DID_MSGSCLIP:
        case DID_MSGSMOUSE:
        case DID_MSGSNC:
        case DID_MSGSKEYBD:
#ifdef FE_IME
        case DID_MSGSIME:
#endif
        case DID_MSGSBM:
        case DID_MSGSCB:
        case DID_MSGSEM:
        case DID_MSGSLB:
        case DID_MSGSSTM:
            for (i = 0; i < gcMsgGroups; i++)
            {
                if (gaMsgGroup[i].idCheckBox == nCmd)
                {
                    pmg = &gaMsgGroup[i];
                    break;
                }
            }

            fChecked = IsDlgButtonChecked(hwnd, pmg->idCheckBox);
            if (fChecked == 1)
                fChecked = FALSE;
            else
                fChecked = TRUE;

            hwndList = GetDlgItem(hwnd, DID_MSGSLIST);

            if (fChecked)
            {
                SendMessage(hwndList, WM_SETREDRAW, FALSE, 0);
                iTopIndex = SendMessage(hwndList, LB_GETTOPINDEX, 0, 0);
            }

            //
            // Get the list of currently selected items.
            //
            cItems = SendMessage(hwndList, LB_GETSELITEMS,
                gcMessages, (LPARAM)gaiSelected);

            //
            // Look for all the messages in this group.
            //
            for (i = 0, iSel = 0; i < gcMessages; i++)
            {
                pmd = (PMSGDESC)SendMessage(hwndList, LB_GETITEMDATA, i, 0);
                if (pmd->Flags & pmg->flMask)
                {
                    //
                    // Bump up through the list of selected items, looking
                    // to see if this item is currently selected.
                    //
                    for (fSel = FALSE; iSel < cItems &&
                        gaiSelected[iSel] <= i; iSel++)
                    {
                        //
                        // A match was found.  The item is selected.
                        //
                        if(gaiSelected[iSel] == i)
                        {
                            fSel = TRUE;
                            break;
                        }
                    }

                    //
                    // Is the current selection state of the item
                    // different from the desired selection state?
                    //
                    if (fSel != fChecked)
                    {
                        //
                        // Update the use counts of all groups that contain
                        // this message.
                        //
                        for (j = 0; j < gcMsgGroups; j++)
                        {
                            if (pmd->Flags & gaMsgGroup[j].flMask)
                            {
                                gaMsgGroup[j].cUseCount += fChecked ? 1 : -1;
                            }
                        }

                        //
                        // Select/deselect the message in the list box.
                        //
                        SendMessage(hwndList, LB_SETSEL, fChecked, i);
                    }
                }
            }

            //
            // Be sure that the checkboxes reflect the updated
            // status of the message group use counts.
            //
            MessagesUpdateCheckBoxes(hwnd);

            if (fChecked)
            {
                SendMessage(hwndList, LB_SETTOPINDEX, iTopIndex, 0);
                SendMessage(hwndList, WM_SETREDRAW, TRUE, 0);
                InvalidateRect(hwndList, NULL, FALSE);
            }

            gcItemsSave = SendMessage(hwndList, LB_GETSELITEMS,
                gcMessages, (LPARAM)gaiSelected);

            break;

        case IDOK:
            hwndList = GetDlgItem(hwnd, DID_MSGSLIST);
            cItems = SendMessage(hwndList, LB_GETSELITEMS,
                gcMessages, (LPARAM)gaiSelected);

            //
            // Unselect all messages.
            //
            for (i = 0; i < gcMessages; i++)
                gaMsgs[i].Flags &= ~MTF_SELECTED;

            //
            // Mark all the messages that are selected.
            //
            for (i = 0; i < cItems; i++)
            {
                pmd = (PMSGDESC)SendMessage(hwndList, LB_GETITEMDATA,
                    gaiSelected[i], 0);
                pmd->Flags |= MTF_SELECTED;
            }

            if (IsDlgButtonChecked(hwnd, DID_MSGSUSER))
                gfMsgsUser = TRUE;
            else
                gfMsgsUser = FALSE;

            if (IsDlgButtonChecked(hwnd, DID_MSGSUNKNOWN))
                gfMsgsUnknown = TRUE;
            else
                gfMsgsUnknown = FALSE;

#if 0  //Debug code!
for (i = 0; i < gcMsgGroups; i++)
{
    iSel = 0;
    for (j = 0; j < cItems; j++)
    {
        pmd = (PMSGDESC)SendMessage(hwndList, LB_GETITEMDATA,
            gaiSelected[j], 0);
        if (pmd->Flags & gaMsgGroup[i].flMask)
            iSel++;
    }

    if (iSel != gaMsgGroup[i].cUseCount)
    {
        DbgPrintf("Use counts are wrong!!!");
        for (j = 0; j < gcMsgGroups; j++)
        {
            DbgPrintf("cMsgs:%d Use:%d", gaMsgGroup[j].cMsgs, gaMsgGroup[j].cUseCount);
        }
    }
}
#endif // end debug code

            EndDialog(hwnd, IDOK);
            return TRUE;

        case IDCANCEL:
            EndDialog(hwnd, IDCANCEL);
            return TRUE;
    }

    return FALSE;
}



/*****************************************************************************\
* MessagesUpdateCheckBoxes
*
* Updates the message group checkboxes in the Messages dialog.
* This routine should be called when the use counts in the
* message group table are changed, so that the state of the
* checkboxes will get updated also.
*
* Arguments:
*   HWND hwnd - Dialog window handle.
*
* Returns:
*   VOID
\*****************************************************************************/

PRIVATE VOID
MessagesUpdateCheckBoxes(
    HWND hwnd
    )
{
    INT i;
    INT fState;

    for (i = 0; i < gcMsgGroups; i++)
    {
        if (gaMsgGroup[i].cUseCount == gaMsgGroup[i].cMsgs)
            fState = 1;
        else if (gaMsgGroup[i].cUseCount == 0)
            fState = 0;
        else
            fState = 2;

        CheckDlgButton(hwnd, gaMsgGroup[i].idCheckBox, fState);
    }
}
