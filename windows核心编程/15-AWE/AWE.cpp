/******************************************************************************
Module:  AWE.cpp
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include <Windowsx.h>
#include <tchar.h>
#include "AddrWindow.h"
#include "Resource.h"


///////////////////////////////////////////////////////////////////////////////


CAddrWindow g_aw[2];             // 2 memory address windows
CAddrWindowStorage g_aws[2];     // 2 storage blocks
const ULONG_PTR g_nChars = 1024; // 1024 character buffers


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {

   chSETDLGICONS(hwnd, IDI_AWE);

   // Create the 2 memory address windows
   chVERIFY(g_aw[0].Create(g_nChars * sizeof(TCHAR)));
   chVERIFY(g_aw[1].Create(g_nChars * sizeof(TCHAR)));

   // Create the 2 storage blocks
   if (!g_aws[0].Allocate(g_nChars * sizeof(TCHAR))) {
      chFAIL("Failed to allocate RAM.\nMost likely reason: "
         "you are not granted the Lock Pages in Memory user right.");
   }
   chVERIFY(g_aws[1].Allocate(g_nChars * sizeof(TCHAR)));

   // Put some default text in the 1st storage block
   g_aws[0].MapStorage(g_aw[0]);
   lstrcpy((PTSTR) (PVOID) g_aw[0], TEXT("Text in Storage 0"));

   // Put some default text in the 2nd storage block
   g_aws[1].MapStorage(g_aw[0]);
   lstrcpy((PTSTR) (PVOID) g_aw[0], TEXT("Text in Storage 1"));

   // Populate the dialog box controls
   for (int n = 0; n <= 1; n++) {
      // Set the combo box for each address window
      int id = ((n == 0) ? IDC_WINDOW0STORAGE : IDC_WINDOW1STORAGE);
      HWND hwndCB = GetDlgItem(hwnd, id);
      ComboBox_AddString(hwndCB, TEXT("No storage"));
      ComboBox_AddString(hwndCB, TEXT("Storage 0"));
      ComboBox_AddString(hwndCB, TEXT("Storage 1"));

      // Window 0 shows Storage 0, Window 1 shows Storage 1
      ComboBox_SetCurSel(hwndCB, n + 1);
      FORWARD_WM_COMMAND(hwnd, id, hwndCB, CBN_SELCHANGE, SendMessage);
      Edit_LimitText(GetDlgItem(hwnd, 
         (n == 0) ? IDC_WINDOW0TEXT : IDC_WINDOW1TEXT), g_nChars);
   }

   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {

   switch (id) {

   case IDCANCEL:
      EndDialog(hwnd, id);
      break;

   case IDC_WINDOW0STORAGE:
   case IDC_WINDOW1STORAGE:
      if (codeNotify == CBN_SELCHANGE) {

         // Show different storage in address window
         int nWindow  = ((id == IDC_WINDOW0STORAGE) ? 0 : 1);
         int nStorage = ComboBox_GetCurSel(hwndCtl) - 1;

         if (nStorage == -1) {   // Show no storage in this window
            chVERIFY(g_aw[nWindow].UnmapStorage());
         } else {
            if (!g_aws[nStorage].MapStorage(g_aw[nWindow])) {
               // Couldn't map storage in window
               chVERIFY(g_aw[nWindow].UnmapStorage());
               ComboBox_SetCurSel(hwndCtl, 0);  // Force "No storage"
               chMB("This storage can be mapped only once.");
            }
         }
         
         // Update the address window's text display
         HWND hwndText = GetDlgItem(hwnd, 
            ((nWindow == 0) ? IDC_WINDOW0TEXT : IDC_WINDOW1TEXT));
         MEMORY_BASIC_INFORMATION mbi;
         VirtualQuery(g_aw[nWindow], &mbi, sizeof(mbi));
         // Note: mbi.State == MEM_RESERVE if no storage is in address window
         EnableWindow(hwndText, (mbi.State == MEM_COMMIT));
         Edit_SetText(hwndText, IsWindowEnabled(hwndText) 
            ? (PCTSTR) (PVOID) g_aw[nWindow] : TEXT("(No storage)"));
      }
      break;

   case IDC_WINDOW0TEXT:
   case IDC_WINDOW1TEXT:
      if (codeNotify == EN_CHANGE) {
         // Update the storage in the address window
         int nWindow = ((id == IDC_WINDOW0TEXT) ? 0 : 1);
         Edit_GetText(hwndCtl, (PTSTR) (PVOID) g_aw[nWindow], g_nChars);
      }
      break;
   }
}


///////////////////////////////////////////////////////////////////////////////


INT_PTR WINAPI Dlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

   switch (uMsg) {
      chHANDLE_DLGMSG(hwnd, WM_INITDIALOG, Dlg_OnInitDialog);
      chHANDLE_DLGMSG(hwnd, WM_COMMAND,    Dlg_OnCommand);
   }

   return(FALSE);
}


///////////////////////////////////////////////////////////////////////////////



int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int) {

   chWindows2000Required();

   DialogBox(hinstExe, MAKEINTRESOURCE(IDD_AWE), NULL, Dlg_Proc);
   return(0);
}


//////////////////////////////// End of File //////////////////////////////////
