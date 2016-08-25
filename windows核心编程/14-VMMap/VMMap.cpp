/******************************************************************************
Module:  VMMap.cpp
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include <psapi.h>
#include <windowsx.h>
#include <tchar.h>
#include <stdio.h>         // For sprintf
#include "..\04-ProcessInfo\Toolhelp.h"
#include "Resource.h"
#include "VMQuery.h"


///////////////////////////////////////////////////////////////////////////////


DWORD g_dwProcessId = 0;  // Which process to walk?
BOOL  g_fExpandRegions = FALSE;
CToolhelp g_toolhelp;


// GetMappedFileName is only on Windows 2000 in PSAPI.DLL
// If this function exists on the host system, we'll use it
typedef DWORD (WINAPI* PFNGETMAPPEDFILENAME)(HANDLE, PVOID, PTSTR, DWORD);
static PFNGETMAPPEDFILENAME g_pfnGetMappedFileName = NULL;


///////////////////////////////////////////////////////////////////////////////


// I use this function to obtain the dump figures in the book.
void CopyControlToClipboard(HWND hwnd) {
   TCHAR szClipData[128 * 1024] = { 0 };

   int nCount = ListBox_GetCount(hwnd);
   for (int nNum = 0; nNum < nCount; nNum++) {
      TCHAR szLine[1000];
      ListBox_GetText(hwnd, nNum, szLine);
      _tcscat(szClipData, szLine);
      _tcscat(szClipData, TEXT("\r\n"));
   }        

   OpenClipboard(NULL); 
   EmptyClipboard();

   // Clipboard accepts only data that is in a block allocated 
   // with GlobalAlloc using the GMEM_MOVEABLE and GMEM_DDESHARE flags.
   HGLOBAL hClipData = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, 
      sizeof(TCHAR) * (_tcslen(szClipData) + 1));
   PTSTR pClipData = (PTSTR) GlobalLock(hClipData);
   
   _tcscpy(pClipData, szClipData);

#ifdef UNICODE
   BOOL fOk = (SetClipboardData(CF_UNICODETEXT, hClipData) == hClipData);
#else
   BOOL fOk = (SetClipboardData(CF_TEXT, hClipData) == hClipData);
#endif
   CloseClipboard();

   if (!fOk) {
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


PTSTR GetProtectText(DWORD dwProtect, PTSTR szBuf, BOOL fShowFlags) {

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
   _tcscpy(szBuf, p);
   if (fShowFlags) {
      _tcscat(szBuf, TEXT(" "));
      _tcscat(szBuf, (dwProtect & PAGE_GUARD)        ? TEXT("G") : TEXT("-"));
      _tcscat(szBuf, (dwProtect & PAGE_NOCACHE)      ? TEXT("N") : TEXT("-"));
      _tcscat(szBuf, (dwProtect & PAGE_WRITECOMBINE) ? TEXT("W") : TEXT("-"));
   }
   return(szBuf);
}


///////////////////////////////////////////////////////////////////////////////


void ConstructRgnInfoLine(HANDLE hProcess, PVMQUERY pVMQ, 
   PTSTR szLine, int nMaxLen) {

   _stprintf(szLine, TEXT("%p     %s  %16u  "),
      pVMQ->pvRgnBaseAddress,
      GetMemStorageText(pVMQ->dwRgnStorage),
      pVMQ->RgnSize);

   if (pVMQ->dwRgnStorage != MEM_FREE) {
      wsprintf(_tcschr(szLine, 0), TEXT("%5u  "), pVMQ->dwRgnBlocks);
      GetProtectText(pVMQ->dwRgnProtection, _tcschr(szLine, 0), FALSE);
   }

   _tcscat(szLine, TEXT("     "));

   // Try to obtain the module pathname for this region.
   int nLen = _tcslen(szLine);
   if (pVMQ->pvRgnBaseAddress != NULL) {
      MODULEENTRY32 me = { sizeof(me) };
      
      if (g_toolhelp.ModuleFind(pVMQ->pvRgnBaseAddress, &me)) {
         lstrcat(&szLine[nLen], me.szExePath);
      } else {
         // This is not a module; see if it's a memory-mapped file
         if (g_pfnGetMappedFileName != NULL) {
            DWORD d = g_pfnGetMappedFileName(hProcess,  
               pVMQ->pvRgnBaseAddress, szLine + nLen, nMaxLen - nLen);
            if (d == 0) {
               // NOTE: GetMappedFileName modifies the string when it fails
               szLine[nLen] = 0;
            }
         }
      }
   }

   if (pVMQ->fRgnIsAStack) {
      _tcscat(szLine, TEXT("Thread Stack"));
   }
}


///////////////////////////////////////////////////////////////////////////////


void ConstructBlkInfoLine(PVMQUERY pVMQ, PTSTR szLine, int nMaxLen) {

   _stprintf(szLine, TEXT("   %p  %s  %16u         "),
      pVMQ->pvBlkBaseAddress,
      GetMemStorageText(pVMQ->dwBlkStorage),
      pVMQ->BlkSize);

   if (pVMQ->dwBlkStorage != MEM_FREE) {
      GetProtectText(pVMQ->dwBlkProtection, _tcschr(szLine, 0), TRUE);
   }
}


///////////////////////////////////////////////////////////////////////////////


void Refresh(HWND hwndLB, DWORD dwProcessId, BOOL fExpandRegions) {

   // Delete contents of list box & add a horizontal scroll bar
   ListBox_ResetContent(hwndLB);
   ListBox_SetHorizontalExtent(hwndLB, 300 * LOWORD(GetDialogBaseUnits()));

   // Is the process still running?
   HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, 
      FALSE, dwProcessId);

   if (hProcess == NULL) {
      ListBox_AddString(hwndLB, TEXT(""));   // Blank line, looks better
      ListBox_AddString(hwndLB, 
         TEXT("    The process ID identifies a process that is not running"));
      return;
   }

   // Grab a new snapshot of the process
   g_toolhelp.CreateSnapshot(TH32CS_SNAPALL, dwProcessId);
   
   // Walk the virtual address space, adding entries to the list box.
   BOOL fOk = TRUE;
   PVOID pvAddress = NULL;

   SetWindowRedraw(hwndLB, FALSE);
   while (fOk) {

      VMQUERY vmq;
      fOk = VMQuery(hProcess, pvAddress, &vmq);

      if (fOk) {
         // Construct the line to be displayed, and add it to the list box.
         TCHAR szLine[1024];
         ConstructRgnInfoLine(hProcess, &vmq, szLine, sizeof(szLine));
         ListBox_AddString(hwndLB, szLine);

         if (fExpandRegions) {
            for (DWORD dwBlock = 0; fOk && (dwBlock < vmq.dwRgnBlocks);
               dwBlock++) {

               ConstructBlkInfoLine(&vmq, szLine, sizeof(szLine));
               ListBox_AddString(hwndLB, szLine);

               // Get the address of the next region to test.
               pvAddress = ((PBYTE) pvAddress + vmq.BlkSize);
               if (dwBlock < vmq.dwRgnBlocks - 1) {
                  // Don't query the memory info after the last block.
                  fOk = VMQuery(hProcess, pvAddress, &vmq);
               }
            }
         }

         // Get the address of the next region to test.
         pvAddress = ((PBYTE) vmq.pvRgnBaseAddress + vmq.RgnSize);
      }
   }
   SetWindowRedraw(hwndLB, TRUE);
   CloseHandle(hProcess);
}


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {

   chSETDLGICONS(hwnd, IDI_VMMAP);

   // Show which process we're walking in the window's caption
   TCHAR szCaption[MAX_PATH * 2];
   GetWindowText(hwnd, szCaption, chDIMOF(szCaption));
   g_toolhelp.CreateSnapshot(TH32CS_SNAPALL, g_dwProcessId);
   PROCESSENTRY32 pe = { sizeof(pe) };
   wsprintf(&szCaption[lstrlen(szCaption)], TEXT(" (PID=%u \"%s\")"), 
      g_dwProcessId, g_toolhelp.ProcessFind(g_dwProcessId, &pe) 
         ? pe.szExeFile : TEXT("unknown"));
   SetWindowText(hwnd, szCaption);

   // VMMap has so much info to show, let's maximize it by default
   ShowWindow(hwnd, SW_MAXIMIZE);

   // Force the list box to refresh itself
   Refresh(GetDlgItem(hwnd, IDC_LISTBOX), g_dwProcessId, g_fExpandRegions);
   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnSize(HWND hwnd, UINT state, int cx, int cy) {

   // The list box always fills the whole client area
   SetWindowPos(GetDlgItem(hwnd, IDC_LISTBOX), NULL, 0, 0, cx, cy, 
      SWP_NOZORDER);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {

   switch (id) {
      case IDCANCEL:
         EndDialog(hwnd, id);
         break;

      case ID_REFRESH:
         Refresh(GetDlgItem(hwnd, IDC_LISTBOX), 
            g_dwProcessId, g_fExpandRegions);
         break;

      case ID_EXPANDREGIONS:
         g_fExpandRegions = g_fExpandRegions ? FALSE: TRUE;
         Refresh(GetDlgItem(hwnd, IDC_LISTBOX), 
            g_dwProcessId, g_fExpandRegions);
         break;

      case ID_COPYTOCLIPBOARD:
         CopyControlToClipboard(GetDlgItem(hwnd, IDC_LISTBOX));
         break;
   }
}


///////////////////////////////////////////////////////////////////////////////


INT_PTR WINAPI Dlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

   switch (uMsg) {
      chHANDLE_DLGMSG(hwnd, WM_INITDIALOG, Dlg_OnInitDialog);
      chHANDLE_DLGMSG(hwnd, WM_COMMAND,    Dlg_OnCommand);
      chHANDLE_DLGMSG(hwnd, WM_SIZE,       Dlg_OnSize);
   }
   return(FALSE);
}


///////////////////////////////////////////////////////////////////////////////


int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int) {

   CToolhelp::EnableDebugPrivilege();

   // Try to load PSAPI.DLL and get the address of GetMappedFileName
   HMODULE hmodPSAPI = LoadLibrary(TEXT("PSAPI"));
   if (hmodPSAPI != NULL) {
#ifdef UNICODE
      g_pfnGetMappedFileName = (PFNGETMAPPEDFILENAME) 
         GetProcAddress(hmodPSAPI, "GetMappedFileNameW");
#else
      g_pfnGetMappedFileName = (PFNGETMAPPEDFILENAME) 
         GetProcAddress(hmodPSAPI, "GetMappedFileNameA");
#endif      
   }

   g_dwProcessId = _ttoi(pszCmdLine);
   if (g_dwProcessId == 0) {
      g_dwProcessId = GetCurrentProcessId();
   }

   DialogBox(hinstExe, MAKEINTRESOURCE(IDD_VMMAP), NULL, Dlg_Proc);

   if (hmodPSAPI != NULL) 
      FreeLibrary(hmodPSAPI); // Free PSAPI.DLL if we loaded it

   return(0);
}


//////////////////////////////// End of File //////////////////////////////////
