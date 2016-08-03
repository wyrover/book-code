/******************************************************************************
Module:  VMMap.cpp
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#include "..\CommonFiles\CmnHdr.h"     /* See Appendix A. */
#include "..\CommonFiles\Toolhelp.h"
#include <psapi.h>
#include <windowsx.h>
#include <tchar.h>
#include "Resource.h"
#include "VMQuery.h"
#include <strsafe.h>


// Look for the .lib corresponding to psapi.dll
#pragma comment(lib, "psapi")


///////////////////////////////////////////////////////////////////////////////


DWORD g_dwProcessId = 0;  // Which process to walk?
BOOL  g_bExpandRegions = FALSE;
CToolhelp g_toolhelp;



///////////////////////////////////////////////////////////////////////////////


// I use this function to obtain the dump figures in the book.
void CopyControlToClipboard(HWND hWnd) {
   TCHAR szClipData[128 * 1024] = { 0 };

   int nCount = ListBox_GetCount(hWnd);
   for (int nNum = 0; nNum < nCount; nNum++) {
      TCHAR szLine[1000];
      ListBox_GetText(hWnd, nNum, szLine);
      _tcscat_s(szClipData, _countof(szClipData), szLine);
      _tcscat_s(szClipData, _countof(szClipData), TEXT("\r\n"));
   }        

   OpenClipboard(NULL); 
   EmptyClipboard();

   // Clipboard accepts only data that is in a block allocated 
   // with GlobalAlloc using the GMEM_MOVEABLE and GMEM_DDESHARE flags.
   HGLOBAL hClipData = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, 
      sizeof(TCHAR) * (_tcslen(szClipData) + 1));
   PTSTR pClipData = (PTSTR) GlobalLock(hClipData);
   
   _tcscpy_s(pClipData, _tcslen(szClipData) + 1, szClipData);

#ifdef UNICODE
   BOOL bOk = (SetClipboardData(CF_UNICODETEXT, hClipData) == hClipData);
#else
   BOOL bOk = (SetClipboardData(CF_TEXT, hClipData) == hClipData);
#endif
   CloseClipboard();

   if (!bOk) {
      GlobalFree(hClipData);
      chMB("Error putting text on the clipboard");
   }
}


///////////////////////////////////////////////////////////////////////////////


PCTSTR GetMemStorageText(DWORD dwStorage) {

   PCTSTR p = TEXT("Unknown");
   switch (dwStorage) {
   case MEM_FREE:    p = TEXT("Free   "); break;
   case MEM_RESERVE: p = TEXT("Reserve"); break;
   case MEM_IMAGE:   p = TEXT("Image  "); break;
   case MEM_MAPPED:  p = TEXT("Mapped "); break;
   case MEM_PRIVATE: p = TEXT("Private"); break;
   }
   return(p);
}


///////////////////////////////////////////////////////////////////////////////


PTSTR GetProtectText(DWORD dwProtect, PTSTR szBuf, size_t chSize, 
   BOOL bShowFlags) {

   PCTSTR p = TEXT("Unknown");
   switch (dwProtect & ~(PAGE_GUARD | PAGE_NOCACHE | PAGE_WRITECOMBINE)) {
   case PAGE_READONLY:          p = TEXT("-R--"); break;
   case PAGE_READWRITE:         p = TEXT("-RW-"); break;
   case PAGE_WRITECOPY:         p = TEXT("-RWC"); break;
   case PAGE_EXECUTE:           p = TEXT("E---"); break;
   case PAGE_EXECUTE_READ:      p = TEXT("ER--"); break;
   case PAGE_EXECUTE_READWRITE: p = TEXT("ERW-"); break;
   case PAGE_EXECUTE_WRITECOPY: p = TEXT("ERWC"); break;
   case PAGE_NOACCESS:          p = TEXT("----"); break;
   }
   _tcscpy_s(szBuf, chSize, p);
   if (bShowFlags) {
      _tcscat_s(szBuf, chSize, TEXT(" "));
      _tcscat_s(szBuf, chSize, (dwProtect & PAGE_GUARD)        
         ? TEXT("G") : TEXT("-"));
      _tcscat_s(szBuf, chSize, (dwProtect & PAGE_NOCACHE)      
         ? TEXT("N") : TEXT("-"));
      _tcscat_s(szBuf, chSize, (dwProtect & PAGE_WRITECOMBINE) 
         ? TEXT("W") : TEXT("-"));
   }
   return(szBuf);
}


///////////////////////////////////////////////////////////////////////////////


void ConstructRgnInfoLine(HANDLE hProcess, PVMQUERY pVMQ, 
   PTSTR szLine, int cchMaxLen) {

   StringCchPrintf(szLine, cchMaxLen, TEXT("%p\t%s\t%12u\t"),
      pVMQ->pvRgnBaseAddress,
      GetMemStorageText(pVMQ->dwRgnStorage),
      pVMQ->RgnSize);

   if (pVMQ->dwRgnStorage != MEM_FREE) {
      StringCchPrintf(_tcschr(szLine, 0), cchMaxLen - _tcslen(szLine), 
         TEXT("%u\t"), pVMQ->dwRgnBlocks);
      GetProtectText(pVMQ->dwRgnProtection, _tcschr(szLine, 0), 
         cchMaxLen - _tcslen(szLine), FALSE);
   } else {
      _tcscat_s(szLine, cchMaxLen, TEXT("\t"));
   }

   _tcscat_s(szLine, cchMaxLen, TEXT("\t"));

   // Try to obtain the module pathname for this region.
   if ((pVMQ->dwRgnStorage != MEM_FREE) && (pVMQ->pvRgnBaseAddress != NULL)) {

      MODULEENTRY32 me = { sizeof(me) };
      if (g_toolhelp.ModuleFind(pVMQ->pvRgnBaseAddress, &me)) {
         _tcscat_s(szLine, cchMaxLen, me.szExePath);
      } else {
		 int cchLen = _tcslen(szLine);
         // This is not a module; see if it's a memory-mapped file
         DWORD dwLen = GetMappedFileName(hProcess, 
            pVMQ->pvRgnBaseAddress, szLine + cchLen, cchMaxLen - cchLen);
         if (dwLen == 0) {
            // NOTE: GetMappedFileName modifies the string when it fails
            szLine[cchLen] = 0;
         }
      }
   }

   if (pVMQ->bRgnIsAStack) {
      _tcscat_s(szLine, cchMaxLen, TEXT("Thread Stack"));
   }
}


///////////////////////////////////////////////////////////////////////////////


void ConstructBlkInfoLine(PVMQUERY pVMQ, PTSTR szLine, int cchMaxLen) {

   _stprintf_s(szLine, cchMaxLen, TEXT("   %p\t%s\t%12u\t"),
      pVMQ->pvBlkBaseAddress,
      GetMemStorageText(pVMQ->dwBlkStorage),
      pVMQ->BlkSize);

   if (pVMQ->dwBlkStorage != MEM_FREE) {
      // add an empty cell for number of regions
      _tcscat_s(szLine, cchMaxLen, TEXT("\t"));
      GetProtectText(pVMQ->dwBlkProtection, _tcschr(szLine, 0), 
         cchMaxLen - _tcslen(szLine), TRUE);
   } else {
      _tcscat_s(szLine, cchMaxLen, TEXT("\t"));
   }

   _tcscat_s(szLine, cchMaxLen, TEXT("\t"));
}


///////////////////////////////////////////////////////////////////////////////


void Refresh(HWND hWndLB, DWORD dwProcessId, BOOL bExpandRegions) {

   // Delete contents of list box & add a horizontal scroll bar
   ListBox_ResetContent(hWndLB);
   ListBox_SetHorizontalExtent(hWndLB, 300 * LOWORD(GetDialogBaseUnits()));

   // Is the process still running?
   HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, 
      FALSE, dwProcessId);

   if (hProcess == NULL) {
      ListBox_AddString(hWndLB, TEXT(""));   // Blank line, looks better
      ListBox_AddString(hWndLB, 
         TEXT("    The process ID identifies a process that is not running"));
      return;
   }

   // Grab a new snapshot of the process
   g_toolhelp.CreateSnapshot(TH32CS_SNAPALL, dwProcessId);
   
   // Walk the virtual address space, adding entries to the list box.
   BOOL bOk = TRUE;
   PVOID pvAddress = NULL;

   SetWindowRedraw(hWndLB, FALSE);
   while (bOk) {

      VMQUERY vmq;
      bOk = VMQuery(hProcess, pvAddress, &vmq);

      if (bOk) {
         // Construct the line to be displayed, and add it to the list box.
         TCHAR szLine[1024];
         ConstructRgnInfoLine(hProcess, &vmq, szLine, _countof(szLine));
         ListBox_AddString(hWndLB, szLine);

         if (bExpandRegions) {
            for (DWORD dwBlock = 0; bOk && (dwBlock < vmq.dwRgnBlocks);
               dwBlock++) {

               ConstructBlkInfoLine(&vmq, szLine, _countof(szLine));
               ListBox_AddString(hWndLB, szLine);

               // Get the address of the next region to test.
               pvAddress = ((PBYTE) pvAddress + vmq.BlkSize);
               if (dwBlock < vmq.dwRgnBlocks - 1) {
                  // Don't query the memory info after the last block.
                  bOk = VMQuery(hProcess, pvAddress, &vmq);
               }
            }
         }

         // Get the address of the next region to test.
         pvAddress = ((PBYTE) vmq.pvRgnBaseAddress + vmq.RgnSize);
      }
   }
   SetWindowRedraw(hWndLB, TRUE);
   CloseHandle(hProcess);
}


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hWnd, HWND hWndFocus, LPARAM lParam) {

   chSETDLGICONS(hWnd, IDI_VMMAP);

   // Show which process we're walking in the window's caption
   TCHAR szCaption[MAX_PATH * 2];
   GetWindowText(hWnd, szCaption, _countof(szCaption));
   g_toolhelp.CreateSnapshot(TH32CS_SNAPALL, g_dwProcessId);
   PROCESSENTRY32 pe = { sizeof(pe) };
   StringCchPrintf(&szCaption[_tcslen(szCaption)], 
      _countof(szCaption) - _tcslen(szCaption), TEXT(" (PID=%u \"%s\")"), 
      g_dwProcessId, g_toolhelp.ProcessFind(g_dwProcessId, &pe) 
         ? pe.szExeFile : TEXT("unknown"));
   SetWindowText(hWnd, szCaption);

   // VMMap has so much info to show, let's maximize it by default
   ShowWindow(hWnd, SW_MAXIMIZE);

   // Set the columns width
   int aTabs[5];
   aTabs[0] = 48;
   aTabs[1] = aTabs[0] + 40;
   aTabs[2] = aTabs[1] + 52;  // count
   aTabs[3] = aTabs[2] + 12;  // flags
   aTabs[4] = aTabs[3] + 20;  // description
   ListBox_SetTabStops(GetDlgItem(hWnd, IDC_LISTBOX), _countof(aTabs), aTabs);

   // Force the list box to refresh itself
   Refresh(GetDlgItem(hWnd, IDC_LISTBOX), g_dwProcessId, g_bExpandRegions);
   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnSize(HWND hWnd, UINT state, int cx, int cy) {

   // The list box always fills the whole client area
   SetWindowPos(GetDlgItem(hWnd, IDC_LISTBOX), NULL, 0, 0, cx, cy, 
      SWP_NOZORDER);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify) {

   switch (id) {
      case IDCANCEL:
         EndDialog(hWnd, id);
         break;

      case ID_REFRESH:
         Refresh(GetDlgItem(hWnd, IDC_LISTBOX), 
            g_dwProcessId, g_bExpandRegions);
         break;

      case ID_EXPANDREGIONS:
         g_bExpandRegions = g_bExpandRegions ? FALSE: TRUE;
         Refresh(GetDlgItem(hWnd, IDC_LISTBOX), 
            g_dwProcessId, g_bExpandRegions);
         break;

      case ID_COPYTOCLIPBOARD:
         CopyControlToClipboard(GetDlgItem(hWnd, IDC_LISTBOX));
         break;
   }
}


///////////////////////////////////////////////////////////////////////////////


INT_PTR WINAPI Dlg_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

   switch (uMsg) {
      chHANDLE_DLGMSG(hWnd, WM_INITDIALOG, Dlg_OnInitDialog);
      chHANDLE_DLGMSG(hWnd, WM_COMMAND,    Dlg_OnCommand);
      chHANDLE_DLGMSG(hWnd, WM_SIZE,       Dlg_OnSize);
   }
   return(FALSE);
}


///////////////////////////////////////////////////////////////////////////////


int WINAPI _tWinMain(HINSTANCE hInstExe, HINSTANCE, PTSTR pszCmdLine, int) {

   CToolhelp::EnablePrivilege(SE_DEBUG_NAME, TRUE);

   g_dwProcessId = _ttoi(pszCmdLine);
   if (g_dwProcessId == 0) {
      g_dwProcessId = GetCurrentProcessId();
      MessageBox(NULL, 
         TEXT("No Process Id passed on command line. Defaulting to this process"), 
         TEXT("VMMap"), MB_ICONINFORMATION);
   }

   DialogBox(hInstExe, MAKEINTRESOURCE(IDD_VMMAP), NULL, Dlg_Proc);

   return(0);
}



//////////////////////////////// End of File //////////////////////////////////


