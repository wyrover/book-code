/*++
Copyright (c) Advanced Windows Debugging (ISBN 0321374460) from Addison-Wesley Professional.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/

#include <windows.h>
#include <stdio.h>

#define MAX_CONN_LEN    30

VOID HelperFunction(WCHAR* pszConnectionString);

void __cdecl wmain (int argc, WCHAR* args[])
{
    if (argc==2)
    {
        HelperFunction(args[1]);
        wprintf (L"Connection to %s established\n",args[1]);
    }
    else
    {
        wprintf (L"Please specify connection string on the command line\n");
    }
}

VOID HelperFunction(WCHAR* pszConnectionString)
{
    WCHAR pszCopy[MAX_CONN_LEN];

    wcscpy(pszCopy, pszConnectionString);
    //
    // ...
    // Establish connection
    // ...
    //
}
