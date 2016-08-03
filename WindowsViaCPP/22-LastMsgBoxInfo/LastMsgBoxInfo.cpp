/******************************************************************************
Module:  LastMsgBoxInfo.cpp
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#include "..\CommonFiles\CmnHdr.h"     /* See Appendix A. */
#include <windowsx.h>
#include <tchar.h>
#include "Resource.h"
#include "..\22-LastMsgBoxInfoLib\LastMsgBoxInfoLib.h"


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hWnd, HWND hWndFocus, LPARAM lParam) {

   chSETDLGICONS(hWnd, IDI_LASTMSGBOXINFO);
   SetDlgItemText(hWnd, IDC_INFO, 
      TEXT("Waiting for a Message Box to be dismissed"));
   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnSize(HWND hWnd, UINT state, int cx, int cy) {

   SetWindowPos(GetDlgItem(hWnd, IDC_INFO), NULL, 
      0, 0, cx, cy, SWP_NOZORDER);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify) {

   switch (id) {
      case IDCANCEL:
         EndDialog(hWnd, id);
         break;
   }
}


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnCopyData(HWND hWnd, HWND hWndFrom, PCOPYDATASTRUCT pcds) {

   // Some hooked process sent us some message box info, display it
   SetDlgItemTextW(hWnd, IDC_INFO, (PCWSTR) pcds->lpData);
   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


INT_PTR WINAPI Dlg_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

   switch (uMsg) {
      chHANDLE_DLGMSG(hWnd, WM_INITDIALOG, Dlg_OnInitDialog);
      chHANDLE_DLGMSG(hWnd, WM_SIZE,       Dlg_OnSize);
      chHANDLE_DLGMSG(hWnd, WM_COMMAND,    Dlg_OnCommand);
      chHANDLE_DLGMSG(hWnd, WM_COPYDATA,   Dlg_OnCopyData);
   }
   return(FALSE);
}


///////////////////////////////////////////////////////////////////////////////


int WINAPI _tWinMain(HINSTANCE hInstExe, HINSTANCE, PTSTR pszCmdLine, int) {

   DWORD dwThreadId = 0;
   LastMsgBoxInfo_HookAllApps(TRUE, dwThreadId);
   DialogBox(hInstExe, MAKEINTRESOURCE(IDD_LASTMSGBOXINFO), NULL, Dlg_Proc);
   LastMsgBoxInfo_HookAllApps(FALSE, 0);
   return(0);
}


//////////////////////////////// End of File //////////////////////////////////
