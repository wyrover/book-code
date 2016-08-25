/******************************************************************************
Module:  Spreadsheet.cpp
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include <windowsx.h>
#include <tchar.h>
#include "Resource.h"
#include "VMArray.h"


///////////////////////////////////////////////////////////////////////////////


HWND g_hwnd;   // Global window handle used for SEH reporting

const int g_nNumRows = 256;
const int g_nNumCols = 1024;

// Declare the contents of a single cell in the spreadsheet
typedef struct { 
   DWORD dwValue;
   BYTE  bDummy[1020]; 
} CELL, *PCELL;


// Declare the data type for an entire spreadsheet
typedef CELL SPREADSHEET[g_nNumRows][g_nNumCols];
typedef SPREADSHEET *PSPREADSHEET;


///////////////////////////////////////////////////////////////////////////////


// A spreadsheet is a 2-dimensional array of CELLs
class CVMSpreadsheet : public CVMArray<CELL> {
public:
   CVMSpreadsheet() : CVMArray<CELL>(g_nNumRows * g_nNumCols) {}

private:
   LONG OnAccessViolation(PVOID pvAddrTouched, BOOL fAttemptedRead,
      PEXCEPTION_POINTERS pep, BOOL fRetryUntilSuccessful);
};


///////////////////////////////////////////////////////////////////////////////


LONG CVMSpreadsheet::OnAccessViolation(PVOID pvAddrTouched, BOOL fAttemptedRead,
   PEXCEPTION_POINTERS pep, BOOL fRetryUntilSuccessful) {

   TCHAR sz[200];
   wsprintf(sz, TEXT("Violation: Attempting to %s"),
      fAttemptedRead ? TEXT("Read") : TEXT("Write"));
   SetDlgItemText(g_hwnd, IDC_LOG, sz);

   LONG lDisposition = EXCEPTION_EXECUTE_HANDLER;
   if (!fAttemptedRead) {

      // Return whatever the base class says to do
      lDisposition = CVMArray<CELL>::OnAccessViolation(pvAddrTouched, 
         fAttemptedRead, pep, fRetryUntilSuccessful);
   }

   return(lDisposition);
}

///////////////////////////////////////////////////////////////////////////////


// This is the global CVMSpreadsheet object
static CVMSpreadsheet g_ssObject;

// Create a global pointer that points to the entire spreadsheet region
SPREADSHEET& g_ss = * (PSPREADSHEET) (PCELL) g_ssObject;


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {

   chSETDLGICONS(hwnd, IDI_SPREADSHEET);

   g_hwnd = hwnd; // Save for SEH reporting

   // Put default values in the dialog box controls
   Edit_LimitText(GetDlgItem(hwnd, IDC_ROW),    3);
   Edit_LimitText(GetDlgItem(hwnd, IDC_COLUMN), 4);
   Edit_LimitText(GetDlgItem(hwnd, IDC_VALUE),  7);
   SetDlgItemInt(hwnd, IDC_ROW,    100,   FALSE);
   SetDlgItemInt(hwnd, IDC_COLUMN, 100,   FALSE);
   SetDlgItemInt(hwnd, IDC_VALUE,  12345, FALSE);
   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {

   int nRow, nCol;

   switch (id) {
      case IDCANCEL:
         EndDialog(hwnd, id);
         break;

      case IDC_ROW:
         // User modified the row, update the UI
         nRow = GetDlgItemInt(hwnd, IDC_ROW, NULL, FALSE);
         EnableWindow(GetDlgItem(hwnd, IDC_READCELL),  
            chINRANGE(0, nRow, g_nNumRows - 1));
         EnableWindow(GetDlgItem(hwnd, IDC_WRITECELL), 
            chINRANGE(0, nRow, g_nNumRows - 1));
         break;

      case IDC_COLUMN:
         // User modified the column, update the UI
         nCol = GetDlgItemInt(hwnd, IDC_COLUMN, NULL, FALSE);
         EnableWindow(GetDlgItem(hwnd, IDC_READCELL),  
            chINRANGE(0, nCol, g_nNumCols - 1));
         EnableWindow(GetDlgItem(hwnd, IDC_WRITECELL), 
            chINRANGE(0, nCol, g_nNumCols - 1));
         break;

      case IDC_READCELL:
         // Try to read a value from the user's selected cell
         SetDlgItemText(g_hwnd, IDC_LOG, TEXT("No violation raised"));
         nRow = GetDlgItemInt(hwnd, IDC_ROW, NULL, FALSE);
         nCol = GetDlgItemInt(hwnd, IDC_COLUMN, NULL, FALSE);
         __try {
            SetDlgItemInt(hwnd, IDC_VALUE, g_ss[nRow][nCol].dwValue, FALSE);
         }
         __except (
            g_ssObject.ExceptionFilter(GetExceptionInformation(), FALSE)) {

            // The cell is not backed by storage, the cell contains nothing.
            SetDlgItemText(hwnd, IDC_VALUE, TEXT(""));
         }
         break;

      case IDC_WRITECELL:
         // Try to read a value from the user's selected cell
         SetDlgItemText(g_hwnd, IDC_LOG, TEXT("No violation raised"));
         nRow = GetDlgItemInt(hwnd, IDC_ROW, NULL, FALSE);
         nCol = GetDlgItemInt(hwnd, IDC_COLUMN, NULL, FALSE);

         // If the cell is not backed by storage, an access violation is
         // raised causing storage to automatically be committed,
         g_ss[nRow][nCol].dwValue = 
            GetDlgItemInt(hwnd, IDC_VALUE, NULL, FALSE);
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

   DialogBox(hinstExe, MAKEINTRESOURCE(IDD_SPREADSHEET), NULL, Dlg_Proc);
   return(0);
}


//////////////////////////////// End of File //////////////////////////////////
