/******************************************************************************
Module:  ProcessInfo.cpp
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include <windowsx.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <stdarg.h>
#include <stdio.h>
#include "Toolhelp.h"
#include "Resource.h"


///////////////////////////////////////////////////////////////////////////////


// Adds a string to an edit control
void AddText(HWND hwnd, PCTSTR pszFormat, ...) {

   va_list argList;
   va_start(argList, pszFormat);

   TCHAR sz[20 * 1024];
   Edit_GetText(hwnd, sz, chDIMOF(sz));
   _vstprintf(_tcschr(sz, 0), pszFormat, argList);
   Edit_SetText(hwnd, sz);
   va_end(argList);
}


///////////////////////////////////////////////////////////////////////////////


VOID Dlg_PopulateProcessList(HWND hwnd) {

   HWND hwndList = GetDlgItem(hwnd, IDC_PROCESSMODULELIST);
   SetWindowRedraw(hwndList, FALSE);
   ComboBox_ResetContent(hwndList);

   CToolhelp thProcesses(TH32CS_SNAPPROCESS);
   PROCESSENTRY32 pe = { sizeof(pe) };
   BOOL fOk = thProcesses.ProcessFirst(&pe);
   for (; fOk; fOk = thProcesses.ProcessNext(&pe)) {
      TCHAR sz[1024];

      // Place the process name (without its path) & ID in the list
      PCTSTR pszExeFile = _tcsrchr(pe.szExeFile, TEXT('\\'));
      if (pszExeFile == NULL) pszExeFile = pe.szExeFile;
      else pszExeFile++; // Skip over the slash
      wsprintf(sz, TEXT("%s     (0x%08X)"), pszExeFile, pe.th32ProcessID);
      int n = ComboBox_AddString(hwndList, sz);

      // Associate the process ID with the added item
      ComboBox_SetItemData(hwndList, n, pe.th32ProcessID);
   }
   ComboBox_SetCurSel(hwndList, 0);  // Select the first entry

   // Simulate the user selecting this first item so that the
   // results pane shows something interesting
   FORWARD_WM_COMMAND(hwnd, IDC_PROCESSMODULELIST, 
      hwndList, CBN_SELCHANGE, SendMessage);

   SetWindowRedraw(hwndList, TRUE);
   InvalidateRect(hwndList, NULL, FALSE);
}


///////////////////////////////////////////////////////////////////////////////


VOID Dlg_PopulateModuleList(HWND hwnd) {

   HWND hwndModuleHelp = GetDlgItem(hwnd, IDC_MODULEHELP);
   ListBox_ResetContent(hwndModuleHelp);

   CToolhelp thProcesses(TH32CS_SNAPPROCESS);
   PROCESSENTRY32 pe = { sizeof(pe) };
   BOOL fOk = thProcesses.ProcessFirst(&pe);
   for (; fOk; fOk = thProcesses.ProcessNext(&pe)) {

      CToolhelp thModules(TH32CS_SNAPMODULE, pe.th32ProcessID);
      MODULEENTRY32 me = { sizeof(me) };
      BOOL fOk = thModules.ModuleFirst(&me);
      for (; fOk; fOk = thModules.ModuleNext(&me)) {
        int n = ListBox_FindStringExact(hwndModuleHelp, -1, me.szExePath);
         if (n == LB_ERR) {
          // This module hasn't been added before
            ListBox_AddString(hwndModuleHelp, me.szExePath);
         }
      }
   }


   HWND hwndList = GetDlgItem(hwnd, IDC_PROCESSMODULELIST);
   SetWindowRedraw(hwndList, FALSE);
   ComboBox_ResetContent(hwndList);
   int nNumModules = ListBox_GetCount(hwndModuleHelp);
   for (int i = 0; i < nNumModules; i++) {
      TCHAR sz[1024];
      ListBox_GetText(hwndModuleHelp, i, sz);
      // Place module name (without its path) in the list
      int nIndex = ComboBox_AddString(hwndList, _tcsrchr(sz, TEXT('\\')) + 1);
      // Associate the index of the full path with the added item
      ComboBox_SetItemData(hwndList, nIndex, i);
   }

   ComboBox_SetCurSel(hwndList, 0);  // Select the first entry

   // Simulate the user selecting this first item so that the
   // results pane shows something interesting
   FORWARD_WM_COMMAND(hwnd, IDC_PROCESSMODULELIST, 
      hwndList, CBN_SELCHANGE, SendMessage);

   SetWindowRedraw(hwndList, TRUE);
   InvalidateRect(hwndList, NULL, FALSE);
}


///////////////////////////////////////////////////////////////////////////////


PVOID GetModulePreferredBaseAddr(DWORD dwProcessId, PVOID pvModuleRemote) {

   PVOID pvModulePreferredBaseAddr = NULL;
   IMAGE_DOS_HEADER idh;
   IMAGE_NT_HEADERS inth;

   // Read the remote module's DOS header
   Toolhelp32ReadProcessMemory(dwProcessId, 
      pvModuleRemote, &idh, sizeof(idh), NULL);

   // Verify the DOS image header
   if (idh.e_magic == IMAGE_DOS_SIGNATURE) {
      // Read the remote module's NT header
      Toolhelp32ReadProcessMemory(dwProcessId, 
         (PBYTE) pvModuleRemote + idh.e_lfanew, &inth, sizeof(inth), NULL);

      // Verify the NT image header
      if (inth.Signature == IMAGE_NT_SIGNATURE) {
         // This is valid NT header, get the image's preferred base address
         pvModulePreferredBaseAddr = (PVOID) inth.OptionalHeader.ImageBase;
      }
   }
   return(pvModulePreferredBaseAddr);
}


///////////////////////////////////////////////////////////////////////////////


VOID ShowProcessInfo(HWND hwnd, DWORD dwProcessID) {

   SetWindowText(hwnd, TEXT(""));   // Clear the output box

   CToolhelp th(TH32CS_SNAPALL, dwProcessID);

   // Show Process details
   PROCESSENTRY32 pe = { sizeof(pe) };
   BOOL fOk = th.ProcessFirst(&pe);
   for (; fOk; fOk = th.ProcessNext(&pe)) {
      if (pe.th32ProcessID == dwProcessID) {
         AddText(hwnd, TEXT("Filename: %s\r\n"), pe.szExeFile);
         AddText(hwnd, TEXT("   PID=%08X, ParentPID=%08X, ")
            TEXT("PriorityClass=%d, Threads=%d, Heaps=%d\r\n"),
            pe.th32ProcessID, pe.th32ParentProcessID, 
            pe.pcPriClassBase, pe.cntThreads,
            th.HowManyHeaps());
         break;   // No need to continue looping
      }
   }


   // Show Modules in the Process
   // Number of characters to display an address
   const int cchAddress = sizeof(PVOID) * 2;
   AddText(hwnd, TEXT("\r\nModules Information:\r\n")
      TEXT("  Usage  %-*s(%-*s)  %8s  Module\r\n"),
      cchAddress, TEXT("BaseAddr"),
      cchAddress, TEXT("ImagAddr"), TEXT("Size"));

   MODULEENTRY32 me = { sizeof(me) };
   fOk = th.ModuleFirst(&me);
   for (; fOk; fOk = th.ModuleNext(&me)) {
      if (me.ProccntUsage == 65535) {
         // Module was implicitly loaded and cannot be unloaded
         AddText(hwnd, TEXT("  Fixed"));
      } else {
         AddText(hwnd, TEXT("  %5d"), me.ProccntUsage);
      }
      PVOID pvPreferredBaseAddr = 
         GetModulePreferredBaseAddr(pe.th32ProcessID, me.modBaseAddr);
      if (me.modBaseAddr == pvPreferredBaseAddr) {
         AddText(hwnd, TEXT("  %p %*s   %8u  %s\r\n"), 
            me.modBaseAddr, cchAddress, TEXT(""), 
            me.modBaseSize, me.szExePath);
      } else {
         AddText(hwnd, TEXT("  %p(%p)  %8u  %s\r\n"), 
            me.modBaseAddr, pvPreferredBaseAddr, me.modBaseSize, me.szExePath);
      }
   }

   
   // Show threads in the process
   AddText(hwnd, TEXT("\r\nThread Information:\r\n")
      TEXT("      TID     Priority\r\n"));
   THREADENTRY32 te = { sizeof(te) };
   fOk = th.ThreadFirst(&te);
   for (; fOk; fOk = th.ThreadNext(&te)) {
      if (te.th32OwnerProcessID == dwProcessID) {
         int nPriority = te.tpBasePri + te.tpDeltaPri;
         if ((te.tpBasePri < 16) && (nPriority > 15)) nPriority = 15;
         if ((te.tpBasePri > 15) && (nPriority > 31)) nPriority = 31;
         if ((te.tpBasePri < 16) && (nPriority <  1)) nPriority =  1;
         if ((te.tpBasePri > 15) && (nPriority < 16)) nPriority = 16;
         AddText(hwnd, TEXT("   %08X       %2d\r\n"), 
            te.th32ThreadID, nPriority);
      }
   }
}


///////////////////////////////////////////////////////////////////////////////


VOID ShowModuleInfo(HWND hwnd, PCTSTR pszModulePath) {

   SetWindowText(hwnd, TEXT(""));   // Clear the output box

   CToolhelp thProcesses(TH32CS_SNAPPROCESS);
   PROCESSENTRY32 pe = { sizeof(pe) };
   BOOL fOk = thProcesses.ProcessFirst(&pe);
   AddText(hwnd, TEXT("Pathname: %s\r\n\r\n"), pszModulePath);
   AddText(hwnd, TEXT("Process Information:\r\n"));
   AddText(hwnd, TEXT("     PID    BaseAddr  Process\r\n"));
   for (; fOk; fOk = thProcesses.ProcessNext(&pe)) {
      CToolhelp thModules(TH32CS_SNAPMODULE, pe.th32ProcessID);
      MODULEENTRY32 me = { sizeof(me) };
      BOOL fOk = thModules.ModuleFirst(&me);
      for (; fOk; fOk = thModules.ModuleNext(&me)) {
         if (_tcscmp(me.szExePath, pszModulePath) == 0) {
            AddText(hwnd, TEXT("  %08X  %p  %s\r\n"), 
               pe.th32ProcessID, me.modBaseAddr, pe.szExeFile);
         }
      }
   }
}


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {
   
   chSETDLGICONS(hwnd, IDI_PROCESSINFO);

   // Hide the module-helper listbox.
   ShowWindow(GetDlgItem(hwnd, IDC_MODULEHELP), SW_HIDE);

   // Have the results window use a fixed-pitch font
   SetWindowFont(GetDlgItem(hwnd, IDC_RESULTS), 
      GetStockFont(ANSI_FIXED_FONT), FALSE);

   // By default, show the running processes
   Dlg_PopulateProcessList(hwnd);

   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnSize(HWND hwnd, UINT state, int cx, int cy) {

   RECT rc;
   int n = LOWORD(GetDialogBaseUnits());

   HWND hwndCtl = GetDlgItem(hwnd, IDC_PROCESSMODULELIST);
   GetClientRect(hwndCtl, &rc);
   SetWindowPos(hwndCtl, NULL, n, n, cx - n - n, rc.bottom, SWP_NOZORDER);

   hwndCtl = GetDlgItem(hwnd, IDC_RESULTS);
   SetWindowPos(hwndCtl, NULL, n, n + rc.bottom + n, 
      cx - n - n, cy - (n + rc.bottom + n) - n, SWP_NOZORDER);

   return(0);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {

   static BOOL s_fProcesses = TRUE;

   switch (id) {
      case IDCANCEL:
         EndDialog(hwnd, id);
         break;

      case ID_PROCESSES:
         s_fProcesses = TRUE;
         EnableMenuItem(GetMenu(hwnd), ID_VMMAP, MF_BYCOMMAND | MF_ENABLED);
         DrawMenuBar(hwnd);
         Dlg_PopulateProcessList(hwnd);
         break;

      case ID_MODULES:
         EnableMenuItem(GetMenu(hwnd), ID_VMMAP, MF_BYCOMMAND | MF_GRAYED);
         DrawMenuBar(hwnd);
         s_fProcesses = FALSE;
         Dlg_PopulateModuleList(hwnd);
         break;

      case IDC_PROCESSMODULELIST:
         if (codeNotify == CBN_SELCHANGE) {
            DWORD dw = ComboBox_GetCurSel(hwndCtl);
            if (s_fProcesses) {
               dw = (DWORD) ComboBox_GetItemData(hwndCtl, dw); // Process ID
               ShowProcessInfo(GetDlgItem(hwnd, IDC_RESULTS), dw);
            } else {
               // Index in helper listbox of full path
               dw = (DWORD) ComboBox_GetItemData(hwndCtl, dw); 
               TCHAR szModulePath[1024];
               ListBox_GetText(GetDlgItem(hwnd, IDC_MODULEHELP), 
               dw, szModulePath);
               ShowModuleInfo(GetDlgItem(hwnd, IDC_RESULTS), szModulePath);
            }
         }
         break;

      case ID_VMMAP:
         STARTUPINFO si = { sizeof(si) };
         PROCESS_INFORMATION pi;
         TCHAR szCmdLine[1024];
         HWND hwndCB = GetDlgItem(hwnd, IDC_PROCESSMODULELIST);
         DWORD dwProcessId = (DWORD)
            ComboBox_GetItemData(hwndCB, ComboBox_GetCurSel(hwndCB));
         wsprintf(szCmdLine, TEXT("\"14 VMMap\" %d"), dwProcessId);
         BOOL fOk = CreateProcess(NULL, szCmdLine, NULL, NULL, 
            FALSE, 0, NULL, NULL, &si, &pi);
         if (fOk) {
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
         } else {
            chMB("Failed to execute VMMAP.EXE.");
         }
         break;
   }
}


///////////////////////////////////////////////////////////////////////////////


INT_PTR WINAPI Dlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
   
   switch (uMsg) {
      chHANDLE_DLGMSG(hwnd, WM_INITDIALOG, Dlg_OnInitDialog);
      chHANDLE_DLGMSG(hwnd, WM_SIZE,       Dlg_OnSize);
      chHANDLE_DLGMSG(hwnd, WM_COMMAND,    Dlg_OnCommand);
   }
   return(FALSE);
}


///////////////////////////////////////////////////////////////////////////////


int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int) {

   CToolhelp::EnableDebugPrivilege(TRUE);
   DialogBox(hinstExe, MAKEINTRESOURCE(IDD_PROCESSINFO), NULL, Dlg_Proc);
   CToolhelp::EnableDebugPrivilege(FALSE);
   return(0);
}


//////////////////////////////// End of File //////////////////////////////////
