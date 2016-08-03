/******************************************************************************
Module:  ProcessInfo.cpp
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#include "..\CommonFiles\CmnHdr.h"     /* See Appendix A. */
#include "..\CommonFiles\Toolhelp.h"
#include <windowsx.h>
#include <stdarg.h>
#include <stdio.h>
#include "Resource.h"

#include <winternl.h>   // for Windows internal declarations.
#include <aclapi.h>     // for ACL management.
#include <shlwapi.h>    // for StrFormatKBSize.
#include <shlobj.h>     // for IsUserAnAdmin.
#include <AclApi.h>     // for ACL/ACE functions.

#include <tchar.h>
#include <StrSafe.h> 


#pragma comment (lib,"shlwapi.lib")
#pragma comment (lib,"shell32.lib")



// static variables
TOKEN_ELEVATION_TYPE s_elevationType = TokenElevationTypeDefault;
BOOL                 s_bIsAdmin = FALSE;
const int			 s_cchAddress = sizeof(PVOID) * 2;


///////////////////////////////////////////////////////////////////////////////


// Add a string to an edit control
void AddText(HWND hwnd, PCTSTR pszFormat, ...) {

   va_list argList;
   va_start(argList, pszFormat);

   TCHAR sz[20 * 1024];
   Edit_GetText(hwnd, sz, _countof(sz));
   _vstprintf_s(_tcschr(sz, TEXT('\0')), _countof(sz) - _tcslen(sz), 
      pszFormat, argList);
   Edit_SetText(hwnd, sz);
   va_end(argList);
}


///////////////////////////////////////////////////////////////////////////////


BOOL GetProcessIntegrityLevel(HANDLE hProcess, PDWORD pIntegrityLevel, 
   PDWORD pPolicy, PDWORD pResourceIntegrityLevel, PDWORD pResourcePolicy) {
   
   HANDLE hToken = NULL;
   if (!OpenProcessToken(hProcess, TOKEN_READ, &hToken)) {
      return(FALSE);
   }

   BOOL bReturn = FALSE;
   
   // First, compute the size of the buffer to get the Integrity level
   DWORD dwNeededSize = 0;
   if (!GetTokenInformation(
      hToken, TokenIntegrityLevel, NULL, 0, &dwNeededSize)) {

      PTOKEN_MANDATORY_LABEL pTokenInfo = NULL;
      if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
         // Second, allocate a memory block with the the required size 
         pTokenInfo = (PTOKEN_MANDATORY_LABEL)LocalAlloc(0, dwNeededSize);
         if (pTokenInfo != NULL) {
            // And finally, ask for the integrity level
            if (GetTokenInformation(hToken, TokenIntegrityLevel, pTokenInfo, 
               dwNeededSize, &dwNeededSize)) {

               *pIntegrityLevel = 
                  *GetSidSubAuthority(
                     pTokenInfo->Label.Sid, 
                     (*GetSidSubAuthorityCount(pTokenInfo->Label.Sid)-1)
                     );
               bReturn = TRUE;
            }
          
            // Don't forget to free the memory
            LocalFree(pTokenInfo);
         }
      }
   }

   // Try to get the policy if the integrity level was available
   if (bReturn) {
      *pPolicy = TOKEN_MANDATORY_POLICY_OFF;
      dwNeededSize = sizeof(DWORD);
      GetTokenInformation(hToken, TokenMandatoryPolicy, pPolicy, 
         dwNeededSize, &dwNeededSize);
   }
   
   // Look for the resource policy
   *pResourceIntegrityLevel = 0; // 0 means none explicitely set
   *pResourcePolicy = 0;
   
   PACL pSACL = NULL;
   PSECURITY_DESCRIPTOR pSD = NULL;
   DWORD dwResult = ERROR_SUCCESS;
   
   // Look for the no-read-up/no-write-up policy in the SACL
   if (hToken != NULL) {
      dwResult = 
         GetSecurityInfo(
            hProcess, SE_KERNEL_OBJECT,
            LABEL_SECURITY_INFORMATION,
            NULL, NULL, NULL, 
            &pSACL, &pSD
          );
      if (dwResult == ERROR_SUCCESS) {
         if (pSACL != NULL) {
            SYSTEM_MANDATORY_LABEL_ACE* pACE = NULL;
            if ((pSACL->AceCount > 0) && (GetAce(pSACL, 0, (PVOID*)&pACE))) {
               if (pACE != NULL) {
                  SID* pSID = (SID*)(&pACE->SidStart);
                  *pResourceIntegrityLevel = pSID->SubAuthority[0];
                  *pResourcePolicy = pACE->Mask;
               }
            }
         }
      }
      
      // Cleanup memory allocated on our behalf
      if (pSD != NULL) LocalFree(pSD); 
   }


   // Don't forget to close the token handle.
   CloseHandle(hToken);

   return(bReturn);   
}

BOOL GetProcessIntegrityLevel(DWORD PID, PDWORD pIntegrityLevel, 
   PDWORD pPolicy, PDWORD pResourceIntegrityLevel, PDWORD pResourcePolicy) {
   
   // Sanity checks
   if ((PID <= 0) || (pIntegrityLevel == NULL))
      return(FALSE);

   // Check if we can get information for this process
   HANDLE hProcess = OpenProcess(
      READ_CONTROL | PROCESS_QUERY_INFORMATION, 
      FALSE, PID);

   if (hProcess == NULL)
      return(FALSE);

   BOOL bReturn = GetProcessIntegrityLevel(hProcess, pIntegrityLevel, 
      pPolicy, pResourceIntegrityLevel, pResourcePolicy);

   // Don't forget to release the process handle
   CloseHandle(hProcess);

   return(bReturn);
}



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
      if (pszExeFile == NULL) {
         pszExeFile = pe.szExeFile;
      } else {
         pszExeFile++; // Skip over the slash
      }

      // Append the code/resource integrity level and policy
      DWORD dwCodeIntegrityLevel = 0;
      DWORD dwCodePolicy = TOKEN_MANDATORY_POLICY_OFF;
      DWORD dwResourcePolicy = 0;
      DWORD dwResourceIntegrityLevel = 0;
      TCHAR szCodeDetails[256];
      szCodeDetails[0] = TEXT('\0');
      TCHAR szResourceDetails[256];
      szResourceDetails[0] = TEXT('\0');
      if (GetProcessIntegrityLevel(pe.th32ProcessID, &dwCodeIntegrityLevel, 
         &dwCodePolicy, &dwResourceIntegrityLevel, &dwResourcePolicy)) {
         switch (dwCodeIntegrityLevel) {
            case SECURITY_MANDATORY_LOW_RID:
               _tcscpy_s(szCodeDetails, _countof(szCodeDetails), 
                  TEXT("- Low "));
               break;

            case SECURITY_MANDATORY_MEDIUM_RID:
               _tcscpy_s(szCodeDetails, _countof(szCodeDetails), 
                  TEXT("- Medium "));
               break;

            case SECURITY_MANDATORY_HIGH_RID:
               _tcscpy_s(szCodeDetails, _countof(szCodeDetails), 
                  TEXT("- High "));
               break;

            case SECURITY_MANDATORY_SYSTEM_RID:
               _tcscpy_s(szCodeDetails, _countof(szCodeDetails), 
                  TEXT("- System "));
               break;

            default:
               _tcscpy_s(szCodeDetails, _countof(szCodeDetails), 
                  TEXT("- ??? "));
         }

         if (dwCodePolicy == TOKEN_MANDATORY_POLICY_OFF) { // = 0
            _tcscat_s(szCodeDetails, 
               _countof(szCodeDetails), TEXT(" + no policy"));
         } else {
            if ((dwCodePolicy & TOKEN_MANDATORY_POLICY_VALID_MASK) == 0) {
               _tcscat_s(szCodeDetails, _countof(szCodeDetails), 
                  TEXT(" + ???"));
            } else {
               if ((dwCodePolicy & TOKEN_MANDATORY_POLICY_NO_WRITE_UP)
                  == TOKEN_MANDATORY_POLICY_NO_WRITE_UP) { 
                  _tcscat_s(szCodeDetails, _countof(szCodeDetails), 
                     TEXT(" + no write-up"));
               }

               if ((dwCodePolicy & TOKEN_MANDATORY_POLICY_NEW_PROCESS_MIN)
                  == TOKEN_MANDATORY_POLICY_NEW_PROCESS_MIN) { 
                  _tcscat_s(szCodeDetails, _countof(szCodeDetails), 
                     TEXT(" + new process min"));
               }
            }
         }

         switch (dwResourceIntegrityLevel) {
            case SECURITY_MANDATORY_LOW_RID:
               _tcscpy_s(szResourceDetails, 
                  _countof(szResourceDetails), TEXT("Low"));
               break;

            case SECURITY_MANDATORY_MEDIUM_RID:
               _tcscpy_s(szResourceDetails, 
                  _countof(szResourceDetails), TEXT("Medium"));
               break;

            case SECURITY_MANDATORY_HIGH_RID:
               _tcscpy_s(szResourceDetails, 
                  _countof(szResourceDetails), TEXT("High"));
               break;

            case SECURITY_MANDATORY_SYSTEM_RID:
               _tcscpy_s(szResourceDetails, 
                  _countof(szResourceDetails), TEXT("System"));
               break;

            case 0:
               _tcscpy_s(szResourceDetails, 
                  _countof(szResourceDetails), TEXT("Not set"));
               break;

            default:
               _tcscpy_s(szResourceDetails, 
                  _countof(szResourceDetails), TEXT("???"));
          }


         if (dwResourcePolicy == 0) { // = 0
            _tcscat_s(szResourceDetails, 
               _countof(szResourceDetails), TEXT(" + 0 policy"));
         } else {
            if ((dwResourcePolicy & TOKEN_MANDATORY_POLICY_VALID_MASK) == 0) {
               _tcscat_s(szResourceDetails, 
                  _countof(szResourceDetails), TEXT(" + ???"));
            } else {
               if ((dwResourcePolicy & SYSTEM_MANDATORY_LABEL_NO_WRITE_UP)
                  == SYSTEM_MANDATORY_LABEL_NO_WRITE_UP) { 
                  _tcscat_s(szResourceDetails, 
                     _countof(szResourceDetails), 
                     TEXT(" + no write-up"));
               }

               if ((dwResourcePolicy & SYSTEM_MANDATORY_LABEL_NO_READ_UP)
                  == SYSTEM_MANDATORY_LABEL_NO_READ_UP) { 
                  _tcscat_s(szResourceDetails, 
                     _countof(szResourceDetails), 
                     TEXT(" + no read-up"));
               }
               if ((dwResourcePolicy & SYSTEM_MANDATORY_LABEL_NO_EXECUTE_UP)
                  == SYSTEM_MANDATORY_LABEL_NO_EXECUTE_UP) { 
                  _tcscat_s(szResourceDetails, 
                     _countof(szResourceDetails), 
                     TEXT(" + no execute-up"));
               }
            }
         }
      }

      StringCchPrintf(sz, _countof(sz), TEXT("%s     (0x%08X)  %s    [%s]"), 
         pszExeFile, pe.th32ProcessID, szCodeDetails, szResourceDetails);
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



/* 
   from  http://msdn.microsoft.com/msdnmag/issues/02/06/debug/
         Escape from DLL Hell with Custom Debugging and 
         Instrumentation Tools and Utilities

   The PEB (Process Environment Block) is an undocumented structure 
   which varies from version to version of Windows.
   However, WinDbg 
   (from http://www.microsoft.com/whdc/devtools/debugging/default.mspx) 
   provides a command that list the "documented" fields of a structure. 
      dt nt!_PEB 
      dt nt!_RTL_USER_PROCESS_PARAMETERS
*/

typedef struct
{
   DWORD Filler[4];
   DWORD InfoBlockAddress;
} __PEB;

typedef struct
{
   DWORD Filler[17];
   DWORD wszCmdLineAddress;
} __INFOBLOCK;



// NtQueryInformationProcess is declared in winternl.h
typedef NTSTATUS (CALLBACK *PFN_NTQUERYINFORMATIONPROCESS)(
   HANDLE ProcessHandle, 
   PROCESSINFOCLASS ProcessInformationClass,
   PVOID ProcessInformation,
   ULONG ProcessInformationLength,
   PULONG ReturnLength OPTIONAL
   );


NTSTATUS _NtQueryInformationProcess(
    HANDLE hProcess,
    PROCESSINFOCLASS pic,
    PVOID pPI,
    ULONG cbSize,
    PULONG pLength
    ) {
    
   HMODULE hNtDll = LoadLibrary(TEXT("ntdll.dll"));
   if (hNtDll == NULL) {
      return(-1);
   }

   NTSTATUS lStatus = -1;  // error by default.

   // Note that function name is not UNICODE
   PFN_NTQUERYINFORMATIONPROCESS pfnNtQIP = 
      (PFN_NTQUERYINFORMATIONPROCESS)GetProcAddress(
         hNtDll, "NtQueryInformationProcess");
   if (pfnNtQIP != NULL) {
      lStatus = pfnNtQIP(hProcess, pic, pPI, cbSize, pLength);
   }   
   
   FreeLibrary(hNtDll);
   return(lStatus);
}


BOOL GetProcessCmdLine(HANDLE hProcess, LPTSTR szCmdLine, DWORD Size) {

   // Sanity checks
   if ((hProcess == NULL) || (szCmdLine == NULL) || (Size == 0))
      return(FALSE);

   // 0. Get the Process Environment Block address
   int   iReturn = 1;
   DWORD dwSize;
   SIZE_T size;

   PROCESS_BASIC_INFORMATION  pbi;
   // The PEB was supposed to always be at address 0x7ffdf000 in XP...
   // ... but, here is the "right" way to get it now in Vista.
   iReturn =
      _NtQueryInformationProcess(
         hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), &dwSize);

   // NtQueryInformationProcess returns a negative value if it fails
   if (iReturn >= 0) {
      // 1. Find the Process Environment Block
      __PEB PEB;
	  size = dwSize;
      if (!ReadProcessMemory(hProcess, pbi.PebBaseAddress, &PEB, 
         sizeof(PEB), &size)) {
         // Call GetLastError() if you need to know why
         return(FALSE);
      }

      // 2. From this PEB, get the address of the block containing 
      // a pointer to the CmdLine
      __INFOBLOCK Block;
      if (!ReadProcessMemory(hProcess, (LPVOID)PEB.InfoBlockAddress, 
         &Block, sizeof(Block), &size)) {
         // Call GetLastError() if you need to know why
         return(FALSE);
      }

      // 3. Get the CmdLine
      wchar_t wszCmdLine[MAX_PATH+1];
      if (!ReadProcessMemory(hProcess, (LPVOID)Block.wszCmdLineAddress, 
         wszCmdLine, MAX_PATH*sizeof(wchar_t), &size)) {
         // Call GetLastError() if you need to know why
         return(FALSE);
      }

      // 4. Skip the application pathname
      //    it can be empty, "c:\...\app.exe" or c:\...\app.exe
      wchar_t* pPos = wszCmdLine;
      if (*pPos != L'\0') {
         if (*pPos == L'"') {
         // Find the next " character
            pPos = wcschr(&pPos[1], L'"');
         } else {
         // Find the next SPACE character
            pPos = wcschr(&pPos[1], L' ');
         }

         // Skip it
         if (pPos != NULL)
            pPos++;
      }

      // Copy it back
      if (pPos != NULL) {
         if (*pPos != L'\0') {
#ifdef UNICODE
            // Both strings are in UNICODE.
            _tcscpy_s(szCmdLine, Size, pPos);   
#else
            // from UNICODE to ANSI
            MultiByteToWideChar(CP_ACP, 0, szCmdLine, Size, 
               pPos, wcslen(pPos)); 
#endif
         }
         else
            szCmdLine[0] = TEXT('\0');
      }
      else
         szCmdLine[0] = TEXT('\0');
   }
   else {
      return(FALSE);
   }

   return(TRUE);
}


BOOL GetProcessCmdLine(DWORD PID, LPTSTR szCmdLine, DWORD Size) {

   // Sanity checks
   if ((PID <= 0) || (szCmdLine == NULL))
      return(FALSE);

   // Check if we can get information for this process
   HANDLE hProcess = 
      OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, PID);
   if (hProcess == NULL)
      return(FALSE);

   BOOL bReturn = GetProcessCmdLine(hProcess, szCmdLine, Size);

   // Don't forget to release the process handle
   CloseHandle(hProcess);

   return(bReturn);
}


BOOL GetProcessOwner(HANDLE hProcess, LPTSTR szOwner, size_t cchSize) {

   // Sanity checks
   if ((szOwner == NULL) || (cchSize == 0))
      return(FALSE);

   // Default value
   szOwner[0] = TEXT('\0');

   // Gget process token
   HANDLE hToken = NULL;
   CToolhelp::EnablePrivilege(SE_TCB_NAME, TRUE);
   if (!OpenProcessToken(hProcess, TOKEN_QUERY, &hToken)) {
      CToolhelp::EnablePrivilege(SE_TCB_NAME, FALSE);
      return(FALSE);
   }

   // Obtain the size of the user information in the token.
   DWORD cbti = 0;
   GetTokenInformation(hToken, TokenUser, NULL, 0, &cbti);

   // Call should have failed due to zero-length buffer.
   if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
      // Allocate buffer for user information in the token.
      PTOKEN_USER ptiUser = 
         (PTOKEN_USER)HeapAlloc(GetProcessHeap(), 0, cbti);
      if (ptiUser != NULL) {
         // Retrieve the user information from the token.
         if (GetTokenInformation(hToken, TokenUser, ptiUser, cbti, &cbti)) {
            SID_NAME_USE   snu;
            TCHAR          szUser[MAX_PATH];
            DWORD          chUser = MAX_PATH;
            PDWORD         pcchUser = &chUser;
            TCHAR          szDomain[MAX_PATH];
            DWORD          chDomain = MAX_PATH;
            PDWORD         pcchDomain = &chDomain;

            // Retrieve user name and domain name based on user's SID.
            if (
                  LookupAccountSid(
                     NULL,
                     ptiUser->User.Sid,
                     szUser,
                     pcchUser,
                     szDomain,
                     pcchDomain,
                     &snu
                     )
               ) {
               // build the owner string as \\DomainName\UserName
               _tcscpy_s(szOwner, cchSize, TEXT("\\\\"));
               _tcscat_s(szOwner, cchSize, szDomain);
               _tcscat_s(szOwner, cchSize, TEXT("\\"));
               _tcscat_s(szOwner, cchSize, szUser);
            }
         }

         // Don't forget to free memory buffer
         HeapFree(GetProcessHeap(), 0, ptiUser);
      }
   }

   // Don't forget to free process token
   CloseHandle(hToken);

   // Restore privileges
   CToolhelp::EnablePrivilege(SE_TCB_NAME, TRUE);

   return(TRUE);
}


BOOL GetProcessOwner(DWORD PID, LPTSTR szOwner, DWORD cchSize) {

   // Sanity checks
   if ((PID <= 0) || (szOwner == NULL))
      return(FALSE);

   // Check if we can get information for this process
   HANDLE hProcess = 
      OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, PID);
   if (hProcess == NULL)
      return(FALSE);

   BOOL bReturn = GetProcessOwner(hProcess, szOwner, cchSize);

   // Don't forget to release the process handle
   CloseHandle(hProcess);

   return(bReturn);
}


VOID FormatSizeInKB(DWORD dwSize, DWORD nCharacters, 
   LPTSTR szSize, size_t cchSize) {
   
   TCHAR szFormattedSize[64];
   if (StrFormatKBSize(dwSize, szFormattedSize, 
      _countof(szFormattedSize)) == NULL) {
      StringCchPrintf(szFormattedSize, _countof(szFormattedSize), TEXT("%8u"), dwSize); 
   }

   // Format to the right nCharacter width if needed.
   if (_tcslen(szFormattedSize) < nCharacters) {
      DWORD current = 0;
      for(current = 0; 
         current < (nCharacters - _tcslen(szFormattedSize)); 
         current++) {
         szSize[current] = TEXT(' ');
      }
      szSize[current] = TEXT('\0');      

      _tcscat_s(szSize, cchSize - current, szFormattedSize);
   }
}

VOID ShowProcessInfo(HWND hwnd, DWORD dwProcessID) {

   SetWindowText(hwnd, TEXT(""));   // Clear the output box

   CToolhelp th(TH32CS_SNAPALL, dwProcessID);

   // Show Process details
   PROCESSENTRY32 pe = { sizeof(pe) };
   BOOL fOk = th.ProcessFirst(&pe);
   for (; fOk; fOk = th.ProcessNext(&pe)) {
      if (pe.th32ProcessID == dwProcessID) {
         TCHAR szCmdLine[1024];
         if (GetProcessCmdLine(dwProcessID, szCmdLine, _countof(szCmdLine))) {
            AddText(hwnd, 
               TEXT("Command line: %s %s\r\n"), pe.szExeFile, szCmdLine);
         } else {
            AddText(hwnd, TEXT("Filename: %s\r\n"), pe.szExeFile);
         }
         AddText(hwnd, TEXT("   PID=%08X, ParentPID=%08X, ")
            TEXT("PriorityClass=%d, Threads=%d, Heaps=%d\r\n"),
            pe.th32ProcessID, pe.th32ParentProcessID, 
            pe.pcPriClassBase, pe.cntThreads,
            th.HowManyHeaps());
         TCHAR szOwner[MAX_PATH+1];
         if (GetProcessOwner(dwProcessID, szOwner, MAX_PATH)) {
            AddText(hwnd, TEXT("Owner: %s\r\n"), szOwner);
         }

         break;   // No need to continue looping
      }
   }


   // Show Modules in the Process
   // Number of characters to display an address
   AddText(hwnd, TEXT("\r\nModules Information:\r\n")
      TEXT("  Usage  %-*s(%-*s)  %10s  Module\r\n"),
      s_cchAddress, TEXT("BaseAddr"),
      s_cchAddress, TEXT("ImagAddr"), TEXT("Size"));

   MODULEENTRY32 me = { sizeof(me) };
   fOk = th.ModuleFirst(&me);
   for (; fOk; fOk = th.ModuleNext(&me)) {
      if (me.ProccntUsage == 65535) {
         // Module was implicitly loaded and cannot be unloaded
         AddText(hwnd, TEXT("  Fixed"));
      } else {
         AddText(hwnd, TEXT("  %5d"), me.ProccntUsage);
      }

      // Try to format the size in kb.
      TCHAR szFormattedSize[64];
      if (StrFormatKBSize(me.modBaseSize, szFormattedSize, 
         _countof(szFormattedSize)) == NULL)
      {
         StringCchPrintf(szFormattedSize, _countof(szFormattedSize), 
            TEXT("%10u"), me.modBaseSize); 
      }
      
      PVOID pvPreferredBaseAddr = 
         GetModulePreferredBaseAddr(pe.th32ProcessID, me.modBaseAddr);
      if (me.modBaseAddr == pvPreferredBaseAddr) {
         AddText(hwnd, TEXT("  %p %*s   %10s  %s\r\n"), 
            me.modBaseAddr, s_cchAddress, TEXT(""), 
            szFormattedSize, me.szExePath);
      } else {
         AddText(hwnd, TEXT("  %p(%p)  %10s  %s\r\n"), 
            me.modBaseAddr, pvPreferredBaseAddr, 
            szFormattedSize, me.szExePath);
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
   AddText(hwnd, TEXT("     PID    %-*s  Process\r\n"), 
	   s_cchAddress, TEXT("BaseAddr"));

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


BOOL GetProcessElevation(TOKEN_ELEVATION_TYPE* pElevationType, BOOL* pIsAdmin) {

   HANDLE hToken = NULL;
   DWORD dwSize; 

   // Get current process token
   if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
      return(FALSE);

   BOOL bResult = FALSE;

   // Retrieve elevation type information 
   if (GetTokenInformation(hToken, TokenElevationType, 
      pElevationType, sizeof(TOKEN_ELEVATION_TYPE), &dwSize)) {
      // Create the SID corresponding to the Administrators group
      byte adminSID[SECURITY_MAX_SID_SIZE];
      dwSize = sizeof(adminSID);
      CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, &adminSID, 
         &dwSize);

      if (*pElevationType == TokenElevationTypeLimited) {
         // Get handle to linked token (will have one if we are lua)
         HANDLE hUnfilteredToken = NULL;
         GetTokenInformation(hToken, TokenLinkedToken, (VOID*) 
            &hUnfilteredToken, sizeof(HANDLE), &dwSize);

         // Check if this original token contains admin SID
         if (CheckTokenMembership(hUnfilteredToken, &adminSID, pIsAdmin)) {
            bResult = TRUE;
         }

         // Don't forget to close the unfiltered token
         CloseHandle(hUnfilteredToken);
      } else {
         *pIsAdmin = IsUserAnAdmin();
         bResult = TRUE;
      }
   }

   // Don't forget to close the process token
   CloseHandle(hToken);

   return(bResult);
}



///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {
   
   chSETDLGICONS(hwnd, IDI_PROCESSINFO);

   // Is set to TRUE if the Administrator privileges 
   // are available; either because running elevated
   // or simply because UAC has been disabled.
   BOOL bCanReadSystemProcesses = FALSE;

   // Show if we are running with filtered token or not
   if (GetProcessElevation(&s_elevationType, &s_bIsAdmin)) {
      // prefix title with elevation
      TCHAR szTitle[64];

      switch(s_elevationType) {
         // Default user or UAC is disabled
         case TokenElevationTypeDefault:  
            if (IsUserAnAdmin()) {
               _tcscpy_s(szTitle, _countof(szTitle), 
                  TEXT("Default Administrator: ")); 
               bCanReadSystemProcesses = true;
            } else {
               _tcscpy_s(szTitle, _countof(szTitle), 
                  TEXT("Default: ")); 
            }
         break;
         
         // Process has been successfully elevated
         case TokenElevationTypeFull:
            if (IsUserAnAdmin()) {
               _tcscpy_s(szTitle, _countof(szTitle), 
                  TEXT("Elevated Administrator: ")); 
               bCanReadSystemProcesses = true;
            } else {
               _tcscpy_s(szTitle, _countof(szTitle), 
                  TEXT("Elevated: ")); 
            }
         break;

         // Process is running with limited privileges
         case TokenElevationTypeLimited:
            if (s_bIsAdmin) {
               _tcscpy_s(szTitle, _countof(szTitle), 
                  TEXT("Filtered Administrator: ")); 
            } else {
               _tcscpy_s(szTitle, _countof(szTitle), 
                  TEXT("Filtered: ")); 
            }
         break;
      }

      // Update the dialog title based on the elevation level
      GetWindowText(hwnd, _tcschr(szTitle, TEXT('\0')), 
         _countof(szTitle) - _tcslen(szTitle));
      SetWindowText(hwnd, szTitle);

      // Add the "shield" icon if needed to allow the user
      // to restart the application with elevated privileges
      if (!bCanReadSystemProcesses) {
         Button_SetElevationRequiredState(
            GetDlgItem(hwnd, IDC_BTN_SYSTEM_PROCESSES), 
               !bCanReadSystemProcesses);
      } else {
         // No need to show the button...
         ShowWindow(GetDlgItem(hwnd, IDC_BTN_SYSTEM_PROCESSES), SW_HIDE);
         
         // ... and the combo-box can take the whole width of the dialog box
         MoveWindow(GetDlgItem(hwnd, IDC_BTN_SYSTEM_PROCESSES), 
            0, 0, 0, 0, FALSE);
      }
   }
   
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

   RECT btnRect;
   HWND hwndCtl = GetDlgItem(hwnd, IDC_BTN_SYSTEM_PROCESSES);
   GetClientRect(hwndCtl, &btnRect);

   RECT rc;
   int n = LOWORD(GetDialogBaseUnits());

   hwndCtl = GetDlgItem(hwnd, IDC_PROCESSMODULELIST);
   GetClientRect(hwndCtl, &rc);
   SetWindowPos(hwndCtl, NULL, 
      n + btnRect.right, n, cx - n - n - btnRect.right, rc.bottom, 
      SWP_NOZORDER);

   hwndCtl = GetDlgItem(hwnd, IDC_RESULTS);
   SetWindowPos(hwndCtl, NULL, 
      n, n + rc.bottom + n, cx - n - n, cy - (n + rc.bottom + n) - n, 
      SWP_NOZORDER);

   return(0);
}


///////////////////////////////////////////////////////////////////////////////

DWORD StartElevatedProcess(LPCTSTR szExecutable, LPCTSTR szCmdLine) {

   // Initialize the structure.
   SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };

   // Ask for privileges elevation.
   sei.lpVerb = TEXT("runas");

   // Pass the application to start with high privileges.
   sei.lpFile = szExecutable;

   // Pass the command line.
   sei.lpParameters = szCmdLine;

   // Don't forget this parameter otherwise the window will be hidden.
   sei.nShow = SW_SHOWNORMAL;

   ShellExecuteEx(&sei);
   return(GetLastError());
}

///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {

   static BOOL s_fProcesses = TRUE;

   switch (id) {
      case IDCANCEL:
         EndDialog(hwnd, id);
         break;

      // Restart the application when we are not running 
      // as Elevated Administrator.
      case IDC_BTN_SYSTEM_PROCESSES: {
         // Hide ourself before trying to start the same application
         // but with elevated privileges.
         ShowWindow(hwnd, SW_HIDE);

         TCHAR szApplication[MAX_PATH];
         DWORD cchLength = _countof(szApplication);
         QueryFullProcessImageName(
            GetCurrentProcess(), 0, szApplication, &cchLength);
         DWORD dwStatus = StartElevatedProcess(szApplication, NULL);
         if (dwStatus == S_OK) {
            // not need to keep on working under lower privileges.
            ExitProcess(0);
         }
         
         // In case of error, show up again.
         ShowWindow(hwnd, SW_SHOWNORMAL);
      }
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

      case ID_VMMAP: {
         TCHAR szCmdLine[32];
         HWND hwndCB = GetDlgItem(hwnd, IDC_PROCESSMODULELIST);
         DWORD dwProcessId = (DWORD)
            ComboBox_GetItemData(hwndCB, ComboBox_GetCurSel(hwndCB));
         StringCchPrintf(szCmdLine, _countof(szCmdLine), TEXT("%d"), 
            dwProcessId);

         DWORD dwStatus = 
            StartElevatedProcess(TEXT("\"14-VMMap.exe\""), szCmdLine);
         if (dwStatus == ERROR_CANCELLED) {
            chMB("Failed to run 14-VMMap.exe: you refused access.");
         }
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


int WINAPI _tWinMain(HINSTANCE hInstanceExe, HINSTANCE, PTSTR pszCmdLine, int) {

   // Enabling the debug privilege allows the application to see
   // information about service applications
   CToolhelp::EnablePrivilege(SE_DEBUG_NAME, TRUE);
   
   // To get access to SACL.
   CToolhelp::EnablePrivilege(SE_SECURITY_NAME, TRUE);     

   // Show main window
   DialogBox(hInstanceExe, MAKEINTRESOURCE(IDD_PROCESSINFO), NULL, Dlg_Proc);

   CToolhelp::EnablePrivilege(SE_SECURITY_NAME, FALSE);     
   CToolhelp::EnablePrivilege(SE_DEBUG_NAME, FALSE);
   return(0);
}


//////////////////////////////// End of File //////////////////////////////////
