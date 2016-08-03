/******************************************************************************
Module:  MMFSparse.cpp
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#include "..\CommonFiles\CmnHdr.h"     /* See Appendix A. */
#include <tchar.h>
#include <WindowsX.h>
#include <WinIoCtl.h>
#include "SparseStream.h"
#include <StrSafe.h>
#include "Resource.h"


//////////////////////////////////////////////////////////////////////////////


// This class makes it easy to work with memory-mapped sparse files
class CMMFSparse : public CSparseStream {
private:
   HANDLE m_hFileMap;      // File-mapping object
   PVOID  m_pvFile;        // Address to start of mapped file

public:
   // Creates a Sparse MMF and maps it in the process' address space.
   CMMFSparse(HANDLE hStream = NULL, DWORD dwStreamSizeMaxLow = 0, 
      DWORD dwStreamSizeMaxHigh = 0);

   // Closes a Sparse MMF
   virtual ~CMMFSparse() { ForceClose(); }

   // Creates a sparse MMF and maps it in the process' address space.
   BOOL Initialize(HANDLE hStream, DWORD dwStreamSizeMaxLow, 
      DWORD dwStreamSizeMaxHigh = 0);

   // MMF to BYTE cast operator returns address of first byte 
   // in the memory-mapped sparse file. 
   operator PBYTE() const { return((PBYTE) m_pvFile); }

   // Allows you to explicitly close the MMF without having
   // to wait for the destructor to be called.
   VOID ForceClose();
};


//////////////////////////////////////////////////////////////////////////////


CMMFSparse::CMMFSparse(HANDLE hStream, DWORD dwStreamSizeMaxLow, 
   DWORD dwStreamSizeMaxHigh) {

   Initialize(hStream, dwStreamSizeMaxLow, dwStreamSizeMaxHigh);
}


//////////////////////////////////////////////////////////////////////////////


BOOL CMMFSparse::Initialize(HANDLE hStream, DWORD dwStreamSizeMaxLow, 
   DWORD dwStreamSizeMaxHigh) {

   if (m_hFileMap != NULL) 
      ForceClose();

   // Initialize to NULL in case something goes wrong
   m_hFileMap = m_pvFile = NULL;

   BOOL bOk = TRUE;  // Assume success

   if (hStream != NULL) {
      if ((dwStreamSizeMaxLow == 0) && (dwStreamSizeMaxHigh == 0)) {
         DebugBreak();  // Illegal stream size
      }

      CSparseStream::Initialize(hStream);
      bOk = MakeSparse();  // Make the stream sparse
      if (bOk) {
         // Create a file-mapping object
         m_hFileMap = ::CreateFileMapping(hStream, NULL, PAGE_READWRITE, 
            dwStreamSizeMaxHigh, dwStreamSizeMaxLow, NULL);

         if (m_hFileMap != NULL) {
            // Map the stream into the process' address space
            m_pvFile = ::MapViewOfFile(m_hFileMap, 
               FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, 0);
         } else {
            // Failed to map the file, cleanup
            CSparseStream::Initialize(NULL);
            ForceClose();
            bOk = FALSE;
         }
      }
   }
   return(bOk);
}


//////////////////////////////////////////////////////////////////////////////


VOID CMMFSparse::ForceClose() {

   // Cleanup everything that was done successfully
   if (m_pvFile != NULL) { 
      ::UnmapViewOfFile(m_pvFile); 
      m_pvFile = NULL; 
   }
   if (m_hFileMap != NULL) { 
      ::CloseHandle(m_hFileMap);   
      m_hFileMap = NULL; 
   }
}


//////////////////////////////////////////////////////////////////////////////


#define STREAMSIZE      (1 * 1024 * 1024)    // 1 MB (1024 KB)
HANDLE g_hStream = INVALID_HANDLE_VALUE;
CMMFSparse g_mmf;
TCHAR g_szPathname[MAX_PATH] = TEXT("\0");


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hWnd, HWND hWndFocus, LPARAM lParam) {

   chSETDLGICONS(hWnd, IDI_MMFSPARSE);

   // Initialize the dialog box controls.
   EnableWindow(GetDlgItem(hWnd, IDC_OFFSET), FALSE);
   Edit_LimitText(GetDlgItem(hWnd, IDC_OFFSET), 4);
   SetDlgItemInt(hWnd, IDC_OFFSET, 1000, FALSE);

   EnableWindow(GetDlgItem(hWnd, IDC_BYTE), FALSE);
   Edit_LimitText(GetDlgItem(hWnd, IDC_BYTE), 3);
   SetDlgItemInt(hWnd, IDC_BYTE, 5, FALSE);

   EnableWindow(GetDlgItem(hWnd, IDC_WRITEBYTE), FALSE);
   EnableWindow(GetDlgItem(hWnd, IDC_READBYTE),  FALSE);
   EnableWindow(GetDlgItem(hWnd, IDC_FREEALLOCATEDREGIONS), FALSE);

   // Store the file in a writable folder
   GetCurrentDirectory(_countof(g_szPathname), g_szPathname);
   _tcscat_s(g_szPathname, _countof(g_szPathname), TEXT("\\MMFSparse"));

   // Check to see if the volume supports sparse files
   TCHAR szVolume[16];
   PTSTR pEndOfVolume = _tcschr(g_szPathname, _T('\\'));
   if (pEndOfVolume == NULL) {
      chFAIL("Impossible to find the Volume for the default document folder.");
      DestroyWindow(hWnd);
      return(TRUE);
   }
   _tcsncpy_s(szVolume, _countof(szVolume), 
      g_szPathname, pEndOfVolume - g_szPathname + 1);
   if (!CSparseStream::DoesFileSystemSupportSparseStreams(szVolume)) {
      chFAIL("Volume of default document folder does not support sparse MMF.");
      DestroyWindow(hWnd);
      return(TRUE);
   }

   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_ShowAllocatedRanges(HWND hWnd) {

   // Fill in the Allocated Ranges edit control
   DWORD dwNumEntries;
   FILE_ALLOCATED_RANGE_BUFFER* pfarb = 
      g_mmf.QueryAllocatedRanges(&dwNumEntries);

   if (dwNumEntries == 0) {
      SetDlgItemText(hWnd, IDC_FILESTATUS, 
         TEXT("No allocated ranges in the file"));
   } else {
      TCHAR sz[4096] = { 0 };
      for (DWORD dwEntry = 0; dwEntry < dwNumEntries; dwEntry++) {
         StringCchPrintf(_tcschr(sz, _T('\0')), _countof(sz) - _tcslen(sz),
            TEXT("Offset: %7.7u, Length: %7.7u\r\n"), 
            pfarb[dwEntry].FileOffset.LowPart, pfarb[dwEntry].Length.LowPart);
      }
      SetDlgItemText(hWnd, IDC_FILESTATUS, sz);
   }
   g_mmf.FreeAllocatedRanges(pfarb);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify) {
   
   switch (id) {
      case IDCANCEL:
         if (g_hStream != INVALID_HANDLE_VALUE) 
            CloseHandle(g_hStream);
         EndDialog(hWnd, id);
         break;

      case IDC_CREATEMMF:
         {
         g_hStream = CreateFile(g_szPathname, GENERIC_READ | GENERIC_WRITE, 
            0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
         if (g_hStream == INVALID_HANDLE_VALUE) {
            chFAIL("Failed to create file.");
            return;
         }

         // Create a 1MB (1024 KB) MMF using the file
         if (!g_mmf.Initialize(g_hStream, STREAMSIZE)) {
            chFAIL("Failed to initialize Sparse MMF.");
            CloseHandle(g_hStream);
            g_hStream = NULL;
            return;
         }
         Dlg_ShowAllocatedRanges(hWnd);

         // Enable/disable the other controls.
         EnableWindow(GetDlgItem(hWnd, IDC_CREATEMMF), FALSE);
         EnableWindow(GetDlgItem(hWnd, IDC_OFFSET),    TRUE);
         EnableWindow(GetDlgItem(hWnd, IDC_BYTE),      TRUE);
         EnableWindow(GetDlgItem(hWnd, IDC_WRITEBYTE), TRUE);
         EnableWindow(GetDlgItem(hWnd, IDC_READBYTE),  TRUE);
         EnableWindow(GetDlgItem(hWnd, IDC_FREEALLOCATEDREGIONS), TRUE);

         // Force the Offset edit control to have the focus.
         SetFocus(GetDlgItem(hWnd, IDC_OFFSET));
         }
         break;

      case IDC_WRITEBYTE:
         {
         BOOL bTranslated;
         DWORD dwOffset = GetDlgItemInt(hWnd, IDC_OFFSET, &bTranslated, FALSE);
         if (bTranslated) {
            g_mmf[dwOffset * 1024] = (BYTE) 
               GetDlgItemInt(hWnd, IDC_BYTE, NULL, FALSE);
            Dlg_ShowAllocatedRanges(hWnd);
         }
         }
         break;

      case IDC_READBYTE:
         {
         BOOL bTranslated;
         DWORD dwOffset = GetDlgItemInt(hWnd, IDC_OFFSET, &bTranslated, FALSE);
         if (bTranslated) {
            SetDlgItemInt(hWnd, IDC_BYTE, g_mmf[dwOffset * 1024], FALSE);
            Dlg_ShowAllocatedRanges(hWnd);
         }
         }
         break;

      case IDC_FREEALLOCATEDREGIONS:
         // Normally the destructor causes the file-mapping to close.
         // But, in this case, we want to force it so that we can reset 
         // a portion of the file back to all zeros.
         g_mmf.ForceClose();

         // We call ForceClose above because attempting to zero a portion of 
         // the file while it is mapped, causes DeviceIoControl to fail with 
         // error ERROR_USER_MAPPED_FILE ("The requested operation cannot 
         // be performed on a file with a user-mapped section open.")
         g_mmf.DecommitPortionOfStream(0, STREAMSIZE);

         // We need to close the file handle and reopen it in order to 
         // flush the sparse state.
         CloseHandle(g_hStream);
         g_hStream = CreateFile(g_szPathname, GENERIC_READ | GENERIC_WRITE, 
            0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
         if (g_hStream == INVALID_HANDLE_VALUE) {
            chFAIL("Failed to create file.");
            return;
         }

         // Reset the MMF wrapper for the new file handle.
         g_mmf.Initialize(g_hStream, STREAMSIZE);

         // Update the UI.
         Dlg_ShowAllocatedRanges(hWnd);
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


int WINAPI _tWinMain(HINSTANCE hInstExe, HINSTANCE, PTSTR pszCmdLine, int) {

   DialogBox(hInstExe, MAKEINTRESOURCE(IDD_MMFSPARSE), NULL, Dlg_Proc);
   return(0);
}


//////////////////////////////// End of File //////////////////////////////////
