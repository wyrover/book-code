#define STRICT
#include <windows.h>
#include <tchar.h>

/****************************************************************************
*                                 reportError
* Inputs:
*       DWORD err: Error code
* Result: void
*       
* Effect: 
*       Pops up message box reporting error
****************************************************************************/

void reportError(DWORD err)
    {
     LPTSTR str;

     FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                    0, err,
                    0, (LPTSTR)&str,
                    0, NULL);
     MessageBox(NULL, str, _T(""), MB_ICONSTOP | MB_OK | MB_TASKMODAL);
     LocalFree(str);
     
    }
