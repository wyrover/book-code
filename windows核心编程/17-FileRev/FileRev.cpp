/******************************************************************************
Module:  FileRev.cpp
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include <windowsx.h>
#include <tchar.h>
#include <commdlg.h>
#include <string.h>        // For _strrev
#include "Resource.h"


///////////////////////////////////////////////////////////////////////////////


#define FILENAME  TEXT("FILEREV.DAT")


///////////////////////////////////////////////////////////////////////////////


BOOL FileReverse(PCTSTR pszPathname, PBOOL pfIsTextUnicode) {

   *pfIsTextUnicode = FALSE;  // Assume text is Unicode

   // Open the file for reading and writing.
   HANDLE hFile = CreateFile(pszPathname, GENERIC_WRITE | GENERIC_READ, 0, 
      NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

   if (hFile == INVALID_HANDLE_VALUE) {
      chMB("File could not be opened.");
      return(FALSE);
   }

   // Get the size of the file (I assume the whole file can be mapped).
   DWORD dwFileSize = GetFileSize(hFile, NULL);

   // Create the file-mapping object. The file-mapping object is 1 character 
   // bigger than the file size so that a zero character can be placed at the 
   // end of the file to terminate the string (file). Because I don't yet know
   // if the file contains ANSI or Unicode characters, I assume worst case
   // and add the size of a WCHAR instead of CHAR.
   HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 
      0, dwFileSize + sizeof(WCHAR), NULL);

   if (hFileMap == NULL) {
      chMB("File map could not be opened.");
      CloseHandle(hFile);
      return(FALSE);
   }

   // Get the address where the first byte of the file is mapped into memory.
   PVOID pvFile = MapViewOfFile(hFileMap, FILE_MAP_WRITE, 0, 0, 0);

   if (pvFile == NULL) {
      chMB("Could not map view of file.");
      CloseHandle(hFileMap);
      CloseHandle(hFile);
      return(FALSE);
   }

   // Does the buffer contain ANSI or Unicode?
   int iUnicodeTestFlags = -1;   // Try all tests
   *pfIsTextUnicode = IsTextUnicode(pvFile, dwFileSize, &iUnicodeTestFlags);

   if (!*pfIsTextUnicode) {
      // For all the file manipulations below, we explicitly use ANSI 
      // functions because we are processing an ANSI file.

      // Put a zero character at the very end of the file.
      PSTR pchANSI = (PSTR) pvFile;
      pchANSI[dwFileSize / sizeof(CHAR)] = 0;

      // Reverse the contents of the file.
      _strrev(pchANSI);

      // Convert all "\n\r" combinations back to "\r\n" to 
      // preserve the normal end-of-line sequence.
      pchANSI = strchr(pchANSI, '\n'); // Find first '\n'.

      while (pchANSI != NULL) {
         // We have found an occurrence....
         *pchANSI++ = '\r';   // Change '\n' to '\r'.
         *pchANSI++ = '\n';   // Change '\r' to '\n'.
         pchANSI = strchr(pchANSI, '\n'); // Find the next occurrence.
      }

   } else {
      // For all the file manipulations below, we explicitly use Unicode
      // functions because we are processing a Unicode file.

      // Put a zero character at the very end of the file.
      PWSTR pchUnicode = (PWSTR) pvFile;
      pchUnicode[dwFileSize / sizeof(WCHAR)] = 0;

      if ((iUnicodeTestFlags & IS_TEXT_UNICODE_SIGNATURE) != 0) {
         // If the first character is the Unicode BOM (byte-order-mark), 
         // 0xFEFF, keep this character at the beginning of the file.
         pchUnicode++;
      }

      // Reverse the contents of the file.
      _wcsrev(pchUnicode);

      // Convert all "\n\r" combinations back to "\r\n" to 
      // preserve the normal end-of-line sequence.
      pchUnicode = wcschr(pchUnicode, L'\n'); // Find first '\n'.

      while (pchUnicode != NULL) {
         // We have found an occurrence....
         *pchUnicode++ = L'\r';   // Change '\n' to '\r'.
         *pchUnicode++ = L'\n';   // Change '\r' to '\n'.
         pchUnicode = wcschr(pchUnicode, L'\n'); // Find the next occurrence.
      }
   }

   // Clean up everything before exiting.
   UnmapViewOfFile(pvFile);
   CloseHandle(hFileMap);

   // Remove trailing zero character added earlier.
   SetFilePointer(hFile, dwFileSize, NULL, FILE_BEGIN);
   SetEndOfFile(hFile);
   CloseHandle(hFile);

   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {

   chSETDLGICONS(hwnd, IDI_FILEREV);

   // Initialize the dialog box by disabling the Reverse button
   EnableWindow(GetDlgItem(hwnd, IDC_REVERSE), FALSE);
   return(TRUE);
}



///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
   
   TCHAR szPathname[MAX_PATH];

   switch (id) {
      case IDCANCEL:
         EndDialog(hwnd, id);
         break;

      case IDC_FILENAME:
         EnableWindow(GetDlgItem(hwnd, IDC_REVERSE), 
            Edit_GetTextLength(hwndCtl) > 0);
         break;

      case IDC_REVERSE:
         GetDlgItemText(hwnd, IDC_FILENAME, szPathname, chDIMOF(szPathname));

         // Make copy of input file so that we don't destroy it
         if (!CopyFile(szPathname, FILENAME, FALSE)) {
            chMB("New file could not be created.");
            break;
         }

         BOOL fIsTextUnicode;
         if (FileReverse(FILENAME, &fIsTextUnicode)) {
            SetDlgItemText(hwnd, IDC_TEXTTYPE, 
               fIsTextUnicode ? TEXT("Unicode") : TEXT("ANSI"));

            // Spawn Notepad to see the fruits of our labors.
            STARTUPINFO si = { sizeof(si) };
            PROCESS_INFORMATION pi;
            TCHAR sz[] = TEXT("Notepad ") FILENAME;
            if (CreateProcess(NULL, sz,
               NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {

               CloseHandle(pi.hThread);
               CloseHandle(pi.hProcess);
            }
         }
         break;

      case IDC_FILESELECT:
         OPENFILENAME ofn = { OPENFILENAME_SIZE_VERSION_400 };
         ofn.hwndOwner = hwnd;
         ofn.lpstrFile = szPathname;
         ofn.lpstrFile[0] = 0;
         ofn.nMaxFile = chDIMOF(szPathname);
         ofn.lpstrTitle = TEXT("Select file for reversing");
         ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
         GetOpenFileName(&ofn);
         SetDlgItemText(hwnd, IDC_FILENAME, ofn.lpstrFile);
         SetFocus(GetDlgItem(hwnd, IDC_REVERSE));
         break;
   }
}


///////////////////////////////////////////////////////////////////////////////


INT_PTR WINAPI Dlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
   
   switch (uMsg) {
      chHANDLE_DLGMSG(hwnd, WM_INITDIALOG,  Dlg_OnInitDialog);
      chHANDLE_DLGMSG(hwnd, WM_COMMAND,     Dlg_OnCommand);
   }
   return(FALSE);
}


///////////////////////////////////////////////////////////////////////////////


int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int) {

   DialogBox(hinstExe, MAKEINTRESOURCE(IDD_FILEREV), NULL, Dlg_Proc);
   return(0);
}


//////////////////////////////// End of File //////////////////////////////////
