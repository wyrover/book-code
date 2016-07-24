/**************************************************************************\
* dialogs.c -- dialog procedure support for uconvert
*
*         Steve Firebaugh
*         Microsoft Developer Support
*         Copyright (c) 1992-1997 Microsoft Corporation
*
\**************************************************************************/
#define UNICODE


#include <windows.h>
#include "uconvert.h"
#include "install.h"


/* Define affecting the positioning of child windows in dialog. */
#define DLGBORDER    GetSystemMetrics (SM_CXFRAME)*2


/***************************************************************************\
*    FUNCTION: ConversionOptionsProc
*
* Fill Dlg with state information on WM_INITDIALOG.
*  Take it back down and change internal state on WM_COMMAND, IDOK.
*
* Global variables:
*  gMBFlags
*  gWCFlags
*  glpDefaultChar
*  glpUsedDefaultChar
*
\***************************************************************************/
LRESULT CALLBACK ConversionOptionsProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

  switch (message) {

    /******************************************************************\
    *  WM_INITDIALOG
    *
    * Set radio buttons appropriately.
    \******************************************************************/
    case WM_INITDIALOG:
      if (gMBFlags & MB_PRECOMPOSED)
        SendDlgItemMessage (hwnd, DID_PRECOMPOSED, BM_SETCHECK, 1, 0);

      if (gMBFlags & MB_COMPOSITE)
        SendDlgItemMessage (hwnd, DID_COMPOSITE, BM_SETCHECK, 1, 0);

      if (gMBFlags & MB_USEGLYPHCHARS)
        SendDlgItemMessage (hwnd, DID_USEGLYPHCHARS, BM_SETCHECK, 1, 0);

      if (gWCFlags & WC_COMPOSITECHECK)
        SendDlgItemMessage (hwnd, DID_COMPOSITECHECK, BM_SETCHECK, 1, 0);

      if (gWCFlags & WC_DISCARDNS)
        SendDlgItemMessage (hwnd, DID_DISCARDNS, BM_SETCHECK, 1, 0);

      if (gWCFlags & WC_SEPCHARS)
        SendDlgItemMessage (hwnd, DID_SEPCHARS, BM_SETCHECK, 1, 0);

      if (gWCFlags & WC_DEFAULTCHAR)
        SendDlgItemMessage (hwnd, DID_DEFAULTCHAR, BM_SETCHECK, 1, 0);

      SendDlgItemMessage (hwnd, DID_EFDEFAULTCHAR, EM_LIMITTEXT, 1, 0);
      SetDlgItemTextA (hwnd, DID_EFDEFAULTCHAR, glpDefaultChar);

      if (gUsedDefaultChar)
        SendDlgItemMessage (hwnd, DID_USEDDEFAULTCHAR, BM_SETCHECK, 1, 0);


      SetFocus (GetDlgItem (hwnd, IDOK));
    return FALSE;


    case WM_COMMAND:
      switch (LOWORD (wParam)) {
        case IDCANCEL:
          EndDialog (hwnd, FALSE);
        break;

        /******************************************************************\
        *  WM_COMMAND, IDOK
        *
        * Get state from radio buttons and others.
        \******************************************************************/
        case IDOK: {

          if (SendDlgItemMessage(hwnd, DID_PRECOMPOSED, BM_GETCHECK, 0,0))
            gMBFlags |= MB_PRECOMPOSED;
          else
            gMBFlags &= ~MB_PRECOMPOSED;

          if (SendDlgItemMessage(hwnd, DID_COMPOSITE, BM_GETCHECK, 0,0))
            gMBFlags |= MB_COMPOSITE;
          else
            gMBFlags &= ~MB_COMPOSITE;

          if (SendDlgItemMessage(hwnd, DID_USEGLYPHCHARS, BM_GETCHECK, 0,0))
            gMBFlags |= MB_USEGLYPHCHARS;
          else
            gMBFlags &= ~MB_USEGLYPHCHARS;

          if (SendDlgItemMessage(hwnd, DID_DISCARDNS, BM_GETCHECK, 0,0))
            gWCFlags |= WC_DISCARDNS;
          else
            gWCFlags &= ~WC_DISCARDNS;

          if (SendDlgItemMessage(hwnd, DID_COMPOSITECHECK, BM_GETCHECK, 0,0))
            gWCFlags |= WC_COMPOSITECHECK;
          else
            gWCFlags &= ~WC_COMPOSITECHECK;

          if (SendDlgItemMessage(hwnd, DID_SEPCHARS, BM_GETCHECK, 0,0))
            gWCFlags |= WC_SEPCHARS;
          else
            gWCFlags &= ~WC_SEPCHARS;

          if (SendDlgItemMessage(hwnd, DID_DEFAULTCHAR, BM_GETCHECK, 0,0))
            gWCFlags |= WC_DEFAULTCHAR;
          else
            gWCFlags &= ~WC_DEFAULTCHAR;

          GetDlgItemTextA (hwnd, DID_EFDEFAULTCHAR, glpDefaultChar, 2); // CHAR + NULL

          EndDialog (hwnd, TRUE);
        } break;


        /******************************************************************\
        *  WM_COMMAND, DID_*
        *
        * Manage radio button pattern.
        \******************************************************************/
        case DID_PRECOMPOSED:
          SendDlgItemMessage (hwnd, DID_PRECOMPOSED, BM_SETCHECK, TRUE, 0);
          SendDlgItemMessage (hwnd, DID_COMPOSITE,   BM_SETCHECK, FALSE, 0);
        break;

        case DID_COMPOSITE:
          SendDlgItemMessage (hwnd, DID_PRECOMPOSED, BM_SETCHECK, FALSE, 0);
          SendDlgItemMessage (hwnd, DID_COMPOSITE,   BM_SETCHECK, TRUE, 0);
        break;

      }
    break; /* end WM_COMMAND */



    case WM_SYSCOMMAND:
      if (LOWORD (wParam) == SC_CLOSE)
          EndDialog (hwnd, FALSE);
    break;

  } /* end switch */
  return FALSE;
}



/***************************************************************************\
*    FUNCTION: SourceOptionsProc
*
* Fill Dlg with state information on WM_INITDIALOG.
*  Take it back down and change internal state on WM_COMMAND, IDOK.
*
* Global variables:
*   gTypeSource
*   giSourceCodePage
*   giDestinationCodePage
*
\***************************************************************************/
LRESULT CALLBACK SourceOptionsProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
TCHAR buffer[50];

  switch (message) {

    /******************************************************************\
    *  WM_INITDIALOG
    *
    * Set radio buttons appropriately.
    \******************************************************************/
    case WM_INITDIALOG:
      SetWindowText (hwnd, LoadResourceString(IDS_INTERPRET_SOURCE_AS));
      ListInstalledTables (GetDlgItem (hwnd, DID_CODEPAGELIST),CB_ADDSTRING, TRUE);
      if (gTypeSource == TYPEUNICODE)
        SendDlgItemMessage (hwnd, DID_RBUNICODE, BM_SETCHECK, 1, 0);

      /*
       * if gTypeSource == TYPECODEPAGE, then see if it is one of the
       *  standard radio buttons.  If so, check the right one.
       *  if not, check, "other" and select correct entry in combobox.
       */
      else if (gTypeSource == TYPECODEPAGE) {
        if (giSourceCodePage == GetACP())
          SendDlgItemMessage (hwnd, DID_RBANSICP, BM_SETCHECK, 1, 0);
        else if (giSourceCodePage == GetOEMCP())
          SendDlgItemMessage (hwnd, DID_RBOEMCP, BM_SETCHECK, 1, 0);
        else {
          SendDlgItemMessage (hwnd, DID_RBOTHERCP, BM_SETCHECK, 1, 0);
          wsprintf (buffer, TEXT("%d"), giSourceCodePage);
          SendDlgItemMessage (hwnd, DID_CODEPAGELIST, CB_SELECTSTRING, 0, (LPARAM)buffer);
        }
      } else {
        // OK to not be specified here.  Wait for user to make choice.
      }


      SetFocus (GetDlgItem (hwnd, IDOK));
    return FALSE;


    case WM_COMMAND:
      switch (LOWORD (wParam)) {
        case IDCANCEL:
          EndDialog (hwnd, FALSE);
        break;

        /******************************************************************\
        *  WM_COMMAND, IDOK
        *
        * Get state from radio buttons and others.
        \******************************************************************/
        case IDOK: {
          BOOL success;

          if (SendDlgItemMessage(hwnd, DID_RBUNICODE, BM_GETCHECK, 0,0)) {
            gTypeSource = TYPEUNICODE;
          } else if (SendDlgItemMessage(hwnd, DID_RBANSICP, BM_GETCHECK, 0,0)) {
            gTypeSource = TYPECODEPAGE;
            giSourceCodePage=GetACP();
          } else if (SendDlgItemMessage(hwnd, DID_RBOEMCP, BM_GETCHECK, 0,0)) {
            gTypeSource = TYPECODEPAGE;
            giSourceCodePage=GetOEMCP();
          } else if (SendDlgItemMessage(hwnd, DID_RBOTHERCP, BM_GETCHECK, 0,0)) {
            gTypeSource = TYPECODEPAGE;
            giSourceCodePage=GetDlgItemInt (hwnd, DID_CODEPAGELIST, &success, FALSE);
          } else
            gTypeSource = TYPEUNKNOWN;
          EndDialog (hwnd, TRUE);
        } break;
      }
    break; /* end WM_COMMAND */


    case WM_SYSCOMMAND:
      if (LOWORD (wParam) == SC_CLOSE)
          EndDialog (hwnd, FALSE);
    break;

  } /* end switch */
  return FALSE;
}

/***************************************************************************\
*    FUNCTION: DestinationOptionsProc
*
* Fill Dlg with state information on WM_INITDIALOG.
*  Take it back down and change internal state on WM_COMMAND, IDOK.
*
*
* Global variables:
*   gTypeSource
*   giSourceCodePage
*   giDestinationCodePage
*
\***************************************************************************/
LRESULT CALLBACK DestinationOptionsProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
TCHAR buffer[50];

  switch (message) {

    /******************************************************************\
    *  WM_INITDIALOG
    *
    * Set radio buttons appropriately.
    \******************************************************************/
    case WM_INITDIALOG:
      SetWindowText (hwnd, LoadResourceString(IDS_CONVERT_DEST_TO));
      ListInstalledTables (GetDlgItem (hwnd, DID_CODEPAGELIST),CB_ADDSTRING, TRUE);

      /* if source is unicode, destination will be code page. */
      if (gTypeSource == TYPEUNICODE) {
        EnableWindow (GetDlgItem (hwnd, DID_RBUNICODE), FALSE);

        if (giDestinationCodePage == GetACP())
          SendDlgItemMessage (hwnd, DID_RBANSICP, BM_SETCHECK, 1, 0);
        else if (giDestinationCodePage == GetOEMCP())
          SendDlgItemMessage (hwnd, DID_RBOEMCP, BM_SETCHECK, 1, 0);
        else {
          SendDlgItemMessage (hwnd, DID_RBOTHERCP, BM_SETCHECK, 1, 0);
          wsprintf (buffer, TEXT("%d"), giDestinationCodePage);
          SendDlgItemMessage (hwnd, DID_CODEPAGELIST, CB_SELECTSTRING, 0, (LPARAM)buffer);
        }

      /* otherwise destination is unicode, so disable most of the checkboxes. */
      } else if (gTypeSource == TYPECODEPAGE) {
        SendDlgItemMessage (hwnd, DID_RBUNICODE, BM_SETCHECK, 1, 0);
        EnableWindow (GetDlgItem (hwnd, DID_RBANSICP), FALSE);
        EnableWindow (GetDlgItem (hwnd, DID_RBOEMCP), FALSE);
        EnableWindow (GetDlgItem (hwnd, DID_RBOTHERCP), FALSE);
        EnableWindow (GetDlgItem (hwnd, DID_CODEPAGELIST), FALSE);
      } else {
        EndDialog (hwnd, FALSE);  // shouldn't get here.
      }

      SetFocus (GetDlgItem (hwnd, IDOK));
    return FALSE;


    case WM_COMMAND:
      switch (LOWORD (wParam)) {
        case IDCANCEL:
          EndDialog (hwnd, FALSE);
        break;

        /******************************************************************\
        *  WM_COMMAND, IDOK
        *
        * Get state from radio buttons and others.
        \******************************************************************/
        case IDOK: {
          BOOL success;
          if (SendDlgItemMessage(hwnd, DID_RBUNICODE, BM_GETCHECK, 0,0)) {
            // Do nothing. gTypeSource already implies dest <-> unicode.
          } else if (SendDlgItemMessage(hwnd, DID_RBANSICP, BM_GETCHECK, 0,0)) {
            giDestinationCodePage=GetACP();
          } else if (SendDlgItemMessage(hwnd, DID_RBOEMCP, BM_GETCHECK, 0,0)) {
            giDestinationCodePage=GetOEMCP();
          } else if (SendDlgItemMessage(hwnd, DID_RBOTHERCP, BM_GETCHECK, 0,0)) {
            giDestinationCodePage=GetDlgItemInt (hwnd, DID_CODEPAGELIST, &success, FALSE);
          }

          EndDialog (hwnd, TRUE);
        } break;
      }
    break; /* end WM_COMMAND */

    case WM_SYSCOMMAND:
      if (LOWORD (wParam) == SC_CLOSE)
          EndDialog (hwnd, FALSE);
    break;

  } /* end switch */
  return FALSE;
}


/***************************************************************************\
*    FUNCTION: ViewSourceProc
*
* Fill Text, Name, and Type information into the dialog.
*  Set a proper font to display the text depending on what type it is.
*
\***************************************************************************/
LRESULT CALLBACK ViewSourceProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
RECT rect;

  switch (message) {

    /******************************************************************\
    *  WM_INITDIALOG
    \******************************************************************/
    case WM_INITDIALOG:

      /* Text is unicode... use *W() variants of functions. */
      if (gTypeSource == TYPEUNICODE) {
        WCHAR buffer[MAX_PATH];
        LOGFONTW logfont;
        HFONT hFont;

        SetWindowTextW (GetDlgItem (hwnd, DID_TEXT), (LPCWSTR)pSourceData);
        GetWindowTextW (hwndName0, buffer, MAX_PATH);
        SetWindowTextW (GetDlgItem (hwnd, DID_NAME), buffer);
        GetWindowTextW (hwndCodePage0, buffer, MAX_PATH);
        SetWindowTextW (GetDlgItem (hwnd, DID_TYPE), buffer);

        GetObjectW (GetStockObject (SYSTEM_FONT), sizeof(LOGFONTW), &logfont);
        logfont.lfCharSet = UNICODE_CHARSET;
        lstrcpyW (logfont.lfFaceName, L"Lucida Sans Unicode");
        hFont = CreateFontIndirectW (&logfont);
        SendMessageW (GetDlgItem (hwnd, DID_TEXT), WM_SETFONT, (WPARAM) hFont, 0);


      /* Text is codepage... use *A() variants of functions. */
      } else {
        char buffer[MAX_PATH];
        LOGFONTA logfont;
        HFONT hFont;

        SetWindowTextA (GetDlgItem (hwnd, DID_TEXT), pSourceData);
        GetWindowTextA (hwndName0, buffer, MAX_PATH);
        SetWindowTextA (GetDlgItem (hwnd, DID_NAME), buffer);
        GetWindowTextA (hwndCodePage0, buffer, MAX_PATH);
        SetWindowTextA (GetDlgItem (hwnd, DID_TYPE), buffer);

        GetObjectA (GetStockObject (SYSTEM_FONT), sizeof(LOGFONTA), &logfont);
        logfont.lfCharSet = giSourceCodePage;
        lstrcpyA (logfont.lfFaceName, "System");
        hFont = CreateFontIndirectA (&logfont);
        SendMessageA (GetDlgItem (hwnd, DID_TEXT), WM_SETFONT, (WPARAM) hFont, 0);

      }
      SetWindowText (hwnd, LoadResourceString(IDS_VIEW_SOURCE_TITLE));
      GetClientRect (hwnd, &rect);
      SendMessage (hwnd, WM_SIZE, 0,
                 MAKELPARAM ((rect.right - rect.left), (rect.bottom - rect.top)));
      return TRUE;
    break;

    case WM_SIZE: {
      HWND hwndText;

      hwndText = GetDlgItem (hwnd, DID_TEXT);
      MoveWindow (hwndText, DLGBORDER, 60, (int) LOWORD(lParam) - 2*DLGBORDER,
                                (int) HIWORD(lParam) - 60 - DLGBORDER , TRUE);
    }

    case WM_COMMAND:
      switch (LOWORD (wParam)) {
        case IDCANCEL:
        case IDOK:
          EndDialog (hwnd, TRUE);
      }
    break; /* end WM_COMMAND */

    case WM_SYSCOMMAND:
      if (LOWORD (wParam) == SC_CLOSE)
          EndDialog (hwnd, FALSE);
    break;

  } /* end switch */
  return FALSE;
}



/***************************************************************************\
*    FUNCTION: ViewDestinationProc
*
* Fill Text, Name, and Type information into the dialog.
*  Set a proper font to display the text depending on what type it is.
*
\***************************************************************************/
LRESULT CALLBACK ViewDestinationProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
RECT rect;

  switch (message) {

    /******************************************************************\
    *  WM_INITDIALOG
    \******************************************************************/
    case WM_INITDIALOG:

      /* Destination text is unicode... use *W() variants of functions. */
      if (gTypeSource != TYPEUNICODE) {
        WCHAR buffer[MAX_PATH];
        LOGFONTW logfont;
        HFONT hFont;

        SetWindowTextW (GetDlgItem (hwnd, DID_TEXT), (LPCWSTR)pDestinationData);
        GetWindowTextW (hwndName1, buffer, MAX_PATH);
        SetWindowTextW (GetDlgItem (hwnd, DID_NAME), buffer);
        GetWindowTextW (hwndCodePage1, buffer, MAX_PATH);
        SetWindowTextW (GetDlgItem (hwnd, DID_TYPE), buffer);

        GetObjectW (GetStockObject (SYSTEM_FONT), sizeof(LOGFONTW), &logfont);
        logfont.lfCharSet = UNICODE_CHARSET;
        lstrcpyW (logfont.lfFaceName, L"Lucida Sans Unicode");
        hFont = CreateFontIndirectW (&logfont);
        SendMessageW (GetDlgItem (hwnd, DID_TEXT), WM_SETFONT, (WPARAM) hFont, 0);


      /* Destination text is codepage... use *A() variants of functions. */
      } else {
        char buffer[MAX_PATH];
        LOGFONTA logfont;
        HFONT hFont;

        SetWindowTextA (GetDlgItem (hwnd, DID_TEXT), pDestinationData);
        GetWindowTextA (hwndName1, buffer, MAX_PATH);
        SetWindowTextA (GetDlgItem (hwnd, DID_NAME), buffer);
        GetWindowTextA (hwndCodePage1, buffer, MAX_PATH);
        SetWindowTextA (GetDlgItem (hwnd, DID_TYPE), buffer);

        GetObjectA (GetStockObject (SYSTEM_FONT), sizeof(LOGFONTA), &logfont);
        logfont.lfCharSet = giDestinationCodePage;
        lstrcpyA (logfont.lfFaceName, "System");
        hFont = CreateFontIndirectA (&logfont);
        SendMessageA (GetDlgItem (hwnd, DID_TEXT), WM_SETFONT, (WPARAM) hFont, 0);

      }
      SetWindowText (hwnd, LoadResourceString(IDS_VIEW_DEST_TITLE));
      GetClientRect (hwnd, &rect);
      SendMessage (hwnd, WM_SIZE, 0,
                 MAKELPARAM ((rect.right - rect.left),(rect.bottom - rect.top)));

      return TRUE;
    break;


    case WM_SIZE: {
      HWND hwndText;

      hwndText = GetDlgItem (hwnd, DID_TEXT);
      MoveWindow (hwndText, DLGBORDER, 60, (int) LOWORD(lParam) - 2*DLGBORDER,
                                (int) HIWORD(lParam) - 60 - DLGBORDER , TRUE);
    }

    case WM_COMMAND:
      switch (LOWORD (wParam)) {
        case IDCANCEL:
        case IDOK:
          EndDialog (hwnd, TRUE);
      }
    break; /* end WM_COMMAND */

    case WM_SYSCOMMAND:
      if (LOWORD (wParam) == SC_CLOSE)
          EndDialog (hwnd, FALSE);
    break;

  } /* end switch */
  return FALSE;
}
