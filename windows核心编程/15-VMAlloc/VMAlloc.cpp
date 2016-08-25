/******************************************************************************
Module:  VMAlloc.cpp
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include <WindowsX.h>
#include <tchar.h>
#include "Resource.h"


///////////////////////////////////////////////////////////////////////////////


// The number of bytes in a page on this host machine.
UINT g_uPageSize = 0;


// A dummy data structure used for the array.
typedef struct {
   BOOL fInUse;
   BYTE bOtherData[2048 - sizeof(BOOL)];
} SOMEDATA, *PSOMEDATA;


// The number of structures in the array
#define MAX_SOMEDATA    (50)

// Pointer to an array of data structures
PSOMEDATA g_pSomeData = NULL;

// The rectangular area in the window occupied by the memory map
RECT g_rcMemMap;


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {
   
   chSETDLGICONS(hwnd, IDI_VMALLOC);

   // Initialize the dialog box by disabling all the nonsetup controls.
   EnableWindow(GetDlgItem(hwnd, IDC_INDEXTEXT),      FALSE);
   EnableWindow(GetDlgItem(hwnd, IDC_INDEX),          FALSE);
   EnableWindow(GetDlgItem(hwnd, IDC_USE),            FALSE);
   EnableWindow(GetDlgItem(hwnd, IDC_CLEAR),          FALSE);
   EnableWindow(GetDlgItem(hwnd, IDC_GARBAGECOLLECT), FALSE);

   // Get the coordinates of the memory map display.
   GetWindowRect(GetDlgItem(hwnd, IDC_MEMMAP), &g_rcMemMap);
   MapWindowPoints(NULL, hwnd, (LPPOINT) &g_rcMemMap, 2);

   // Destroy the window that identifies the location of the memory map
   DestroyWindow(GetDlgItem(hwnd, IDC_MEMMAP));
   
   // Put the page size in the dialog box just for the user's information.
   TCHAR szBuf[10];
   wsprintf(szBuf, TEXT("%d KB"), g_uPageSize / 1024);
   SetDlgItemText(hwnd, IDC_PAGESIZE, szBuf);

   // Initialize the edit control.
   SetDlgItemInt(hwnd, IDC_INDEX, 0, FALSE);
   
   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnDestroy(HWND hwnd) {

   if (g_pSomeData != NULL)
      VirtualFree(g_pSomeData, 0, MEM_RELEASE);
}


///////////////////////////////////////////////////////////////////////////////


VOID GarbageCollect(PVOID pvBase, DWORD dwNum, DWORD dwStructSize) {

   static DWORD s_uPageSize = 0;

   if (s_uPageSize == 0) {
      // Get the page size used on this CPU.
      SYSTEM_INFO si;
      GetSystemInfo(&si);
      s_uPageSize = si.dwPageSize;
   }

   UINT uMaxPages = dwNum * dwStructSize / g_uPageSize;
   for (UINT uPage = 0; uPage < uMaxPages; uPage++) {
      BOOL fAnyAllocsInThisPage = FALSE;
      UINT uIndex     = uPage  * g_uPageSize / dwStructSize;
      UINT uIndexLast = uIndex + g_uPageSize / dwStructSize;

      for (; uIndex < uIndexLast; uIndex++) {
         MEMORY_BASIC_INFORMATION mbi;
         VirtualQuery(&g_pSomeData[uIndex], &mbi, sizeof(mbi));
         fAnyAllocsInThisPage = ((mbi.State == MEM_COMMIT) && 
            * (PBOOL) ((PBYTE) pvBase + dwStructSize * uIndex));

         // Stop checking this page, we know we can't decommit it.
         if (fAnyAllocsInThisPage) break;
      }

      if (!fAnyAllocsInThisPage) {
         // No allocated structures in this page; decommit it.
         VirtualFree(&g_pSomeData[uIndexLast - 1], dwStructSize, MEM_DECOMMIT);
      }
   }
}


///////////////////////////////////////////////////////////////////////////////

         
void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {

   UINT uIndex = 0;

   switch (id) {
      case IDCANCEL:
         EndDialog(hwnd, id);
         break;

      case IDC_RESERVE:
         // Reserve enough address space to hold the array of structures.
         g_pSomeData = (PSOMEDATA) VirtualAlloc(NULL, 
            MAX_SOMEDATA * sizeof(SOMEDATA), MEM_RESERVE, PAGE_READWRITE);

         // Disable the Reserve button and enable all the other controls.
         EnableWindow(GetDlgItem(hwnd, IDC_RESERVE),        FALSE);
         EnableWindow(GetDlgItem(hwnd, IDC_INDEXTEXT),      TRUE);
         EnableWindow(GetDlgItem(hwnd, IDC_INDEX),          TRUE);
         EnableWindow(GetDlgItem(hwnd, IDC_USE),            TRUE);
         EnableWindow(GetDlgItem(hwnd, IDC_GARBAGECOLLECT), TRUE);

         // Force the index edit control to have the focus.
         SetFocus(GetDlgItem(hwnd, IDC_INDEX));
         
         // Force the memory map to update
         InvalidateRect(hwnd, &g_rcMemMap, FALSE);
         break;

      case IDC_INDEX:
         if (codeNotify != EN_CHANGE)
            break;

        uIndex = GetDlgItemInt(hwnd, id, NULL, FALSE);
         if ((g_pSomeData != NULL) && chINRANGE(0, uIndex, MAX_SOMEDATA - 1)) {
            MEMORY_BASIC_INFORMATION mbi;
            VirtualQuery(&g_pSomeData[uIndex], &mbi, sizeof(mbi));
            BOOL fOk = (mbi.State == MEM_COMMIT);
            if (fOk)
               fOk = g_pSomeData[uIndex].fInUse;

            EnableWindow(GetDlgItem(hwnd, IDC_USE),  !fOk);
            EnableWindow(GetDlgItem(hwnd, IDC_CLEAR), fOk);

         } else {
            EnableWindow(GetDlgItem(hwnd, IDC_USE),   FALSE);
            EnableWindow(GetDlgItem(hwnd, IDC_CLEAR), FALSE);
         }
         break;
         

      case IDC_USE:
         uIndex = GetDlgItemInt(hwnd, IDC_INDEX, NULL, FALSE);
         if (chINRANGE(0, uIndex, MAX_SOMEDATA - 1)) {

            // NOTE: New pages are always zeroed by the system
            VirtualAlloc(&g_pSomeData[uIndex], sizeof(SOMEDATA), 
               MEM_COMMIT, PAGE_READWRITE);

            g_pSomeData[uIndex].fInUse = TRUE;

            EnableWindow(GetDlgItem(hwnd, IDC_USE),   FALSE);
            EnableWindow(GetDlgItem(hwnd, IDC_CLEAR), TRUE);

            // Force the Clear button control to have the focus.
            SetFocus(GetDlgItem(hwnd, IDC_CLEAR));
         
            // Force the memory map to update
            InvalidateRect(hwnd, &g_rcMemMap, FALSE);
         }
         break;

      case IDC_CLEAR:
         uIndex = GetDlgItemInt(hwnd, IDC_INDEX, NULL, FALSE);
         if (chINRANGE(0, uIndex, MAX_SOMEDATA - 1)) {
            g_pSomeData[uIndex].fInUse = FALSE;
            EnableWindow(GetDlgItem(hwnd, IDC_USE),   TRUE);
            EnableWindow(GetDlgItem(hwnd, IDC_CLEAR), FALSE);

            // Force the Use button control to have the focus.
            SetFocus(GetDlgItem(hwnd, IDC_USE));
         }
         break;

      case IDC_GARBAGECOLLECT:
         GarbageCollect(g_pSomeData, MAX_SOMEDATA, sizeof(SOMEDATA));

         // Force the memory map to update
         InvalidateRect(hwnd, &g_rcMemMap, FALSE);
         break;
   }
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnPaint(HWND hwnd) {    // Update the memory map

   PAINTSTRUCT ps;
   BeginPaint(hwnd, &ps);

   UINT uMaxPages = MAX_SOMEDATA * sizeof(SOMEDATA) / g_uPageSize;
   UINT uMemMapWidth = g_rcMemMap.right - g_rcMemMap.left;
   
   if (g_pSomeData == NULL) {

      // The memory has yet to be reserved.
      Rectangle(ps.hdc, g_rcMemMap.left, g_rcMemMap.top,
         g_rcMemMap.right - uMemMapWidth % uMaxPages, g_rcMemMap.bottom);

   } else {

      // Walk the virtual address space, painting the memory map
      for (UINT uPage = 0; uPage < uMaxPages; uPage++) {

         UINT uIndex = uPage * g_uPageSize / sizeof(SOMEDATA);
         UINT uIndexLast = uIndex + g_uPageSize / sizeof(SOMEDATA);
         for (; uIndex < uIndexLast; uIndex++) {

            MEMORY_BASIC_INFORMATION mbi;
            VirtualQuery(&g_pSomeData[uIndex], &mbi, sizeof(mbi));

            int nBrush = 0;
            switch (mbi.State) {
               case MEM_FREE:    nBrush = WHITE_BRUSH; break;
               case MEM_RESERVE: nBrush = GRAY_BRUSH;  break;
               case MEM_COMMIT:  nBrush = BLACK_BRUSH; break;
            }

            SelectObject(ps.hdc, GetStockObject(nBrush));
            Rectangle(ps.hdc,
               g_rcMemMap.left + uMemMapWidth / uMaxPages * uPage,
               g_rcMemMap.top,
               g_rcMemMap.left + uMemMapWidth / uMaxPages * (uPage + 1),
               g_rcMemMap.bottom);
         }
      }
   }

   EndPaint(hwnd, &ps);
}


///////////////////////////////////////////////////////////////////////////////


INT_PTR WINAPI Dlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
   
   switch (uMsg) {
      chHANDLE_DLGMSG(hwnd, WM_INITDIALOG, Dlg_OnInitDialog);
      chHANDLE_DLGMSG(hwnd, WM_COMMAND,    Dlg_OnCommand);
      chHANDLE_DLGMSG(hwnd, WM_PAINT,      Dlg_OnPaint);
      chHANDLE_DLGMSG(hwnd, WM_DESTROY,    Dlg_OnDestroy);
   }
   return(FALSE);
}


///////////////////////////////////////////////////////////////////////////////


int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int) {

   // Get the page size used on this CPU.
   SYSTEM_INFO si;
   GetSystemInfo(&si);
   g_uPageSize = si.dwPageSize;

   DialogBox(hinstExe, MAKEINTRESOURCE(IDD_VMALLOC), NULL, Dlg_Proc);
   return(0);
}


//////////////////////////////// End of File //////////////////////////////////
