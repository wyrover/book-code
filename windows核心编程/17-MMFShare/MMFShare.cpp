/******************************************************************************
Module:  MMFShare.cpp
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include <windowsx.h>
#include <tchar.h>
#include "Resource.h"


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {

   chSETDLGICONS(hwnd, IDI_MMFSHARE);

   // Initialize the edit control with some test data.
   Edit_SetText(GetDlgItem(hwnd, IDC_DATA), TEXT("Some test data"));

   // Disable the Close button because the file can't
   // be closed if it was never created or opened.
   Button_Enable(GetDlgItem(hwnd, IDC_CLOSEFILE), FALSE);
   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
   
   // Handle of the open memory-mapped file
   static HANDLE s_hFileMap = NULL;

   switch (id) {
      case IDCANCEL:
         EndDialog(hwnd, id);
         break;

      case IDC_CREATEFILE:
         if (codeNotify != BN_CLICKED)
            break;

         // Create a paging file-backed MMF to contain the edit control text.
         // The MMF is 4 KB at most and is named MMFSharedData.
         s_hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, 
            PAGE_READWRITE, 0, 4 * 1024, TEXT("MMFSharedData"));

         if (s_hFileMap != NULL) {

            if (GetLastError() == ERROR_ALREADY_EXISTS) {
               chMB("Mapping already exists - not created.");
               CloseHandle(s_hFileMap);

            } else {

               // File mapping created successfully.

               // Map a view of the file into the address space.
               PVOID pView = MapViewOfFile(s_hFileMap,
                  FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

               if (pView != NULL) {
                  // Put edit text into the MMF.
                  Edit_GetText(GetDlgItem(hwnd, IDC_DATA), 
                     (PTSTR) pView, 4 * 1024);

                  // Protect the MMF storage by unmapping it.
                  UnmapViewOfFile(pView);

                  // The user can't create another file right now.
                  Button_Enable(hwndCtl, FALSE);

                  // The user closed the file.
                  Button_Enable(GetDlgItem(hwnd, IDC_CLOSEFILE), TRUE);

               } else {
                  chMB("Can't map view of file.");
               }
            }

         } else {
            chMB("Can't create file mapping.");
         }
         break;

      case IDC_CLOSEFILE:
         if (codeNotify != BN_CLICKED)
            break;

         if (CloseHandle(s_hFileMap)) {
            // User closed the file, fix up the buttons.
            Button_Enable(GetDlgItem(hwnd, IDC_CREATEFILE), TRUE);
            Button_Enable(hwndCtl, FALSE);
         }
         break;

      case IDC_OPENFILE:
         if (codeNotify != BN_CLICKED)
            break;

         // See if a memory-mapped file named MMFSharedData already exists.
         HANDLE hFileMapT = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE,
            FALSE, TEXT("MMFSharedData"));

         if (hFileMapT != NULL) {
            // The MMF does exist, map it into the process's address space.
            PVOID pView = MapViewOfFile(hFileMapT, 
               FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

            if (pView != NULL) {

               // Put the contents of the MMF into the edit control.
               Edit_SetText(GetDlgItem(hwnd, IDC_DATA), (PTSTR) pView);
               UnmapViewOfFile(pView);
            } else {
               chMB("Can't map view.");
            }

            CloseHandle(hFileMapT);

         } else {
            chMB("Can't open mapping.");
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

   DialogBox(hinstExe, MAKEINTRESOURCE(IDD_MMFSHARE), NULL, Dlg_Proc);
   return(0);
}


//////////////////////////////// End of File //////////////////////////////////
