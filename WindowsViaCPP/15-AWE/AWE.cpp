/******************************************************************************
Module:  AWE.cpp
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#include "..\CommonFiles\CmnHdr.h"     /* See Appendix A. */
#include <Windowsx.h>
#include <tchar.h>
#include "AddrWindow.h"
#include "Resource.h"
#include <StrSafe.h>


///////////////////////////////////////////////////////////////////////////////


CAddrWindow g_aw[2];             // 2 memory address windows
CAddrWindowStorage g_aws[2];     // 2 storage blocks
const ULONG_PTR g_nChars = 1024; // 1024 character buffers
const DWORD g_cbBufferSize = g_nChars * sizeof(TCHAR);


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hWnd, HWND hWndFocus, LPARAM lParam) {

   chSETDLGICONS(hWnd, IDI_AWE);

   // Create the 2 memory address windows
   chVERIFY(g_aw[0].Create(g_cbBufferSize));
   chVERIFY(g_aw[1].Create(g_cbBufferSize));

   // Create the 2 storage blocks
   if (!g_aws[0].Allocate(g_cbBufferSize)) {
      chFAIL("Failed to allocate RAM.\nMost likely reason: "
         "you are not granted the Lock Pages in Memory user right.");
   }
   chVERIFY(g_aws[1].Allocate(g_nChars * sizeof(TCHAR)));

   // Put some default text in the 1st storage block
   g_aws[0].MapStorage(g_aw[0]);
   _tcscpy_s((PTSTR) (PVOID) g_aw[0], g_cbBufferSize, TEXT("Text in Storage 0"));

   // Put some default text in the 2nd storage block
   g_aws[1].MapStorage(g_aw[0]);
   _tcscpy_s((PTSTR) (PVOID) g_aw[0], g_cbBufferSize, TEXT("Text in Storage 1"));

   // Populate the dialog box controls
   for (int n = 0; n <= 1; n++) {
      // Set the combo box for each address window
      int id = ((n == 0) ? IDC_WINDOW0STORAGE : IDC_WINDOW1STORAGE);
      HWND hWndCB = GetDlgItem(hWnd, id);
      ComboBox_AddString(hWndCB, TEXT("No storage"));
      ComboBox_AddString(hWndCB, TEXT("Storage 0"));
      ComboBox_AddString(hWndCB, TEXT("Storage 1"));

      // Window 0 shows Storage 0, Window 1 shows Storage 1
      ComboBox_SetCurSel(hWndCB, n + 1);
      FORWARD_WM_COMMAND(hWnd, id, hWndCB, CBN_SELCHANGE, SendMessage);
      Edit_LimitText(GetDlgItem(hWnd, 
         (n == 0) ? IDC_WINDOW0TEXT : IDC_WINDOW1TEXT), g_nChars);
   }

   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify) {

   switch (id) {

   case IDCANCEL:
      EndDialog(hWnd, id);
      break;

   case IDC_WINDOW0STORAGE:
   case IDC_WINDOW1STORAGE:
      if (codeNotify == CBN_SELCHANGE) {

         // Show different storage in address window
         int nWindow  = ((id == IDC_WINDOW0STORAGE) ? 0 : 1);
         int nStorage = ComboBox_GetCurSel(hWndCtl) - 1;

         if (nStorage == -1) {   // Show no storage in this window
            chVERIFY(g_aw[nWindow].UnmapStorage());
         } else {
            if (!g_aws[nStorage].MapStorage(g_aw[nWindow])) {
               // Couldn't map storage in window
               chVERIFY(g_aw[nWindow].UnmapStorage());
               ComboBox_SetCurSel(hWndCtl, 0);  // Force "No storage"
               chMB("This storage can be mapped only once.");
            }
         }
         
         // Update the address window's text display
         HWND hWndText = GetDlgItem(hWnd, 
            ((nWindow == 0) ? IDC_WINDOW0TEXT : IDC_WINDOW1TEXT));
         MEMORY_BASIC_INFORMATION mbi;
         VirtualQuery(g_aw[nWindow], &mbi, sizeof(mbi));
         // Note: mbi.State == MEM_RESERVE if no storage is in address window
         EnableWindow(hWndText, (mbi.State == MEM_COMMIT));
         Edit_SetText(hWndText, IsWindowEnabled(hWndText) 
            ? (PCTSTR) (PVOID) g_aw[nWindow] : TEXT("(No storage)"));
      }
      break;

   case IDC_WINDOW0TEXT:
   case IDC_WINDOW1TEXT:
      if (codeNotify == EN_CHANGE) {
         // Update the storage in the address window
         int nWindow = ((id == IDC_WINDOW0TEXT) ? 0 : 1);
         Edit_GetText(hWndCtl, (PTSTR) (PVOID) g_aw[nWindow], g_nChars);
      }
      break;
   }
}


///////////////////////////////////////////////////////////////////////////////


INT_PTR WINAPI Dlg_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

   switch (uMsg) {
      chHANDLE_DLGMSG(hWnd, WM_INITDIALOG, Dlg_OnInitDialog);
      chHANDLE_DLGMSG(hWnd, WM_COMMAND,    Dlg_OnCommand);
   }

   return(FALSE);
}


///////////////////////////////////////////////////////////////////////////////



int WINAPI _tWinMain(HINSTANCE hInstExe, HINSTANCE, PTSTR, int) {

   DialogBox(hInstExe, MAKEINTRESOURCE(IDD_AWE), NULL, Dlg_Proc);
   return(0);
}


//////////////////////////////// End of File //////////////////////////////////
