/******************************************************************************
Module:  MMFSparse.cpp
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include <tchar.h>
#include <WindowsX.h>
#include <WinIoCtl.h>
#include "SparseStream.h"
#include "Resource.h"


//////////////////////////////////////////////////////////////////////////////


// This class makes it easy to work with memory-mapped sparse files
class CMMFSparse : public CSparseStream {
private:
   HANDLE m_hfilemap;      // File-mapping object
   PVOID  m_pvFile;        // Address to start of mapped file

public:
   // Creates a Sparse MMF and maps it in the process's address space.
   CMMFSparse(HANDLE hstream = NULL, SIZE_T dwStreamSizeMax = 0);

   // Closes a Sparse MMF
   virtual ~CMMFSparse() { ForceClose(); }

   // Creates a sparse MMF and maps it in the process's address space.
   BOOL Initialize(HANDLE hstream, SIZE_T dwStreamSizeMax);

   // MMF to BYTE cast operator returns address of first byte 
   // in the memory-mapped sparse file. 
   operator PBYTE() const { return((PBYTE) m_pvFile); }

   // Allows you to explicitly close the MMF without having
   // to wait for the destructor to be called.
   VOID ForceClose();
};


//////////////////////////////////////////////////////////////////////////////


CMMFSparse::CMMFSparse(HANDLE hstream, SIZE_T dwStreamSizeMax) {

   Initialize(hstream, dwStreamSizeMax);
}


//////////////////////////////////////////////////////////////////////////////


BOOL CMMFSparse::Initialize(HANDLE hstream, SIZE_T dwStreamSizeMax) {

   if (m_hfilemap != NULL) 
      ForceClose();

   // Initialize to NULL in case something goes wrong
   m_hfilemap = m_pvFile = NULL;

   BOOL fOk = TRUE;  // Assume success

   if (hstream != NULL) {
      if (dwStreamSizeMax == 0) {
         DebugBreak();  // Illegal stream size
      }

      CSparseStream::Initialize(hstream);
      fOk = MakeSparse();  // Make the stream sparse
      if (fOk) {
         // Create a file-mapping object
         m_hfilemap = ::CreateFileMapping(hstream, NULL, PAGE_READWRITE, 
            (DWORD) (dwStreamSizeMax >> 32I64), (DWORD) dwStreamSizeMax, NULL);

         if (m_hfilemap != NULL) {
            // Map the stream into the process's address space
            m_pvFile = ::MapViewOfFile(m_hfilemap, 
               FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, 0);
         } else {
            // Failed to map the file, cleanup
            CSparseStream::Initialize(NULL);
            ForceClose();
            fOk = FALSE;
         }
      }
   }
   return(fOk);
}


//////////////////////////////////////////////////////////////////////////////


VOID CMMFSparse::ForceClose() {

   // Cleanup everything that was done sucessfully
   if (m_pvFile != NULL) { 
      ::UnmapViewOfFile(m_pvFile); 
      m_pvFile = NULL; 
   }
   if (m_hfilemap != NULL) { 
      ::CloseHandle(m_hfilemap);   
      m_hfilemap = NULL; 
   }
}


//////////////////////////////////////////////////////////////////////////////


#define STREAMSIZE      (1 * 1024 * 1024)    // 1 MB (1024 KB)
TCHAR szPathname[] = TEXT("C:\\MMFSparse.");
HANDLE g_hstream = INVALID_HANDLE_VALUE;
CMMFSparse g_mmf;


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {

   chSETDLGICONS(hwnd, IDI_MMFSPARSE);

   // Initialize the dialog box controls.
   EnableWindow(GetDlgItem(hwnd, IDC_OFFSET), FALSE);
   Edit_LimitText(GetDlgItem(hwnd, IDC_OFFSET), 4);
   SetDlgItemInt(hwnd, IDC_OFFSET, 1000, FALSE);

   EnableWindow(GetDlgItem(hwnd, IDC_BYTE), FALSE);
   Edit_LimitText(GetDlgItem(hwnd, IDC_BYTE), 3);
   SetDlgItemInt(hwnd, IDC_BYTE, 5, FALSE);

   EnableWindow(GetDlgItem(hwnd, IDC_WRITEBYTE), FALSE);
   EnableWindow(GetDlgItem(hwnd, IDC_READBYTE),  FALSE);
   EnableWindow(GetDlgItem(hwnd, IDC_FREEALLOCATEDREGIONS), FALSE);

   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_ShowAllocatedRanges(HWND hwnd) {

   // Fill in the Allocated Ranges edit control
   DWORD dwNumEntries;
   FILE_ALLOCATED_RANGE_BUFFER* pfarb = 
      g_mmf.QueryAllocatedRanges(&dwNumEntries);

   if (dwNumEntries == 0) {
      SetDlgItemText(hwnd, IDC_FILESTATUS, 
         TEXT("No allocated ranges in the file"));
   } else {
      TCHAR sz[4096] = { 0 };
      for (DWORD dwEntry = 0; dwEntry < dwNumEntries; dwEntry++) {
         wsprintf(_tcschr(sz, 0), TEXT("Offset: %7.7u, Length: %7.7u\r\n"), 
            pfarb[dwEntry].FileOffset.LowPart, pfarb[dwEntry].Length.LowPart);
      }
      SetDlgItemText(hwnd, IDC_FILESTATUS, sz);
   }
   g_mmf.FreeAllocatedRanges(pfarb);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
   
   switch (id) {
      case IDCANCEL:
         if (g_hstream != INVALID_HANDLE_VALUE) 
            CloseHandle(g_hstream);
         EndDialog(hwnd, id);
         break;

      case IDC_CREATEMMF:
         // Create the file
         g_hstream = CreateFile(szPathname, GENERIC_READ | GENERIC_WRITE, 
            0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
         if (g_hstream == INVALID_HANDLE_VALUE) {
            chFAIL("Failed to create file.");
         }

         // Create a 1MB (1024 KB) MMF using the file
         if (!g_mmf.Initialize(g_hstream, STREAMSIZE)) {
            chFAIL("Failed to initialize Sparse MMF.");
         }
         Dlg_ShowAllocatedRanges(hwnd);

         // Enable/disable the other controls.
         EnableWindow(GetDlgItem(hwnd, IDC_CREATEMMF), FALSE);
         EnableWindow(GetDlgItem(hwnd, IDC_OFFSET),    TRUE);
         EnableWindow(GetDlgItem(hwnd, IDC_BYTE),      TRUE);
         EnableWindow(GetDlgItem(hwnd, IDC_WRITEBYTE), TRUE);
         EnableWindow(GetDlgItem(hwnd, IDC_READBYTE),  TRUE);
         EnableWindow(GetDlgItem(hwnd, IDC_FREEALLOCATEDREGIONS), TRUE);

         // Force the Offset edit control to have the focus.
         SetFocus(GetDlgItem(hwnd, IDC_OFFSET));
         break;

      case IDC_WRITEBYTE:
         {
         BOOL fTranslated;
         DWORD dwOffset = GetDlgItemInt(hwnd, IDC_OFFSET, &fTranslated, FALSE);
         if (fTranslated) {
            g_mmf[dwOffset * 1024] = (BYTE) 
               GetDlgItemInt(hwnd, IDC_BYTE, NULL, FALSE);
            Dlg_ShowAllocatedRanges(hwnd);
         }
         }
         break;

      case IDC_READBYTE:
         {
         BOOL fTranslated;
         DWORD dwOffset = GetDlgItemInt(hwnd, IDC_OFFSET, &fTranslated, FALSE);
         if (fTranslated) {
            SetDlgItemInt(hwnd, IDC_BYTE, g_mmf[dwOffset * 1024], FALSE);
            Dlg_ShowAllocatedRanges(hwnd);
         }
         }
         break;

      case IDC_FREEALLOCATEDREGIONS:
         // Normally the destructor causes the file-mapping to close.
         // But, in this case, we wish to force it so that we can reset 
         // a portion of the file back to all zeroes.
         g_mmf.ForceClose();

         // We call ForceClose above because attempting to zero a portion of 
         // the file while it is mapped, causes DeviceIoControl to fail with 
         // error ERROR_USER_MAPPED_FILE ("The requested operation cannot 
         // be performed on a file with a user-mapped section open.")
         g_mmf.DecommitPortionOfStream(0, STREAMSIZE);
         g_mmf.Initialize(g_hstream, STREAMSIZE);
         Dlg_ShowAllocatedRanges(hwnd);
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

   DialogBox(hinstExe, MAKEINTRESOURCE(IDD_MMFSPARSE), NULL, Dlg_Proc);
   return(0);
}


//////////////////////////////// End of File //////////////////////////////////
