/******************************************************************************
Module:  CustomizedWER.cpp
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <stdlib.h>  // for _countof definition
#include "..\CommonFiles\CmnHdr.h"     /* See Appendix A. */
#include <Windows.h>
#include <Windowsx.h>
#include <WerAPI.h>

#include <tchar.h>
#include <StrSafe.h>

#include "CustomizedWER.h"

// Required library for Windows Error Reporting
#pragma comment(lib, "wer")


///////////////////////////////////////////////////////////////////////////////
// Global variable used during problem reporting
typedef struct _MORE_INFO {
   DWORD dwCode;
   DWORD dwValue;
} MORE_INFO;

// Don't use stack or heap allocation to avoid corruption
MORE_INFO s_moreInfo1 = {0, 0};
MORE_INFO s_moreInfo2 = {0, 0};

// Store user choices related to shutdown options
BOOL s_bCustom = TRUE;
BOOL s_bAllowJITDebug = TRUE;
BOOL s_bGenerateReport = TRUE;


///////////////////////////////////////////////////////////////////////////////


void EnableAutomaticJITDebug(BOOL bAutomaticDebug) {

   // Create the subkey if necessary
   const LPCTSTR szKeyName = TEXT("Software\\Microsoft\\Windows\\Windows Error \
Reporting\\DebugApplications");
   HKEY hKey = NULL;
   DWORD dwDisposition = 0;
   LSTATUS lResult = ERROR_SUCCESS;
   lResult = RegCreateKeyEx(HKEY_CURRENT_USER, szKeyName, 0, NULL,
       REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDisposition);
   if (lResult != ERROR_SUCCESS) {
      MessageBox(NULL, TEXT("RegCreateKeyEx failed"), 
         TEXT("EnableAutomaticJITDebug"), MB_OK | MB_ICONHAND);
      return;
   }
      
   // Give the right value to the Registry entry
   DWORD dwValue = bAutomaticDebug ? 1 : 0;
   TCHAR szFullpathName[MAX_PATH];
   GetModuleFileName(NULL, szFullpathName, _countof(szFullpathName));
   LPTSTR pszExeName = _tcsrchr(szFullpathName, TEXT('\\'));
   if (pszExeName != NULL) {
      // Skip the '\'
      pszExeName++;
      
      // Set the value
      lResult = RegSetValueEx(hKey, pszExeName, 0, REG_DWORD, 
         (const BYTE*)&dwValue, sizeof(dwValue));
      if (lResult != ERROR_SUCCESS) {
         MessageBox(NULL, TEXT("RegSetValueEx failed"), 
            TEXT("EnableAutomaticJITDebug"), MB_OK | MB_ICONHAND);
         return;
      }
   }
}


///////////////////////////////////////////////////////////////////////////////


LONG GenerateWerReport(struct _EXCEPTION_POINTERS* pExceptionInfo) {

   // Default return value
   LONG lResult = EXCEPTION_CONTINUE_SEARCH;

   // Avoid stack problem because wri is a big structure
   static WER_REPORT_INFORMATION wri = { sizeof(wri) };

   // Set the report details
   StringCchCopyW(wri.wzFriendlyEventName, _countof(wri.wzFriendlyEventName),
       L"Unexpected Error - 0x12345678");
   StringCchCopyW(wri.wzApplicationName, _countof(wri.wzApplicationName), 
      L"Wintellect Applications Suite");
   GetModuleFileNameW(NULL, (WCHAR*)&(wri.wzApplicationPath), 
      _countof(wri.wzApplicationPath));
   StringCchCopyW(wri.wzDescription, _countof(wri.wzDescription), 
      L"This problem report is generated for testing purpose");

   HREPORT hReport = NULL;
   
   // Create a report and set additional information
   __try {                        // V-- instead of the default APPCRASH_EVENT
      HRESULT hr = WerReportCreate(L"Unexpected Error", 
         WerReportApplicationCrash, &wri, &hReport);   

      if (FAILED(hr)) {
         MessageBox(NULL, TEXT("WerReportCreate failed"), 
            TEXT("GenerateWerReport"), MB_OK | MB_ICONHAND);
         return(EXCEPTION_CONTINUE_SEARCH);
      }
      if (hReport == NULL) {
         MessageBox(NULL, TEXT("WerReportCreate failed"), 
            TEXT("GenerateWerReport"), MB_OK | MB_ICONHAND);
         return(EXCEPTION_CONTINUE_SEARCH);
      }

      // Set more details important to help fix the problem
      WerReportSetParameter(hReport, WER_P0, 
         L"Application Name", L"26-CustomizedWER.exe");
      WerReportSetParameter(hReport, WER_P1, 
         L"Application Version", L"5.0.0.0");
      WerReportSetParameter(hReport, WER_P2, 
         L"Last Action", L"Server Request #12");
      WerReportSetParameter(hReport, WER_P3, 
         L"Last Connected Server", L"http://www.wintellect.com");

      // Add a dump file corresponding to the exception information
      WER_EXCEPTION_INFORMATION wei;
      wei.bClientPointers = FALSE;              // We are in the process where
      wei.pExceptionPointers = pExceptionInfo;  // pExceptionInfo is valid
      hr = WerReportAddDump(
         hReport, GetCurrentProcess(), GetCurrentThread(), 
         WerDumpTypeHeapDump, &wei, NULL, 0);
      if (FAILED(hr)) {
         MessageBox(NULL, TEXT("WerReportAddDump failed"), 
            TEXT("GenerateWerReport"), MB_OK | MB_ICONHAND);
         return(EXCEPTION_CONTINUE_SEARCH);
      }

      // Let memory blocks be visible from a mini-dump
      s_moreInfo1.dwCode = 0x1;
      s_moreInfo1.dwValue = 0xDEADBEEF;
      s_moreInfo2.dwCode = 0x2;
      s_moreInfo2.dwValue = 0x0BADBEEF;
      hr = WerRegisterMemoryBlock(&s_moreInfo1, sizeof(s_moreInfo1));
      if (hr != S_OK) { // Don't want S_FALSE
         MessageBox(NULL, TEXT("First WerRegisterMemoryBlock failed"), 
            TEXT("GenerateWerReport"), MB_OK | MB_ICONHAND);
         return(EXCEPTION_CONTINUE_SEARCH);
      }
      hr = WerRegisterMemoryBlock(&s_moreInfo2, sizeof(s_moreInfo2));
      if (hr != S_OK) { // Don't want S_FALSE
         MessageBox(NULL, TEXT("Second WerRegisterMemoryBlock failed"), 
            TEXT("GenerateWerReport"), MB_OK | MB_ICONHAND);
         return(EXCEPTION_CONTINUE_SEARCH);
      }

      // Add more files to this particular report
      wchar_t wszFilename[] = L"MoreData.txt";
      char textData[] = "Contains more information about the execution \r\n\
context when the problem occurred. The goal is to \r\n\
help figure out the root cause of the issue.";
      // Note that error checking is removed for readability
      HANDLE hFile = CreateFileW(wszFilename, GENERIC_WRITE, 0, NULL, 
         CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
      DWORD dwByteWritten = 0;
      WriteFile(hFile, (BYTE*)textData, sizeof(textData), &dwByteWritten, 
         NULL);
      CloseHandle(hFile);
      hr = WerReportAddFile(hReport, wszFilename, WerFileTypeOther, 
         WER_FILE_ANONYMOUS_DATA);
      if (FAILED(hr)) {
         MessageBox(NULL, TEXT("WerReportAddFile failed"), 
            TEXT("GenerateWerReport"), MB_OK | MB_ICONHAND);
         return(EXCEPTION_CONTINUE_SEARCH);
      }

      // It is also possible to use WerRegisterFile 
      char textRegisteredData[] = "Contains more information about the \
execution \r\ncontext when the problem occurred. The goal is to \r\n\
help figure out the root cause of the issue.";
      // Note that error checking is removed for readability
      hFile = CreateFileW(L"RegisteredData1.txt", GENERIC_WRITE, 0, NULL, 
         CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
      dwByteWritten = 0;
      WriteFile(hFile, (BYTE*)textRegisteredData, sizeof(textRegisteredData),
         &dwByteWritten, NULL);
      CloseHandle(hFile);
      hr = WerRegisterFile(L"RegisteredData1.txt", WerRegFileTypeOther, 
         WER_FILE_ANONYMOUS_DATA);
      if (FAILED(hr)) {
         MessageBox(NULL, TEXT("First WerRegisterFile failed"), 
            TEXT("GenerateWerReport"), MB_OK | MB_ICONHAND);
         return(EXCEPTION_CONTINUE_SEARCH);
      }
      hFile = CreateFileW(L"RegisteredData2.txt", GENERIC_WRITE, 0, NULL, 
         CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
      dwByteWritten = 0;
      WriteFile(hFile, (BYTE*)textRegisteredData, sizeof(textRegisteredData),
         &dwByteWritten, NULL);
      CloseHandle(hFile);
      hr = WerRegisterFile(L"RegisteredData2.txt", WerRegFileTypeOther, 
         WER_FILE_DELETE_WHEN_DONE);   // File is deleted after WerReportSubmit
      if (FAILED(hr)) {
         MessageBox(NULL, TEXT("Second WerRegisterFile failed"), 
            TEXT("GenerateWerReport"), MB_OK | MB_ICONHAND);
         return(EXCEPTION_CONTINUE_SEARCH);
      }
      
      // Submit the report 
      WER_SUBMIT_RESULT wsr;
      DWORD submitOptions = 
         WER_SUBMIT_QUEUE |
         WER_SUBMIT_OUTOFPROCESS |
         WER_SUBMIT_NO_CLOSE_UI;  // Don't show any UI
      hr = WerReportSubmit(hReport, WerConsentApproved, submitOptions, &wsr); 
      if (FAILED(hr)) {
         MessageBox(NULL, TEXT("WerReportSubmit failed"), 
            TEXT("GenerateWerReport"), MB_OK | MB_ICONHAND);
         return(EXCEPTION_CONTINUE_SEARCH);
      }
      
      // The submission was successful, but we might need to check the result 
      switch(wsr)
      {
         case WerReportQueued:
         case WerReportUploaded: // To exit the process
            lResult = EXCEPTION_EXECUTE_HANDLER;   
            break;
         
         case WerReportDebug: // To end up in the debugger
            lResult = EXCEPTION_CONTINUE_SEARCH;   
            break;
         
         default: // Let the OS handle the exception
            lResult = EXCEPTION_CONTINUE_SEARCH;   
            break;
      }
      
      // In our case, we always exit the process after the report generation
      lResult = EXCEPTION_EXECUTE_HANDLER;
   }
   __finally {
      // Don't forget to close the report handle
      if (hReport != NULL) {
         WerReportCloseHandle(hReport);
         hReport = NULL;
      }
   }

   return(lResult);
}


///////////////////////////////////////////////////////////////////////////////


static BOOL s_bFirstTime = TRUE;

LONG WINAPI CustomUnhandledExceptionFilter(
   struct _EXCEPTION_POINTERS* pExceptionInfo) {

   // When the debugger gets attached and you stop the debugging session,
   // the execution resumes here...
   // So this case is detected and the application exits silently 
   if (s_bFirstTime)
      s_bFirstTime = FALSE;
   else
      ExitProcess(pExceptionInfo->ExceptionRecord->ExceptionCode);

   // Check shutdown options
   if (!s_bCustom)
      // Let Windows treat the exception 
      return(UnhandledExceptionFilter(pExceptionInfo));

   // Allow global unwind by default
   LONG lReturn = EXCEPTION_EXECUTE_HANDLER;

   // Let the user choose between Debug or Close application
   // except if JIT-debugging was disabled in the options
   int iChoice = IDCANCEL;
   if (s_bAllowJITDebug) {
      iChoice = MessageBox(NULL, 
         TEXT("Click RETRY if you want to debug\nClick CANCEL to quit"), 
         TEXT("The application must stop"), MB_RETRYCANCEL | MB_ICONHAND);
   }

   if (iChoice == IDRETRY) {
      // Force JIT-debugging for this application
      EnableAutomaticJITDebug(TRUE);

      // Ask Windows to JIT-attach the default debugger
      lReturn = EXCEPTION_CONTINUE_SEARCH;
   } else {
      // The application will be terminated
      lReturn = EXCEPTION_EXECUTE_HANDLER;

      // But check if we need to generate a problem report first
      if (s_bGenerateReport)
         GenerateWerReport(pExceptionInfo);
   }
   
   return(lReturn);   
}


///////////////////////////////////////////////////////////////////////////////


void TriggerException() {

   // Trigger an exception wrapped by a finally block
   // that is only executed if a global unwind occurs
   __try {
      TCHAR* p = NULL;
      *p = TEXT('a');
   }
   __finally {
      MessageBox(NULL, TEXT("Finally block is executed"), NULL, MB_OK);
   }
}




///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hWnd, HWND hWndFocus, LPARAM lParam) {

   // Set dialog icon
   chSETDLGICONS(hWnd, IDI_MAINDLG);

   // Choose custom options
   Button_SetCheck(GetDlgItem(hWnd, IDR_CUSTOM), TRUE);
   Button_SetCheck(GetDlgItem(hWnd, IDC_ALLOW_DEBUG), TRUE);
   Button_SetCheck(GetDlgItem(hWnd, IDC_GENERATE_REPORT), TRUE);

   // Change the default focused control
   SetFocus(GetDlgItem(hWnd, IDR_CUSTOM));
   return(FALSE);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify) {

   switch (id) {
      case IDR_DEFAULT:
         // Disable custom options
         EnableWindow(GetDlgItem(hWnd, IDC_ALLOW_DEBUG), FALSE);
         EnableWindow(GetDlgItem(hWnd, IDC_GENERATE_REPORT), FALSE);
         break;

      case IDR_CUSTOM:
         // Enable custom options
         EnableWindow(GetDlgItem(hWnd, IDC_ALLOW_DEBUG), TRUE);
         EnableWindow(GetDlgItem(hWnd, IDC_GENERATE_REPORT), TRUE);
         break;

      case IDOK:
         // Keep track of the user choices about shutdown behavior
         s_bCustom = 
            (Button_GetCheck(GetDlgItem(hWnd, IDR_CUSTOM)) == BST_CHECKED);
         s_bAllowJITDebug = 
            (Button_GetCheck(GetDlgItem(hWnd, IDC_ALLOW_DEBUG)) 
            == BST_CHECKED);
         s_bGenerateReport = 
            (Button_GetCheck(GetDlgItem(hWnd, IDC_GENERATE_REPORT)) 
            == BST_CHECKED);

         TriggerException();
         break;

      case IDCANCEL:
         EndDialog(hWnd, id);
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


int APIENTRY _tWinMain(HINSTANCE hInstExe, HINSTANCE, LPTSTR, int) {

   int iReturn = 0;

   // Disable the automatic JIT-debugger attachment
   // that could have been enabled in CustomUnhandledExceptionFilter
   // in a prior execution of the same application
   EnableAutomaticJITDebug(FALSE);

   // Protect the code with our own exception filter
   __try {
      DialogBox(hInstExe, MAKEINTRESOURCE(IDD_MAINDLG), NULL, Dlg_Proc);
   }
   __except(CustomUnhandledExceptionFilter(GetExceptionInformation())) {
      MessageBox(NULL, TEXT("Bye bye"), NULL, MB_OK);
      ExitProcess(GetExceptionCode());
   }

   return(iReturn);
}

