/******************************************************************************
Module:  SysInfo.cpp
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include <windowsx.h>
#include <tchar.h>
#include <stdio.h>
#include "Resource.h"


///////////////////////////////////////////////////////////////////////////////


// Set to TRUE if the app is running on Windows 9x.
BOOL g_fWin9xIsHost = FALSE;


///////////////////////////////////////////////////////////////////////////////


// This function accepts a number and converts it to a
// string, inserting commas where appropriate.
PTSTR BigNumToString(LONG lNum, PTSTR szBuf) {

   TCHAR szNum[100];
   wsprintf(szNum, TEXT("%d"), lNum);
   NUMBERFMT nf;
   nf.NumDigits = 0;
   nf.LeadingZero = FALSE;
   nf.Grouping = 3;
   nf.lpDecimalSep = TEXT(".");
   nf.lpThousandSep = TEXT(",");
   nf.NegativeOrder = 0;
   GetNumberFormat(LOCALE_USER_DEFAULT, 0, szNum, &nf, szBuf, 100);
   return(szBuf);
}


///////////////////////////////////////////////////////////////////////////////


void ShowCPUInfo(HWND hwnd, WORD wProcessorArchitecture, WORD wProcessorLevel, 
   WORD wProcessorRevision) {

   TCHAR szCPUArch[64]  = TEXT("(unknown)");
   TCHAR szCPULevel[64] = TEXT("(unknown)");
   TCHAR szCPURev[64]   = TEXT("(unknown)");

   switch (wProcessorArchitecture) {
      case PROCESSOR_ARCHITECTURE_INTEL: 
         lstrcpy(szCPUArch, TEXT("Intel")); 
         switch (wProcessorLevel) {
         case 3: case 4:
            wsprintf(szCPULevel, TEXT("80%c86"), wProcessorLevel + '0');
            if (!g_fWin9xIsHost) 
               wsprintf(szCPURev, TEXT("%c%d"), 
               HIBYTE(wProcessorRevision) + TEXT('A'), 
               LOBYTE(wProcessorRevision));
            break;

         case 5:
            wsprintf(szCPULevel, TEXT("Pentium"));
            if (!g_fWin9xIsHost) 
               wsprintf(szCPURev, TEXT("Model %d, Stepping %d"),
                  HIBYTE(wProcessorRevision), LOBYTE(wProcessorRevision));
            break;

         case 6:
            wsprintf(szCPULevel, TEXT("Pentium Pro or Pentium II"));
            if (!g_fWin9xIsHost) 
               wsprintf(szCPURev, TEXT("Model %d, Stepping %d"),
                  HIBYTE(wProcessorRevision), LOBYTE(wProcessorRevision));
            break;
         }
         break;

      case PROCESSOR_ARCHITECTURE_ALPHA: 
         lstrcpy(szCPUArch, TEXT("Alpha"));
         wsprintf(szCPULevel, TEXT("%d"), wProcessorLevel);
         wsprintf(szCPURev, TEXT("Model %c, Pass %d"), 
            HIBYTE(wProcessorRevision) + TEXT('A'), 
            LOBYTE(wProcessorRevision));
         break;

      case PROCESSOR_ARCHITECTURE_IA64:
         lstrcpy(szCPUArch, TEXT("IA-64"));
         wsprintf(szCPULevel, TEXT("%d"), wProcessorLevel);
         wsprintf(szCPURev, TEXT("Model %c, Pass %d"), 
            HIBYTE(wProcessorRevision) + TEXT('A'), 
            LOBYTE(wProcessorRevision));
         break;

      case PROCESSOR_ARCHITECTURE_ALPHA64:
         lstrcpy(szCPUArch, TEXT("Alpha64"));
         wsprintf(szCPULevel, TEXT("%d"), wProcessorLevel);
         wsprintf(szCPURev, TEXT("Model %c, Pass %d"), 
            HIBYTE(wProcessorRevision) + TEXT('A'), 
            LOBYTE(wProcessorRevision));
         break;

      case PROCESSOR_ARCHITECTURE_UNKNOWN:
      default:
         wsprintf(szCPUArch, TEXT("Unknown"));
         break;
   }
   SetDlgItemText(hwnd, IDC_PROCARCH,  szCPUArch);
   SetDlgItemText(hwnd, IDC_PROCLEVEL, szCPULevel);
   SetDlgItemText(hwnd, IDC_PROCREV,   szCPURev);
}


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {

   chSETDLGICONS(hwnd, IDI_SYSINFO);

   SYSTEM_INFO sinf;
   GetSystemInfo(&sinf);

   if (g_fWin9xIsHost) {
      sinf.wProcessorLevel = (WORD) (sinf.dwProcessorType / 100);
   }

   ShowCPUInfo(hwnd, sinf.wProcessorArchitecture,
      sinf.wProcessorLevel, sinf.wProcessorRevision);

   TCHAR szBuf[50];
   SetDlgItemText(hwnd, IDC_PAGESIZE,
      BigNumToString(sinf.dwPageSize, szBuf));

   _stprintf(szBuf, TEXT("%p"), sinf.lpMinimumApplicationAddress);
   SetDlgItemText(hwnd, IDC_MINAPPADDR, szBuf);

   _stprintf(szBuf, TEXT("%p"), sinf.lpMaximumApplicationAddress);
   SetDlgItemText(hwnd, IDC_MAXAPPADDR, szBuf);

   _stprintf(szBuf, TEXT("0x%016I64X"), (__int64) sinf.dwActiveProcessorMask);
   SetDlgItemText(hwnd, IDC_ACTIVEPROCMASK, szBuf);

   SetDlgItemText(hwnd, IDC_NUMOFPROCS,
      BigNumToString(sinf.dwNumberOfProcessors, szBuf));

   SetDlgItemText(hwnd, IDC_ALLOCGRAN,
      BigNumToString(sinf.dwAllocationGranularity, szBuf));

   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {

   switch (id) {
      case IDCANCEL:
         EndDialog(hwnd, id);
         break;
   }
}


///////////////////////////////////////////////////////////////////////////////


INT_PTR WINAPI Dlg_Proc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {

   switch (uMsg) {
      chHANDLE_DLGMSG(hDlg, WM_INITDIALOG, Dlg_OnInitDialog);
      chHANDLE_DLGMSG(hDlg, WM_COMMAND,    Dlg_OnCommand);
   }
   return(FALSE);
}


///////////////////////////////////////////////////////////////////////////////


int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int) {

   OSVERSIONINFO vi = { sizeof(vi) };
   GetVersionEx(&vi);
   g_fWin9xIsHost = (vi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS);

   DialogBox(hinstExe, MAKEINTRESOURCE(IDD_SYSINFO), NULL, Dlg_Proc);
   return(0);
}


//////////////////////////////// End of File //////////////////////////////////
