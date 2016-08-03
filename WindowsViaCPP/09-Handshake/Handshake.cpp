/******************************************************************************
Module:  Handshake.cpp
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#include "..\CommonFiles\CmnHdr.h"     /* See Appendix A. */
#include <windowsx.h>
#include <tchar.h>
#include "Resource.h"


///////////////////////////////////////////////////////////////////////////////


// This event is signaled when the client has a request for the server
HANDLE g_hevtRequestSubmitted;

// This event is signaled when the server has a result for the client
HANDLE g_hevtResultReturned;

// The buffer shared between the client and server threads
TCHAR  g_szSharedRequestAndResultBuffer[1024];

// The special value sent from the client that causes the 
// server thread to terminate cleanly.
TCHAR  g_szServerShutdown[] = TEXT("Server Shutdown");

// The server thread will check that the main dialog is no longer alive
// when the shutdown message is received.
HWND   g_hMainDlg;


///////////////////////////////////////////////////////////////////////////////


// This is the code executed by the server thread
DWORD WINAPI ServerThread(PVOID pvParam) {

   // Assume that the server thread is to run forever
   BOOL fShutdown = FALSE;

   while (!fShutdown) {

      // Wait for the client to submit a request
      WaitForSingleObject(g_hevtRequestSubmitted, INFINITE);

      // Check to see if the client wants the server to terminate
      fShutdown = 
         (g_hMainDlg == NULL) &&
         (_tcscmp(g_szSharedRequestAndResultBuffer, g_szServerShutdown) == 0);

      if (!fShutdown) {
         // Process the client's request (reverse the string)
         _tcsrev(g_szSharedRequestAndResultBuffer);
      }

      // Let the client process the request's result
      SetEvent(g_hevtResultReturned);
   }

   // The client wants us to shut down, exit
   return(0);
}


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {

   chSETDLGICONS(hwnd, IDI_HANDSHAKE);

   // Initialize the edit control with some test data request
   Edit_SetText(GetDlgItem(hwnd, IDC_REQUEST), TEXT("Some test data"));

   // Store the main dialog window handle
   g_hMainDlg = hwnd;

   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {

   switch (id) {

      case IDCANCEL:
         EndDialog(hwnd, id);
         break;

      case IDC_SUBMIT:  // Submit a request to the server thread
      
         // Copy the request string into the shared data buffer
         Edit_GetText(GetDlgItem(hwnd, IDC_REQUEST), 
            g_szSharedRequestAndResultBuffer, 
            _countof(g_szSharedRequestAndResultBuffer));

         // Let the server thread know that a request is ready in the buffer
         // Wait for the server to process the request and give us the result
         SignalObjectAndWait(g_hevtRequestSubmitted, g_hevtResultReturned, INFINITE, false);

         // Let the user know the result
         Edit_SetText(GetDlgItem(hwnd, IDC_RESULT), 
            g_szSharedRequestAndResultBuffer);

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


int WINAPI _tWinMain(HINSTANCE hInstanceExe, HINSTANCE, PTSTR, int) {

   // Create & initialize the 2 nonsignaled, auto-reset events
   g_hevtRequestSubmitted = CreateEvent(NULL, FALSE, FALSE, NULL);
   g_hevtResultReturned   = CreateEvent(NULL, FALSE, FALSE, NULL);

   // Spawn the server thread
   DWORD dwThreadID;
   HANDLE hThreadServer = chBEGINTHREADEX(NULL, 0, ServerThread, NULL, 
      0, &dwThreadID);

   // Execute the client thread's user interface
   DialogBox(hInstanceExe, MAKEINTRESOURCE(IDD_HANDSHAKE), NULL, Dlg_Proc);
   g_hMainDlg = NULL;

   // The client's UI is closing, have the server thread shutdown
   _tcscpy_s(g_szSharedRequestAndResultBuffer, 
      _countof(g_szSharedRequestAndResultBuffer), g_szServerShutdown);
   SetEvent(g_hevtRequestSubmitted);

   // Wait for the server thread to acknowledge the shutdown AND
   // wait for the server thread to fully terminate
   HANDLE h[2];
   h[0] = g_hevtResultReturned;
   h[1] = hThreadServer;
   WaitForMultipleObjects(2, h, TRUE, INFINITE);
   
   // Properly clean up everything
   CloseHandle(hThreadServer);      
   CloseHandle(g_hevtRequestSubmitted);      
   CloseHandle(g_hevtResultReturned);      

   // The client thread terminates with the whole process
   return(0);
}


//////////////////////////////// End of File //////////////////////////////////
