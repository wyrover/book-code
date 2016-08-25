/******************************************************************************
Module:  TimedMsgBox.cpp
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include <tchar.h>


//////////////////////////////////////////////////////////////////////////////


// The caption of our message box
TCHAR g_szCaption[] = TEXT("Timed Message Box");


// How many seconds we'll display the message box
int g_nSecLeft = 0;


// This is STATIC window control ID for a message box
#define ID_MSGBOX_STATIC_TEXT    0x0000ffff


//////////////////////////////////////////////////////////////////////////////


VOID WINAPI MsgBoxTimeout(PVOID pvContext, BOOLEAN fTimeout) {

   // NOTE: Due to a thread race condition, it is possible (but very unlikely)
   // that the message box will not be created when we get here.
   HWND hwnd = FindWindow(NULL, g_szCaption);

   if (hwnd != NULL) {
      // The window does exist; update the time remaining.
      TCHAR sz[100];
      wsprintf(sz, TEXT("You have %d seconds to respond"), g_nSecLeft--);
      SetDlgItemText(hwnd, ID_MSGBOX_STATIC_TEXT, sz);

      if (g_nSecLeft == 0) {
         // The time is up; force the message box to exit.
         EndDialog(hwnd, IDOK);
      }
   } else {

      // The window does not exist yet; do nothing this time.
      // We'll try again in another second.
   }
}


//////////////////////////////////////////////////////////////////////////////


int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int) {

   chWindows9xNotAllowed();

   // How many seconds we'll give the user to respond
   g_nSecLeft = 10;

   // Create a multishot 1 second timer that begins firing after 1 second.
   HANDLE hTimerQTimer;
   CreateTimerQueueTimer(&hTimerQTimer, NULL, MsgBoxTimeout, NULL, 
      1000, 1000, 0);

   // Display the message box
   MessageBox(NULL, TEXT("You have 10 seconds to respond"), 
      g_szCaption, MB_OK);

   // Cancel the timer & delete the timer queue
   DeleteTimerQueueTimer(NULL, hTimerQTimer, NULL);

   // Let us know if the user responded or if we timed-out.
   MessageBox(NULL, 
      (g_nSecLeft == 0) ? TEXT("Timeout") : TEXT("User responded"), 
         TEXT("Result"), MB_OK);

   return(0);
}


//////////////////////////////// End of File /////////////////////////////////
