/******************************************************************************
Module:  LastMsgBoxInfo.cpp
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include <windowsx.h>
#include <tchar.h>
#include "Resource.h"
#include "..\22-LastMsgBoxInfoLib\LastMsgBoxInfoLib.h"


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {

   chSETDLGICONS(hwnd, IDI_LASTMSGBOXINFO);
   SetDlgItemText(hwnd, IDC_INFO, 
      TEXT("Waiting for a Message Box to be dismissed"));
   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnSize(HWND hwnd, UINT state, int cx, int cy) {

   SetWindowPos(GetDlgItem(hwnd, IDC_INFO), NULL, 
      0, 0, cx, cy, SWP_NOZORDER);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {

   switch (id) {
      case IDCANCEL:
         EndDialog(hwnd, id);
         break;
   }
}


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnCopyData(HWND hwnd, HWND hwndFrom, PCOPYDATASTRUCT pcds) {

   // Some hooked process sent us some message box info, display it
   SetDlgItemTextA(hwnd, IDC_INFO, (PCSTR) pcds->lpData);
   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


INT_PTR WINAPI Dlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

   switch (uMsg) {
      chHANDLE_DLGMSG(hwnd, WM_INITDIALOG, Dlg_OnInitDialog);
      chHANDLE_DLGMSG(hwnd, WM_SIZE,       Dlg_OnSize);
      chHANDLE_DLGMSG(hwnd, WM_COMMAND,    Dlg_OnCommand);
      chHANDLE_DLGMSG(hwnd, WM_COPYDATA,   Dlg_OnCopyData);
   }
   return(FALSE);
}


///////////////////////////////////////////////////////////////////////////////


int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int) {

   DWORD dwThreadId = 0;
#ifdef _DEBUG
   HWND hwnd = FindWindow(NULL, TEXT("Untitled - Paint"));
   dwThreadId = GetWindowThreadProcessId(hwnd, NULL);
#endif

   LastMsgBoxInfo_HookAllApps(TRUE, dwThreadId);
   DialogBox(hinstExe, MAKEINTRESOURCE(IDD_LASTMSGBOXINFO), NULL, Dlg_Proc);
   LastMsgBoxInfo_HookAllApps(FALSE, 0);
   return(0);
}


//////////////////////////////// End of File //////////////////////////////////
