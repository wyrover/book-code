/******************************************************************************
Module:  SEHTerm.cpp
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#include "..\CmnHdr.h"     /* See Appendix A. */
#include <tchar.h>


///////////////////////////////////////////////////////////////////////////////


int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int) {

   __try {
      int n = MessageBox(NULL, TEXT("Perform invalid memory access?"), 
         TEXT("SEHTerm: In try block"), MB_YESNO);

      if (n == IDYES) {
         * (PBYTE) NULL = 5;  // This causes an access violation
      }
   }
   __finally {
      PCTSTR psz = AbnormalTermination() 
         ? TEXT("Abnormal termination") : TEXT("Normal termination");
      MessageBox(NULL, psz, TEXT("SEHTerm: In finally block"), MB_OK);
   }

   MessageBox(NULL, TEXT("Normal process termination"), 
      TEXT("SEHTerm: After finally block"), MB_OK);

   return(0);
}


//////////////////////////////// End of File //////////////////////////////////
