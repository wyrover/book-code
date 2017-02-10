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
#include <conio.h>
#include "debug.h"
#include "testapp.h"


void LazyInitialization()
{
    HKEY softwareKey = NULL;
    LONG retCode = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software", 0, MAXIMUM_ALLOWED, &softwareKey);
    if (retCode != ERROR_SUCCESS)
    {
        TRACE_ERR(L"RegOpenKeyEx failed.", retCode);
    }

    HKEY bookKey = NULL;
    retCode = RegCreateKey(bookKey, L"Advanced Windows Debugging", &bookKey);
    if (retCode != ERROR_SUCCESS)
    {
        TRACE_ERR(L"RegCreateKey failed.", retCode);
    }

    RegCloseKey(bookKey);
    RegCloseKey(softwareKey);


    BOOL otherCode = ImpersonateSelf(SecurityImpersonation);

    if (otherCode == FALSE)
    {
        TRACE(L"ImpersonateSelf failed.");
    }
    
    HANDLE threadToken = NULL;
    otherCode = OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, FALSE, &threadToken);
    if (otherCode == FALSE)
    {
        TRACE(L"OpenThreadToken failed.");
    }
    if (threadToken) CloseHandle(threadToken);
        
    HANDLE event = CreateEvent(NULL, FALSE, FALSE, L"07sample");
    CloseHandle(event);

    HANDLE threadTokenAsSelf = NULL;

    otherCode = OpenThreadToken(GetCurrentThread(), TOKEN_QUERY |TOKEN_IMPERSONATE , TRUE, &threadTokenAsSelf);
    if (otherCode == FALSE)
    {
        TRACE(L"OpenThreadToken AsSelf failed.");
    }

    RevertToSelf();

    otherCode = ImpersonateLoggedOnUser(threadTokenAsSelf);
    if (otherCode == FALSE)
    {
        TRACE(L"ImpersonateSelf failed.");
    }

    if (threadTokenAsSelf) CloseHandle(threadTokenAsSelf);
    
    RevertToSelf();
    
}

void Sample2()
{
    HANDLE userToken = NULL;
    BOOL retCode =  LogonUser(L"Test1", NULL, L"TestUser1", LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT,  &userToken);
    if (retCode != TRUE)
    {
        TRACE(L"LogonUser failed.");
        return;
    }
    ImpersonateLoggedOnUser(userToken);

    LazyInitialization();
    
    RevertToSelf();
    CloseHandle(userToken);
}
    