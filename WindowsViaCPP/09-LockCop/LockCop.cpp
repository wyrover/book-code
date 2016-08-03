/******************************************************************************
Module:  LockCop.cpp
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#include "..\CommonFiles\CmnHdr.h"     /* See Appendix A. */
#include "..\CommonFiles\ToolHelp.h"
#include "ChainParser.h"
#include "resource.h"

#include <windowsx.h>
#include <tchar.h>
#include <StrSafe.h>


///////////////////////////////////////////////////////////////////////////////


// Global Variables
HINSTANCE   g_hInstance;
HWND        g_hDlg;

#define DETAILS_CTRL GetDlgItem(g_hDlg, IDC_EDIT_DETAILS)

///////////////////////////////////////////////////////////////////////////////


// Adds a string to the "Details" edit control
void AddText(PCTSTR pszFormat, ...) {

   va_list argList;
   va_start(argList, pszFormat);

   TCHAR sz[20 * 1024];

   Edit_GetText(DETAILS_CTRL, sz, _countof(sz));
   _vstprintf_s(
      _tcschr(sz, TEXT('\0')), _countof(sz) - _tcslen(sz), 
      pszFormat, argList);
   Edit_SetText(DETAILS_CTRL, sz);
   va_end(argList);
}



///////////////////////////////////////////////////////////////////////////////


void OnRefreshProcesses()
{
   HWND hwndList = GetDlgItem(g_hDlg, IDC_COMBO_PROCESS);
   SetWindowRedraw(hwndList, FALSE);
   ComboBox_ResetContent(hwndList);

   CToolhelp thProcesses(TH32CS_SNAPPROCESS);
   PROCESSENTRY32 pe = { sizeof(pe) };
   BOOL fOk = thProcesses.ProcessFirst(&pe);
   for (; fOk; fOk = thProcesses.ProcessNext(&pe)) {
      TCHAR sz[1024];

      // Place the process name (without its path) & ID in the list
      PCTSTR pszExeFile = _tcsrchr(pe.szExeFile, TEXT('\\'));
      if (pszExeFile == NULL) {
         pszExeFile = pe.szExeFile;
      } else {
         pszExeFile++; // Skip over the slash
      }

      StringCchPrintf(sz, _countof(sz), TEXT("%04u - %s"), pe.th32ProcessID, pszExeFile);
      int n = ComboBox_AddString(hwndList, sz);

      // Associate the process ID with the added item
      ComboBox_SetItemData(hwndList, n, pe.th32ProcessID);
   }
   ComboBox_SetCurSel(hwndList, 0);  // Select the first entry

   // Simulate the user selecting this first item so that the
   // results pane shows something interesting
   FORWARD_WM_COMMAND(g_hDlg, IDC_COMBO_PROCESS, 
      hwndList, CBN_SELCHANGE, SendMessage);

   SetWindowRedraw(hwndList, TRUE);
   InvalidateRect(hwndList, NULL, FALSE);
}


///////////////////////////////////////////////////////////////////////////////


void OnUpdateLocks()
{
   SetWindowText(DETAILS_CTRL, TEXT(""));   // Clear the output box

   // Get current process from the combo box
   HWND hwndCtl = GetDlgItem(g_hDlg, IDC_COMBO_PROCESS);
   DWORD dwSelection = ComboBox_GetCurSel(hwndCtl);
   DWORD PID = (DWORD) ComboBox_GetItemData(hwndCtl, dwSelection);

   AddText(TEXT("Threads in process %u\r\n"), PID);
   
   CChainParser parser(DETAILS_CTRL);
   parser.ParseThreads(PID);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {

   switch (id) {
      case IDOK:
      case IDCANCEL:
         // User has clicked on the OK button
         // or dismissed the dialog with ESCAPE
         EndDialog(hwnd, id);
         break;

      case IDC_COMBO_PROCESS:
         if (codeNotify == CBN_SELCHANGE) {
            OnUpdateLocks();
         }
         break;

      case IDC_BTN_REFRESH:
         OnRefreshProcesses();
         break;

      case IDC_BTN_UPDATE:  
         OnUpdateLocks();
         break;
   }
}


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {

   chSETDLGICONS(hwnd, IDI_LOCKCOP);

   // Keep track of the main dialog window handle
   g_hDlg = hwnd;

   // Have the results window use a fixed-pitch font
   SetWindowFont(GetDlgItem(hwnd, IDC_EDIT_DETAILS), 
      GetStockFont(ANSI_FIXED_FONT), FALSE);

   // Fill up the process combo-box
   OnRefreshProcesses();
   
   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


INT_PTR WINAPI Dlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

   switch (uMsg) {
      chHANDLE_DLGMSG(hwnd, WM_COMMAND,    Dlg_OnCommand);
      chHANDLE_DLGMSG(hwnd, WM_INITDIALOG, Dlg_OnInitDialog);
   }

   return(FALSE);
}


///////////////////////////////////////////////////////////////////////////////


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

   // Keep track of the module handle
   g_hInstance = hInstance;

   // Enabling the debug privilege allows the application to see
   // information about service applications
   CToolhelp::EnablePrivilege(SE_DEBUG_NAME, TRUE);

   // Show main window 
   DialogBox(hInstance, MAKEINTRESOURCE(IDD_LOCKCOP), NULL, Dlg_Proc);

   // Restore privileges
   // Even though it is not really important since the process is exiting
   CToolhelp::EnablePrivilege(SE_DEBUG_NAME, FALSE);


   return(0);
}
