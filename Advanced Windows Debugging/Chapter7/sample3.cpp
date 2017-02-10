/*++
Copyright (c) Advanced Windows Debugging (ISBN 0321374460) from Addison-Wesley Professional.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "testapp.h"

void Sample3()
{
   
    HKEY bookKey =  NULL; 

    LONG result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\Book", 0, 
        MAXIMUM_ALLOWED, &bookKey); 

    if (bookKey == NULL && GetLastError()==ERROR_FILE_NOT_FOUND) 
        result = RegCreateKeyEx(HKEY_LOCAL_MACHINE, L"Software\\Book", 0, 0, 0, 
        MAXIMUM_ALLOWED, 0, &bookKey, 0); 
    
    if (bookKey != NULL) RegCloseKey(bookKey);
}

