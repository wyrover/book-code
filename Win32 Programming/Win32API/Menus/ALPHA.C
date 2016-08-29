#include "stdSDK.h"
#include "resource.h"
#include "alpha.h"

static void
alpha_OnDestroy(HWND hwnd)
{
 HWND alpha = GetDlgItem(hwnd, IDC_ALPHA);
 if(alpha != NULL)
    { /* delete font */
     HFONT font = GetWindowFont(alpha);
     if(font != NULL)
        { /* delete it */
         SetWindowFont(alpha, NULL, FALSE);
         DeleteFont(font);
        } /* delete it */
    } /* delete font */
}

static void
alpha_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
 if(id == IDCANCEL)
     EndDialog(hwnd, TRUE);
}

static BOOL
alpha_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
 HWND alpha = GetDlgItem(hwnd, IDC_ALPHA);
 HFONT font;
 int height;
 RECT r;
 
 GetClientRect(alpha, &r);
 height = (3 * (r.bottom - r.top)) / 4;

 font = CreateFont(height, 
                         0, // width
                         0, // escapement
                         0, // rotation,
                         FW_NORMAL, // weight
                         0,     // italic
                         0,     // underline
                         0,     // strikeout
                         DEFAULT_CHARSET,
                         OUT_DEVICE_PRECIS,
                         0,     // clip precision
                         PROOF_QUALITY,
                         VARIABLE_PITCH | FF_DONTCARE,
                         _T("Symbol"));
 if (font != NULL)
    { /* have one */
     SetWindowFont(alpha, font, FALSE);
    } /* have one */

 return TRUE;
}

BOOL CALLBACK
alphaDlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
 switch(message)
    { /* message */
     case WM_DESTROY:
             return HANDLE_WM_DESTROY(hwnd, wParam, lParam, alpha_OnDestroy);
     case WM_INITDIALOG:
             return HANDLE_WM_INITDIALOG(hwnd, wParam, lParam, alpha_OnInitDialog);
     case WM_COMMAND:
             return HANDLE_WM_COMMAND(hwnd, wParam, lParam, alpha_OnCommand);
    } /* message */
 return FALSE;
}

void doAlpha(HWND hwnd)
    {
     DialogBox(GetWindowInstance(hwnd), MAKEINTRESOURCE(IDD_ALPHA), hwnd,
                        (DLGPROC)alphaDlgProc);
    }
